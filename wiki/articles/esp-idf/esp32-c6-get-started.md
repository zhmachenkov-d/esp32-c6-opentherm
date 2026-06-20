# ESP-IDF Get Started (ESP32-C6)

> Sources: Espressif Systems, Unknown
> Raw: [2026-06-20-esp32-c6-get-started](../../raw/esp-idf/2026-06-20-esp32-c6-get-started.md)

## Overview

The [ESP-IDF Get Started guide for ESP32-C6](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/) walks through setting up the full software development environment: toolchain, build tools (CMake/Ninja), and ESP-IDF itself. It then covers building, flashing, and monitoring a first project on an ESP32-C6 board.

This is the master-branch (latest) documentation; stable releases have separate doc trees.

## ESP32-C6 chip capabilities

| Feature | Detail |
|---------|--------|
| Wi-Fi | Wi-Fi 6, 2.4 GHz |
| Bluetooth | BLE |
| 802.15.4 | Thread / Zigbee |
| CPU | 32-bit RISC-V single-core |
| Process | 40 nm |

Built-in security hardware and multiple peripherals. Espressif positions ESP-IDF for IoT apps with Wi-Fi, Bluetooth, and power management.

For Zigbee cluster definitions (HVAC thermostat, etc.), see [Zigbee Cluster Library (Rev 8)](../zigbee/zigbee-cluster-library.md).

## Prerequisites

### Hardware

- ESP32-C6 board (official: [DevKitC-1](https://docs.espressif.com/projects/espressif-esp-dev-kits/en/latest/esp32c6/esp32-c6-devkitc-1/index.html) or DevKitM-1; or P2P-compatible boards such as the [WeAct ESP32-C6-A](../esp32/weact-esp32-c6-a-dev-board.md))
- USB cable (micro-USB or USB-C depending on board)
- Windows, Linux, or macOS host

### Software stack

1. **Toolchain** — cross-compiler for ESP32-C6 (RISC-V)
2. **Build tools** — CMake + Ninja
3. **ESP-IDF** — APIs, libraries, and helper scripts

## Installation (EIM)

Install via the **ESP-IDF Installation Manager (EIM)**:

- **GUI** — recommended for interactive setup
- **CLI** — for CI/CD and automation

Platform-specific guides: Windows, Linux, macOS (linked from the upstream get-started page).

## Build workflow

After installation, choose one path:

| Path | Tools |
|------|-------|
| IDE | [Espressif-IDE](https://github.com/espressif/idf-eclipse-plugin) (Eclipse) or [ESP-IDF VS Code extension](https://github.com/espressif/vscode-esp-idf-extension) |
| Command line | OS-specific "Start a Project" instructions (build → flash → monitor) |

Typical CLI flow: create project → `idf.py set-target esp32c6` → `idf.py menuconfig` → `idf.py build` → `idf.py flash monitor`.

## Uninstall

Via EIM GUI (Remove per version, or Purge All) or CLI:

```bash
eim remove v5.4.2   # specific version
eim purge             # all versions
```

## Relevance to this project

`esp32-c6-opentherm` firmware should target `esp32c6` in ESP-IDF. Use DevKitC-1 as the board profile when working with the WeAct ESP32-C6-A (P2P compatible). Install ESP-IDF via EIM before building this repo.

## See Also

- [WeAct ESP32-C6-A Dev Board](../esp32/weact-esp32-c6-a-dev-board.md) — project hardware
- Upstream: [Get Started — ESP32-C6](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/)
- ESP-IDF repo: https://github.com/espressif/esp-idf
