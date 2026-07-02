#include "ot_zcl_route.h"

#include "zcl/esp_zigbee_zcl_thermostat.h"

#include "opentherm_wrapper.h"
#include "ot_bridge.h"
#include "ot_data_catalog.h"
#include "ot_encoding.h"
#include "zb_ot_bridge.h"

static bool is_standard_route(ot_zcl_route_t route)
{
    return route == OT_ROUTE_THERMOSTAT || route == OT_ROUTE_ANALOG_IN || route == OT_ROUTE_ANALOG_OUT ||
           route == OT_ROUTE_MULTISTATE;
}

static uint16_t slave_status_to_running_state(uint16_t raw)
{
    const uint8_t slave = (uint8_t)(raw & 0xFF);
    uint16_t state = 0;

    if ((slave & ((1U << 1) | (1U << 3))) != 0) {
        state |= ESP_ZB_ZCL_THERMOSTAT_RUNNING_STATE_HEAT_STATE_ON_BIT;
    }
    return state;
}

static esp_err_t report_thermostat_read(uint8_t id, uint16_t raw, ot_data_status_t status)
{
    const bool valid = !ot_data_is_invalid(status);

    switch (id) {
    case 0: {
        uint16_t running = slave_status_to_running_state(raw);
        return zb_ot_bridge_set_report_thermostat_attr(ESP_ZB_ZCL_ATTR_THERMOSTAT_THERMOSTAT_RUNNING_STATE_ID,
                                                       &running);
    }
    case 1: {
        int16_t centi = valid ? ot_f88_to_centi_i16(raw) : ot_zcl_invalid_sentinel(OT_TYPE_F88);
        return zb_ot_bridge_set_report_thermostat_attr(ESP_ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID, &centi);
    }
    case 7: {
        uint8_t demand = 0;
        if (valid) {
            float pct = ot_f88_to_float(raw);
            if (pct < 0.0f) {
                pct = 0.0f;
            }
            if (pct > 100.0f) {
                pct = 100.0f;
            }
            demand = (uint8_t)(pct + 0.5f);
        }
        return zb_ot_bridge_set_report_thermostat_attr(ESP_ZB_ZCL_ATTR_THERMOSTAT_PI_COOLING_DEMAND_ID, &demand);
    }
    case 8: {
        int16_t centi = valid ? ot_f88_to_centi_i16(raw) : ot_zcl_invalid_sentinel(OT_TYPE_F88);
        return zb_ot_bridge_set_report_thermostat_attr(ESP_ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_HEATING_SETPOINT_ID,
                                                       &centi);
    }
    case 25: {
        int16_t centi = valid ? ot_f88_to_centi_i16(raw) : ot_zcl_invalid_sentinel(OT_TYPE_F88);
        return zb_ot_bridge_report_local_temperature(centi);
    }
    case 57: {
        int16_t centi = valid ? ot_f88_to_centi_i16(raw) : ot_zcl_invalid_sentinel(OT_TYPE_F88);
        return zb_ot_bridge_set_report_thermostat_attr(ESP_ZB_ZCL_ATTR_THERMOSTAT_MAX_HEAT_SETPOINT_LIMIT_ID, &centi);
    }
    default:
        return ESP_ERR_NOT_SUPPORTED;
    }
}

static esp_err_t report_analog_read(uint8_t id, const ot_catalog_entry_t *entry, uint16_t raw, ot_data_status_t status)
{
    const uint8_t endpoint = zb_ot_bridge_endpoint_for_id(id);
    if (endpoint == 0) {
        return ESP_ERR_NOT_FOUND;
    }

    const bool valid = !ot_data_is_invalid(status);
    float value = 0.0f;

    switch (entry->type) {
    case OT_TYPE_F88:
        value = ot_f88_to_float(raw);
        break;
    case OT_TYPE_S16:
        value = (float)(int16_t)raw / 100.0f;
        break;
    case OT_TYPE_U8:
        value = (float)(uint8_t)(raw & 0xFF);
        break;
    case OT_TYPE_U16:
        value = (float)raw;
        break;
    default:
        value = (float)raw;
        break;
    }

    if (entry->zcl_route == OT_ROUTE_ANALOG_IN) {
        return zb_ot_bridge_report_analog_input(endpoint, value, valid);
    }
    return zb_ot_bridge_report_analog_output(endpoint, value, valid);
}

static esp_err_t report_multistate_read(uint8_t id, const ot_catalog_entry_t *entry, uint16_t raw, ot_data_status_t status)
{
    const uint8_t endpoint = zb_ot_bridge_endpoint_for_id(id);
    if (endpoint == 0) {
        return ESP_ERR_NOT_FOUND;
    }

    const bool valid = !ot_data_is_invalid(status);
    uint16_t state = 0;

    if (entry->type == OT_TYPE_U8) {
        state = (uint16_t)(raw & 0xFF);
    } else {
        state = raw;
    }

    return zb_ot_bridge_report_multistate(endpoint, state, valid);
}

esp_err_t ot_zcl_route_apply_read(uint8_t id, uint16_t raw, ot_data_status_t status)
{
    const ot_catalog_entry_t *entry = ot_data_catalog_get(id);
    if (entry == NULL || !is_standard_route(entry->zcl_route)) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    switch (entry->zcl_route) {
    case OT_ROUTE_THERMOSTAT:
        return report_thermostat_read(id, raw, status);
    case OT_ROUTE_ANALOG_IN:
    case OT_ROUTE_ANALOG_OUT:
        return report_analog_read(id, entry, raw, status);
    case OT_ROUTE_MULTISTATE:
        return report_multistate_read(id, entry, raw, status);
    default:
        return ESP_ERR_NOT_SUPPORTED;
    }
}

static esp_err_t write_thermostat(uint8_t id, const ot_zcl_write_t *write, ot_data_status_t *status_out)
{
    switch (id) {
    case 1:
        if (write->fmt == OT_ZCL_WRITE_CENTI) {
            return ot_bridge_on_heating_setpoint(write->v.centi);
        }
        break;
    case 7: {
        if (write->fmt != OT_ZCL_WRITE_U8) {
            break;
        }
        uint16_t raw = ot_float_to_f88((float)write->v.u8);
        if (!opentherm_write_id(id, raw, status_out)) {
            return ESP_FAIL;
        }
        return ESP_OK;
    }
    case 8:
    case 57: {
        if (write->fmt != OT_ZCL_WRITE_CENTI) {
            break;
        }
        uint16_t raw = ot_centi_i16_to_f88(write->v.centi);
        if (!opentherm_write_id(id, raw, status_out)) {
            return ESP_FAIL;
        }
        return ESP_OK;
    }
    default:
        break;
    }
    return ESP_ERR_NOT_SUPPORTED;
}

static esp_err_t write_analog_out(uint8_t id, const ot_catalog_entry_t *entry, const ot_zcl_write_t *write,
                                  ot_data_status_t *status_out)
{
    if (entry->zcl_route != OT_ROUTE_ANALOG_OUT || !entry->writable) {
        return ESP_ERR_NOT_SUPPORTED;
    }
    if (write->fmt != OT_ZCL_WRITE_FLOAT) {
        return ESP_ERR_INVALID_ARG;
    }

    uint16_t raw = 0;
    if (entry->type == OT_TYPE_F88) {
        raw = ot_float_to_f88(write->v.f);
    } else {
        raw = (uint16_t)write->v.f;
    }

    if (!opentherm_write_id(id, raw, status_out)) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t write_multistate(uint8_t id, const ot_catalog_entry_t *entry, const ot_zcl_write_t *write,
                                  ot_data_status_t *status_out)
{
    if (!entry->writable) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    uint16_t raw = 0;
    switch (write->fmt) {
    case OT_ZCL_WRITE_U8:
        raw = write->v.u8;
        break;
    case OT_ZCL_WRITE_U16:
        raw = write->v.u16;
        break;
    case OT_ZCL_WRITE_FLOAT:
        if (entry->type == OT_TYPE_U8) {
            raw = (uint16_t)(uint8_t)write->v.f;
        } else {
            raw = (uint16_t)write->v.f;
        }
        break;
    default:
        return ESP_ERR_INVALID_ARG;
    }

    if (!opentherm_write_id(id, raw, status_out)) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t ot_zcl_route_apply_write(uint8_t id, const ot_zcl_write_t *write, ot_data_status_t *status_out)
{
    const ot_catalog_entry_t *entry = ot_data_catalog_get(id);
    if (entry == NULL || write == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (!is_standard_route(entry->zcl_route)) {
        return ESP_ERR_NOT_SUPPORTED;
    }
    if (!entry->writable) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    switch (entry->zcl_route) {
    case OT_ROUTE_THERMOSTAT:
        return write_thermostat(id, write, status_out);
    case OT_ROUTE_ANALOG_IN:
        return ESP_ERR_NOT_SUPPORTED;
    case OT_ROUTE_ANALOG_OUT:
        return write_analog_out(id, entry, write, status_out);
    case OT_ROUTE_MULTISTATE:
        return write_multistate(id, entry, write, status_out);
    default:
        return ESP_ERR_NOT_SUPPORTED;
    }
}
