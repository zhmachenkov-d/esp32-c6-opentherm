---
type: Playbook
title: Local Temperature Mapping
description: OpenTherm Data ID 25 Tboiler to Zigbee LocalTemperature reporting.
tags: [bridge, temperature]
timestamp: 2026-07-02T00:00:00Z
---

Maps [Data ID 25 Tboiler](/opentherm/data-id-25-tboiler.md) reads to [LocalTemperature](/zigbee/attribute-local-temperature.md) attribute reports.

## Data flow

```
ot_poll_task (~1 Hz)
  → opentherm_get_boiler_temperature()   // OT ID 25 READ
  → float_to_centi_c(t_boiler)
  → zb_thermostat_ed_report_local_temperature(temp_centi_c)
```

Reports only when value changes and Zigbee network is joined.

## Encoding

| Layer | Format | Example (45.5°C) |
|-------|--------|------------------|
| OpenTherm | f8.8 | per [OpenTherm Data Encoding](/opentherm/opentherm-data-encoding.md) |
| Firmware float | °C | 45.5 |
| Zigbee on-air | int16, 0.01°C units | 4550 |

## Poll loop timing

| Step | Delay |
|------|-------|
| Status keepalive (ID 0) | — |
| Inter-frame gap | 120 ms |
| Tboiler read | — |
| Loop remainder | 880 ms |
| **Total period** | ~1000 ms |

Meets OpenTherm master **≥1 s** message requirement.

## v1 semantics

**Boiler flow water temperature**, not room air. Label climate entities accordingly in Home Assistant.

# Citations

[1] `main/ot_bridge.c` — `ot_poll_task`, `float_to_centi_c`
[2] `main/zb_thermostat_ed.c` — `zb_thermostat_ed_report_local_temperature`
