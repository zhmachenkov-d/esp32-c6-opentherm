---
type: Reference
title: ESP32-C6 Series Datasheet
description: SoC overview — RISC-V, Wi-Fi 6, BLE 5, Zigbee/Thread, peripherals, packages.
tags: [esp32, datasheet]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/esp32/esp32-c6-datasheet-en.md
---

The ESP32-C6 combines HP/LP RISC-V cores with tri-radio wireless: 2.4 GHz Wi-Fi 6, Bluetooth 5 LE, and IEEE 802.15.4 (Zigbee 3.0 / Thread 1.3). Targets IoT hubs and smart-home devices.

| Ordering code | Flash | Package | GPIOs |
|---------------|-------|---------|-------|
| ESP32-C6 | External | QFN40 | 30 |
| ESP32-C6FH4 | 4 MB in-package | QFN32 | 22 |

## Wireless

- **Wi-Fi 6** — 1T1R 2.4 GHz, OFDMA, up to 150 Mbps (802.11n fallback)
- **BLE** — Bluetooth 5.3, mesh, +20 dBm high-power mode
- **802.15.4** — 250 Kbps OQPSK; Thread 1.3 and Zigbee 3.0

## CPU and memory

| Resource | Specification |
|----------|---------------|
| HP RISC-V | Up to 160 MHz |
| LP RISC-V | Up to 20 MHz |
| HP SRAM | 512 KB |
| LP SRAM | 16 KB (retained in deep-sleep) |
| ROM | 320 KB |

Peripherals include 2× UART, SPI, I2C, I2S, **2× TWAI**, USB Serial/JTAG, LED PWM, MCPWM, and GDMA. See [ESP32-C6 Technical Reference Manual](/esp32/esp32-c6-technical-reference-manual.md) for register-level detail.

# Citations

[1] [wiki/raw/esp32/esp32-c6-datasheet-en.md](wiki/raw/esp32/esp32-c6-datasheet-en.md)
