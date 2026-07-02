#include "ot_serial_harness.h"

#include "sdkconfig.h"

#if CONFIG_OT_SERIAL_HARNESS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_console.h"
#include "esp_log.h"

#include "opentherm_wrapper.h"
#include "ot_data_catalog.h"

static const char *TAG = "OT_CLI";

static const char *status_to_string(ot_data_status_t status)
{
    switch (status) {
    case OT_DATA_NONE:
        return "NONE";
    case OT_DATA_TIMEOUT:
        return "TIMEOUT";
    case OT_DATA_BUS_INVALID:
        return "BUS_INVALID";
    case OT_DATA_READ_ACK:
        return "READ_ACK";
    case OT_DATA_WRITE_ACK:
        return "WRITE_ACK";
    case OT_DATA_INVALID:
        return "DATA_INVALID";
    case OT_DATA_UNKNOWN_ID:
        return "UNKNOWN_DATA_ID";
    default:
        return "UNKNOWN";
    }
}

static const char *type_to_string(ot_value_type_t type)
{
    switch (type) {
    case OT_TYPE_F88:
        return "f8.8";
    case OT_TYPE_FLAG8:
        return "flag8";
    case OT_TYPE_U8:
        return "u8";
    case OT_TYPE_U16:
        return "u16";
    case OT_TYPE_S16:
        return "s16";
    case OT_TYPE_SPECIAL:
        return "special";
    case OT_TYPE_MIXED:
        return "mixed";
    default:
        return "?";
    }
}

static const char *tier_to_string(ot_poll_tier_t tier)
{
    return tier == OT_POLL_FAST ? "fast" : "slow";
}

static const char *route_to_string(ot_zcl_route_t route)
{
    switch (route) {
    case OT_ROUTE_NONE:
        return "none";
    case OT_ROUTE_THERMOSTAT:
        return "thermostat";
    case OT_ROUTE_ANALOG_IN:
        return "analog_in";
    case OT_ROUTE_ANALOG_OUT:
        return "analog_out";
    case OT_ROUTE_MULTISTATE:
        return "multistate";
    case OT_ROUTE_SPILLOVER:
        return "spillover";
    default:
        return "?";
    }
}

static int cmd_ot_read(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ot_read <id>\n");
        return 1;
    }

    const int id = (int)strtol(argv[1], NULL, 0);
    if (id < 0 || id > 127) {
        printf("ID must be 0-127\n");
        return 1;
    }

    uint16_t data = 0;
    ot_data_status_t status = OT_DATA_NONE;
    opentherm_read_id((uint8_t)id, &data, &status);
    printf("id=%d data=0x%04x status=%s\n", id, data, status_to_string(status));
    return 0;
}

static int cmd_ot_write(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: ot_write <id> <hex>\n");
        return 1;
    }

    const int id = (int)strtol(argv[1], NULL, 0);
    if (id < 0 || id > 127) {
        printf("ID must be 0-127\n");
        return 1;
    }

    const uint16_t data = (uint16_t)strtoul(argv[2], NULL, 0);
    ot_data_status_t status = OT_DATA_NONE;
    const bool ok = opentherm_write_id((uint8_t)id, data, &status);
    printf("id=%d data=0x%04x status=%s ok=%d\n", id, data, status_to_string(status), ok ? 1 : 0);
    return ok ? 0 : 1;
}

static int cmd_ot_cat(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ot_cat <id>\n");
        return 1;
    }

    const int id = (int)strtol(argv[1], NULL, 0);
    if (id < 0 || id > 127) {
        printf("ID must be 0-127\n");
        return 1;
    }

    const ot_catalog_entry_t *entry = ot_catalog_get((uint8_t)id);
    printf("id=%u type=%s tier=%s writable=%d route=%s\n", entry->id, type_to_string(entry->type),
           tier_to_string(entry->poll_tier), entry->writable ? 1 : 0, route_to_string(entry->zcl_route));
    return 0;
}

static void register_ot_commands(void)
{
    const esp_console_cmd_t ot_read_cmd = {
        .command = "ot_read",
        .help = "Read OpenTherm data ID (0-127)",
        .hint = NULL,
        .func = &cmd_ot_read,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&ot_read_cmd));

    const esp_console_cmd_t ot_write_cmd = {
        .command = "ot_write",
        .help = "Write OpenTherm data ID with raw uint16 payload",
        .hint = NULL,
        .func = &cmd_ot_write,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&ot_write_cmd));

    const esp_console_cmd_t ot_cat_cmd = {
        .command = "ot_cat",
        .help = "Show catalog metadata for data ID",
        .hint = NULL,
        .func = &cmd_ot_cat,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&ot_cat_cmd));
}

void ot_serial_harness_init(void)
{
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = "ot> ";

    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));

    register_ot_commands();
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
    ESP_LOGI(TAG, "OpenTherm serial harness ready (ot_read, ot_write, ot_cat)");
}

#else

void ot_serial_harness_init(void)
{
}

#endif /* CONFIG_OT_SERIAL_HARNESS */
