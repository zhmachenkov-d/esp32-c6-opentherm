#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "opentherm_wrapper.h"
#include "ot_data_catalog.h"

#ifdef __cplusplus
extern "C" {
#endif

float ot_f88_to_float(uint16_t raw);
uint16_t ot_float_to_f88(float celsius);
int16_t ot_f88_to_centi_i16(uint16_t raw);
uint16_t ot_centi_i16_to_f88(int16_t centi);
bool ot_flag8_get(uint16_t raw, uint8_t bit);
uint16_t ot_u16_get(uint16_t raw);
bool ot_data_is_invalid(ot_data_status_t status);
int16_t ot_zcl_invalid_sentinel(ot_value_type_t type);

#ifdef __cplusplus
}
#endif
