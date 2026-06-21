#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

#define HA_OT_BRIDGE_ENDPOINT 10

void zb_thermostat_ed_start(void);
bool zb_thermostat_ed_is_joined(void);
esp_err_t zb_thermostat_ed_set_local_temperature(int16_t temp_centi_c);
esp_err_t zb_thermostat_ed_report_local_temperature(int16_t temp_centi_c);
