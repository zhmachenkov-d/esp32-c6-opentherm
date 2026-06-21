# ESP32-C6 Series Datasheet

> Sources: Espressif Systems, 2023 (v0.5 pre-release)
> Raw: [esp32-c6-datasheet-en](../../raw/esp32/esp32-c6-datasheet-en.md)

## Overview

The ESP32-C6 is an ultra-low-power SoC from Espressif combining a high-performance (HP) RISC-V core, a low-power (LP) RISC-V core, and tri-radio wireless: 2.4 GHz Wi-Fi 6 (802.11ax), Bluetooth 5 LE, and IEEE 802.15.4 (Zigbee 3.0 / Thread 1.3). All radios share one antenna with internal coexistence. The chip targets IoT hubs, smart-home devices, and low-power sensor nodes.

Two ordering codes ship in different packages:

| Ordering code | In-package flash | Package | GPIOs |
|---------------|------------------|---------|-------|
| ESP32-C6 | None (external flash) | QFN40 (5×5 mm) | 30 |
| ESP32-C6FH4 | 4 MB Quad SPI | QFN32 (5×5 mm) | 22 |

Ambient temperature range: −40 °C to 105 °C for both variants.

## Wireless

**Wi-Fi 6 (802.11ax)** — 1T1R, 2.4 GHz (2412–2484 MHz). 20 MHz non-AP mode, MCS0–9, OFDMA, MU-MIMO, beamformee, TWT. Backward compatible with 802.11b/g/n (up to 150 Mbps, 20/40 MHz). Four virtual interfaces; Station, SoftAP, combined, and promiscuous modes.

**Bluetooth LE** — Bluetooth 5.3 certified; mesh; 125 Kbps–2 Mbps; high-power mode (+20 dBm); advertising extensions; internal coexistence with Wi-Fi.

**IEEE 802.15.4** — OQPSK PHY, 250 Kbps, 2.4 GHz; Thread 1.3 and Zigbee 3.0 compliant.

## CPU and memory

| Resource | Specification |
|----------|---------------|
| HP RISC-V | Up to 160 MHz, 4-stage pipeline; CoreMark 441.32 @ 160 MHz |
| LP RISC-V | Up to 20 MHz, 2-stage pipeline |
| L1 cache | 32 KB |
| ROM | 320 KB |
| HP SRAM | 512 KB |
| LP SRAM | 16 KB (retained in Deep-sleep) |

External flash via SPI / Dual SPI / Quad SPI / QPI; flash controller with cache; in-circuit programming (ICP) supported. Flash up to 4 MB (in-package on FH4 variant).

## Peripherals

**Analog** — 12-bit SAR ADC (up to 7 channels); on-chip temperature sensor.

**Digital** — 2× UART, 1× LP UART, 2× flash SPI + 1× general SPI, I2C + LP I2C, I2S, pulse counter, USB Serial/JTAG, **2× TWAI** (ISO 11898-1 / CAN 2.0), SDIO 2.0 slave, LED PWM (6 ch), MCPWM, RMT, PARLIO, GDMA (3 TX / 3 RX), ETM.

**Timers** — 52-bit system timer, 2× 54-bit GP timers, 3× digital watchdogs, 1× analog watchdog.

GPIO count depends on package: 30 (QFN40) or 22 (QFN32). IO MUX supports multiple alternate functions per pin; LP IO MUX for low-power GPIO. See the raw datasheet for pin tables and strapping-pin defaults.

## Power management

Four modes: Active, Modem-sleep, Light-sleep, Deep-sleep. Deep-sleep current ~7 µA; LP memory stays powered. Fine-grained control via clock frequency, duty cycle, Wi-Fi mode, and per-block power gating.

## Security

Secure boot, flash encryption, 4096-bit OTP (1792 bits user), TEE controller, APM. Hardware crypto: AES-128/256, ECC, HMAC, RSA, SHA, digital signature, XTS-AES for external memory, RNG.

## RF performance

Integrated balun, PA, and LNA. TX power up to +21 dBm (802.11b) and +19.5 dBm (802.11ax). BLE RX sensitivity down to −106 dBm @ 125 Kbps.

## Relevance to this project

This repo targets ESP32-C6 for OpenTherm boiler communication. Key datasheet takeaways:

- **TWAI controllers** — Two CAN-compatible peripherals; OpenTherm is not CAN but shares timing/isolation concerns with other field-bus interfaces.
- **802.15.4 / Zigbee** — On-chip radio supports Zigbee thermostat clusters documented in the [Zigbee Cluster Library](../zigbee/zigbee-cluster-library.md).
- **GPIO budget** — QFN40 (ESP32-C6) exposes 30 GPIOs; confirm pin mux against the [WeAct ESP32-C6-A](weact-esp32-c6-a-dev-board.md) schematic when wiring the OpenTherm interface.
- **Low power** — Deep-sleep at 7 µA suits always-on thermostat / gateway duty cycles.

Full pin maps, electrical limits, and RF tables are in the raw source; use the [Technical Reference Manual](esp32-c6-technical-reference-manual.md) for register-level detail.

## See Also

- [ESP32-C6 Technical Reference Manual](esp32-c6-technical-reference-manual.md)
- [ESP32-C6 Hardware Design Guidelines](esp32-c6-hardware-design-guidelines.md)
- [WeAct ESP32-C6-A Dev Board](weact-esp32-c6-a-dev-board.md)
- [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md)
- [Zigbee Cluster Library](../zigbee/zigbee-cluster-library.md)
- Espressif datasheet (upstream): https://www.espressif.com/sites/default/files/documentation/esp32-c6_datasheet_en.pdf
