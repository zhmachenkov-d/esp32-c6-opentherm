# Zigbee Cluster Library (Rev 8)

> Sources: Zigbee Alliance, 2019-12-01
> Raw: [2019-12-zigbee-cluster-library-rev8](../../raw/zigbee/2019-12-zigbee-cluster-library-rev8.md)

## Overview

The [Zigbee Cluster Library (ZCL)](https://zigbeealliance.org/wp-content/uploads/2021/10/07-5123-08-Zigbee-Cluster-Library.pdf) — document **07-5123 Revision 8** (December 2019) — is the standard dictionary of **clusters** (attributes + commands) used for interoperable Zigbee devices. It complements the Zigbee stack spec and Application Architecture (Dotdot data model).

ESP32-C6 supports 802.15.4 Thread/Zigbee; if this project adds a Zigbee thermostat or gateway path (alongside OpenTherm), ZCL is the attribute/command reference — especially **Chapter 6 HVAC**.

## Cluster model

```
Node (one network address)
 └── Endpoint (0–255)
      ├── Server clusters (input list) — expose attributes
      └── Client clusters (output list) — send commands to remote servers
```

| Role | Simple descriptor | Typical behaviour |
|------|-------------------|-----------------|
| **Server** | Input cluster | Holds attributes; responds to reads/writes |
| **Client** | Output cluster | Initiates commands to matching server on another device |

Each cluster has a **16-bit cluster ID**, **ClusterRevision** attribute, and defined attributes/commands. Devices discover supported clusters via the **simple descriptor** on each endpoint.

## Document structure

| Chapter | Content |
|---------|---------|
| 1 | Introduction, terms, references |
| 2 | **Foundation** — global commands (Read/Write Attributes, reporting), data types |
| 3 | General clusters (Basic, Identify, Groups, On/Off, Level, …) |
| 4–5 | Measurement, security |
| **6** | **HVAC** — Thermostat, Fan, Pump, … |
| 7–15 | Closure, lighting, Smart Energy, OTA, commissioning, appliances |

Cluster namespace is **flat**; chapter groupings are organizational only.

## HVAC clusters (Chapter 6)

| Cluster ID | Name | Purpose |
|------------|------|---------|
| **0x0200** | Pump Configuration and Control | Pump setup/status |
| **0x0201** | **Thermostat** | Heating/cooling control — primary HVAC cluster |
| **0x0202** | Fan Control | Fan speed/mode |
| **0x0203** | Dehumidification Control | Dehumidifier control |
| **0x0204** | Thermostat User Interface Configuration | Remote UI config |

## Thermostat cluster (0x0201)

**Type 2** cluster — server pushes reports to client. Temperatures on-air use **int16 Celsius** (0.01°C units) unless noted as f8.8.

### Key attributes

| ID | Name | Type | Notes |
|----|------|------|-------|
| 0x0000 | LocalTemperature | int16 | Current temp (mandatory, reportable) |
| 0x0011 | OccupiedCoolingSetpoint | f8.8 | Cooling setpoint when occupied |
| 0x0012 | OccupiedHeatingSetpoint | f8.8 | Heating setpoint when occupied |
| 0x001c | SystemMode | enum8 | Off / Auto / Cool / Heat / Emergency / … |
| 0x0029 | ThermostatRunningState | map16 | Heat, cool, fan relay states |

**SystemMode values:** Off (0x00), Auto (0x01), Cool (0x03), Heat (0x04), Emergency heating (0x05), Precooling (0x06), Fan only (0x07), Dry (0x08), Sleep (0x09).

Attribute sets group related IDs: Information (0x000), Settings (0x001), Schedule & HVAC Relay (0x002), Setpoint Change Tracking (0x003).

### Related clusters on same endpoint

Thermostat server may coexist with **Alarms** (mandatory for alarms), **Temperature Measurement** client (remote sensor), **Occupancy Sensing** client.

## Foundation commands (Chapter 2)

Common global commands used with any cluster:

- **Read Attributes** / **Write Attributes**
- **Configure Reporting** — attribute change notifications
- **Discover Attributes**

Access flags in attribute tables: **R**, **W**, **P** (reportable), **M** (mandatory), **O** (optional).

## General utility clusters (selected)

| ID | Cluster | Use |
|----|---------|-----|
| 0x0000 | Basic | Device info, power source |
| 0x0003 | Identify | Commissioning find-me |
| 0x0004 | Groups | Group addressing |
| 0x0006 | On/Off | Binary actuators |
| 0x0008 | Level Control | Dimming / modulation |

## Relevance to esp32-c6-opentherm

| Path | ZCL role |
|------|----------|
| **OpenTherm (primary)** | Boiler control via OT/+ — see [OpenTherm Protocol v2.2](opentherm-protocol-v2-2.md); ZCL not required |
| **Zigbee (optional future)** | ESP32-C6 + [ESP-Zigbee-SDK](https://github.com/espressif/esp-zigbee-sdk) — implement Thermostat (0x0201) server or client using this spec |

ZCL Thermostat attributes (setpoint, mode, local temp) are conceptually analogous to OpenTherm IDs 0/1/25 but on a different wire protocol. A hybrid device might bridge Zigbee thermostat commands to OpenTherm boiler control.

## ESP32 Zigbee resources

- [Espressif ESP-Zigbee-SDK](https://github.com/espressif/esp-zigbee-sdk) — includes HA thermostat example (`HA_thermostat`)
- ZCL PDF mirror: [CSA IoT copy](https://csa-iot.org/wp-content/uploads/2022/01/07-5123-08-Zigbee-Cluster-Library-1.pdf)

## See Also

- [OpenTherm Protocol v2.2](../opentherm/opentherm-protocol-v2-2.md) — boiler bus protocol for this project
- [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md) — ESP32-C6 Thread/Zigbee capability
- [WeAct ESP32-C6-A Dev Board](../esp32/weact-esp32-c6-a-dev-board.md)
- Full spec: [07-5123-08 Zigbee Cluster Library PDF](https://zigbeealliance.org/wp-content/uploads/2021/10/07-5123-08-Zigbee-Cluster-Library.pdf)
