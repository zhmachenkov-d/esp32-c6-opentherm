idf_component_register(
    SRCS "OpenTherm.cpp" "opentherm_wrapper.cpp"
    INCLUDE_DIRS "."
    REQUIRES driver esp_timer
)
