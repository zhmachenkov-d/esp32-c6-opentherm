# ESP32-C6 Technical Reference Manual

> Source: https://github.com/WeActStudio/WeActStudio.ESP32-C6-A/blob/main/Doc/esp32-c6_technical_reference_manual_en.pdf
> Collected: 2026-06-21
> Published: 2023

Espressif Systems — Pre-release v0.3 (1277 pages). Upstream: https://www.espressif.com/documentation/esp32-c6_technical_reference_manual_en.pdf

Binary copy: [esp32-c6_technical_reference_manual_en.pdf](esp32-c6_technical_reference_manual_en.pdf)

## About This Document

The ESP32-C6 is targeted at developers working on low level software projects that use the ESP32-C6 SoC. It describes the hardware modules listed below for the ESP32-C6 SoC and other products in ESP32-C6 series. The modules detailed in this document provide an overview, list of features, hardware architecture details, any necessary programming procedures, as well as register descriptions.

## Release Status at a Glance

Note that this manual is still work in progress. See release progress below:

| No. | ESP32-C6 Chapters | Progress |
|-----|-------------------|----------|
| 1 | High-Performance CPU | Published |
| 2 | RISC-V Trace Encoder (TRACE) | Published |
| 3 | Low-Power CPU | Published |
| 4 | GDMA Controller (GDMA) | Published |
| 5 | System and Memory | Published |
| 6 | eFuse Controller | Published |
| 7 | IO MUX and GPIO Matrix (GPIO, IO MUX) | Published |
| 8 | Reset and Clock | Published |
| 9 | Chip Boot Control | Published |
| 10 | Interrupt Matrix (INTMTX) | Published |
| 11 | Event Task Matrix (SOC_ETM) | Published |
| 12 | Low-Power Management | [to be added later] |
| 13 | System Timer (SYSTIMER) | Published |
| 14 | Timer Group (TIMG) | Published |
| 15 | Watchdog Timers (WDT) | Published |
| 16 | Permission Control (PMS) | Published |
| 17 | System Registers (HP_SYSTEM) | Published |
| 18 | Debug Assistant (ASSIST_DEBUG) | Published |
| 19 | AES Accelerator (AES) | Published |
| 20 | ECC Accelerator (ECC) | Published |
| 21 | HMAC Accelerator (HMAC) | Published |
| 22 | RSA Accelerator (RSA) | Published |
| 23 | SHA Accelerator (SHA) | Published |
| 24 | Digital Signature (DS) | Published |
| 25 | External Memory Encryption and Decryption (XTS_AES) | Published |
| 26 | Random Number Generator (RNG) | Published |
| 27 | UART Controller (UART, LP_UART, UHCI) | Published |
| 28 | SPI Controller (SPI) | Published |
| 29 | I2C Controller (I2C) | Published |
| 30 | I2S Controller (I2S) | Published |
| 31 | Pulse Count Controller (PCNT) | Published |
| 32 | USB Serial/JTAG Controller (USB_SERIAL_JTAG) | Published |
| 33 | Two-wire Automotive Interface (TWAI) | Published |
| 34 | SDIO 2.0 Slave Controller (SDIO) | Published |
| 35 | LED PWM Controller (LEDC) | Published |
| 36 | Motor Control PWM (MCPWM) | Published |
| 37 | Remote Control Peripheral (RMT) | Published |
| 38 | Parallel IO Controller (PARL_IO) | Published |
| 39 | On-Chip Sensor and Analog Signal Processing | Published |

Full table of contents with section-level page numbers is in the binary PDF (1277 pages).

## 5 System and Memory

### 5.1 Overview

ESP32-C6 is an ultra-low power and highly-integrated system that integrates:

- a high-performance 32-bit RISC-V single-core processor (HP CPU), four-stage pipeline, clock frequency up to 160 MHz
- a low-power 32-bit RISC-V single-core processor (LP CPU), two-stage pipeline, clock frequency up to 20 MHz

All internal memory, external memory, and peripherals are located on the HP CPU and LP CPU buses.

### 5.2 Features

- Address Space: 832 KB internal memory; 832 KB peripheral space; 16 MB external memory virtual address space; 512 KB internal DMA address space
- Internal Memory: 320 KB ROM; 512 KB HP SRAM; 16 KB LP SRAM
- External Memory: up to 16 MB external flash
- Peripheral Space: 52 modules/peripherals in total
- GDMA: 8 GDMA-supported modules/peripherals

### 5.3.5 Modules/Peripherals Address Mapping (selected)

| Target | Low Address | High Address | Size |
|--------|-------------|--------------|------|
| UART Controller 0 (UART0) | 0x6000_0000 | 0x6000_0FFF | 4 KB |
| UART Controller 1 (UART1) | 0x6000_1000 | 0x6000_1FFF | 4 KB |
| Two-wire Automotive Interface 0 (TWAI0) | 0x6000_B000 | 0x6000_BFFF | 4 KB |
| Two-wire Automotive Interface 1 (TWAI1) | 0x6000_D000 | 0x6000_DFFF | 4 KB |
| USB Serial/JTAG Controller | 0x6000_F000 | 0x6000_FFFF | 4 KB |
| Remote Control Peripheral (RMT) | 0x6000_6000 | 0x6000_6FFF | 4 KB |
| IO MUX | 0x6009_0000 | 0x6009_0FFF | 4 KB |
| GPIO Matrix | 0x6009_1000 | 0x6009_1FFF | 4 KB |
| Power/Clock/Reset (PCR) Register | 0x6009_6000 | 0x6009_6FFF | 4 KB |

HP CPU can access all peripherals listed; LP CPU can access all except TRACE, ASSIST_DEBUG, and INTPRI.

## 7 IO MUX and GPIO Matrix (GPIO, IO MUX)

### 7.1 Overview

The ESP32-C6 chip features 31 GPIO pins. Each pin can be used as a general-purpose I/O, or be connected to an internal peripheral signal. Through GPIO matrix, IO MUX, and low-power (LP) IO MUX, peripheral input signals can be from any IO pins, and peripheral output signals can be routed to any GPIO pins.

Note:

- The 31 GPIO pins are numbered from GPIO0 ~ GPIO30.
- For chip variants without an in-package flash, GPIO14 is not led out to any chip pins.
- For chip variants with an in-package flash, GPIO24 ~ GPIO30 are dedicated to connecting the in-package flash. GPIO10 ~ GPIO11 are not led out. The remaining 22 GPIO pins (GPIO0 ~ GPIO9, GPIO12 ~ GPIO23) are configurable by users.

### 7.2 Features

GPIO matrix:

- Full-switching matrix between peripheral input/output signals and GPIO pins
- 85 peripheral input signals sourced from any GPIO pins
- 93 peripheral output signals routed to any GPIO pins
- Signal synchronization, GPIO Filter, Glitch Filter, SDM output

IO MUX:

- Better high-frequency digital performance for SPI, JTAG, UART bypassing GPIO matrix
- Per-pin configuration register IO_MUX_GPIOn_REG

LP IO MUX:

- Control of eight LP GPIO pins (GPIO0 ~ GPIO7) for ULP and LP system peripherals

### 7.11 Peripheral Signal List (TWAI entries)

| No. | Input Signal | Output Signal | Direct IO MUX |
|-----|--------------|---------------|---------------|
| 73 | twai0_rx | twai0_tx | no |
| 77 | twai1_rx | twai1_tx | no |

TWAI signals route via GPIO matrix (not direct IO MUX). Additional TWAI outputs: twai0/1_bus_off_on, twai0/1_clkout, twai0/1_standby.

## 32 Two-wire Automotive Interface (TWAI)

The Two-wire Automotive Interface (TWAI®) is a multi-master, multi-cast communication protocol with functions such as error detection and signaling and inbuilt message priorities and arbitration. The TWAI protocol is suited for automotive and industrial applications.

ESP32-C6 contains two TWAI controllers named TWAI 0 and TWAI 1. Each controller can individually be connected to a TWAI bus via an external transceiver.

### 32.1 Features

Each of the two TWAI controllers on ESP32-C6 support the following features:

- Compatibility with ISO 11898-1 protocol (CAN Specification 2.0)
- Standard Frame Format (11-bit ID) and Extended Frame Format (29-bit ID)
- Bit rates from 1 Kbit/s to 1 Mbit/s
- Multiple modes of operation: Normal; Listen-only (no influence on bus); Self-test (no acknowledgment required during data transmission)
- 64-byte Receive FIFO
- Special transmissions: Single-shot transmissions; Self Reception
- Acceptance Filter (supports single and dual filter modes)
- Error detection and handling: Error Counters; Configurable Error Warning Limit; Error Code Capture; Arbitration Lost Capture; Automatic Transceiver Standby

### 32.2.1 TWAI Properties

- Single Channel and Non-Return-to-Zero: half-duplex bus; NRZ encoding
- Bit Values: dominant (logical 0) overrides recessive (logical 1)
- Bit Stuffing: after five consecutive identical bits, a complementary bit is inserted
- Multi-cast: all nodes receive the same bits
- Multi-master: any node can initiate transmission
- Message Priority and Arbitration: arbitration field determines bus winner
- Error Detection and Signaling: nodes transmit error frames on detected errors
- Fault Confinement: error counters; node switches off when threshold exceeded
- Configurable Bit Rate: all nodes on a bus must use the same rate

### 32.4.1 Modes

The ESP32-C6 TWAI controller has two working modes: Reset Mode and Operation Mode (TWAI_RESET_MODE bit).

**Reset Mode** — Required to modify configuration registers. Controller disconnected from bus; cannot transmit or receive.

**Operation Mode** — Controller connects to bus; configuration registers write-protected. Sub-modes:

- Normal Mode: transmit and receive including error/overload frames
- Self-test Mode: transmission considered successful without ACK
- Listen-only Mode: receive only; passive on bus; error counters frozen

When exiting Reset Mode, controller waits for 11 consecutive recessive bits before fully connecting.

### 32.4.2 Bit Timing

Bit rate configured in Reset Mode via TWAI_BUS_TIMING_0_REG (0x18) and TWAI_BUS_TIMING_1_REG (0x1c):

- BRP: Baud Rate Prescaler — Time Quanta clock derived from TWAI core clock (default XTAL 40 MHz)
- SJW: Synchronization Jump Width
- PBS1, PBS2: Phase Buffer Segment lengths
- SAM: triple sampling enable for low/medium speed buses

### 32.4.3 Interrupts

Eight interrupts in TWAI_INT_ST_REG (enabled via TWAI_INT_ENA_REG):

- Receive Interrupt (RXI)
- Transmit Interrupt (TXI)
- Error Warning Interrupt (EWI)
- Data Overrun Interrupt (DOI)
- Error Passive Interrupt
- Arbitration Lost Interrupt (ALI)
- Bus Error Interrupt (BEI)
- Bus Idle Status Interrupt (BISI)

### 32.3.1 Register access note

Most TWAI registers only contain useful data at bits [7:0]; bits [31:8] ignored on writes, return 0 on reads.

Transmit Buffer, Receive Buffer, and Acceptance Filter share address range 0x0040–0x0070:

- Reset Mode: all reads/writes map to Acceptance Filter registers
- Operation Mode: reads map to Receive Buffer; writes map to Transmit Buffer

### 32.4.7 Error Management

Each node maintains Transmit Error Counter (TEC) and Receive Error Counter (REC) in TWAI_TX_ERR_CNT_REG and TWAI_RX_ERR_CNT_REG.

- Error Warning Limit (EWL): default 96; triggers interrupt when TEC or REC ≥ EWL
- Error Passive: TEC or REC > 127
- Bus-Off: TEC > 255; controller enters Reset Mode; recovery requires 128 occurrences of 11 consecutive recessive bits
