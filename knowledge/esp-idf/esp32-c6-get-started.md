---
type: Reference
title: ESP-IDF Get Started (ESP32-C6)
description: Toolchain install, build, flash, and monitor workflow for ESP32-C6.
tags: [esp-idf, toolchain]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/esp-idf/2026-06-20-esp32-c6-get-started.md
---

[ESP-IDF Get Started for ESP32-C6](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/) covers toolchain setup, ESP-IDF installation, and first build/flash/monitor cycle.

## Prerequisites

- Board: [WeAct ESP32-C6-A](/esp32/weact-esp32-c6-a.md) or ESP32-C6-DevKitC-1
- USB cable; Windows, Linux, or macOS host

## Software stack

1. **Toolchain** — RISC-V cross-compiler for ESP32-C6
2. **Build tools** — CMake + Ninja
3. **ESP-IDF** — APIs, Zigbee stack, helper scripts

Install via **ESP-IDF Installation Manager (EIM)** — GUI or CLI.

## Build workflow

```bash
idf.py set-target esp32c6
idf.py build
idf.py -p /dev/ttyACM0 flash monitor
```

This project uses ESP-IDF **5.4** in the dev container. First Zigbee flash may require `erase-flash`.

# Citations

[1] [wiki/raw/esp-idf/2026-06-20-esp32-c6-get-started.md](wiki/raw/esp-idf/2026-06-20-esp32-c6-get-started.md)
