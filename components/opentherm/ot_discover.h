#pragma once

#include <stdbool.h>

#include "esp_err.h"

#include "ot_runtime_catalog.h"

esp_err_t ot_catalog_init(void);
esp_err_t ot_catalog_start(void);
bool ot_catalog_has_cache(void);
bool ot_catalog_is_validated(void);
const ot_runtime_catalog_t *ot_catalog_get(void);
esp_err_t ot_discover_all(ot_runtime_catalog_t *out);
esp_err_t ot_catalog_validate(const ot_runtime_catalog_t *cached, ot_runtime_catalog_t *out, bool *changed);
esp_err_t ot_catalog_force_rescan(void);
esp_err_t ot_catalog_clear(void);
