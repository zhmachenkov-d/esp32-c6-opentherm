---
type: Attribute
title: OccupiedHeatingSetpoint Attribute
description: ZCL Thermostat 0x0012 — heating setpoint; v1 accepts boiler water target.
tags: [zigbee, setpoint]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md
---

Zigbee Thermostat attribute **0x0012 OccupiedHeatingSetpoint** — heating setpoint when occupied. Type **f8.8** in ZCL; firmware receives **int16 0.01°C** via write callback.

## v1 firmware mapping

| Zigbee | OpenTherm | Direction |
|--------|-----------|-----------|
| OccupiedHeatingSetpoint | [Data ID 1 TSet](/opentherm/data-id-1-tset.md) | ZCL write → OT WRITE |

Home Assistant and other coordinators write this attribute; the bridge converts to OpenTherm control setpoint. See [Water Setpoint Mapping](/bridge/water-setpoint-mapping.md).

**Important:** v1 interprets this as **water setpoint**, not room air setpoint.

# Citations

[1] [wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md](wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md)
