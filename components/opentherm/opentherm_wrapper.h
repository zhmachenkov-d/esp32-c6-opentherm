#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OT_DATA_NONE = 0,
    OT_DATA_TIMEOUT,
    OT_DATA_BUS_INVALID,
    OT_DATA_READ_ACK,
    OT_DATA_WRITE_ACK,
    OT_DATA_INVALID,
    OT_DATA_UNKNOWN_ID,
} ot_data_status_t;

void opentherm_init(int in_pin, int out_pin);
void opentherm_process(void);
bool opentherm_set_boiler_status(bool central_heating, bool hot_water);
bool opentherm_set_boiler_temperature(float celsius);
float opentherm_get_boiler_temperature(void);
bool opentherm_send_status_keepalive(bool central_heating, bool hot_water);
bool opentherm_read_id(uint8_t id, uint16_t *data_out, ot_data_status_t *status_out);
bool opentherm_write_id(uint8_t id, uint16_t data, ot_data_status_t *status_out);

#ifdef __cplusplus
}
#endif
