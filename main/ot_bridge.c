#include <stdint.h>

#include "ot_bridge.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "zcl/esp_zigbee_zcl_thermostat.h"

#include "opentherm_wrapper.h"
#include "ot_data_catalog.h"
#include "ot_discover.h"
#include "ot_zcl_route.h"
#include "zb_ot_bridge.h"

static const char *TAG = "OT_BRIDGE";

#define OT_POLL_FAST_INTERVAL_MS 1000
#define OT_POLL_SLOW_INTERVAL_MS 60000
#define OT_POLL_PROMOTE_MS (5 * 60 * 1000)

static bool s_ch_enabled = true;
static bool s_dhw_enabled = false;
static float s_water_setpoint_c = 60.0f;

static uint8_t s_poll_ids[128];
static uint8_t s_poll_id_count = 0;
static uint8_t s_poll_rr_index = 0;
static TickType_t s_last_polled_tick[128];
static TickType_t s_promoted_until[128];
static uint16_t s_last_raw[128];
static ot_data_status_t s_last_status[128];
static bool s_last_cached[128];

static float centi_c_to_float(int16_t centi_c)
{
    return centi_c / 100.0f;
}

static bool is_standard_route(ot_zcl_route_t route)
{
    return route == OT_ROUTE_THERMOSTAT || route == OT_ROUTE_ANALOG_IN || route == OT_ROUTE_ANALOG_OUT ||
           route == OT_ROUTE_MULTISTATE;
}

static void rebuild_poll_list(void)
{
    const ot_runtime_catalog_t *catalog = ot_catalog_get();
    s_poll_id_count = 0;

    for (uint8_t i = 0; i < catalog->count && s_poll_id_count < 128; i++) {
        const uint8_t id = catalog->ids[i];
        const ot_catalog_entry_t *entry = ot_data_catalog_get(id);
        if (entry != NULL && is_standard_route(entry->zcl_route)) {
            s_poll_ids[s_poll_id_count++] = id;
        }
    }
}

static bool should_poll_id(uint8_t id, const ot_catalog_entry_t *entry)
{
    const TickType_t now = xTaskGetTickCount();

    if (now < s_promoted_until[id]) {
        return true;
    }
    if (entry->poll_tier == OT_POLL_FAST) {
        return true;
    }

    const TickType_t elapsed = now - s_last_polled_tick[id];
    return elapsed >= pdMS_TO_TICKS(OT_POLL_SLOW_INTERVAL_MS);
}

static bool value_changed(uint8_t id, uint16_t raw, ot_data_status_t status)
{
    if (!s_last_cached[id]) {
        return true;
    }
    return s_last_raw[id] != raw || s_last_status[id] != status;
}

void ot_poll_promote(uint8_t id)
{
    if (id >= 128) {
        return;
    }
    s_promoted_until[id] = xTaskGetTickCount() + pdMS_TO_TICKS(OT_POLL_PROMOTE_MS);
}

void ot_bridge_init(void)
{
    s_ch_enabled = true;
    s_dhw_enabled = false;
    s_water_setpoint_c = 60.0f;
    s_poll_id_count = 0;
    s_poll_rr_index = 0;

    for (uint8_t i = 0; i < 128; i++) {
        s_last_polled_tick[i] = 0;
        s_promoted_until[i] = 0;
        s_last_cached[i] = false;
    }
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

static void poll_one_standard_id(void)
{
    if (s_poll_id_count == 0) {
        return;
    }

    const uint8_t start = s_poll_rr_index;
    do {
        const uint8_t id = s_poll_ids[s_poll_rr_index];
        s_poll_rr_index = (s_poll_rr_index + 1) % s_poll_id_count;

        const ot_catalog_entry_t *entry = ot_data_catalog_get(id);
        if (entry == NULL || !should_poll_id(id, entry)) {
            continue;
        }

        uint16_t raw = 0;
        ot_data_status_t status = OT_DATA_NONE;
        if (!opentherm_read_id(id, &raw, &status)) {
            ESP_LOGD(TAG, "poll id=%u failed status=%d", id, (int)status);
            s_last_polled_tick[id] = xTaskGetTickCount();
            continue;
        }

        s_last_polled_tick[id] = xTaskGetTickCount();
        if (!value_changed(id, raw, status)) {
            return;
        }

        s_last_raw[id] = raw;
        s_last_status[id] = status;
        s_last_cached[id] = true;

        if (zb_ot_bridge_is_joined()) {
            if (ot_zcl_route_apply_read(id, raw, status) != ESP_OK) {
                ESP_LOGD(TAG, "zcl route read id=%u failed", id);
            }
        }
        return;
    } while (s_poll_rr_index != start);
}

static void ot_poll_task(void *arg)
{
    (void)arg;

    while (!ot_catalog_is_validated()) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    while (!zb_ot_bridge_ot_precheck_done()) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    rebuild_poll_list();

    while (true) {
        const TickType_t loop_start = xTaskGetTickCount();

        opentherm_process();

        if (!opentherm_send_status_keepalive(s_ch_enabled, s_dhw_enabled)) {
            ESP_LOGW(TAG, "OpenTherm keepalive failed");
        }
        vTaskDelay(pdMS_TO_TICKS(120));

        poll_one_standard_id();

        const TickType_t elapsed = xTaskGetTickCount() - loop_start;
        const TickType_t delay_ticks = pdMS_TO_TICKS(OT_POLL_FAST_INTERVAL_MS);
        if (elapsed < delay_ticks) {
            vTaskDelay(delay_ticks - elapsed);
        }
    }
}

void ot_bridge_start(void)
{
    xTaskCreate(ot_poll_task, "ot_poll", 4096, NULL, 4, NULL);
}
