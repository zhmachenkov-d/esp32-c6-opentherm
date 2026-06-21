# ESP32-C6 Hardware Design Guidelines

> Sources: Espressif Systems, 2023 (v1.0)
> Raw: [2023-esp32-c6-hardware-design-guidelines-en](../../raw/esp32/2023-esp32-c6-hardware-design-guidelines-en.md)

## Overview

Espressif's ESP32-C6 Hardware Design Guidelines (v1.0, 2023) cover schematic design and PCB layout for integrating the ESP32-C6 SoC. The document applies to both QFN40 (external flash, up to 16 MB) and QFN32 (in-package flash) packages. Unless noted, examples refer to the QFN40 variant. Core circuitry needs ~20 passives, one 40 MHz crystal, and optional external SPI flash.

## Schematic essentials

Eleven circuit sections: power supply, reset timing, flash, clock, RF, UART, strapping pins, GPIO, ADC, USB, and SDIO.

### Power

| Rail | Pins | Notes |
|------|------|-------|
| Digital LP | VDDPST1 (pin 5) | 3.0–3.6 V; 0.1 µF decoupling each |
| Digital HP | VDDPST2 (pin 28) | 3.0–3.6 V; 0.1 µF decoupling each |
| Flash I/O | VDD_SPI (pin 23) | From VDDPST2 via RSPI; 0.1 µF + 1 µF; ≥3.0 V for flash |
| Analog | VDDA3P3 (2, 3), VDDA1 (37), VDDA2 (40) | 3.0–3.6 V; 10 µF + 1 µF on VDDA3P3; CLC filter (inductor ≥500 mA) |

Recommended supply: **3.3 V, ≥500 mA**. Add 10 µF at power entrance and an ESD diode.

### Reset and boot timing

- **CHIP_PU** must not float. RC delay (typical R=10 kΩ, C=1 µF) ensures rails stabilize before enable.
- Reset voltage VIL_nRST: −0.3 V to 0.25 × VDDPST1.
- Minimum timing: tSTBL ≥50 µs (rails stable before CHIP_PU high); tRST ≥50 µs (CHIP_PU low for reset).
- Unstable or slow-ramp supplies may need an external reset IC (threshold ~3.0 V for 3.3 V flash).

**Boot mode strapping** (GPIO8 / GPIO9):

| Mode | GPIO8 | GPIO9 |
|------|-------|-------|
| SPI Boot (default) | any | 1 |
| Download Boot | 1 | 0 |
| Invalid | 0 | 0 |

Hold time for strapping read: ≥3 ms after CHIP_PU high. Do not add large capacitors on GPIO9.

### Clock

- **Mandatory:** 40 MHz crystal, ±10 ppm accuracy. Load caps from CL formula; tune after test.
- Series inductor (~24 nH initial) on XTAL_P to reduce harmonics affecting RF.
- **Optional RTC:** 32.768 kHz crystal (ESR ≤70 kΩ) or external clock on XTAL_32K_P; pins become GPIO if unused.

### Flash, UART, USB, SDIO

- **Flash (QFN40):** Zero-ohm series resistors on SPI lines to limit drive, reduce RF coupling.
- **UART0:** Fixed pins; 499 Ω on U0TXD for 80 MHz harmonic suppression. Other UART TX lines: series resistor recommended.
- **USB Serial/JTAG:** GPIO12/13 as D−/D+; series R + cap to GND near chip.
- **SDIO slave:** Fixed GPIOs; pull-ups and optional series resistors on all lines.

### RF schematic

50 Ω controlled impedance on PCB. Chip-side CLC matching (values board-specific — do not copy module values). Antenna-side CLC matching if space allows. Target S11 ≈ 30+j0 Ω at 2442 MHz. RF pin may float if wireless unused.

### GPIO usage

Configure via IO MUX or GPIO matrix. After reset, check default pull states; add external pull-up/down on high-Z pins. Avoid flash-occupied pins. Respect strapping states at power-up.

### ADC

0.1 µF filter cap pin-to-ground when using ADC.

## PCB layout

Four-layer stack preferred (signal / GND / power / power+GND). Two-layer acceptable with solid ground under RF and crystal.

### Antenna placement

- Best: module PCB antenna **outside** base board edge, feed point at board edge.
- On-board antenna: **≥15 mm keep-out** (no copper, traces, components); cut base board under antenna if possible.
- Verify RF with housing and measure end-product throughput/range.

### Power layout

- Main 3.3 V trace ≥25 mil; VDDA3P3 ≥20 mil; others ≥10 mil.
- Star topology from power entry; decoupling caps at each pin; ≥2 vias when crossing layers.
- VDDA3P3 isolated from RF/GPIO; CLC filter via to inner layer with keep-out.
- Chip EPAD: ≥9 ground vias.

### Crystal layout

≥2.4 mm from chip clock pins; no vias on clock traces; caps at crystal ends; no digital traces underneath.

### RF layout

50 Ω trace on top layer, no vias, 135° bends. π-match at chip. Optional harmonic stub (15 mil, ~100 Ω) at first cap. Keep UART, USB, crystals, and high-speed buses away from antenna; surround UART with ground.

### Flash, UART, USB, SDIO layout

SPI on inner layer with ground shielding. UART0 resistors near chip, away from crystal. USB differential pairs, length-matched. SDIO data lines ±3 mil vs CLK; total path ≤2500 mil (≤2000 preferred); no plane crossings.

## Troubleshooting RF performance

| Symptom | Likely cause | Fix |
|---------|--------------|-----|
| Poor TX despite small ripple | Ripple measured wrong mode | Test in normal TX; ripple <80 mV (11n MCS7) or <120 mV (11b) |
| Poor TX, ripple OK | Crystal quality/offset or layout interference | Re-layout crystal area |
| TX power/EVM off target | RF impedance mismatch | Tune π-match at antenna |
| Good TX, poor RX | Coupling to antenna | Move crystal/UART away from RF |

## Firmware download

**UART:** IO8 high + IO9 low → Download Boot → flash via UART0 → IO9 high/float → SPI Boot → reboot.

**USB:** Same strapping if flash empty; otherwise flash directly. If firmware disables USB, revert to UART/strapping for next flash. Keep UART0 header as fallback.

## Relevance to this project

OpenTherm interface design on the WeAct ESP32-C6-A should follow these guidelines when adding custom circuits:

- **GPIO selection** — Avoid flash pins and respect boot strapping (GPIO8/9). Confirm mux against the [WeAct schematic](weact-esp32-c6-a-dev-board.md).
- **Power** — OpenTherm opto/isolation adds load; ensure 3.3 V rail meets 500 mA with adequate bulk/decoupling near analog pins if sensing ADC is used.
- **UART0** — Keep download header accessible; series resistor on TX if routing near RF-sensitive areas.
- **RF** — Custom PCB additions (relays, high-voltage OpenTherm front-end) must stay out of the 15 mm antenna keep-out and away from crystal/UART traces.

## See Also

- [ESP32-C6 Technical Reference Manual](esp32-c6-technical-reference-manual.md)
- [ESP32-C6 Series Datasheet](esp32-c6-datasheet.md)
- [WeAct ESP32-C6-A Dev Board](weact-esp32-c6-a-dev-board.md)
- [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md)
- Upstream PDF: https://www.espressif.com/sites/default/files/documentation/esp32-c6_hardware_design_guidelines_en.pdf
