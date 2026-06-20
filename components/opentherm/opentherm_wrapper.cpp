#include "opentherm_wrapper.h"

#include "OpenTherm.h"

static OpenTherm *s_ot = nullptr;

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

extern "C" bool opentherm_set_boiler_status(bool central_heating, bool hot_water)
{
    if (s_ot == nullptr) {
        return false;
    }
    opentherm_process();
    unsigned long resp = s_ot->setBoilerStatus(central_heating, hot_water);
    return OpenTherm::isValidResponse(resp);
}

extern "C" bool opentherm_set_boiler_temperature(float celsius)
{
    if (s_ot == nullptr) {
        return false;
    }
    opentherm_process();
    return s_ot->setBoilerTemperature(celsius);
}

extern "C" float opentherm_get_boiler_temperature(void)
{
    if (s_ot == nullptr) {
        return 0.0f;
    }
    opentherm_process();
    return s_ot->getBoilerTemperature();
}

extern "C" bool opentherm_send_status_keepalive(bool central_heating, bool hot_water)
{
    return opentherm_set_boiler_status(central_heating, hot_water);
}
