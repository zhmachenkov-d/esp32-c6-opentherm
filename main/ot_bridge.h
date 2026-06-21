#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "esp_err.h"

void ot_bridge_init(void);
void ot_bridge_start(void);
esp_err_t ot_bridge_on_heating_setpoint(int16_t temp_centi_c);
esp_err_t ot_bridge_on_system_mode(uint8_t mode);
