#include "ot_encoding.h"

float ot_f88_to_float(uint16_t raw)
{
    const float f = (raw & 0x8000) ? -(0x10000L - raw) / 256.0f : raw / 256.0f;
    return f;
}

uint16_t ot_float_to_f88(float celsius)
{
    if (celsius < 0.0f) {
        celsius = 0.0f;
    }
    if (celsius > 100.0f) {
        celsius = 100.0f;
    }
    return (uint16_t)(celsius * 256.0f);
}

int16_t ot_f88_to_centi_i16(uint16_t raw)
{
    return (int16_t)(ot_f88_to_float(raw) * 100.0f);
}

uint16_t ot_centi_i16_to_f88(int16_t centi)
{
    return ot_float_to_f88(centi / 100.0f);
}

bool ot_flag8_get(uint16_t raw, uint8_t bit)
{
    if (bit >= 16) {
        return false;
    }
    return (raw >> bit) & 1;
}

uint16_t ot_u16_get(uint16_t raw)
{
    return raw;
}

bool ot_data_is_invalid(ot_data_status_t status)
{
    return status == OT_DATA_INVALID;
}

int16_t ot_zcl_invalid_sentinel(ot_value_type_t type)
{
    switch (type) {
    case OT_TYPE_F88:
    case OT_TYPE_S16:
        return (int16_t)0x8000;
    default:
        return (int16_t)0x8000;
    }
}
