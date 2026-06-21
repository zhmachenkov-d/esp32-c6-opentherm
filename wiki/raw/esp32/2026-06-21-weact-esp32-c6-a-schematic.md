# WeAct ESP32-C6-A Schematic (ESP32_C6_A_Sch.pdf)

> Source: https://github.com/WeActStudio/WeActStudio.ESP32-C6-A/blob/main/Hardware/ESP32_C6_A_Sch.pdf
> Collected: 2026-06-21
> Published: Unknown

WeActStudio — electrical schematic for the ESP32-C6-A development board (`Hardware/ESP32_C6_A_Sch.pdf`).

Binary copy: [esp32-c6-a-sch.pdf](esp32-c6-a-sch.pdf)

## Document properties

| Property | Value |
|----------|-------|
| Pages | 1 |
| Format | PDF schematic (Altium export) |
| Content | Full board schematic: module, power, USB, UART bridge, headers, buttons, RGB LED |

## Core components

| Ref | Part / function |
|-----|-----------------|
| U5 | ESP32-C6-WROOM-1-N4/N8 (Espressif module) |
| U2 | LDO — `+5V` in, `VCC_3V3` / `ESP_3V3` out |
| U4 | USB-to-UART bridge (DTR/RTS for auto-programming) |
| USB1, USB2 | USB Type-C connectors (shared `VBUS`, `USB_DP`, `USB_DN`, CC lines) |
| H1, H2 | 2×16 pin headers (DevKitC-1-style pinout) |
| SW1 | BOOT button (straps boot mode) |
| SW2 | Button on `IO9` |
| — | RESET — pulls `CHIP_PU` low via reset switch |
| WS2812 | WS2812B-B/W addressable RGB LED on `IO8` |
| U3 | Red status LED (`LED0603R-红色`) |

## Power

- `+5V` from USB `VBUS` feeds the LDO (U2) and USB-to-UART VDD.
- LDO output `VCC_3V3` / `ESP_3V3` powers the module and logic.
- Bulk/decoupling: `C1` 10 µF, `C2`/`C4`/`C7`/`C8`/`C10`/`C11` 0.1 µF, `C3`/`C5`/`C6`/`C9`/`C12` 10 µF (values as labeled on schematic).
- `R1` 10 kΩ pull-up on `CHIP_PU` (EN).

## USB

- Type-C CC pins: `R2`, `R3`, `R4` each 5.1 kΩ to GND (CC1/CC2 configuration).
- `USB_DP` / `USB_DN` routed to module native USB and to USB-to-UART `UD+`/`UD-`.
- ESD protection diodes on USB lines (`D1`–`D4`, `D5`–`D8`, etc., as drawn).

## Module pinout (U5, ESP32-C6-WROOM-1)

Module pins as labeled on schematic (left column → right column):

| Pin | Signal | Pin | Signal |
|-----|--------|-----|--------|
| 1 | GND | 29 | GND |
| 2 | IO2 | 28 | 3V3 |
| 3 | IO3 | 27 | EN (`CHIP_PU`) |
| 4 | TXD0 (`U0TXD`) | 26 | IO4 |
| 5 | RXD0 (`U0RXD`) | 25 | IO5 |
| 6 | IO15 | 24 | IO6 |
| 7 | NC | 23 | IO7 |
| 8 | IO23 | 22 | IO0 |
| 9 | IO22 | 21 | IO1 |
| 10 | IO21 | 20 | IO8 |
| 11 | IO20 | 19 | IO10 |
| 12 | IO19 | 18 | IO11 |
| 13 | IO18 | 17 | IO12 |
| 14 | IO9 | 16 | IO13 |
| 15 | EPAD (GND) | | |

## Header pinout (H1, H2)

### H1 (16 pins)

| Pin | Signal |
|-----|--------|
| 1 | `CHIP_PU` |
| 2 | IO4 |
| 3 | IO5 |
| 4 | IO6 |
| 5 | IO7 |
| 6 | IO0 |
| 7 | IO1 |
| 8 | IO8 |
| 9 | IO10 |
| 10 | IO11 |
| 11 | IO2 |
| 12 | IO3 |
| 13–16 | (continued per schematic silk — GND / NC as routed) |

### H2 (16 pins)

| Pin | Signal |
|-----|--------|
| 1 | U0TXD |
| 2 | U0RXD |
| 3 | IO15 |
| 4 | IO23 |
| 5 | IO22 |
| 6 | IO21 |
| 7 | IO20 |
| 8 | IO19 |
| 9 | IO18 |
| 10 | IO9 |
| 11–12 | IO13, IO12 (also broken out on module side) |
| 13–16 | (continued per schematic — duplicate module pins / GND) |

Header numbering matches ESP32-C6-DevKitC-1 P2P layout. Pair with the [board shape PDF](2026-06-21-weact-esp32c6-a-board-shape.md) for physical pin positions.

## User interface

- **BOOT (`SW1`)** — boot-strapping for firmware download mode.
- **RESET** — momentary reset of `CHIP_PU`.
- **SW2 on IO9** — user button (with `R10` 0.1 µF debounce cap).
- **WS2812 on IO8** — RGB LED; `R6` 0 Ω series on data path; level shifting via `Q1`/`Q2` from 3V3 to 5V domain.
- **Red LED (U3)** — power/status indicator.

## USB-to-UART (programming)

- Bridge `U4`: `TXD`/`RXD` ↔ `U0TXD`/`U0RXD`; `DTR`/`RTS` for auto-reset/boot entry.
- `R5`, `R9`, `R13` 10 kΩ; `R11`, `R12` 5.1 kΩ on control lines as drawn.
- `R14`, `D7`, `DTR`/`RTS` network ties into `CHIP_PU` and boot strap (auto-program circuit).

## Documentation block (on schematic)

Embedded links on the drawing:

- esp-idf: https://github.com/espressif/esp-idf
- esp32-C6 get-started: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/
- esp32-C6: https://www.espressif.com/zh-hans/products/socs/esp32-C6
- GitHub: https://github.com/WeActStudio/WeActStudio.ESP32-C6-A

## Usage

Reference for:

- Confirming which GPIOs are exposed on headers vs. used on-board (USB, UART bridge, WS2812, buttons)
- Power budget and 3V3 rail (`LDO` from 5V USB)
- Auto-programming / reset circuit when designing custom programmers or shields
- OpenTherm or other off-board interface wiring (available GPIOs, strapping pins)

Pair with `ESP32C6-A Board Shape 外形.pdf` for mechanical placement.
