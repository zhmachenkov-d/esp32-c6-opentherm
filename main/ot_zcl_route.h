#pragma once

#include <stdint.h>

#include "esp_err.h"
#include "opentherm_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OT_ZCL_WRITE_RAW,
    OT_ZCL_WRITE_CENTI,
    OT_ZCL_WRITE_U8,
    OT_ZCL_WRITE_FLOAT,
    OT_ZCL_WRITE_U16,
} ot_zcl_write_fmt_t;

typedef struct {
    ot_zcl_write_fmt_t fmt;
    union {
        uint16_t raw;
        int16_t centi;
        uint8_t u8;
        float f;
        uint16_t u16;
    } v;
} ot_zcl_write_t;

esp_err_t ot_zcl_route_apply_read(uint8_t id, uint16_t raw, ot_data_status_t status);
esp_err_t ot_zcl_route_apply_write(uint8_t id, const ot_zcl_write_t *write, ot_data_status_t *status_out);

#ifdef __cplusplus
}
#endif
