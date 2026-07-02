#pragma once

#include <stdbool.h>
#include <stdint.h>

#define OT_CATALOG_NVS_VERSION 1

typedef struct {
    uint8_t version;
    uint8_t ids[128];
    uint8_t count;
    uint32_t saved_at_s;
} ot_runtime_catalog_t;

void ot_runtime_catalog_reset(ot_runtime_catalog_t *catalog);
bool ot_runtime_catalog_contains(const ot_runtime_catalog_t *catalog, uint8_t id);
bool ot_runtime_catalog_add_sorted(ot_runtime_catalog_t *catalog, uint8_t id);
bool ot_runtime_catalog_equals(const ot_runtime_catalog_t *a, const ot_runtime_catalog_t *b);
