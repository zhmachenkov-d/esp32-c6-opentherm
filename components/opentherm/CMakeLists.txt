idf_component_register(
    SRCS "OpenTherm.cpp" "opentherm_wrapper.cpp" "ot_data_catalog.c" "ot_encoding.c"
         "ot_runtime_catalog.c" "ot_catalog_nvs.c" "ot_discover.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_timer nvs_flash
)
