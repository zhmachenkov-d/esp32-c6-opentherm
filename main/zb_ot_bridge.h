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
uint8_t zb_ot_bridge_endpoint_for_id(uint8_t ot_id);
uint8_t zb_ot_bridge_id_for_endpoint(uint8_t endpoint);
esp_err_t zb_ot_bridge_set_local_temperature(int16_t temp_centi_c);
esp_err_t zb_ot_bridge_report_local_temperature(int16_t temp_centi_c);
esp_err_t zb_ot_bridge_set_report_thermostat_attr(uint16_t attr_id, void *value);
esp_err_t zb_ot_bridge_report_analog_input(uint8_t endpoint, float present_value, bool valid);
esp_err_t zb_ot_bridge_report_analog_output(uint8_t endpoint, float present_value, bool valid);
esp_err_t zb_ot_bridge_report_multistate(uint8_t endpoint, uint16_t state, bool valid);
esp_err_t zb_add_discovery_endpoint(uint8_t ot_id, zb_discovery_cluster_type_t cluster_type);
