---
type: Reference
title: ESP32-C6 Hardware Design Guidelines
description: Schematic and PCB layout checklist — power, RF, strapping, flash, USB.
tags: [esp32, hardware-design]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/esp32/2023-esp32-c6-hardware-design-guidelines-en.md
---

Espressif v1.0 hardware design guidelines for ESP32-C6 QFN40 (external flash) and QFN32 (in-package flash) integrations.

## Power

Recommended **3.3 V, ≥500 mA** supply with decoupling on VDDPST1, VDDPST2, VDD_SPI, and analog rails (VDDA3P3 with 10 µF + 1 µF).

## Boot strapping (GPIO8 / GPIO9)

| Mode | GPIO8 | GPIO9 |
|------|-------|-------|
| SPI Boot (default) | any | 1 |
| Download Boot | 1 | 0 |
| Invalid | 0 | 0 |

Hold time ≥3 ms after CHIP_PU high. Do not add large capacitors on GPIO9.

## RF and layout

- 40 MHz crystal with load caps per vendor spec
- Keep RF trace short; follow Espressif reference layout for antenna matching
- Ground stitching and keep-out under antenna area

See [WeAct ESP32-C6-A](/esp32/weact-esp32-c6-a.md) for a DevKitC-1-compatible module board implementing these guidelines.

# Citations

[1] [wiki/raw/esp32/2023-esp32-c6-hardware-design-guidelines-en.md](wiki/raw/esp32/2023-esp32-c6-hardware-design-guidelines-en.md)
