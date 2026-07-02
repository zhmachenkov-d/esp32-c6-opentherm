#include "ot_runtime_catalog.h"

#include <string.h>

void ot_runtime_catalog_reset(ot_runtime_catalog_t *catalog)
{
    if (catalog == NULL) {
        return;
    }
    memset(catalog, 0, sizeof(*catalog));
    catalog->version = OT_CATALOG_NVS_VERSION;
}

bool ot_runtime_catalog_contains(const ot_runtime_catalog_t *catalog, uint8_t id)
{
    if (catalog == NULL) {
        return false;
    }

    int lo = 0;
    int hi = (int)catalog->count - 1;
    while (lo <= hi) {
        const int mid = lo + (hi - lo) / 2;
        const uint8_t mid_id = catalog->ids[mid];
        if (mid_id == id) {
            return true;
        }
        if (mid_id < id) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return false;
}

bool ot_runtime_catalog_add_sorted(ot_runtime_catalog_t *catalog, uint8_t id)
{
    if (catalog == NULL || catalog->count >= sizeof(catalog->ids)) {
        return false;
    }
    if (ot_runtime_catalog_contains(catalog, id)) {
        return true;
    }

    uint8_t insert_at = catalog->count;
    for (uint8_t i = 0; i < catalog->count; i++) {
        if (catalog->ids[i] > id) {
            insert_at = i;
            break;
        }
    }

    for (int i = (int)catalog->count - 1; i >= (int)insert_at; i--) {
        catalog->ids[i + 1] = catalog->ids[i];
    }
    catalog->ids[insert_at] = id;
    catalog->count++;
    return true;
}

bool ot_runtime_catalog_equals(const ot_runtime_catalog_t *a, const ot_runtime_catalog_t *b)
{
    if (a == NULL || b == NULL) {
        return false;
    }
    if (a->count != b->count || a->version != b->version) {
        return false;
    }
    return memcmp(a->ids, b->ids, a->count) == 0;
}
