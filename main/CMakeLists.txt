idf_component_register(
    SRCS "main.c" "zb_thermostat_ed.c" "ot_bridge.c" "zcl_utility.c"
    INCLUDE_DIRS "."
    REQUIRES opentherm nvs_flash esp_driver_uart ieee802154
             espressif__esp-zigbee-lib espressif__esp-zboss-lib
)
