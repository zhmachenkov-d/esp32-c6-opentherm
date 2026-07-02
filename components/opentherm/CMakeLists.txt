idf_component_register(
    SRCS "OpenTherm.cpp" "opentherm_wrapper.cpp" "ot_data_catalog.c" "ot_encoding.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_timer
)
