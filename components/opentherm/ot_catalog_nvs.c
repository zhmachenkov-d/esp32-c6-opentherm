#include "ot_catalog_nvs.h"

#include <string.h>

#include "nvs.h"
#include "nvs_flash.h"

static const char *NVS_NAMESPACE = "ot_catalog";
static const char *NVS_KEY = "disc";

bool ot_catalog_nvs_has_cache(void)
{
    nvs_handle_t handle;
    if (nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }

    size_t size = 0;
    const esp_err_t err = nvs_get_blob(handle, NVS_KEY, NULL, &size);
    nvs_close(handle);
    return err == ESP_OK && size == sizeof(ot_runtime_catalog_t);
}

esp_err_t ot_catalog_nvs_load(ot_runtime_catalog_t *out)
{
    if (out == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return err;
    }

    size_t size = sizeof(*out);
    err = nvs_get_blob(handle, NVS_KEY, out, &size);
    nvs_close(handle);

    if (err != ESP_OK) {
        return err;
    }
    if (size != sizeof(*out) || out->version != OT_CATALOG_NVS_VERSION || out->count > 128) {
        return ESP_ERR_INVALID_VERSION;
    }
    return ESP_OK;
}

esp_err_t ot_catalog_nvs_save(const ot_runtime_catalog_t *catalog)
{
    if (catalog == NULL || catalog->count > 128) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_set_blob(handle, NVS_KEY, catalog, sizeof(*catalog));
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }
    nvs_close(handle);
    return err;
}

esp_err_t ot_catalog_nvs_clear(void)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return ESP_OK;
    }
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_erase_key(handle, NVS_KEY);
    if (err == ESP_OK || err == ESP_ERR_NVS_NOT_FOUND) {
        err = nvs_commit(handle);
    }
    nvs_close(handle);
    return err;
}
