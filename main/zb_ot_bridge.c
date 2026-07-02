/*
 * Zigbee End Device: Thermostat (ep 10), discovery-driven standard clusters (ep 11–19),
 * spillover cluster skeleton (ep 20).
 */

#include "zb_ot_bridge.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_check.h"
#include "esp_log.h"

#include "ha/esp_zigbee_ha_standard.h"
#include "zcl/esp_zigbee_zcl_analog_input.h"
#include "zcl/esp_zigbee_zcl_analog_output.h"
#include "zcl/esp_zigbee_zcl_common.h"
#include "zcl/esp_zigbee_zcl_multistate_input.h"
#include "zcl/esp_zigbee_zcl_thermostat.h"
#include "zcl_utility.h"

#include "esp_zigbee_attribute.h"
#include "esp_zigbee_cluster.h"
#include "esp_zigbee_endpoint.h"

#include "opentherm_wrapper.h"
#include "ot_bridge.h"
#include "ot_data_catalog.h"
#include "ot_discover.h"
#include "ot_zcl_route.h"

#if !defined ZB_ED_ROLE
#error Define ZB_ED_ROLE in sdkconfig (CONFIG_ZB_ZED=y) to compile End Device firmware.
#endif

static const char *TAG = "ZB_OT_BRIDGE";

static bool s_network_joined = false;
static bool s_ot_precheck_done = false;
static esp_zb_ep_list_t *s_ep_list = NULL;
static uint8_t s_next_discovery_ep = OT_DISCOVERY_EP_MIN;
static uint8_t s_ot_id_to_endpoint[128];
static zb_discovery_cluster_type_t s_endpoint_cluster_type[256];

#define INSTALLCODE_POLICY_ENABLE false
#define ED_AGING_TIMEOUT ESP_ZB_ED_AGING_TIMEOUT_64MIN
#define ED_KEEP_ALIVE 3000
#define ESP_ZB_PRIMARY_CHANNEL_MASK ESP_ZB_TRANSCEIVER_ALL_CHANNELS_MASK
#define OT_MANDATORY_READ_DELAY_MS 120

#define ESP_ZB_DEFAULT_RADIO_CONFIG() \
    {                                 \
        .radio_mode = ZB_RADIO_MODE_NATIVE, \
    }

#define ESP_ZB_DEFAULT_HOST_CONFIG() \
    {                                \
        .host_connection_mode = ZB_HOST_CONNECTION_MODE_NONE, \
    }

#define ESP_MANUFACTURER_NAME "\x12""esp32-c6-opentherm"
#define ESP_MODEL_IDENTIFIER "\x0e""OT-ZB-Bridge-v2"

#define ESP_ZB_ZED_CONFIG()                                       \
    {                                                             \
        .esp_zb_role = ESP_ZB_DEVICE_TYPE_ED,                     \
        .install_code_policy = INSTALLCODE_POLICY_ENABLE,         \
        .nwk_cfg.zed_cfg = {                                      \
            .ed_timeout = ED_AGING_TIMEOUT,                       \
            .keep_alive = ED_KEEP_ALIVE,                          \
        },                                                        \
    }

static void bdb_start_top_level_commissioning_cb(uint8_t mode_mask)
{
    ESP_RETURN_ON_FALSE(esp_zb_bdb_start_top_level_commissioning(mode_mask) == ESP_OK, , TAG,
                        "Failed to start Zigbee commissioning");
}

static uint32_t application_type_for_entry(const ot_catalog_entry_t *entry)
{
    switch (entry->type) {
    case OT_TYPE_F88:
    case OT_TYPE_S16:
        return ESP_ZB_ZCL_AI_TEMPERATURE_BOILER_LEAVING;
    case OT_TYPE_U8:
        if (entry->zcl_route == OT_ROUTE_ANALOG_OUT) {
            return ESP_ZB_ZCL_AI_SET_APP_TYPE_WITH_ID(ESP_ZB_ZCL_AI_APP_TYPE_PERCENTAGE, 0);
        }
        return ESP_ZB_ZCL_AI_SET_APP_TYPE_WITH_ID(ESP_ZB_ZCL_AI_APP_TYPE_COUNT_UNITLESS, 0);
    case OT_TYPE_U16:
        return ESP_ZB_ZCL_AI_SET_APP_TYPE_WITH_ID(ESP_ZB_ZCL_AI_APP_TYPE_COUNT_UNITLESS, 0);
    default:
        return ESP_ZB_ZCL_AI_SET_APP_TYPE_WITH_ID(ESP_ZB_ZCL_AI_APP_TYPE_OTHER, 0);
    }
}

static uint16_t multistate_states_for_id(uint8_t ot_id, const ot_catalog_entry_t *entry)
{
    if (entry->type == OT_TYPE_U8 || ot_id == 4) {
        return 256;
    }
    return 65535;
}

static esp_err_t report_attr(uint8_t endpoint, uint16_t cluster_id, uint16_t attr_id)
{
    esp_zb_zcl_report_attr_cmd_t report_cmd = {
        .zcl_basic_cmd =
            {
                .src_endpoint = endpoint,
            },
        .address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
        .clusterID = cluster_id,
        .attributeID = attr_id,
    };

    esp_zb_lock_acquire(portMAX_DELAY);
    esp_err_t ret = esp_zb_zcl_report_attr_cmd_req(&report_cmd);
    esp_zb_lock_release();
    return ret;
}

uint8_t zb_ot_bridge_endpoint_for_id(uint8_t ot_id)
{
    if (ot_id >= 128) {
        return 0;
    }
    return s_ot_id_to_endpoint[ot_id];
}

uint8_t zb_ot_bridge_id_for_endpoint(uint8_t endpoint)
{
    for (uint8_t id = 0; id < 128; id++) {
        if (s_ot_id_to_endpoint[id] == endpoint) {
            return id;
        }
    }
    return 0xFF;
}

void esp_zb_app_signal_handler(esp_zb_app_signal_t *signal_struct)
{
    uint32_t *p_sg_p = signal_struct->p_app_signal;
    esp_err_t err_status = signal_struct->esp_err_status;
    esp_zb_app_signal_type_t sig_type = *p_sg_p;

    switch (sig_type) {
    case ESP_ZB_ZDO_SIGNAL_SKIP_STARTUP:
        ESP_LOGI(TAG, "Initialize Zigbee stack");
        esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_INITIALIZATION);
        break;
    case ESP_ZB_BDB_SIGNAL_DEVICE_FIRST_START:
    case ESP_ZB_BDB_SIGNAL_DEVICE_REBOOT:
        if (err_status == ESP_OK) {
            ESP_LOGI(TAG, "Device started up in%s factory-reset mode", esp_zb_bdb_is_factory_new() ? "" : " non");
            if (esp_zb_bdb_is_factory_new()) {
                ESP_LOGI(TAG, "Start network steering");
                esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_NETWORK_STEERING);
            } else {
                ESP_LOGI(TAG, "Device rebooted");
                s_network_joined = true;
            }
        } else {
            ESP_LOGW(TAG, "Failed to initialize Zigbee stack (status: %s)", esp_err_to_name(err_status));
        }
        break;
    case ESP_ZB_BDB_SIGNAL_STEERING:
        if (err_status == ESP_OK) {
            esp_zb_ieee_addr_t extended_pan_id;
            esp_zb_get_extended_pan_id(extended_pan_id);
            ESP_LOGI(TAG,
                     "Joined network (Extended PAN ID: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x, PAN ID: 0x%04hx, "
                     "Channel:%d, Short Address: 0x%04hx)",
                     extended_pan_id[7], extended_pan_id[6], extended_pan_id[5], extended_pan_id[4], extended_pan_id[3],
                     extended_pan_id[2], extended_pan_id[1], extended_pan_id[0], esp_zb_get_pan_id(),
                     esp_zb_get_current_channel(), esp_zb_get_short_address());
            s_network_joined = true;
        } else {
            ESP_LOGI(TAG, "Network steering was not successful (status: %s)", esp_err_to_name(err_status));
            esp_zb_scheduler_alarm((esp_zb_callback_t)bdb_start_top_level_commissioning_cb,
                                 ESP_ZB_BDB_MODE_NETWORK_STEERING, 1000);
        }
        break;
    default:
        ESP_LOGD(TAG, "ZDO signal: %s (0x%x), status: %s", esp_zb_zdo_signal_to_string(sig_type), sig_type,
                 esp_err_to_name(err_status));
        break;
    }
}

static esp_err_t zb_thermostat_attribute_handler(const esp_zb_zcl_set_attr_value_message_t *message)
{
    ESP_RETURN_ON_FALSE(message, ESP_FAIL, TAG, "Empty message");
    ESP_RETURN_ON_FALSE(message->info.status == ESP_ZB_ZCL_STATUS_SUCCESS, ESP_ERR_INVALID_ARG, TAG,
                        "Attribute write error status(%d)", message->info.status);

    if (message->info.dst_endpoint != HA_OT_BRIDGE_ENDPOINT) {
        return ESP_OK;
    }
    if (message->info.cluster != ESP_ZB_ZCL_CLUSTER_ID_THERMOSTAT) {
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Thermostat write: attribute(0x%x), size(%d)", message->attribute.id, message->attribute.data.size);

    ot_data_status_t ot_status = OT_DATA_NONE;
    ot_zcl_write_t write = {0};
    esp_err_t err = ESP_OK;
    uint8_t ot_id = 0xFF;

    switch (message->attribute.id) {
    case ESP_ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID:
        if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_S16 && message->attribute.data.value) {
            write.fmt = OT_ZCL_WRITE_CENTI;
            write.v.centi = *(int16_t *)message->attribute.data.value;
            ot_id = 1;
            err = ot_zcl_route_apply_write(1, &write, &ot_status);
        }
        break;
    case ESP_ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID:
        if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_U8 && message->attribute.data.value) {
            err = ot_bridge_on_system_mode(*(uint8_t *)message->attribute.data.value);
        }
        break;
    case ESP_ZB_ZCL_ATTR_THERMOSTAT_PI_COOLING_DEMAND_ID:
        if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_U8 && message->attribute.data.value) {
            write.fmt = OT_ZCL_WRITE_U8;
            write.v.u8 = *(uint8_t *)message->attribute.data.value;
            ot_id = 7;
            err = ot_zcl_route_apply_write(7, &write, &ot_status);
        }
        break;
    case ESP_ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_HEATING_SETPOINT_ID:
        if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_S16 && message->attribute.data.value) {
            write.fmt = OT_ZCL_WRITE_CENTI;
            write.v.centi = *(int16_t *)message->attribute.data.value;
            ot_id = 8;
            err = ot_zcl_route_apply_write(8, &write, &ot_status);
        }
        break;
    case ESP_ZB_ZCL_ATTR_THERMOSTAT_MAX_HEAT_SETPOINT_LIMIT_ID:
        if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_S16 && message->attribute.data.value) {
            write.fmt = OT_ZCL_WRITE_CENTI;
            write.v.centi = *(int16_t *)message->attribute.data.value;
            ot_id = 57;
            err = ot_zcl_route_apply_write(57, &write, &ot_status);
        }
        break;
    case ESP_ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID:
    case ESP_ZB_ZCL_ATTR_THERMOSTAT_THERMOSTAT_RUNNING_STATE_ID:
        return ESP_ERR_NOT_SUPPORTED;
    default:
        break;
    }

    if (ot_id != 0xFF && err == ESP_OK) {
        ot_poll_promote(ot_id);
    }
    return err;
}

static esp_err_t zb_discovery_attribute_handler(const esp_zb_zcl_set_attr_value_message_t *message)
{
    ESP_RETURN_ON_FALSE(message, ESP_FAIL, TAG, "Empty message");
    ESP_RETURN_ON_FALSE(message->info.status == ESP_ZB_ZCL_STATUS_SUCCESS, ESP_ERR_INVALID_ARG, TAG,
                        "Attribute write error status(%d)", message->info.status);

    const uint8_t endpoint = message->info.dst_endpoint;
    if (endpoint < OT_DISCOVERY_EP_MIN || endpoint > OT_DISCOVERY_EP_MAX) {
        return ESP_OK;
    }

    const uint8_t ot_id = zb_ot_bridge_id_for_endpoint(endpoint);
    if (ot_id == 0xFF) {
        return ESP_ERR_NOT_FOUND;
    }

    const ot_catalog_entry_t *entry = ot_data_catalog_get(ot_id);
    if (entry == NULL) {
        return ESP_ERR_NOT_FOUND;
    }

    if (message->attribute.id != ESP_ZB_ZCL_ATTR_ANALOG_OUTPUT_PRESENT_VALUE_ID &&
        message->attribute.id != ESP_ZB_ZCL_ATTR_MULTI_INPUT_PRESENT_VALUE_ID) {
        return ESP_OK;
    }

    if (entry->zcl_route == OT_ROUTE_ANALOG_IN) {
        return ESP_ERR_NOT_SUPPORTED;
    }
    if (!entry->writable) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    ot_data_status_t ot_status = OT_DATA_NONE;
    ot_zcl_write_t write = {0};
    esp_err_t err = ESP_ERR_INVALID_ARG;

    if (message->info.cluster == ESP_ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT &&
        message->attribute.id == ESP_ZB_ZCL_ATTR_ANALOG_OUTPUT_PRESENT_VALUE_ID &&
        message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_SINGLE &&
        message->attribute.data.value) {
        write.fmt = OT_ZCL_WRITE_FLOAT;
        write.v.f = *(float *)message->attribute.data.value;
        err = ot_zcl_route_apply_write(ot_id, &write, &ot_status);
    } else if (message->info.cluster == ESP_ZB_ZCL_CLUSTER_ID_MULTI_INPUT &&
               message->attribute.id == ESP_ZB_ZCL_ATTR_MULTI_INPUT_PRESENT_VALUE_ID &&
               message->attribute.data.value) {
        if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_U16) {
            write.fmt = OT_ZCL_WRITE_U16;
            write.v.u16 = *(uint16_t *)message->attribute.data.value;
            err = ot_zcl_route_apply_write(ot_id, &write, &ot_status);
        } else if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_U8) {
            write.fmt = OT_ZCL_WRITE_U8;
            write.v.u8 = *(uint8_t *)message->attribute.data.value;
            err = ot_zcl_route_apply_write(ot_id, &write, &ot_status);
        } else if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_SINGLE) {
            write.fmt = OT_ZCL_WRITE_FLOAT;
            write.v.f = *(float *)message->attribute.data.value;
            err = ot_zcl_route_apply_write(ot_id, &write, &ot_status);
        }
    }

    if (err == ESP_OK) {
        ot_poll_promote(ot_id);
    }
    return err;
}

static esp_err_t zb_action_handler(esp_zb_core_action_callback_id_t callback_id, const void *message)
{
    if (callback_id == ESP_ZB_CORE_SET_ATTR_VALUE_CB_ID) {
        const esp_zb_zcl_set_attr_value_message_t *msg = message;
        esp_err_t thermo_err = zb_thermostat_attribute_handler(msg);
        if (thermo_err != ESP_OK) {
            return thermo_err;
        }
        return zb_discovery_attribute_handler(msg);
    }
    ESP_LOGW(TAG, "Unhandled Zigbee action(0x%x)", callback_id);
    return ESP_OK;
}

static esp_zb_basic_cluster_cfg_t default_basic_cfg(void)
{
    return (esp_zb_basic_cluster_cfg_t){
        .zcl_version = ESP_ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE,
        .power_source = ESP_ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE,
    };
}

static esp_zb_identify_cluster_cfg_t default_identify_cfg(void)
{
    return (esp_zb_identify_cluster_cfg_t){
        .identify_time = ESP_ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE,
    };
}

static esp_zb_cluster_list_t *create_discovery_cluster_list(uint8_t ot_id, zb_discovery_cluster_type_t cluster_type)
{
    const ot_catalog_entry_t *entry = ot_data_catalog_get(ot_id);
    if (entry == NULL) {
        return NULL;
    }

    esp_zb_cluster_list_t *cluster_list = esp_zb_zcl_cluster_list_create();
    if (cluster_list == NULL) {
        return NULL;
    }

    esp_zb_basic_cluster_cfg_t basic_cfg = default_basic_cfg();
    esp_zb_attribute_list_t *basic_cluster = esp_zb_basic_cluster_create(&basic_cfg);
    ESP_ERROR_CHECK(esp_zb_cluster_list_add_basic_cluster(cluster_list, basic_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE));

    esp_zb_identify_cluster_cfg_t identify_cfg = default_identify_cfg();
    esp_zb_attribute_list_t *identify_cluster = esp_zb_identify_cluster_create(&identify_cfg);
    ESP_ERROR_CHECK(esp_zb_cluster_list_add_identify_cluster(cluster_list, identify_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE));

    switch (cluster_type) {
    case ZB_CLUSTER_ANALOG_INPUT: {
        esp_zb_analog_input_cluster_cfg_t cfg = {
            .out_of_service = false,
            .present_value = 0.0f,
            .status_flags = ESP_ZB_ZCL_ANALOG_INPUT_STATUS_FLAG_NORMAL,
        };
        esp_zb_attribute_list_t *cluster = esp_zb_analog_input_cluster_create(&cfg);
        uint32_t app_type = application_type_for_entry(entry);
        ESP_ERROR_CHECK(esp_zb_analog_input_cluster_add_attr(cluster, ESP_ZB_ZCL_ATTR_ANALOG_INPUT_APPLICATION_TYPE_ID,
                                                             &app_type));
        ESP_ERROR_CHECK(esp_zb_cluster_list_add_analog_input_cluster(cluster_list, cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE));
        break;
    }
    case ZB_CLUSTER_ANALOG_OUTPUT: {
        esp_zb_analog_output_cluster_cfg_t cfg = {
            .out_of_service = false,
            .present_value = 0.0f,
            .status_flags = ESP_ZB_ZCL_ANALOG_INPUT_STATUS_FLAG_NORMAL,
        };
        esp_zb_attribute_list_t *cluster = esp_zb_analog_output_cluster_create(&cfg);
        uint32_t app_type = application_type_for_entry(entry);
        ESP_ERROR_CHECK(esp_zb_analog_output_cluster_add_attr(cluster, ESP_ZB_ZCL_ATTR_ANALOG_OUTPUT_APPLICATION_TYPE_ID,
                                                              &app_type));
        ESP_ERROR_CHECK(esp_zb_cluster_list_add_analog_output_cluster(cluster_list, cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE));
        break;
    }
    case ZB_CLUSTER_MULTISTATE_INPUT: {
        const uint16_t states = multistate_states_for_id(ot_id, entry);
        esp_zb_multistate_input_cluster_cfg_t cfg = {
            .number_of_states = states,
            .out_of_service = false,
            .present_value = 0.0f,
            .status_flags = ESP_ZB_ZCL_ANALOG_INPUT_STATUS_FLAG_NORMAL,
        };
        esp_zb_attribute_list_t *cluster = esp_zb_multistate_input_cluster_create(&cfg);
        ESP_ERROR_CHECK(esp_zb_cluster_list_add_multistate_input_cluster(cluster_list, cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE));
        break;
    }
    default:
        return NULL;
    }

    return cluster_list;
}

static uint16_t discovery_device_id(zb_discovery_cluster_type_t cluster_type)
{
    if (cluster_type == ZB_CLUSTER_ANALOG_OUTPUT) {
        return ESP_ZB_HA_LEVEL_CONTROLLABLE_OUTPUT_DEVICE_ID;
    }
    return ESP_ZB_HA_SIMPLE_SENSOR_DEVICE_ID;
}

esp_err_t zb_add_discovery_endpoint(uint8_t ot_id, zb_discovery_cluster_type_t cluster_type)
{
    ESP_RETURN_ON_FALSE(s_ep_list != NULL, ESP_ERR_INVALID_STATE, TAG, "Endpoint list not initialized");

    if (s_next_discovery_ep > OT_DISCOVERY_EP_MAX) {
        ESP_LOGW(TAG, "discovery endpoint pool full, skip ot_id=%u", ot_id);
        return ESP_ERR_NO_MEM;
    }

    esp_zb_cluster_list_t *cluster_list = create_discovery_cluster_list(ot_id, cluster_type);
    ESP_RETURN_ON_FALSE(cluster_list != NULL, ESP_FAIL, TAG, "Failed to create discovery cluster list");

    const uint8_t endpoint_id = s_next_discovery_ep++;
    esp_zb_endpoint_config_t endpoint_config = {
        .endpoint = endpoint_id,
        .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID,
        .app_device_id = discovery_device_id(cluster_type),
        .app_device_version = 1,
    };

    ESP_RETURN_ON_ERROR(esp_zb_ep_list_add_ep(s_ep_list, cluster_list, endpoint_config), TAG,
                        "Failed to add discovery endpoint %u for ot_id=%u", endpoint_id, ot_id);

    s_ot_id_to_endpoint[ot_id] = endpoint_id;
    s_endpoint_cluster_type[endpoint_id] = cluster_type;

    ESP_LOGI(TAG, "discovery endpoint %u ot_id=%u cluster=%d", endpoint_id, ot_id, (int)cluster_type);
    return ESP_OK;
}

static zb_discovery_cluster_type_t route_to_cluster_type(ot_zcl_route_t route)
{
    switch (route) {
    case OT_ROUTE_ANALOG_IN:
        return ZB_CLUSTER_ANALOG_INPUT;
    case OT_ROUTE_ANALOG_OUT:
        return ZB_CLUSTER_ANALOG_OUTPUT;
    case OT_ROUTE_MULTISTATE:
        return ZB_CLUSTER_MULTISTATE_INPUT;
    default:
        return (zb_discovery_cluster_type_t)-1;
    }
}

static bool is_discovery_route(ot_zcl_route_t route)
{
    return route == OT_ROUTE_ANALOG_IN || route == OT_ROUTE_ANALOG_OUT || route == OT_ROUTE_MULTISTATE;
}

static void register_discovery_endpoints(const ot_runtime_catalog_t *catalog)
{
    for (uint8_t i = 0; i < catalog->count; i++) {
        const uint8_t id = catalog->ids[i];
        const ot_catalog_entry_t *entry = ot_data_catalog_get(id);
        if (entry == NULL || !is_discovery_route(entry->zcl_route)) {
            continue;
        }

        const zb_discovery_cluster_type_t cluster_type = route_to_cluster_type(entry->zcl_route);
        if (zb_add_discovery_endpoint(id, cluster_type) != ESP_OK) {
            ESP_LOGW(TAG, "skip discovery endpoint for ot_id=%u", id);
        }
    }
}

static void register_thermostat_attrs(const ot_runtime_catalog_t *catalog)
{
    esp_zb_cluster_list_t *cluster_list = esp_zb_ep_list_get_ep(s_ep_list, HA_OT_BRIDGE_ENDPOINT);
    if (cluster_list == NULL) {
        ESP_LOGW(TAG, "thermostat endpoint missing");
        return;
    }

    esp_zb_attribute_list_t *thermostat_cluster =
        esp_zb_cluster_list_get_cluster(cluster_list, ESP_ZB_ZCL_CLUSTER_ID_THERMOSTAT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    if (thermostat_cluster == NULL) {
        ESP_LOGW(TAG, "thermostat cluster missing");
        return;
    }

    uint8_t pi_demand = 0;
    int16_t unoccupied = 2000;
    int16_t max_heat = 3000;
    uint16_t running_state = 0;

    for (uint8_t i = 0; i < catalog->count; i++) {
        const uint8_t id = catalog->ids[i];
        switch (id) {
        case 0:
            ESP_ERROR_CHECK(esp_zb_thermostat_cluster_add_attr(
                thermostat_cluster, ESP_ZB_ZCL_ATTR_THERMOSTAT_THERMOSTAT_RUNNING_STATE_ID, &running_state));
            break;
        case 7:
            ESP_ERROR_CHECK(esp_zb_thermostat_cluster_add_attr(
                thermostat_cluster, ESP_ZB_ZCL_ATTR_THERMOSTAT_PI_COOLING_DEMAND_ID, &pi_demand));
            break;
        case 8:
            ESP_ERROR_CHECK(esp_zb_thermostat_cluster_add_attr(
                thermostat_cluster, ESP_ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_HEATING_SETPOINT_ID, &unoccupied));
            break;
        case 57:
            ESP_ERROR_CHECK(esp_zb_thermostat_cluster_add_attr(
                thermostat_cluster, ESP_ZB_ZCL_ATTR_THERMOSTAT_MAX_HEAT_SETPOINT_LIMIT_ID, &max_heat));
            break;
        default:
            break;
        }
    }
}

static esp_zb_cluster_list_t *create_spillover_cluster_list(void)
{
    esp_zb_cluster_list_t *cluster_list = esp_zb_zcl_cluster_list_create();
    if (cluster_list == NULL) {
        return NULL;
    }

    esp_zb_basic_cluster_cfg_t basic_cfg = default_basic_cfg();
    esp_zb_attribute_list_t *basic_cluster = esp_zb_basic_cluster_create(&basic_cfg);
    ESP_ERROR_CHECK(esp_zb_cluster_list_add_basic_cluster(cluster_list, basic_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE));

    esp_zb_attribute_list_t *spillover_cluster = esp_zb_zcl_attr_list_create(OT_SPILLOVER_CLUSTER_ID);
    ESP_ERROR_CHECK(esp_zb_cluster_list_add_custom_cluster(cluster_list, spillover_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE));

    return cluster_list;
}

static esp_err_t add_spillover_endpoint(void)
{
    esp_zb_cluster_list_t *cluster_list = create_spillover_cluster_list();
    ESP_RETURN_ON_FALSE(cluster_list != NULL, ESP_FAIL, TAG, "Failed to create spillover cluster list");

    esp_zb_endpoint_config_t endpoint_config = {
        .endpoint = OT_SPILLOVER_ENDPOINT,
        .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID,
        .app_device_id = ESP_ZB_HA_CONFIGURATION_TOOL_DEVICE_ID,
        .app_device_version = 1,
    };

    ESP_RETURN_ON_ERROR(esp_zb_ep_list_add_ep(s_ep_list, cluster_list, endpoint_config), TAG,
                        "Failed to add spillover endpoint %u", OT_SPILLOVER_ENDPOINT);

    zcl_basic_manufacturer_info_t info = {
        .manufacturer_name = ESP_MANUFACTURER_NAME,
        .model_identifier = ESP_MODEL_IDENTIFIER,
    };
    ESP_RETURN_ON_ERROR(
        esp_zcl_utility_add_ep_basic_manufacturer_info(s_ep_list, OT_SPILLOVER_ENDPOINT, &info), TAG,
        "Failed to add spillover basic manufacturer info");

    ESP_LOGI(TAG, "spillover endpoint %u cluster 0x%04x", OT_SPILLOVER_ENDPOINT, OT_SPILLOVER_CLUSTER_ID);
    return ESP_OK;
}

static void wait_for_catalog_ready(void)
{
    while (!ot_catalog_is_validated()) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    ESP_LOGI(TAG, "catalog ready count=%u", ot_catalog_get()->count);
}

static void read_mandatory_ot_ids(void)
{
    uint16_t data = 0;
    ot_data_status_t status = OT_DATA_NONE;

    vTaskDelay(pdMS_TO_TICKS(OT_MANDATORY_READ_DELAY_MS));
    if (opentherm_read_id(0, &data, &status)) {
        ESP_LOGI(TAG, "mandatory read id=0 data=0x%04x status=%d", data, (int)status);
    } else {
        ESP_LOGW(TAG, "mandatory read id=0 failed status=%d", (int)status);
    }

    vTaskDelay(pdMS_TO_TICKS(OT_MANDATORY_READ_DELAY_MS));
    if (opentherm_read_id(3, &data, &status)) {
        ESP_LOGI(TAG, "mandatory read id=3 data=0x%04x status=%d", data, (int)status);
    } else {
        ESP_LOGW(TAG, "mandatory read id=3 failed status=%d", (int)status);
    }

    s_ot_precheck_done = true;
}

static void esp_zb_task(void *pvParameters)
{
    (void)pvParameters;

    wait_for_catalog_ready();
    read_mandatory_ot_ids();

    esp_zb_cfg_t zb_nwk_cfg = ESP_ZB_ZED_CONFIG();
    esp_zb_init(&zb_nwk_cfg);

    esp_zb_thermostat_cfg_t thermostat_cfg = ESP_ZB_DEFAULT_THERMOSTAT_CONFIG();
    thermostat_cfg.thermostat_cfg.occupied_heating_setpoint = 6000;
    thermostat_cfg.thermostat_cfg.system_mode = ESP_ZB_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT;

    s_ep_list = esp_zb_thermostat_ep_create(HA_OT_BRIDGE_ENDPOINT, &thermostat_cfg);
    s_next_discovery_ep = OT_DISCOVERY_EP_MIN;
    for (uint8_t i = 0; i < 128; i++) {
        s_ot_id_to_endpoint[i] = 0;
    }

    zcl_basic_manufacturer_info_t info = {
        .manufacturer_name = ESP_MANUFACTURER_NAME,
        .model_identifier = ESP_MODEL_IDENTIFIER,
    };
    esp_zcl_utility_add_ep_basic_manufacturer_info(s_ep_list, HA_OT_BRIDGE_ENDPOINT, &info);

    const ot_runtime_catalog_t *catalog = ot_catalog_get();
    register_thermostat_attrs(catalog);
    register_discovery_endpoints(catalog);
    ESP_ERROR_CHECK(add_spillover_endpoint());

    esp_zb_device_register(s_ep_list);
    esp_zb_core_action_handler_register(zb_action_handler);
    esp_zb_set_primary_network_channel_set(ESP_ZB_PRIMARY_CHANNEL_MASK);

    ESP_LOGI(TAG, "starting Zigbee stack");
    ESP_ERROR_CHECK(esp_zb_start(false));
    esp_zb_stack_main_loop();
}

bool zb_ot_bridge_is_joined(void)
{
    return s_network_joined;
}

bool zb_ot_bridge_ot_precheck_done(void)
{
    return s_ot_precheck_done;
}

esp_err_t zb_ot_bridge_set_local_temperature(int16_t temp_centi_c)
{
    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_status_t status = esp_zb_zcl_set_attribute_val(
        HA_OT_BRIDGE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_THERMOSTAT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        ESP_ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID, &temp_centi_c, false);
    esp_zb_lock_release();

    return (status == ESP_ZB_ZCL_STATUS_SUCCESS) ? ESP_OK : ESP_FAIL;
}

esp_err_t zb_ot_bridge_report_local_temperature(int16_t temp_centi_c)
{
    ESP_RETURN_ON_ERROR(zb_ot_bridge_set_local_temperature(temp_centi_c), TAG, "Failed to set local temperature");
    return report_attr(HA_OT_BRIDGE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                       ESP_ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID);
}

esp_err_t zb_ot_bridge_set_report_thermostat_attr(uint16_t attr_id, void *value)
{
    if (!zb_ot_bridge_is_joined()) {
        esp_zb_lock_acquire(portMAX_DELAY);
        esp_zb_zcl_set_attribute_val(HA_OT_BRIDGE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                                     ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, attr_id, value, false);
        esp_zb_lock_release();
        return ESP_OK;
    }

    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_status_t status = esp_zb_zcl_set_attribute_val(
        HA_OT_BRIDGE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_THERMOSTAT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, attr_id, value, false);
    esp_zb_lock_release();
    ESP_RETURN_ON_FALSE(status == ESP_ZB_ZCL_STATUS_SUCCESS, ESP_FAIL, TAG, "set thermostat attr 0x%04x failed", attr_id);

    return report_attr(HA_OT_BRIDGE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_THERMOSTAT, attr_id);
}

esp_err_t zb_ot_bridge_report_analog_input(uint8_t endpoint, float present_value, bool valid)
{
    uint8_t reliability = valid ? ESP_ZB_ZCL_ANALOG_INPUT_RELIABILITY_NO_FAULT_DETECTED
                                : ESP_ZB_ZCL_ANALOG_INPUT_RELIABILITY_NO_SENSOR;
    float value = present_value;

    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_set_attribute_val(endpoint, ESP_ZB_ZCL_CLUSTER_ID_ANALOG_INPUT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
                                 ESP_ZB_ZCL_ATTR_ANALOG_INPUT_PRESENT_VALUE_ID, &value, false);
    esp_zb_zcl_set_attribute_val(endpoint, ESP_ZB_ZCL_CLUSTER_ID_ANALOG_INPUT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
                                 ESP_ZB_ZCL_ATTR_ANALOG_INPUT_RELIABILITY_ID, &reliability, false);
    esp_zb_lock_release();

    if (!zb_ot_bridge_is_joined()) {
        return ESP_OK;
    }

    report_attr(endpoint, ESP_ZB_ZCL_CLUSTER_ID_ANALOG_INPUT, ESP_ZB_ZCL_ATTR_ANALOG_INPUT_PRESENT_VALUE_ID);
    return report_attr(endpoint, ESP_ZB_ZCL_CLUSTER_ID_ANALOG_INPUT, ESP_ZB_ZCL_ATTR_ANALOG_INPUT_RELIABILITY_ID);
}

esp_err_t zb_ot_bridge_report_analog_output(uint8_t endpoint, float present_value, bool valid)
{
    float value = valid ? present_value : 0.0f;

    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_set_attribute_val(endpoint, ESP_ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
                                 ESP_ZB_ZCL_ATTR_ANALOG_OUTPUT_PRESENT_VALUE_ID, &value, false);
    esp_zb_lock_release();

    if (!zb_ot_bridge_is_joined()) {
        return ESP_OK;
    }

    return report_attr(endpoint, ESP_ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT, ESP_ZB_ZCL_ATTR_ANALOG_OUTPUT_PRESENT_VALUE_ID);
}

esp_err_t zb_ot_bridge_report_multistate(uint8_t endpoint, uint16_t state, bool valid)
{
    uint8_t reliability = valid ? ESP_ZB_ZCL_MULTI_INPUT_RELIABILITY_NO_FAULT_DETECTED
                                : ESP_ZB_ZCL_MULTI_INPUT_RELIABILITY_NO_SENSOR;
    float present = (float)state;

    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_set_attribute_val(endpoint, ESP_ZB_ZCL_CLUSTER_ID_MULTI_INPUT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
                                 ESP_ZB_ZCL_ATTR_MULTI_INPUT_PRESENT_VALUE_ID, &present, false);
    esp_zb_zcl_set_attribute_val(endpoint, ESP_ZB_ZCL_CLUSTER_ID_MULTI_INPUT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
                                 ESP_ZB_ZCL_ATTR_MULTI_INPUT_RELIABILITY_ID, &reliability, false);
    esp_zb_lock_release();

    if (!zb_ot_bridge_is_joined()) {
        return ESP_OK;
    }

    report_attr(endpoint, ESP_ZB_ZCL_CLUSTER_ID_MULTI_INPUT, ESP_ZB_ZCL_ATTR_MULTI_INPUT_PRESENT_VALUE_ID);
    return report_attr(endpoint, ESP_ZB_ZCL_CLUSTER_ID_MULTI_INPUT, ESP_ZB_ZCL_ATTR_MULTI_INPUT_RELIABILITY_ID);
}

void zb_ot_bridge_start(void)
{
    esp_zb_platform_config_t config = {
        .radio_config = ESP_ZB_DEFAULT_RADIO_CONFIG(),
        .host_config = ESP_ZB_DEFAULT_HOST_CONFIG(),
    };

    ESP_ERROR_CHECK(esp_zb_platform_config(&config));
    xTaskCreate(esp_zb_task, "Zigbee_main", 4096, NULL, 5, NULL);
}
