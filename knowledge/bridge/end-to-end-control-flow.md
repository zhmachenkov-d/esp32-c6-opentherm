---
type: Playbook
title: End-to-End Control Flow
description: Zigbee join, Thermostat callbacks, and OpenTherm master poll loop.
tags: [bridge, control-flow]
timestamp: 2026-07-02T00:00:00Z
---

How the **Bridge** firmware connects Zigbee Thermostat cluster commands to OpenTherm master traffic.

## Startup sequence

1. NVS + Zigbee stack init (`zb_thermostat_ed`)
2. Factory-new device → network steering (permit join on coordinator)
3. `ot_bridge_init()` — default CH on, water setpoint 60°C
4. OpenTherm master init on GPIO12/13 (configurable via menuconfig)
5. `ot_bridge_start()` — FreeRTOS poll task after 3 s delay

## Runtime paths

### Inbound (Zigbee → boiler)

| ZCL input | Handler | OpenTherm action |
|-----------|---------|------------------|
| OccupiedHeatingSetpoint write | `ot_bridge_on_heating_setpoint` | ID 1 TSet WRITE — see [Water Setpoint Mapping](/bridge/water-setpoint-mapping.md) |
| SystemMode Heat / Off | `ot_bridge_on_system_mode` | ID 0 status CH enable bit |

### Outbound (boiler → Zigbee)

| OpenTherm read | Handler | ZCL output |
|----------------|---------|------------|
| ID 25 Tboiler | `ot_poll_task` | LocalTemperature report — see [Local Temperature Mapping](/bridge/local-temperature-mapping.md) |
| ID 0 keepalive | `opentherm_send_status_keepalive` | (maintains OT link only) |

## Roles

| Component | Role |
|-----------|------|
| ESP32-C6 | OpenTherm **master** + Zigbee **End Device** Thermostat server |
| Boiler | OpenTherm **slave** |
| Coordinator | ZCL client (Home Assistant / ZHA / Zigbee2MQTT) |

## Verification

1. Serial: `Joined network successfully`
2. Coordinator discovers Thermostat (`OT-ZB-Bridge-v1`)
3. Serial: ~1 Hz `Tboiler` / `LocalTemperature` logs
4. Setpoint change from HA updates OT TSet

# Citations

[1] `main/ot_bridge.c`
[2] `main/zb_thermostat_ed.c`
[3] [OpenTherm Protocol](/opentherm/opentherm-protocol.md)
[4] [Thermostat Cluster](/zigbee/thermostat-cluster.md)
