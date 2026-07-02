---
type: Reference
title: ESP32-C6 Technical Reference Manual
description: Register-level TRM — memory map, GPIO matrix, peripheral addresses.
tags: [esp32, trm]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/esp32/2023-esp32-c6-technical-reference-manual-en.md
---

Espressif's ESP32-C6 TRM (pre-release v0.3) documents register-level programming for the SoC: CPUs, memory map, GPIO, clocks, boot, crypto, and peripherals.

## Memory map (selected)

| Resource | Address | Size |
|----------|---------|------|
| ROM | 0x4000_0000 | 320 KB |
| HP SRAM | 0x4080_0000 | 512 KB |
| LP SRAM | 0x5000_0000 | 16 KB |
| External flash (cached) | 0x4200_0000 | up to 16 MB |
| Peripherals | 0x6000_0000 | 832 KB |

## Peripheral addresses (selected)

| Module | Base address |
|--------|--------------|
| UART0 | 0x6000_0000 |
| GPIO | 0x6009_1000 |
| SPI2 | 0x6002_0000 |
| TWAI0 | 0x6002_5000 |

Use the TRM for full register tables. Pair with [ESP32-C6 Series Datasheet](/esp32/esp32-c6-datasheet.md) for high-level specs and [ESP32-C6 Hardware Design Guidelines](/esp32/esp32-c6-hardware-design-guidelines.md) for schematic rules.

# Citations

[1] [wiki/raw/esp32/2023-esp32-c6-technical-reference-manual-en.md](wiki/raw/esp32/2023-esp32-c6-technical-reference-manual-en.md)
