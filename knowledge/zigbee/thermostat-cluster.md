---
type: Reference
title: Thermostat Cluster
description: ZCL HVAC Thermostat cluster 0x0201 — server attributes and SystemMode.
tags: [zigbee, thermostat, hvac]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md
---

Cluster **0x0201 Thermostat** is a Type 2 server cluster — the device exposes attributes and may push reports to bound clients.

## Key attributes (v1 firmware)

| ID | Name | Role in bridge |
|----|------|----------------|
| 0x0000 | [LocalTemperature](/zigbee/attribute-local-temperature.md) | Reports boiler water temp |
| 0x0012 | [OccupiedHeatingSetpoint](/zigbee/attribute-occupied-heating-setpoint.md) | Accepts water setpoint writes |
| 0x001c | SystemMode | Heat / Off maps to CH enable |

**SystemMode values:** Off (0x00), Heat (0x04) supported in v1; other modes return not supported.

Temperatures on-air use **int16 in 0.01°C units** unless noted as f8.8 in the ZCL spec.

# Citations

[1] [wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md](wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md)
