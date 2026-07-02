set(MAIN_SRCS "main.c" "zb_ot_bridge.c" "ot_bridge.c" "zcl_utility.c" "ot_serial_harness.c")

idf_component_register(
    SRCS ${MAIN_SRCS}
    INCLUDE_DIRS "."
    REQUIRES opentherm nvs_flash esp_driver_uart ieee802154
             espressif__esp-zigbee-lib espressif__esp-zboss-lib console
)
