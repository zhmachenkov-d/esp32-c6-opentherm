---
type: Data ID
title: Data ID 25 Tboiler
description: Boiler flow water temperature; slave READ, f8.8 encoded.
tags: [opentherm, temperature]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md
---

ID **25** reports **boiler flow water temperature** (Tboiler).

- Direction: slave **READ**
- Encoding: [f8.8](/opentherm/opentherm-data-encoding.md)
- Recommended/conditional ID in the spec; widely supported

This firmware exposes this value as Zigbee Thermostat `LocalTemperature` in v1.

# Citations

[1] [wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md](wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md)
