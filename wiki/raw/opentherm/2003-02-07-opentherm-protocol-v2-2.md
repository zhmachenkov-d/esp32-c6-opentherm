# OpenTherm Protocol Specification v2.2

> Source: https://ihormelnyk.com/Content/Pages/opentherm_library/Opentherm%20Protocol%20v2-2.pdf
> Collected: 2026-06-20
> Published: 2003-02-07

The OpenTherm Communications Protocol — A Point-to-Point Communications System for HVAC Controls. Protocol Specification. The OpenTherm Association.

Version 2.2 RELEASE — 7 February 2003. Approved by members of the OpenTherm Association.

## 1. Introduction

### 1.1 Background

OpenTherm was developed for simple point-to-point communication between room controllers and boilers with very low entry-level costs, where home-system buses are too expensive.

### 1.2 Key OpenTherm Characteristics

- Compatibility with non-intelligent boiler systems and low-cost room thermostats
- Two-wire, polarity-free connection for concurrent power supply and data transmission
- Power supply for room controller from boiler line (no batteries required)
- Implementable on low-cost microcontrollers
- Shorting wires at boiler simulates maximum heat demand (installer test feature)
- Transfer of sensor, fault, and configuration data
- Mandatory minimum data objects for modulating control signal

**OT/+** (OpenTherm/plus): digital communications between microprocessor-based devices.

**OT/-** (OpenTherm/Lite): PWM and simple signalling for analogue-only products.

Both use the same physical layer.

### 1.4 Nomenclature

- Room Unit — calculates demand (master in practice)
- Boiler Unit — receives demand (slave)
- CH — Central Heating; DHW — Domestic Hot Water
- AL/DLL/PL — Application/Data-Link/Physical Layer

## 2. System Overview

OpenTherm is point-to-point: one room controller to one boiler. The room controller transmits a water temperature Control Setpoint; the boiler returns fault and system information.

On power-up, room unit tries OT/+ for 20 seconds; if no valid response, switches to OT/-. OT/+ boiler must start within 20 seconds or OT/+ mode requires reset/reconnect.

## 3. Physical Layer

### 3.1 Transmission Line

- 2 wires, polarity-free, untwisted pair (twisted/screened in noisy environments)
- Max length: 50 m; max cable resistance: 2 × 5 Ω

### 3.2 Signal Levels

**Boiler → Room (current):**
- Ihigh: 17..23 mA
- Ilow: 5..9 mA (idle)
- Max open-circuit voltage: 42 Vdc
- Rise/fall time: 50 µs max
- Receive threshold (master): 11.5..14.5 mA, nominal 13 mA

**Room → Boiler (voltage):**
- Vhigh: 15..18 V
- Vlow: 7 V max (idle)
- Rise/fall time: 50 µs max
- Receive threshold (slave): 9.5..12.5 V, nominal 11 V

### 3.4 OT/+ Bit Encoding

- Manchester / bi-phase-L encoding
- Bit '1': active-to-idle transition; Bit '0': idle-to-active transition
- Bit rate: 1000 bit/s nominal; mid-bit transition period 900..1150 µs (nominal 1 ms)

### 3.5 Short-Circuit Feature

Boiler must interpret terminal short-circuit as heat demand within 15 s. Software equivalent: Vlow with no valid frame for 5..15 s.

## 4. OT/+ Data-Link Layer

### 4.2 Frame Format

32-bit frame plus start bit ('1') and stop bit ('1'):

```
| P | MSG-TYPE(3) | SPARE(4) | DATA-ID(8) | DATA-VALUE(16) |
```

- Parity P: even parity over 32 bits
- SPARE: always 0

### 4.2.2 Message Types

| Value | Master→Slave | Value | Slave→Master |
|-------|--------------|-------|--------------|
| 000 | READ-DATA | 100 | READ-ACK |
| 001 | WRITE-DATA | 101 | WRITE-ACK |
| 010 | INVALID-DATA | 110 | DATA-INVALID |
| 011 | reserved | 111 | UNKNOWN-DATAID |

### 4.3 Conversation Format

- Master always initiates; slave responds once
- Slave response: 20 ms to 800 ms after master frame ends
- Master waits min 100 ms between conversations
- Master must communicate at least every 1 s (+15% → max 1.15 s)
- Single request/response per conversation

### 4.4 Read/Write

**READ-DATA:** master requests data-id; slave responds READ-ACK, DATA-INVALID, or UNKNOWN-DATAID.

**WRITE-DATA:** master writes data-id/value; slave responds WRITE-ACK, DATA-INVALID, or UNKNOWN-DATAID.

Default data-value when unused: 0x0000.

## 5. OT/+ Application Layer

### 5.1 Data-ID Ranges

- ID 0..127: OpenTherm pre-defined
- ID 128..255: Test & Diagnostic (members only)

Seven data classes: Control/Status, Configuration, Remote Commands, Sensor/Informational, Remote Boiler Parameters, Transparent Slave Parameters, Fault History, Special Applications.

### 5.1 Data Types

- f8.8: signed fixed point, 8 fractional bits (LSB = 1/256). Example: 21.5°C = 0x1580
- flag8, u8, s8, u16, s16

### 5.2 Mandatory Data Items

| ID | Description | Master | Slave |
|----|-------------|--------|-------|
| 0 | Status flags | READ_DATA required; all master status bits | READ_ACK; all slave status bits |
| 1 | Control setpoint (CH water temp) | WRITE_DATA or INVALID_DATA | WRITE_ACK |
| 3 | Slave configuration flags | READ_DATA (at least at startup) | READ_ACK |
| 14 | Max relative modulation level | recommended on-off mode | WRITE_ACK |
| 17 | Relative modulation level | optional | READ_ACK or DATA_INVALID |
| 25 | Boiler water temperature | optional | READ_ACK or DATA_INVALID |

### 5.3.1 Class 1 — Key IDs

**ID 0 — Status (special READ exchange):**
Master sends READ-DATA(id=0, MasterStatus, 00). Slave responds READ-ACK(id=0, MasterStatus, SlaveStatus).

Master status (HB): bit0 CH enable, bit1 DHW enable, bit2 Cooling enable, bit3 OTC active, bit4 CH2 enable.

Slave status (LB): bit0 fault, bit1 CH active, bit2 DHW active, bit3 flame on, bit4 cooling, bit5 CH2, bit6 diagnostic.

**ID 1 — Control setpoint:** f8.8, 0..100°C. CH enable bit overrides setpoint.

**ID 5 — Application fault flags / OEM fault code**

**ID 8 — Control setpoint CH2 (TsetCH2)**

### 5.3.4 Class 4 — Sensor IDs (selected)

| ID | Name | Type |
|----|------|------|
| 16 | Room setpoint | f8.8 |
| 17 | Relative modulation level | f8.8 |
| 18 | CH water pressure | f8.8 bar |
| 25 | Boiler water temp | f8.8 |
| 26 | DHW temperature | f8.8 |
| 27 | Outside temperature | f8.8 |
| 28 | Return water temperature | f8.8 |
| 116-123 | Burner/pump starts and operation hours | u16 |

### 5.3.3 Class 3 — Remote Commands (ID 4)

Command codes: 1=BLOR (boiler lock-out reset), 2=CHWF (CH water filling).

## 6. OT/- (OpenTherm/Lite)

PWM-based low-end protocol on same physical layer; application data equivalence defined in section 6 of full specification.
