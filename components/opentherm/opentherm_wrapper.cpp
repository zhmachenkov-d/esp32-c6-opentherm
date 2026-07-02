#include "opentherm_wrapper.h"

#include "OpenTherm.h"
#include "ot_data_catalog.h"

static OpenTherm *s_ot = nullptr;

static ot_data_status_t classify_response(unsigned long resp, OpenThermResponseStatus bus_status)
{
    if (bus_status == OpenThermResponseStatus::TIMEOUT) {
        return OT_DATA_TIMEOUT;
    }
    if (bus_status == OpenThermResponseStatus::INVALID) {
        return OT_DATA_BUS_INVALID;
    }

    const auto msg_type = OpenTherm::getMessageType(resp);
    switch (msg_type) {
    case OpenThermMessageType::READ_ACK:
        return OT_DATA_READ_ACK;
    case OpenThermMessageType::WRITE_ACK:
        return OT_DATA_WRITE_ACK;
    case OpenThermMessageType::DATA_INVALID:
        return OT_DATA_INVALID;
    case OpenThermMessageType::UNKNOWN_DATA_ID:
        return OT_DATA_UNKNOWN_ID;
    default:
        return OT_DATA_BUS_INVALID;
    }
}

extern "C" void opentherm_init(int in_pin, int out_pin)
{
    if (s_ot != nullptr) {
        delete s_ot;
    }
    s_ot = new OpenTherm(in_pin, out_pin);
    s_ot->begin();
}

extern "C" void opentherm_process(void)
{
    if (s_ot != nullptr) {
        s_ot->process();
    }
}

extern "C" bool opentherm_read_id(uint8_t id, uint16_t *data_out, ot_data_status_t *status_out)
{
    if (s_ot == nullptr || data_out == nullptr || status_out == nullptr) {
        if (status_out != nullptr) {
            *status_out = OT_DATA_NONE;
        }
        return false;
    }

    opentherm_process();
    const unsigned long request = OpenTherm::buildRequest(OpenThermMessageType::READ_DATA, id, 0);
    const unsigned long resp = s_ot->sendRequest(request);
    const ot_data_status_t status = classify_response(resp, s_ot->getLastResponseStatus());
    *status_out = status;
    *data_out = (uint16_t)(resp & 0xFFFF);

    return status == OT_DATA_READ_ACK || status == OT_DATA_INVALID;
}

extern "C" bool opentherm_write_id(uint8_t id, uint16_t data, ot_data_status_t *status_out)
{
    if (s_ot == nullptr || status_out == nullptr) {
        if (status_out != nullptr) {
            *status_out = OT_DATA_NONE;
        }
        return false;
    }

    if (!ot_catalog_is_writable(id)) {
        *status_out = OT_DATA_NONE;
        return false;
    }

    opentherm_process();
    const unsigned long request = OpenTherm::buildRequest(OpenThermMessageType::WRITE_DATA, id, data);
    const unsigned long resp = s_ot->sendRequest(request);
    const ot_data_status_t status = classify_response(resp, s_ot->getLastResponseStatus());
    *status_out = status;

    return status == OT_DATA_WRITE_ACK;
}

extern "C" bool opentherm_set_boiler_status(bool central_heating, bool hot_water)
{
    if (s_ot == nullptr) {
        return false;
    }
    opentherm_process();
    const unsigned long resp = s_ot->setBoilerStatus(central_heating, hot_water);
    return OpenTherm::isValidResponse(resp);
}

extern "C" bool opentherm_set_boiler_temperature(float celsius)
{
    ot_data_status_t status = OT_DATA_NONE;
    const uint16_t data = (uint16_t)OpenTherm::temperatureToData(celsius);
    return opentherm_write_id(1, data, &status);
}

extern "C" float opentherm_get_boiler_temperature(void)
{
    uint16_t data = 0;
    ot_data_status_t status = OT_DATA_NONE;
    if (!opentherm_read_id(25, &data, &status) || status != OT_DATA_READ_ACK) {
        return 0.0f;
    }
    return OpenTherm::getFloat(data);
}

extern "C" bool opentherm_send_status_keepalive(bool central_heating, bool hot_water)
{
    return opentherm_set_boiler_status(central_heating, hot_water);
}
