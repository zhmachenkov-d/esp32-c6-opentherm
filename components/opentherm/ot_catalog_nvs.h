#pragma once

#include <stdbool.h>

#include "esp_err.h"

#include "ot_runtime_catalog.h"

esp_err_t ot_catalog_nvs_load(ot_runtime_catalog_t *out);
esp_err_t ot_catalog_nvs_save(const ot_runtime_catalog_t *catalog);
esp_err_t ot_catalog_nvs_clear(void);
bool ot_catalog_nvs_has_cache(void);
