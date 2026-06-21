/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: LicenseRef-Included
 */

#pragma once

#include "esp_err.h"
#include "esp_zigbee_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ESP_ZB_ZCL_CLUSTER_ID_BASIC_MANUFACTURER_NAME_MAX_LEN 32
#define ESP_ZB_ZCL_CLUSTER_ID_BASIC_MODEL_IDENTIFIER_MAX_LEN 32

typedef struct zcl_basic_manufacturer_info_s {
    char *manufacturer_name;
    char *model_identifier;
} zcl_basic_manufacturer_info_t;

esp_err_t esp_zcl_utility_add_ep_basic_manufacturer_info(esp_zb_ep_list_t *ep_list, uint8_t endpoint_id,
                                                           zcl_basic_manufacturer_info_t *info);

#ifdef __cplusplus
}
#endif
