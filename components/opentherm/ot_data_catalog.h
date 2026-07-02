#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OT_TYPE_F88,
    OT_TYPE_FLAG8,
    OT_TYPE_U8,
    OT_TYPE_U16,
    OT_TYPE_S16,
    OT_TYPE_SPECIAL,
    OT_TYPE_MIXED,
} ot_value_type_t;

typedef enum {
    OT_POLL_FAST,
    OT_POLL_SLOW,
} ot_poll_tier_t;

typedef enum {
    OT_ROUTE_NONE,
    OT_ROUTE_THERMOSTAT,
    OT_ROUTE_ANALOG_IN,
    OT_ROUTE_ANALOG_OUT,
    OT_ROUTE_MULTISTATE,
    OT_ROUTE_SPILLOVER,
} ot_zcl_route_t;

typedef struct {
    uint8_t id;
    ot_value_type_t type;
    ot_poll_tier_t poll_tier;
    bool writable;
    ot_zcl_route_t zcl_route;
} ot_catalog_entry_t;

extern const ot_catalog_entry_t ot_data_catalog[128];

const ot_catalog_entry_t *ot_data_catalog_get(uint8_t id);
bool ot_catalog_is_writable(uint8_t id);

#ifdef __cplusplus
}
#endif
