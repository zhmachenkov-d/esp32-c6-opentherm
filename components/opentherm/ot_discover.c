#include "ot_discover.h"

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_timer.h"

#include "opentherm_wrapper.h"
#include "ot_catalog_nvs.h"

static const char *TAG = "OT_DISCOVER";

#define OT_DISCOVER_FRAME_DELAY_MS 120

static ot_runtime_catalog_t s_catalog;
static bool s_has_cache;
static bool s_validated;
static SemaphoreHandle_t s_catalog_mutex;

static uint32_t uptime_seconds(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000000ULL);
}

static void log_catalog_diff(const ot_runtime_catalog_t *before, const ot_runtime_catalog_t *after)
{
    for (uint8_t i = 0; i < before->count; i++) {
        const uint8_t id = before->ids[i];
        if (!ot_runtime_catalog_contains(after, id)) {
            ESP_LOGI(TAG, "catalog removed id=%u", id);
        }
    }
    for (uint8_t i = 0; i < after->count; i++) {
        const uint8_t id = after->ids[i];
        if (!ot_runtime_catalog_contains(before, id)) {
            ESP_LOGI(TAG, "catalog added id=%u", id);
        }
    }
}

static esp_err_t probe_id(uint8_t id)
{
    uint16_t data = 0;
    ot_data_status_t status = OT_DATA_NONE;
    const bool available = opentherm_read_id(id, &data, &status);
    (void)data;
    (void)status;
    return available ? ESP_OK : ESP_ERR_NOT_FOUND;
}

static esp_err_t discover_range(ot_runtime_catalog_t *out, const ot_runtime_catalog_t *only_ids)
{
    if (out == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    ot_runtime_catalog_reset(out);
    const int64_t start_us = esp_timer_get_time();

    if (only_ids != NULL) {
        for (uint8_t i = 0; i < only_ids->count; i++) {
            const uint8_t id = only_ids->ids[i];
            if (probe_id(id) == ESP_OK) {
                ot_runtime_catalog_add_sorted(out, id);
            }
            vTaskDelay(pdMS_TO_TICKS(OT_DISCOVER_FRAME_DELAY_MS));
        }
    } else {
        for (uint8_t id = 0; id < 128; id++) {
            if (probe_id(id) == ESP_OK) {
                ot_runtime_catalog_add_sorted(out, id);
            }
            if ((id & 0x0F) == 0x0F) {
                ESP_LOGI(TAG, "discover progress id=%u count=%u", id, out->count);
            }
            vTaskDelay(pdMS_TO_TICKS(OT_DISCOVER_FRAME_DELAY_MS));
        }
    }

    const int64_t elapsed_ms = (esp_timer_get_time() - start_us) / 1000;
    ESP_LOGI(TAG, "discover done count=%u elapsed=%lld ms", out->count, (long long)elapsed_ms);
    return ESP_OK;
}

esp_err_t ot_discover_all(ot_runtime_catalog_t *out)
{
    return discover_range(out, NULL);
}

esp_err_t ot_catalog_validate(const ot_runtime_catalog_t *cached, ot_runtime_catalog_t *out, bool *changed)
{
    if (cached == NULL || out == NULL || changed == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    const esp_err_t err = discover_range(out, cached);
    if (err != ESP_OK) {
        return err;
    }

    *changed = !ot_runtime_catalog_equals(cached, out);
    return ESP_OK;
}

static void set_catalog_snapshot(const ot_runtime_catalog_t *catalog)
{
    if (s_catalog_mutex == NULL) {
        return;
    }
    xSemaphoreTake(s_catalog_mutex, portMAX_DELAY);
    s_catalog = *catalog;
    xSemaphoreGive(s_catalog_mutex);
}

static void discover_task(void *arg)
{
    (void)arg;

    ot_runtime_catalog_t result;
    ot_runtime_catalog_t before;
    bool changed = false;
    esp_err_t err;

    if (!s_has_cache) {
        ESP_LOGI(TAG, "no cache, full discover");
        err = ot_discover_all(&result);
        if (err == ESP_OK) {
            result.saved_at_s = uptime_seconds();
            err = ot_catalog_nvs_save(&result);
        }
    } else {
        ESP_LOGI(TAG, "cache present, validating %u ids", s_catalog.count);
        before = s_catalog;
        err = ot_catalog_validate(&s_catalog, &result, &changed);
        if (err == ESP_OK) {
            if (changed) {
                log_catalog_diff(&before, &result);
                result.saved_at_s = uptime_seconds();
                err = ot_catalog_nvs_save(&result);
            } else {
                result.saved_at_s = before.saved_at_s;
            }
        }
    }

    if (err == ESP_OK) {
        set_catalog_snapshot(&result);
        s_has_cache = true;
    } else {
        ESP_LOGE(TAG, "catalog task failed: %s", esp_err_to_name(err));
    }

    s_validated = (err == ESP_OK);
    ESP_LOGI(TAG, "catalog ready validated=%d count=%u", s_validated ? 1 : 0, s_catalog.count);
    vTaskDelete(NULL);
}

esp_err_t ot_catalog_init(void)
{
    s_catalog_mutex = xSemaphoreCreateMutex();
    if (s_catalog_mutex == NULL) {
        return ESP_ERR_NO_MEM;
    }

    ot_runtime_catalog_reset(&s_catalog);
    s_has_cache = false;
    s_validated = false;

    if (ot_catalog_nvs_has_cache()) {
        const esp_err_t err = ot_catalog_nvs_load(&s_catalog);
        if (err == ESP_OK) {
            s_has_cache = true;
            ESP_LOGI(TAG, "loaded cache count=%u saved_at=%lu s", s_catalog.count,
                     (unsigned long)s_catalog.saved_at_s);
        } else {
            ESP_LOGW(TAG, "cache load failed: %s", esp_err_to_name(err));
            ot_runtime_catalog_reset(&s_catalog);
        }
    }

    return ESP_OK;
}

esp_err_t ot_catalog_start(void)
{
    const BaseType_t ok = xTaskCreate(discover_task, "ot_discover", 4096, NULL, 4, NULL);
    return ok == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
}

bool ot_catalog_has_cache(void)
{
    return s_has_cache;
}

bool ot_catalog_is_validated(void)
{
    return s_validated;
}

const ot_runtime_catalog_t *ot_catalog_get(void)
{
    return &s_catalog;
}

esp_err_t ot_catalog_force_rescan(void)
{
    ot_runtime_catalog_t result;
    esp_err_t err = ot_discover_all(&result);
    if (err != ESP_OK) {
        return err;
    }

    result.saved_at_s = uptime_seconds();
    err = ot_catalog_nvs_save(&result);
    if (err != ESP_OK) {
        return err;
    }

    set_catalog_snapshot(&result);
    s_has_cache = true;
    s_validated = true;
    return ESP_OK;
}

esp_err_t ot_catalog_clear(void)
{
    const esp_err_t err = ot_catalog_nvs_clear();
    if (err != ESP_OK) {
        return err;
    }

    if (s_catalog_mutex != NULL) {
        xSemaphoreTake(s_catalog_mutex, portMAX_DELAY);
    }
    ot_runtime_catalog_reset(&s_catalog);
    s_has_cache = false;
    s_validated = false;
    if (s_catalog_mutex != NULL) {
        xSemaphoreGive(s_catalog_mutex);
    }
    return ESP_OK;
}
