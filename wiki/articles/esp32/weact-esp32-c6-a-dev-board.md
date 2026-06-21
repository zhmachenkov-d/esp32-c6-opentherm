# WeAct ESP32-C6-A Dev Board

> Sources: WeActStudio, Unknown; WeActStudio, 2023-07-17 (board shape); WeActStudio, Unknown (schematic); Espressif Systems, 2023 (v1.0 HW guidelines)
> Raw: [weactstudio-esp32-c6-a](../../raw/esp32/weactstudio-esp32-c6-a.md); [weact-esp32c6-a-board-shape](../../raw/esp32/2026-06-21-weact-esp32c6-a-board-shape.md); [weact-esp32-c6-a-schematic](../../raw/esp32/2026-06-21-weact-esp32-c6-a-schematic.md); [esp32-c6-datasheet-en](../../raw/esp32/esp32-c6-datasheet-en.md)

## Overview

The [WeActStudio ESP32-C6-A](https://github.com/WeActStudio/WeActStudio.ESP32-C6-A) is a compact development board built around Espressif's original ESP32-C6 module. It is designed as a pin-to-pin (P2P) replacement for the official [ESP32-C6-DevKitC-1-N4/8](https://docs.espressif.com/projects/espressif-esp-dev-kits/en/latest/esp32c6/esp32-c6-devkitc-1/index.html), making it a drop-in alternative for firmware and examples targeting that dev kit.

## Hardware

The GitHub repository ships design assets under `Hardware/`:

- **Schematic** — `ESP32_C6_A_Sch.pdf`
- **Board outline** — `ESP32C6-A Board Shape 外形.pdf` — see [Board outline (mechanical)](#board-outline-mechanical) below
- **Altium libraries** — `WeAct-ESP32C6-A.SchLib`, `.PcbLib`, `.IntLib`

Use the schematic and outline when wiring peripherals (e.g. OpenTherm interface circuits) to confirm pin availability and board dimensions.

## Schematic (electrical)

The [schematic PDF](https://github.com/WeActStudio/WeActStudio.ESP32-C6-A/blob/main/Hardware/ESP32_C6_A_Sch.pdf) is a single-page Altium export covering the full board. Binary copy in wiki: [esp32-c6-a-sch.pdf](../../raw/esp32/esp32-c6-a-sch.pdf).

### Block diagram

```
USB Type-C (+5V VBUS)
    ├── LDO (U2) ──► VCC_3V3 / ESP_3V3 ──► ESP32-C6-WROOM-1-N4/N8 (U5)
    ├── USB-to-UART bridge (U4) ──► U0TXD/U0RXD + DTR/RTS auto-program
    └── WS2812 RGB (IO8) + red status LED

H1 / H2 (2×16) ──► DevKitC-1-style GPIO breakout
SW1 BOOT, RESET, SW2 (IO9)
```

### On-board vs. header GPIO

| Function | GPIO / signal | Notes |
|----------|---------------|-------|
| WS2812 RGB LED | IO8 | On-board; level-shifted to 5V domain |
| User button | IO9 | SW2 |
| Native USB | USB_DP, USB_DN | Module + bridge share USB data |
| UART (bridge) | U0TXD, U0RXD | USB serial console / flashing |
| BOOT strap | SW1 | Enters download mode |
| Reset | CHIP_PU | Pull-up R1 10 kΩ; reset switch + auto-program via DTR |

Module is **ESP32-C6-WROOM-1-N4/N8**. Header pins **H1** expose IO0–IO8, IO10–IO11, IO2–IO3, and CHIP_PU; **H2** expose U0TXD, U0RXD, IO15, IO18–IO23, IO9, IO12–IO13 — matching the DevKitC-1 pinout documented on the mechanical drawing.

### Power

5 V from USB feeds an LDO producing 3.3 V for the module and logic. Decoupling caps at module, LDO, and USB sections as labeled (0.1 µF + 10 µF bulk). Use the schematic for exact part refs when debugging supply issues.

Pair with [Board outline (mechanical)](#board-outline-mechanical) for physical pin positions.

## Board outline (mechanical)

The [board shape PDF](https://github.com/WeActStudio/WeActStudio.ESP32-C6-A/blob/main/Hardware/ESP32C6-A%20Board%20Shape%20%E5%A4%96%E5%BD%A2.pdf) is a single-page vector drawing (107.48 × 174.83 mm canvas) showing the PCB outline, USB connector placement, and header pin silkscreen labels. It does not embed numeric dimension callouts as text — open the PDF or measure the physical board for exact mm values when designing enclosures.

Silkscreen labels on the drawing match the DevKitC-1-style header pinout:

| Category | Labels |
|----------|--------|
| Power | 3V3, 5V, G (multiple grounds) |
| Boot / reset | RST, RESET, BOOT |
| Connectivity | USB, UART, TX, RX |
| GPIO | 0–13, 15, 18–23 |
| Other | NC, RGB (on-board LED) |

Binary copy in wiki: [weact-esp32c6-a-board-shape.pdf](../../raw/esp32/weact-esp32c6-a-board-shape.pdf). Pair with the [schematic](2026-06-21-weact-esp32-c6-a-schematic.md) for electrical pin function vs. physical position.

## Documentation bundled in repo

The `Doc/` folder includes Espressif reference PDFs (English copies in-repo):

| Document | In-repo file |
|----------|--------------|
| Datasheet | `esp32-c6_datasheet_en.pdf` — see [ESP32-C6 Series Datasheet](esp32-c6-datasheet.md) |
| Technical Reference Manual | `esp32-c6_technical_reference_manual_en.pdf` — see [ESP32-C6 Technical Reference Manual](esp32-c6-technical-reference-manual.md) |
| Hardware Design Guidelines | `esp32-c6_hardware_design_guidelines_en.pdf` — see [ESP32-C6 Hardware Design Guidelines](esp32-c6-hardware-design-guidelines.md) |

The `Doc/README.md` also links to Chinese PDFs and upstream Espressif pages.

## Software / getting started

See [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md) for toolchain installation and first build. When selecting a board target, use the DevKitC-1 profile or an equivalent `sdkconfig` for ESP32-C6 — this board mirrors that pinout.

- **SoC overview** (Wi-Fi 6, BLE 5, Thread/Zigbee): https://www.espressif.com/zh-hans/products/socs/esp32-C6

## Relevance to this project

This repo (`esp32-c6-opentherm`) targets ESP32-C6 hardware. The WeAct ESP32-C6-A is the likely physical platform: same module family, P2P with Espressif's reference dev kit, and published schematics for custom I/O design.

## See Also

- [ESP32-C6 Technical Reference Manual](esp32-c6-technical-reference-manual.md)
- [ESP32-C6 Hardware Design Guidelines](esp32-c6-hardware-design-guidelines.md)
- [ESP32-C6 Series Datasheet](esp32-c6-datasheet.md)
- [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md)
- Upstream repo: [WeActStudio/WeActStudio.ESP32-C6-A](https://github.com/WeActStudio/WeActStudio.ESP32-C6-A)
