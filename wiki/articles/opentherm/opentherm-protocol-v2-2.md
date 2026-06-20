# OpenTherm Protocol v2.2

> Sources: OpenTherm Association, 2003-02-07
> Raw: [2003-02-07-opentherm-protocol-v2-2](../../raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md)

## Overview

[OpenTherm Protocol Specification v2.2](https://ihormelnyk.com/Content/Pages/opentherm_library/Opentherm%20Protocol%20v2-2.pdf) defines point-to-point communication between a **room unit (master)** and a **boiler (slave)** over a two-wire bus. This project implements the master side on ESP32-C6; the boiler is the slave.

Two protocol levels share the same physical layer:

| Level | Name | Use |
|-------|------|-----|
| OT/+ | OpenTherm/plus | Digital Manchester-encoded frames (target for this firmware) |
| OT/- | OpenTherm/Lite | PWM/analogue fallback |

## Physical layer

| Parameter | Value |
|-----------|-------|
| Wiring | 2-wire, polarity-free, max 50 m |
| Master → slave | Voltage: Vlow ≈ 7 V idle, Vhigh 15..18 V |
| Slave → master | Current: Ilow 5..9 mA idle, Ihigh 17..23 mA |
| Encoding (OT/+) | Manchester, 1000 bit/s, ~1 ms bit period |
| Adapter | MCU GPIO levels need an OpenTherm adapter (7..18 V / mA signalling) |

Galvanic isolation from mains is required on the boiler interface (EN60730-1). Shorting the boiler terminals must register as heat demand within 15 s (installer test).

## Frame format (OT/+)

Each message is a **32-bit frame** with start/stop bits:

```
[P][MSG-TYPE:3][SPARE:4][DATA-ID:8][DATA-VALUE:16]
```

### Message types

| Master sends | Code | Slave replies | Code |
|--------------|------|---------------|------|
| READ-DATA | 000 | READ-ACK | 100 |
| WRITE-DATA | 001 | WRITE-ACK | 101 |
| INVALID-DATA | 010 | DATA-INVALID | 110 |
| — | — | UNKNOWN-DATAID | 111 |

### Timing rules

- Master initiates every conversation; exactly one request + one response
- Slave response window: **20–800 ms** after master frame
- Minimum **100 ms** gap before next master conversation
- Master must send at least one message every **1 s** (max **1.15 s** with tolerance)

Violating the 1 s rule causes boilers to fault or fall back to OT/-.

## Data encoding

Temperatures and setpoints use **f8.8** fixed point: 16-bit signed, 8 fractional bits (divide by 256 for °C).

Example: 21.5°C → `0x1580` (5504 / 256 = 21.5).

Status and configuration fields use **flag8** (8 single-bit flags in one byte).

## Mandatory data IDs

Every OT/+ device must support:

| ID | Name | Direction | Notes |
|----|------|-----------|-------|
| **0** | Status | Master READ | Special exchange: master status in HB, slave status in LB of response |
| **1** | Control setpoint | Master WRITE | CH water temp setpoint 0..100°C; CH enable bit overrides |
| **3** | Slave configuration | Master READ | DHW present, modulating/on-off, cooling, etc. |

Recommended/conditional: ID 14 (max modulation), 17 (relative modulation), 25 (boiler water temp).

## ID 0 — Status bits (most used)

**Master status (HB):**

| Bit | Meaning |
|-----|---------|
| 0 | CH enable |
| 1 | DHW enable |
| 2 | Cooling enable |
| 3 | OTC active |
| 4 | CH2 enable |

**Slave status (LB):**

| Bit | Meaning |
|-----|---------|
| 0 | Fault |
| 1 | CH active |
| 2 | DHW active |
| 3 | Flame on |
| 4 | Cooling active |
| 5 | CH2 active |
| 6 | Diagnostic event |

Initiate with `READ-DATA(id=0, master_status, 0x0000)` → expect `READ-ACK` with slave status. Do not use WRITE for ID 0.

## Common sensor / control IDs

| ID | R/W | Description |
|----|-----|-------------|
| 1 | W | Control setpoint (°C, f8.8) |
| 16 | W | Room setpoint |
| 17 | R | Relative modulation level (0..100%) |
| 25 | R | Boiler flow water temperature |
| 26 | R | DHW temperature |
| 27 | R | Outside temperature |
| 28 | R | Return water temperature |
| 115 | R | OEM diagnostic code |
| 124 | W | OpenTherm version (master) |
| 125 | R | OpenTherm version (slave) |

Full ID map: data classes 1–8 in the specification (IDs 0–127 open area).

## Implementation notes for esp32-c6-opentherm

1. **Role:** ESP32-C6 is the **master** (thermostat/controller); boiler is slave.
2. **Loop:** Poll ID 0 (status) and write ID 1 (setpoint) at least every 1 s; inter-message gap ≥ 100 ms.
3. **Library:** See [Melnyk OpenTherm Library](melnyk-opentherm-library.md) — v2.2 reference implementation for ESP32; port or embed under ESP-IDF.
4. **Hardware:** GPIO cannot drive the bus directly — use an OpenTherm adapter (voltage/current levels per §3.2).

## See Also

- [Melnyk OpenTherm Library](melnyk-opentherm-library.md) — Arduino/ESP32 reference implementation
- [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md)
- [WeAct ESP32-C6-A Dev Board](../esp32/weact-esp32-c6-a-dev-board.md)
- Full spec PDF: [Opentherm Protocol v2-2.pdf](https://ihormelnyk.com/Content/Pages/opentherm_library/Opentherm%20Protocol%20v2-2.pdf)
