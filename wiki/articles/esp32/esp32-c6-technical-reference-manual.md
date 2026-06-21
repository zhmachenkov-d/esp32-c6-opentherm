# ESP32-C6 Technical Reference Manual

> Sources: Espressif Systems, 2023 (Pre-release v0.3)
> Raw: [2023-esp32-c6-technical-reference-manual-en](../../raw/esp32/2023-esp32-c6-technical-reference-manual-en.md)

## Overview

The ESP32-C6 Technical Reference Manual (TRM, pre-release v0.3, 1277 pages) is Espressif's register-level documentation for low-level software on the ESP32-C6 SoC. It covers 39 published hardware chapters — CPUs, memory map, GPIO, clocks, boot, crypto accelerators, and peripherals — with architecture diagrams, programming procedures, and register descriptions. Chapter 12 (Low-Power Management) is not yet published.

This wiki article distills the sections most relevant to firmware and hardware design on this project. Full register tables and remaining chapters are in the raw source and bundled PDF.

## Document status

| Field | Value |
|-------|-------|
| Version | Pre-release v0.3 |
| Pages | 1277 |
| Published chapters | 39 of ~39 listed (LP Management pending) |
| Audience | Low-level driver and register programming |

## System and memory

ESP32-C6 integrates an HP RISC-V core (up to 160 MHz, 4-stage) and an LP RISC-V core (up to 20 MHz, 2-stage) sharing buses to internal memory, external flash, and 52 peripheral modules.

| Resource | Size / range |
|----------|--------------|
| ROM | 320 KB @ 0x4000_0000 |
| HP SRAM | 512 KB @ 0x4080_0000 |
| LP SRAM | 16 KB @ 0x5000_0000 (retained in Deep-sleep) |
| External flash (via cache) | up to 16 MB @ 0x4200_0000 |
| Peripheral space | 832 KB @ 0x6000_0000 |
| L1 cache | 32 KB, 4-way set-associative |

LP SRAM supports high-speed mode (HP clock) or low-speed mode (LP clock) via `LP_AON_FAST_MEM_MUX_SEL`. When HP CPU sleeps, LP SRAM must be in low-speed mode.

## Peripheral address map (selected)

| Module | Base address | Size |
|--------|--------------|------|
| UART0 | 0x6000_0000 | 4 KB |
| UART1 | 0x6000_1000 | 4 KB |
| TWAI0 | 0x6000_B000 | 4 KB |
| TWAI1 | 0x6000_D000 | 4 KB |
| USB Serial/JTAG | 0x6000_F000 | 4 KB |
| RMT | 0x6000_6000 | 4 KB |
| IO MUX | 0x6009_0000 | 4 KB |
| GPIO Matrix | 0x6009_1000 | 4 KB |
| PCR (clock/reset) | 0x6009_6000 | 4 KB |

HP CPU accesses all peripherals; LP CPU accesses all except TRACE, ASSIST_DEBUG, and INTPRI.

## GPIO, IO MUX, and matrix

ESP32-C6 exposes 31 GPIO pins (GPIO0–GPIO30). Routing uses three layers:

1. **GPIO matrix** — full crossbar: 85 peripheral inputs from any pin, 93 outputs to any pin; includes sync, filter, and glitch filter.
2. **IO MUX** — direct pin-to-peripheral paths for high-speed signals (SPI, JTAG, UART); per-pin `IO_MUX_GPIOn_REG`.
3. **LP IO MUX** — GPIO0–GPIO7 for LP peripherals (LP UART, LP I2C).

Package constraints (from TRM Ch. 7):

- **No in-package flash (QFN40):** 30 usable GPIOs; GPIO14 not bonded.
- **In-package flash (QFN32/FH4):** 22 usable GPIOs (GPIO0–9, GPIO12–23); GPIO24–30 reserved for flash.

TWAI signals (`twai0_rx`/`twai0_tx`, `twai1_rx`/`twai1_tx`) route through the GPIO matrix only — not direct IO MUX. Assign pins via `GPIO_FUNCy_IN_SEL_CFG_REG` and matrix output selectors.

Default reset states and drive strengths for each pin are in TRM Table 7-3 (IO MUX Functions List). Boot strapping on GPIO8/GPIO9 is covered in Ch. 9 and the [Hardware Design Guidelines](esp32-c6-hardware-design-guidelines.md).

## TWAI controllers (Ch. 32)

ESP32-C6 has two independent TWAI controllers (TWAI0 @ 0x6000_B000, TWAI1 @ 0x6000_D000). Each connects to a bus through an external transceiver.

### Protocol and features

- ISO 11898-1 / CAN 2.0 compatible
- Standard (11-bit) and Extended (29-bit) frame formats
- Bit rate: 1 Kbit/s – 1 Mbit/s
- 64-byte receive FIFO; 13-byte TX/RX buffers
- Acceptance filter: single or dual filter mode
- Error counters (TEC/REC), error code capture, arbitration-lost capture
- Auto transceiver standby support

### Operating modes

| Mode | Behavior |
|------|----------|
| Reset | Config registers writable; disconnected from bus |
| Normal | Full TX/RX including error frames |
| Self-test | TX succeeds without ACK (loopback testing) |
| Listen-only | RX only; passive; error counters frozen |

Enter Reset Mode to configure bit timing (`TWAI_BUS_TIMING_0/1_REG`), acceptance filter, and error warning limit (default EWL = 96). On exit, wait for 11 recessive bits before bus activity.

Bit timing uses BRP (prescaler from TWAI core clock, default 40 MHz XTAL), SJW, PBS1, PBS2, and optional triple sampling (SAM).

### Interrupts

Eight interrupt sources: receive, transmit, error warning, data overrun, error passive, arbitration lost, bus error, bus idle. Enable per-bit in `TWAI_INT_ENA_REG`; status in `TWAI_INT_ST_REG`.

### Error states

| State | Condition |
|-------|-----------|
| Error Active | TEC and REC ≤ 127 |
| Error Passive | TEC or REC > 127 |
| Bus-Off | TEC > 255; auto Reset Mode; recovery needs 128 × 11 recessive bits |

### OpenTherm note

OpenTherm is **not** CAN/TWAI — it uses Manchester-encoded single-wire frames at ~1 kHz bit timing with different voltage levels. The TWAI peripheral is CAN-compatible (dominant/recessive, NRZ, ISO 11898). Do not drive OpenTherm directly from TWAI pins. For this project, TWAI matters for:

- Understanding the chip's field-bus peripheral options and GPIO matrix routing
- Potential reuse of TWAI transceiver hardware knowledge for isolation/timing design
- ESP-IDF `driver/twai.h` maps to these registers if CAN bus features are added alongside OpenTherm

## Other published chapters (index)

Full register detail for these modules is in the PDF; see raw source for the complete chapter list:

- Ch. 1–3: HP CPU, TRACE, LP CPU
- Ch. 4: GDMA (3 TX + 3 RX channels)
- Ch. 6: eFuse
- Ch. 8–11: Reset/clock, boot control, interrupt matrix, event task matrix
- Ch. 13–15: System timer, timer groups, watchdogs
- Ch. 16–26: Permission control, system registers, debug, crypto (AES/ECC/HMAC/RSA/SHA/DS/XTS-AES), RNG
- Ch. 27–31, 34–39: UART, SPI, I2C, I2S, PCNT, USB Serial/JTAG, SDIO, LEDC, MCPWM, RMT, PARLIO, ADC/sensors

## Relevance to this project

- **Register-level GPIO mux** — Confirm OpenTherm interface pin choices against Ch. 7 signal tables and the [WeAct schematic](weact-esp32-c6-a-dev-board.md).
- **TWAI0/1** — Two CAN controllers available if the design adds CAN alongside OpenTherm; not a substitute for OpenTherm timing.
- **Memory map** — LP SRAM retention and cache behavior inform sleep strategies for always-on thermostat duty cycles.
- **Boot control (Ch. 9)** — GPIO8/GPIO9 strapping for UART download aligns with [Hardware Design Guidelines](esp32-c6-hardware-design-guidelines.md).
- **ESP-IDF** — Higher-level drivers wrap these registers; use TRM when debugging low-level timing, clock gating (PCR), or custom bare-metal code.

## See Also

- [ESP32-C6 Series Datasheet](esp32-c6-datasheet.md)
- [ESP32-C6 Hardware Design Guidelines](esp32-c6-hardware-design-guidelines.md)
- [WeAct ESP32-C6-A Dev Board](weact-esp32-c6-a-dev-board.md)
- [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md)
- [OpenTherm Protocol v2.2](../opentherm/opentherm-protocol-v2-2.md)
- Upstream PDF: https://www.espressif.com/documentation/esp32-c6_technical_reference_manual_en.pdf
