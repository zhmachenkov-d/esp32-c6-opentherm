---
type: Attribute
title: LocalTemperature Attribute
description: ZCL Thermostat 0x0000 — current temperature; v1 reports boiler flow water.
tags: [zigbee, temperature]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md
---

Zigbee Thermostat attribute **0x0000 LocalTemperature** — mandatory, reportable **int16** in 0.01°C units.

## v1 firmware mapping

| Zigbee | OpenTherm | Direction |
|--------|-----------|-----------|
| LocalTemperature | [Data ID 25 Tboiler](/opentherm/data-id-25-tboiler.md) | OT read → ZCL report |

Bridge polls Tboiler ~1 Hz and reports to the coordinator when value changes. See [Local Temperature Mapping](/bridge/local-temperature-mapping.md).

**Important:** v1 reports **boiler flow water temperature**, not room air temperature.

# Citations

[1] [wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md](wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md)
