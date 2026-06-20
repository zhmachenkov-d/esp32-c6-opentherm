#include <limits.h>
#include <stdint.h>

#include "ot_bridge.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "zcl/esp_zigbee_zcl_thermostat.h"

#include "opentherm_wrapper.h"
#include "zb_thermostat_ed.h"

static const char *TAG = "OT_BRIDGE";

static bool s_ch_enabled = true;
static bool s_dhw_enabled = false;
static float s_water_setpoint_c = 60.0f;
static int16_t s_last_reported_temp_centi_c = INT16_MIN;

static int16_t float_to_centi_c(float celsius)
{
    return (int16_t)(celsius * 100.0f);
}

static float centi_c_to_float(int16_t centi_c)
{
    return centi_c / 100.0f;
}

void ot_bridge_init(void)
{
    s_ch_enabled = true;
    s_dhw_enabled = false;
    s_water_setpoint_c = 60.0f;
    s_last_reported_temp_centi_c = INT16_MIN;
}

static void ot_bridge_apply_outputs(void)
{
    if (!opentherm_set_boiler_status(s_ch_enabled, s_dhw_enabled)) {
        ESP_LOGW(TAG, "OpenTherm status write failed");
    }
    vTaskDelay(pdMS_TO_TICKS(120));

    if (!opentherm_set_boiler_temperature(s_water_setpoint_c)) {
        ESP_LOGW(TAG, "OpenTherm TSet write failed");
    }
}

esp_err_t ot_bridge_on_heating_setpoint(int16_t temp_centi_c)
{
    s_water_setpoint_c = centi_c_to_float(temp_centi_c);
    ESP_LOGI(TAG, "Water setpoint -> %.2f C", s_water_setpoint_c);
    ot_bridge_apply_outputs();
    return ESP_OK;
}

esp_err_t ot_bridge_on_system_mode(uint8_t mode)
{
    switch (mode) {
    case ESP_ZB_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT:
        s_ch_enabled = true;
        break;
    case ESP_ZB_ZCL_THERMOSTAT_SYSTEM_MODE_OFF:
        s_ch_enabled = false;
        break;
    default:
        ESP_LOGW(TAG, "Unsupported system mode 0x%02x", mode);
        return ESP_ERR_NOT_SUPPORTED;
    }

    ESP_LOGI(TAG, "System mode 0x%02x -> CH %s", mode, s_ch_enabled ? "on" : "off");
    ot_bridge_apply_outputs();
    return ESP_OK;
}

static void ot_poll_task(void *arg)
{
    (void)arg;

    /* Allow Zigbee stack to start before first OT traffic. */
    vTaskDelay(pdMS_TO_TICKS(3000));

    while (true) {
        opentherm_process();

        if (!opentherm_send_status_keepalive(s_ch_enabled, s_dhw_enabled)) {
            ESP_LOGW(TAG, "OpenTherm keepalive failed");
        }
        vTaskDelay(pdMS_TO_TICKS(120));

        float t_boiler = opentherm_get_boiler_temperature();
        if (t_boiler > 0.0f) {
            int16_t temp_centi_c = float_to_centi_c(t_boiler);
            if (temp_centi_c != s_last_reported_temp_centi_c) {
                s_last_reported_temp_centi_c = temp_centi_c;
                ESP_LOGI(TAG, "Tboiler %.2f C -> LocalTemperature", t_boiler);
                if (zb_thermostat_ed_is_joined()) {
                    if (zb_thermostat_ed_report_local_temperature(temp_centi_c) != ESP_OK) {
                        ESP_LOGW(TAG, "Failed to report LocalTemperature");
                    }
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(880));
    }
}

void ot_bridge_start(void)
{
    xTaskCreate(ot_poll_task, "ot_poll", 4096, NULL, 4, NULL);
}
