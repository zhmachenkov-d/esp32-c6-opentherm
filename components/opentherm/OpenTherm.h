/*
 * OpenTherm.h - OpenTherm Library ported for ESP-IDF
 * Original: Ihor Melnyk (MIT) https://github.com/ihormelnyk/opentherm_library
 */

#ifndef OPENTHERM_H
#define OPENTHERM_H

#include <stdint.h>

enum class OpenThermResponseStatus : uint8_t {
    NONE,
    SUCCESS,
    INVALID,
    TIMEOUT
};

enum class OpenThermMessageType : uint8_t {
    READ_DATA = 0b000,
    READ = READ_DATA,
    WRITE_DATA = 0b001,
    WRITE = WRITE_DATA,
    INVALID_DATA = 0b010,
    RESERVED = 0b011,
    READ_ACK = 0b100,
    WRITE_ACK = 0b101,
    DATA_INVALID = 0b110,
    UNKNOWN_DATA_ID = 0b111
};

typedef OpenThermMessageType OpenThermRequestType;

enum class OpenThermMessageID : uint8_t {
    Status = 0,
    TSet = 1,
    Tboiler = 25,
    Tret = 28,
};

enum class OpenThermStatus : uint8_t {
    NOT_INITIALIZED,
    READY,
    DELAY,
    REQUEST_SENDING,
    RESPONSE_WAITING,
    RESPONSE_START_BIT,
    RESPONSE_RECEIVING,
    RESPONSE_READY,
    RESPONSE_INVALID
};

class OpenTherm {
public:
    OpenTherm(int inPin, int outPin, bool isSlave = false);
    ~OpenTherm();

    volatile OpenThermStatus status;

    void begin();
    bool isReady();
    unsigned long sendRequest(unsigned long request);
    bool sendRequestAsync(unsigned long request);
    static unsigned long buildRequest(OpenThermMessageType type, OpenThermMessageID id, unsigned int data);
    unsigned long getLastResponse();
    OpenThermResponseStatus getLastResponseStatus();
    void handleInterrupt();
    static void handleInterruptHelper(void *ptr);
    void process();
    void end();

    static bool parity(unsigned long frame);
    static OpenThermMessageType getMessageType(unsigned long message);
    static OpenThermMessageID getDataID(unsigned long frame);
    static bool isValidRequest(unsigned long request);
    static bool isValidResponse(unsigned long response);

    static unsigned long buildSetBoilerStatusRequest(bool enableCentralHeating, bool enableHotWater = false,
                                                     bool enableCooling = false,
                                                     bool enableOutsideTemperatureCompensation = false,
                                                     bool enableCentralHeating2 = false);
    static unsigned long buildSetBoilerTemperatureRequest(float temperature);
    static unsigned long buildGetBoilerTemperatureRequest();

    static float getFloat(unsigned long response);
    static unsigned int temperatureToData(float temperature);

    unsigned long setBoilerStatus(bool enableCentralHeating, bool enableHotWater = false, bool enableCooling = false,
                                bool enableOutsideTemperatureCompensation = false, bool enableCentralHeating2 = false);
    bool setBoilerTemperature(float temperature);
    float getBoilerTemperature();

private:
    const int inPin;
    const int outPin;
    const bool isSlave;

    volatile unsigned long response;
    volatile OpenThermResponseStatus responseStatus;
    volatile unsigned long responseTimestamp;
    volatile uint8_t responseBitIndex;

    int readState();
    void setActiveState();
    void setIdleState();
    void activateBoiler();
    void sendBit(bool high);
};

#endif
