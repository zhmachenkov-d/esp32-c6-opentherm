---
type: Data ID
title: Data ID 1 TSet
description: Control setpoint (CH water temperature 0..100°C); master WRITE, f8.8 encoded.
tags: [opentherm, setpoint]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md
---

ID **1** is the **control setpoint** (TSet): central heating water temperature target, 0..100°C.

- Direction: master **WRITE**
- Encoding: [f8.8](/opentherm/opentherm-data-encoding.md)
- CH enable bit in [Data ID 0 Status](/opentherm/data-id-0-status.md) can override setpoint behavior

This firmware maps Zigbee `OccupiedHeatingSetpoint` to this data ID as **water setpoint**, not room air temperature.

# Citations

[1] [wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md](wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md)
