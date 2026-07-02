---
type: Playbook
title: OpenTherm GPIO Wiring
description: Default GPIO12/13 pin assignment for OpenTherm adapter on WeAct ESP32-C6-A.
tags: [bridge, hardware, gpio]
timestamp: 2026-07-02T00:00:00Z
---

Default OpenTherm adapter wiring for this firmware on [WeAct ESP32-C6-A](/esp32/weact-esp32-c6-a.md).

## Pin assignment

| Signal | Default GPIO | Kconfig |
|--------|--------------|---------|
| Adapter input (interrupt) | **GPIO12** | `CONFIG_OT_IN_GPIO` |
| Adapter output | **GPIO13** | `CONFIG_OT_OUT_GPIO` |

Override in `idf.py menuconfig` → **OpenTherm Bridge**.

## Requirements

- GPIO **input must be interrupt-capable** — Manchester decoding uses `handleInterrupt()`
- MCU GPIO cannot drive the OT bus directly — use an [OpenTherm adapter](https://ihormelnyk.com/opentherm_adapter) (7–18 V / mA signalling per [OpenTherm Protocol](/opentherm/opentherm-protocol.md))
- IO12 and IO13 are on header **H2** on the WeAct board

## Library defaults vs this project

Melnyk library examples use GPIO 4/5; this project uses **12/13** per board routing and menuconfig.

# Citations

[1] `main/Kconfig.projbuild` — `OT_IN_GPIO`, `OT_OUT_GPIO`
[2] [Melnyk OpenTherm Library](/opentherm/melnyk-opentherm-library.md)
