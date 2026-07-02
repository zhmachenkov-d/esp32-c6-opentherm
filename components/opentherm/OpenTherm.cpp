/*
 * OpenTherm.cpp - OpenTherm Library ported for ESP-IDF
 * Original: Ihor Melnyk (MIT) https://github.com/ihormelnyk/opentherm_library
 */

#include "OpenTherm.h"

#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static inline unsigned long micros_now()
{
    return (unsigned long)(esp_timer_get_time());
}

OpenTherm::OpenTherm(int inPin, int outPin, bool isSlave)
    : status(OpenThermStatus::NOT_INITIALIZED),
      inPin(inPin),
      outPin(outPin),
      isSlave(isSlave),
      response(0),
      responseStatus(OpenThermResponseStatus::NONE),
      responseTimestamp(0),
      responseBitIndex(0)
{
}

OpenTherm::~OpenTherm()
{
    end();
}

void OpenTherm::begin()
{
    gpio_config_t out_conf = {
        .pin_bit_mask = (1ULL << outPin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&out_conf);

    gpio_config_t in_conf = {
        .pin_bit_mask = (1ULL << inPin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };
    gpio_config(&in_conf);

    static bool isr_service_installed = false;
    if (!isr_service_installed) {
        gpio_install_isr_service(0);
        isr_service_installed = true;
    }

    gpio_isr_handler_add((gpio_num_t)inPin, OpenTherm::handleInterruptHelper, this);

    activateBoiler();
    status = OpenThermStatus::READY;
}

bool IRAM_ATTR OpenTherm::isReady()
{
    return status == OpenThermStatus::READY;
}

int IRAM_ATTR OpenTherm::readState()
{
    return gpio_get_level((gpio_num_t)inPin);
}

void OpenTherm::setActiveState()
{
    gpio_set_level((gpio_num_t)outPin, 0);
}

void OpenTherm::setIdleState()
{
    gpio_set_level((gpio_num_t)outPin, 1);
}

void OpenTherm::activateBoiler()
{
    setIdleState();
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void OpenTherm::sendBit(bool high)
{
    if (high) {
        setActiveState();
    } else {
        setIdleState();
    }
    esp_rom_delay_us(500);
    if (high) {
        setIdleState();
    } else {
        setActiveState();
    }
    esp_rom_delay_us(500);
}

bool OpenTherm::sendRequestAsync(unsigned long request)
{
    portDISABLE_INTERRUPTS();
    const bool ready = isReady();

    if (!ready) {
        portENABLE_INTERRUPTS();
        return false;
    }

    status = OpenThermStatus::REQUEST_SENDING;
    response = 0;
    responseStatus = OpenThermResponseStatus::NONE;

    BaseType_t schedulerState = xTaskGetSchedulerState();
    if (schedulerState == taskSCHEDULER_RUNNING) {
        vTaskSuspendAll();
    }

    portENABLE_INTERRUPTS();

    sendBit(true);
    for (int i = 31; i >= 0; i--) {
        sendBit((request >> i) & 1);
    }
    sendBit(true);
    setIdleState();

    responseTimestamp = micros_now();
    status = OpenThermStatus::RESPONSE_WAITING;

    if (schedulerState == taskSCHEDULER_RUNNING) {
        xTaskResumeAll();
    }

    return true;
}

unsigned long OpenTherm::sendRequest(unsigned long request)
{
    if (!sendRequestAsync(request)) {
        return 0;
    }

    while (!isReady()) {
        process();
        taskYIELD();
    }
    return response;
}

unsigned long OpenTherm::getLastResponse()
{
    return response;
}

OpenThermResponseStatus OpenTherm::getLastResponseStatus()
{
    return responseStatus;
}

void IRAM_ATTR OpenTherm::handleInterrupt()
{
    if (isReady()) {
        if (isSlave && readState() == 1) {
            status = OpenThermStatus::RESPONSE_WAITING;
        } else {
            return;
        }
    }

    unsigned long newTs = micros_now();
    if (status == OpenThermStatus::RESPONSE_WAITING) {
        if (readState() == 1) {
            status = OpenThermStatus::RESPONSE_START_BIT;
            responseTimestamp = newTs;
        } else {
            status = OpenThermStatus::RESPONSE_INVALID;
            responseTimestamp = newTs;
        }
    } else if (status == OpenThermStatus::RESPONSE_START_BIT) {
        if ((newTs - responseTimestamp < 750) && readState() == 0) {
            status = OpenThermStatus::RESPONSE_RECEIVING;
            responseTimestamp = newTs;
            responseBitIndex = 0;
        } else {
            status = OpenThermStatus::RESPONSE_INVALID;
            responseTimestamp = newTs;
        }
    } else if (status == OpenThermStatus::RESPONSE_RECEIVING) {
        if ((newTs - responseTimestamp) > 750) {
            if (responseBitIndex < 32) {
                response = (response << 1) | (unsigned long)(!readState());
                responseTimestamp = newTs;
                responseBitIndex = responseBitIndex + 1;
            } else {
                status = OpenThermStatus::RESPONSE_READY;
                responseTimestamp = newTs;
            }
        }
    }
}

void IRAM_ATTR OpenTherm::handleInterruptHelper(void *ptr)
{
    static_cast<OpenTherm *>(ptr)->handleInterrupt();
}

void OpenTherm::process()
{
    portDISABLE_INTERRUPTS();
    OpenThermStatus st = status;
    unsigned long ts = responseTimestamp;
    portENABLE_INTERRUPTS();

    if (st == OpenThermStatus::READY) {
        return;
    }

    unsigned long newTs = micros_now();
    if (st != OpenThermStatus::NOT_INITIALIZED && st != OpenThermStatus::DELAY && (newTs - ts) > 1000000) {
        status = OpenThermStatus::READY;
        responseStatus = OpenThermResponseStatus::TIMEOUT;
    } else if (st == OpenThermStatus::RESPONSE_INVALID) {
        status = OpenThermStatus::DELAY;
        responseStatus = OpenThermResponseStatus::INVALID;
    } else if (st == OpenThermStatus::RESPONSE_READY) {
        status = OpenThermStatus::DELAY;
        responseStatus = (isSlave ? isValidRequest(response) : isValidResponse(response))
                             ? OpenThermResponseStatus::SUCCESS
                             : OpenThermResponseStatus::INVALID;
    } else if (st == OpenThermStatus::DELAY) {
        if ((newTs - ts) > (isSlave ? 20000 : 100000)) {
            status = OpenThermStatus::READY;
        }
    }
}

void OpenTherm::end()
{
    gpio_isr_handler_remove((gpio_num_t)inPin);
}

bool OpenTherm::parity(unsigned long frame)
{
    uint8_t p = 0;
    while (frame > 0) {
        if (frame & 1) {
            p++;
        }
        frame = frame >> 1;
    }
    return (p & 1);
}

OpenThermMessageType OpenTherm::getMessageType(unsigned long message)
{
    return static_cast<OpenThermMessageType>((message >> 28) & 7);
}

OpenThermMessageID OpenTherm::getDataID(unsigned long frame)
{
    return static_cast<OpenThermMessageID>((frame >> 16) & 0xFF);
}

unsigned long OpenTherm::buildRequest(OpenThermMessageType type, uint8_t id, unsigned int data)
{
    unsigned long request = data;
    if (type == OpenThermMessageType::WRITE_DATA) {
        request |= 1ul << 28;
    }
    request |= ((unsigned long)id) << 16;
    if (parity(request)) {
        request |= (1ul << 31);
    }
    return request;
}

unsigned long OpenTherm::buildRequest(OpenThermMessageType type, OpenThermMessageID id, unsigned int data)
{
    return buildRequest(type, static_cast<uint8_t>(id), data);
}

bool OpenTherm::isValidResponse(unsigned long resp)
{
    if (parity(resp)) {
        return false;
    }
    uint8_t msgType = (resp << 1) >> 29;
    return msgType == (uint8_t)OpenThermMessageType::READ_ACK || msgType == (uint8_t)OpenThermMessageType::WRITE_ACK;
}

bool OpenTherm::isValidRequest(unsigned long request)
{
    if (parity(request)) {
        return false;
    }
    uint8_t msgType = (request << 1) >> 29;
    return msgType == (uint8_t)OpenThermMessageType::READ_DATA || msgType == (uint8_t)OpenThermMessageType::WRITE_DATA;
}

unsigned long OpenTherm::buildSetBoilerStatusRequest(bool enableCentralHeating, bool enableHotWater, bool enableCooling,
                                                     bool enableOutsideTemperatureCompensation, bool enableCentralHeating2)
{
    unsigned int data = enableCentralHeating | (enableHotWater << 1) | (enableCooling << 2) |
                        (enableOutsideTemperatureCompensation << 3) | (enableCentralHeating2 << 4);
    data <<= 8;
    return buildRequest(OpenThermMessageType::READ_DATA, OpenThermMessageID::Status, data);
}

unsigned long OpenTherm::buildSetBoilerTemperatureRequest(float temperature)
{
    unsigned int data = temperatureToData(temperature);
    return buildRequest(OpenThermMessageType::WRITE_DATA, OpenThermMessageID::TSet, data);
}

unsigned long OpenTherm::buildGetBoilerTemperatureRequest()
{
    return buildRequest(OpenThermMessageType::READ_DATA, OpenThermMessageID::Tboiler, 0);
}

float OpenTherm::getFloat(unsigned long resp)
{
    const uint16_t u88 = resp & 0xffff;
    const float f = (u88 & 0x8000) ? -(0x10000L - u88) / 256.0f : u88 / 256.0f;
    return f;
}

unsigned int OpenTherm::temperatureToData(float temperature)
{
    if (temperature < 0) {
        temperature = 0;
    }
    if (temperature > 100) {
        temperature = 100;
    }
    return (unsigned int)(temperature * 256);
}

unsigned long OpenTherm::setBoilerStatus(bool enableCentralHeating, bool enableHotWater, bool enableCooling,
                                         bool enableOutsideTemperatureCompensation, bool enableCentralHeating2)
{
    return sendRequest(buildSetBoilerStatusRequest(enableCentralHeating, enableHotWater, enableCooling,
                                                   enableOutsideTemperatureCompensation, enableCentralHeating2));
}

bool OpenTherm::setBoilerTemperature(float temperature)
{
    unsigned long resp = sendRequest(buildSetBoilerTemperatureRequest(temperature));
    return isValidResponse(resp);
}

float OpenTherm::getBoilerTemperature()
{
    unsigned long resp = sendRequest(buildGetBoilerTemperatureRequest());
    return isValidResponse(resp) ? getFloat(resp) : 0;
}
