---
type: Reference
title: Melnyk OpenTherm Library
description: OpenTherm v2.2 master library for Arduino/ESP32; interrupt-driven Manchester framing.
resource: https://github.com/ihormelnyk/opentherm_library
tags: [opentherm, library, esp32]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/opentherm/2024-02-08-melnyk-opentherm-library.md
---

The [OpenTherm Library](https://github.com/ihormelnyk/opentherm_library) by Ihor Melnyk is a widely used **OpenTherm v2.2 master** implementation for Arduino, ESP8266, and ESP32 (v1.1.5, MIT). It uses GPIO interrupts for Manchester frame timing.

This repo includes it under `components/opentherm/` as a reference for protocol logic, even when using native ESP-IDF APIs.

## Requirements

| Item | Detail |
|------|--------|
| Protocol | OpenTherm v2.2 OT/+ per [OpenTherm Protocol](/opentherm/opentherm-protocol.md) |
| Hardware | OpenTherm adapter — MCU GPIO cannot drive 7–18 V / mA bus directly |
| Pins | 2 GPIOs: interrupt-capable **in** and **out** |
| Timing | Master must communicate ≥ every 1 s — see [OpenTherm Frame Format](/opentherm/opentherm-frame-format.md) |

## API mapping

| Method | Maps to |
|--------|---------|
| `setBoilerStatus(ch, dhw, cooling, …)` | [Data ID 0 Status](/opentherm/data-id-0-status.md) |
| `setBoilerTemperature(°C)` | [Data ID 1 TSet](/opentherm/data-id-1-tset.md) |
| `getBoilerTemperature()` | [Data ID 25 Tboiler](/opentherm/data-id-25-tboiler.md) |

`OpenThermMessageID` enum in `OpenTherm.h` lists spec data IDs 0–127 for advanced reads/writes.

# Citations

[1] [wiki/raw/opentherm/2024-02-08-melnyk-opentherm-library.md](wiki/raw/opentherm/2024-02-08-melnyk-opentherm-library.md)
[2] [github.com/ihormelnyk/opentherm_library](https://github.com/ihormelnyk/opentherm_library)
