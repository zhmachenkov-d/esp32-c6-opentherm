#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

#define HA_OT_BRIDGE_ENDPOINT 10
#define OT_SPILLOVER_ENDPOINT 20
#define OT_DISCOVERY_EP_MIN 11
#define OT_DISCOVERY_EP_MAX 19
#define OT_SPILLOVER_CLUSTER_ID 0xFC01

typedef enum {
    ZB_CLUSTER_ANALOG_INPUT,
    ZB_CLUSTER_ANALOG_OUTPUT,
    ZB_CLUSTER_MULTISTATE_INPUT,
} zb_discovery_cluster_type_t;

void zb_ot_bridge_start(void);
bool zb_ot_bridge_is_joined(void);
bool zb_ot_bridge_ot_precheck_done(void);
esp_err_t zb_ot_bridge_set_local_temperature(int16_t temp_centi_c);
esp_err_t zb_ot_bridge_report_local_temperature(int16_t temp_centi_c);
esp_err_t zb_add_discovery_endpoint(uint8_t ot_id, zb_discovery_cluster_type_t cluster_type);
