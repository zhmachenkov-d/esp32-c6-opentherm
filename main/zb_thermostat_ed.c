/*
 * Zigbee End Device with HA Thermostat cluster server.
 * Join pattern from ESP-IDF HA_on_off_light; cluster from esp-zigbee-lib HA standard.
 */

#include "zb_thermostat_ed.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_check.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "ha/esp_zigbee_ha_standard.h"
#include "zcl_utility.h"

#include "ot_bridge.h"

#if !defined ZB_ED_ROLE
#error Define ZB_ED_ROLE in sdkconfig (CONFIG_ZB_ZED=y) to compile End Device firmware.
#endif

static const char *TAG = "ZB_THERMOSTAT_ED";

static bool s_network_joined = false;

#define INSTALLCODE_POLICY_ENABLE false
#define ED_AGING_TIMEOUT ESP_ZB_ED_AGING_TIMEOUT_64MIN
#define ED_KEEP_ALIVE 3000
#define ESP_ZB_PRIMARY_CHANNEL_MASK ESP_ZB_TRANSCEIVER_ALL_CHANNELS_MASK

#define ESP_ZB_DEFAULT_RADIO_CONFIG() \
    {                                 \
        .radio_mode = ZB_RADIO_MODE_NATIVE, \
    }

#define ESP_ZB_DEFAULT_HOST_CONFIG() \
    {                                \
        .host_connection_mode = ZB_HOST_CONNECTION_MODE_NONE, \
    }

#define ESP_MANUFACTURER_NAME "\x12""esp32-c6-opentherm"
#define ESP_MODEL_IDENTIFIER "\x0e""OT-ZB-Bridge-v1"

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

    switch (message->attribute.id) {
    case ESP_ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID:
        if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_S16 && message->attribute.data.value) {
            int16_t setpoint = *(int16_t *)message->attribute.data.value;
            return ot_bridge_on_heating_setpoint(setpoint);
        }
        break;
    case ESP_ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID:
        if (message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_U8 && message->attribute.data.value) {
            uint8_t mode = *(uint8_t *)message->attribute.data.value;
            return ot_bridge_on_system_mode(mode);
        }
        break;
    default:
        break;
    }

    return ESP_OK;
}

static esp_err_t zb_action_handler(esp_zb_core_action_callback_id_t callback_id, const void *message)
{
    if (callback_id == ESP_ZB_CORE_SET_ATTR_VALUE_CB_ID) {
        return zb_thermostat_attribute_handler((esp_zb_zcl_set_attr_value_message_t *)message);
    }
    ESP_LOGW(TAG, "Unhandled Zigbee action(0x%x)", callback_id);
    return ESP_OK;
}

static void esp_zb_task(void *pvParameters)
{
    esp_zb_cfg_t zb_nwk_cfg = ESP_ZB_ZED_CONFIG();
    esp_zb_init(&zb_nwk_cfg);

    esp_zb_thermostat_cfg_t thermostat_cfg = ESP_ZB_DEFAULT_THERMOSTAT_CONFIG();
    thermostat_cfg.thermostat_cfg.occupied_heating_setpoint = 6000; /* 60.00 C water setpoint default */
    thermostat_cfg.thermostat_cfg.system_mode = ESP_ZB_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT;

    esp_zb_ep_list_t *thermostat_ep = esp_zb_thermostat_ep_create(HA_OT_BRIDGE_ENDPOINT, &thermostat_cfg);

    zcl_basic_manufacturer_info_t info = {
        .manufacturer_name = ESP_MANUFACTURER_NAME,
        .model_identifier = ESP_MODEL_IDENTIFIER,
    };
    esp_zcl_utility_add_ep_basic_manufacturer_info(thermostat_ep, HA_OT_BRIDGE_ENDPOINT, &info);

    esp_zb_device_register(thermostat_ep);
    esp_zb_core_action_handler_register(zb_action_handler);
    esp_zb_set_primary_network_channel_set(ESP_ZB_PRIMARY_CHANNEL_MASK);

    ESP_ERROR_CHECK(esp_zb_start(false));
    esp_zb_stack_main_loop();
}

bool zb_thermostat_ed_is_joined(void)
{
    return s_network_joined;
}

esp_err_t zb_thermostat_ed_set_local_temperature(int16_t temp_centi_c)
{
    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_status_t status = esp_zb_zcl_set_attribute_val(
        HA_OT_BRIDGE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_THERMOSTAT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        ESP_ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID, &temp_centi_c, false);
    esp_zb_lock_release();

    return (status == ESP_ZB_ZCL_STATUS_SUCCESS) ? ESP_OK : ESP_FAIL;
}

esp_err_t zb_thermostat_ed_report_local_temperature(int16_t temp_centi_c)
{
    ESP_RETURN_ON_ERROR(zb_thermostat_ed_set_local_temperature(temp_centi_c), TAG, "Failed to set local temperature");

    esp_zb_zcl_report_attr_cmd_t report_cmd = {
        .zcl_basic_cmd =
            {
                .src_endpoint = HA_OT_BRIDGE_ENDPOINT,
            },
        .address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
        .clusterID = ESP_ZB_ZCL_CLUSTER_ID_THERMOSTAT,
        .attributeID = ESP_ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID,
    };

    esp_zb_lock_acquire(portMAX_DELAY);
    esp_err_t ret = esp_zb_zcl_report_attr_cmd_req(&report_cmd);
    esp_zb_lock_release();

    return ret;
}

void zb_thermostat_ed_start(void)
{
    esp_zb_platform_config_t config = {
        .radio_config = ESP_ZB_DEFAULT_RADIO_CONFIG(),
        .host_config = ESP_ZB_DEFAULT_HOST_CONFIG(),
    };

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_zb_platform_config(&config));
    xTaskCreate(esp_zb_task, "Zigbee_main", 4096, NULL, 5, NULL);
}
