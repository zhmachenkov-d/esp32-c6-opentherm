# Zigbee Cluster Library Specification

> Source: https://zigbeealliance.org/wp-content/uploads/2021/10/07-5123-08-Zigbee-Cluster-Library.pdf
> Collected: 2026-06-20
> Published: 2019-12-01

Document **07-5123 Revision 8**. Date of release: **December 2019**. Sponsored by Zigbee Alliance (now Connectivity Standards Alliance). 1213 pages.

## Abstract

This document defines the Zigbee Cluster Library (ZCL). Keywords: Zigbee, Application, Cluster, Library, ZCL, Dotdot, Dictionary, Data Model.

## 1. Introduction (summary)

Dotdot is a data model defined in Application Architecture [Z5]. Elements include clusters and devices.

The ZCL defines object classes called **clusters** as building blocks for interoperable devices. The library is a repository and dictionary reference, updated as new functionality is added.

**Chapter structure:**
- Chapter 2 — Foundation (cluster model architecture)
- Chapter 3 — General application and utility clusters
- Chapters 4–15 — Clusters grouped by application domain (flat cluster namespace)

## 1.4 Key terms

- **Cluster identifier:** 16-bit ID mapping to one cluster specification; designated input or output in simple descriptor
- **Client:** Output cluster; typically sends commands to remote server
- **Server:** Input cluster; typically holds attributes
- **Device:** Device identifier + mandatory/optional clusters on an endpoint
- **Node:** Single Zigbee application on one stack with one network address
- **Endpoint:** Logical address 0–255 on a node (0 = ZDO configuration, 255 = broadcast all endpoints)
- **Utility cluster:** Commissioning, configuration, discovery — not primary application function
- **Type 1 cluster:** Client-initiated transactions
- **Type 2 cluster:** Server-initiated transactions (e.g. Thermostat)

Access notation: R (read), W (write), P (reportable), M (mandatory), O (optional).

## 2. Foundation (summary)

Cluster library overview: revision via ClusterRevision attribute, base vs derived cluster identifiers, simple descriptor lists input/output clusters per endpoint.

Global commands include Read Attributes, Write Attributes, Configure Reporting.

Global attributes (Table 2-1): ClusterRevision, AttributeReportingStatus.

Broadcast endpoint 0xff; multicast uses group IDs without endpoint.

## 3. General clusters (selected IDs)

| ID | Cluster |
|----|---------|
| 0x0000 | Basic |
| 0x0001 | Power Configuration |
| 0x0003 | Identify |
| 0x0004 | Groups |
| 0x0005 | Scenes |
| 0x0006 | On/Off |
| 0x0008 | Level Control |

## 6. HVAC functional domain

### Table 6-1 — HVAC clusters

| ID | Cluster Name |
|----|--------------|
| 0x0200 | Pump Configuration and Control |
| 0x0201 | Thermostat |
| 0x0202 | Fan Control |
| 0x0203 | Dehumidification Control |
| 0x0204 | Thermostat User Interface Configuration |

### 6.3 Thermostat cluster (0x0201)

- **Classification:** Type 2 (server to client), Application, Base
- Temperatures on-air are **degrees Celsius** (int16, 0.01°C resolution); f8.8 for setpoints in some contexts
- **LocalTemperature** (0x0000): int16, mandatory, reportable — `LocalTemperature = 100 × (°C + calibration)`
- **OccupiedHeatingSetpoint** (0x0012), **OccupiedCoolingSetpoint** (0x0011): f8.8 setpoints when occupied
- **SystemMode** (0x001c): Off, Auto, Cool, Heat, Emergency heating, Precooling, Fan only, Dry, Sleep
- **ThermostatRunningState** (0x0029): relay state (heat/cool/fan)
- Attribute sets: 0x000 Information, 0x001 Settings, 0x002 Schedule & HVAC Relay, 0x003 Setpoint Change Tracking, 0x004 AC Information

Weekly schedule extension requires StartOfWeek, NumberOfWeeklyTransitions, Set/Get Weekly Schedule commands when supported.

## Other chapters (listed)

- Chapter 4 — Measurement & Sensing
- Chapter 5 — Security & Safety
- Chapter 7 — Closure
- Chapter 8 — HVAC (duplicate numbering in TOC — HVAC is Ch.6)
- Chapter 9 — Lighting
- Chapter 10 — Smart Energy
- Chapter 11 — Over the Air Upgrades
- Chapter 12 — Telecommunications
- Chapter 13 — Commissioning
- Chapter 14 — Retail Services
- Chapter 15 — Appliances

## References (normative, selected)

- [Z1] Zigbee Specification
- [Z5] Application Architecture
- [Z6] Base Device Behavior Specification
- [A1] ASHRAE 135-2004 (BACnet) — related building automation protocol
