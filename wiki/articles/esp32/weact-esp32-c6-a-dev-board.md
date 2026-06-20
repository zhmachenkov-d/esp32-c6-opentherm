# WeAct ESP32-C6-A Dev Board

> Sources: WeActStudio, Unknown
> Raw: [weactstudio-esp32-c6-a](../../raw/esp32/weactstudio-esp32-c6-a.md)

## Overview

The [WeActStudio ESP32-C6-A](https://github.com/WeActStudio/WeActStudio.ESP32-C6-A) is a compact development board built around Espressif's original ESP32-C6 module. It is designed as a pin-to-pin (P2P) replacement for the official [ESP32-C6-DevKitC-1-N4/8](https://docs.espressif.com/projects/espressif-esp-dev-kits/en/latest/esp32c6/esp32-c6-devkitc-1/index.html), making it a drop-in alternative for firmware and examples targeting that dev kit.

## Hardware

The GitHub repository ships design assets under `Hardware/`:

- **Schematic** — `ESP32_C6_A_Sch.pdf`
- **Board outline** — `ESP32C6-A Board Shape 外形.pdf`
- **Altium libraries** — `WeAct-ESP32C6-A.SchLib`, `.PcbLib`, `.IntLib`

Use the schematic and outline when wiring peripherals (e.g. OpenTherm interface circuits) to confirm pin availability and board dimensions.

## Documentation bundled in repo

The `Doc/` folder includes Espressif reference PDFs (English copies in-repo):

| Document | In-repo file |
|----------|--------------|
| Datasheet | `esp32-c6_datasheet_en.pdf` |
| Technical Reference Manual | `esp32-c6_technical_reference_manual_en.pdf` |
| Hardware Design Guidelines | `esp32-c6_hardware_design_guidelines_en.pdf` |

The `Doc/README.md` also links to Chinese PDFs and upstream Espressif pages.

## Software / getting started

See [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md) for toolchain installation and first build. When selecting a board target, use the DevKitC-1 profile or an equivalent `sdkconfig` for ESP32-C6 — this board mirrors that pinout.

- **SoC overview** (Wi-Fi 6, BLE 5, Thread/Zigbee): https://www.espressif.com/zh-hans/products/socs/esp32-C6

## Relevance to this project

This repo (`esp32-c6-opentherm`) targets ESP32-C6 hardware. The WeAct ESP32-C6-A is the likely physical platform: same module family, P2P with Espressif's reference dev kit, and published schematics for custom I/O design.

## See Also

- [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md)
- Upstream repo: [WeActStudio/WeActStudio.ESP32-C6-A](https://github.com/WeActStudio/WeActStudio.ESP32-C6-A)
