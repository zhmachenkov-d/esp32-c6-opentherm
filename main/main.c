#include "esp_log.h"
#include "nvs_flash.h"

#include "opentherm_wrapper.h"
#include "ot_bridge.h"
#include "ot_discover.h"
#include "zb_thermostat_ed.h"
#if CONFIG_OT_SERIAL_HARNESS
#include "ot_serial_harness.h"
#endif

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "OpenTherm-Zigbee bridge starting");

    ESP_ERROR_CHECK(nvs_flash_init());

    ot_bridge_init();
    opentherm_init(CONFIG_OT_IN_GPIO, CONFIG_OT_OUT_GPIO);
    ESP_ERROR_CHECK(ot_catalog_init());
    ESP_ERROR_CHECK(ot_catalog_start());
    ot_bridge_start();
    zb_thermostat_ed_start();

#if CONFIG_OT_SERIAL_HARNESS
    ot_serial_harness_init();
#endif

    ESP_LOGI(TAG, "OpenTherm GPIO in=%d out=%d", CONFIG_OT_IN_GPIO, CONFIG_OT_OUT_GPIO);
}
