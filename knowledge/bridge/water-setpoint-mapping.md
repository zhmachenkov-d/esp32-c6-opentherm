---
type: Playbook
title: Water Setpoint Mapping
description: Zigbee OccupiedHeatingSetpoint to OpenTherm Data ID 1 TSet conversion.
tags: [bridge, setpoint]
timestamp: 2026-07-02T00:00:00Z
---

Maps [OccupiedHeatingSetpoint](/zigbee/attribute-occupied-heating-setpoint.md) writes to [Data ID 1 TSet](/opentherm/data-id-1-tset.md) on the OpenTherm bus.

## Data flow

```
Coordinator Write Attributes (0x0012)
  → ot_bridge_on_heating_setpoint(temp_centi_c)
  → s_water_setpoint_c = temp_centi_c / 100.0
  → opentherm_set_boiler_temperature(°C)   // OT ID 1 WRITE
```

## Encoding

| Layer | Format | Example (60°C) |
|-------|--------|----------------|
| Zigbee on-air | int16, 0.01°C units | 6000 |
| Firmware float | °C | 60.0 |
| OpenTherm | f8.8 | per [OpenTherm Data Encoding](/opentherm/opentherm-data-encoding.md) |

## Timing

Setpoint writes trigger `ot_bridge_apply_outputs()` immediately: status (ID 0) then TSet (ID 1) with **≥120 ms** gap between frames per [OpenTherm Frame Format](/opentherm/opentherm-frame-format.md).

## v1 semantics

**Water setpoint only** — coordinators must not assume room air temperature control. Document this in Home Assistant climate entity configuration.

# Citations

[1] `main/ot_bridge.c` — `ot_bridge_on_heating_setpoint`, `ot_bridge_apply_outputs`
