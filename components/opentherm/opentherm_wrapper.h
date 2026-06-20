#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void opentherm_init(int in_pin, int out_pin);
void opentherm_process(void);
bool opentherm_set_boiler_status(bool central_heating, bool hot_water);
bool opentherm_set_boiler_temperature(float celsius);
float opentherm_get_boiler_temperature(void);
bool opentherm_send_status_keepalive(bool central_heating, bool hot_water);

#ifdef __cplusplus
}
#endif
