# ESP32-C6 Series Datasheet

> Source: https://github.com/WeActStudio/WeActStudio.ESP32-C6-A/blob/main/Doc/esp32-c6_datasheet_en.pdf
> Collected: 2026-06-21
> Published: 2023 (v0.5 pre-release)

Upstream Espressif document: https://www.espressif.com/sites/default/files/documentation/esp32-c6_datasheet_en.pdf

ESP32-C6 Series
Datasheet
Ultra-low-power SoC with RISC-V single-core microprocessor
2.4 GHz Wi-Fi 6 (802.11 ax), Bluetooth ® 5 (LE), Zigbee and Thread (802.15.4)
Flash up to 4 MB
30 or 22 GPIOs, rich set of peripherals
QFN40 (5×5 mm) or QFN32 (5×5 mm) package
Including:
ESP32-C6
ESP32-C6FH4
Pre-release v0.5
Espressif Systems
Copyright © 2023
www.espressif.com
Product Overview
The ESP32-C6 SoC (System on Chip) supports Wi-Fi 6 in 2.4 GHz band, Bluetooth 5, Zigbee 3.0 and Thread
1.3. It consists of a high-performance (HP) 32-bit RISC-V processor, an low-power (LP) 32-bit RISC-V processor,
wireless baseband and MAC (Wi-Fi, Bluetooth LE, and 802.15.4), RF module, and numerous peripherals. Wi-Fi,
Bluetooth and 802.15.4 coexist with each other and share the same antenna.
The functional block diagram of the SoC is shown below.
CPU System Wireless MAC and 
Baseband
Wi-Fi MAC Wi-Fi 
Baseband
Bluetooth LE 
Link Controller
Bluetooth LE 
Baseband
2.4 GHz Balun + Switch
2.4 GHz Receiver
2.4 GHz Transmitter
RF Synthesizer
RF
Security
HP RISC-V
32-bit
Microprocessor
JTAG
Cache
Peripherals 
Espressif’s ESP32-C6 Wi-Fi + Bluetooth® Low Energy + 802.15.4 SoC
SRAM
MCPWM
GPIO
UARTTWAI®
General-Purpose 
Timers
I2S
I2C
PCNT
LED PWM
USB Serial/
JTAG
SPI
RMT
ADC
System Timer
LP IO
Temperature 
Sensor
LP 
Memory
RTC Watchdog 
Timer
Power Management Unit
Power Management
Digital 
SignatureAES HMAC⚙⚙ ⚙
Modules having power in speciﬁc power modes:
Active
Active and Modem-sleep
Active, Modem-sleep, Light-sleep; optional in Light-sleep⚙
GDMA
ETM⚙
SDIO 2.0 
Slave
802.15.4
Baseband
802.15.4
MAC
RSASHA⚙ ⚙ ECC⚙
Brownout
Detector
LP UART
LP I2C
LP RISC-V
32-bit
Microprocessor
All modes
ROM
PARLIO
eFuse 
Controller
Main System 
Watchdog Timers
Super 
Watchdog
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
⚙
RNG Clock 
Glitch Filter
TEE
Controller
⚙ ⚙ ⚙
Flash 
Encryption
Secure 
Boot
⚙ APM⚙
⚙optional in Deep-sleep
⚙
⚙
⚙
⚙
ESP32-C6 Functional Block Diagram
For more information on power consumption, see Section3.8 Low Power Management.
Features
Wi-Fi
• 1T1R in 2.4 GHz band
• Operating frequency: 2412~ 2484 MHz
• IEEE 802.11ax-compliant
– 20 MHz-only non-AP mode
– MCS0 ~MCS9
– Uplink and downlink OFDMA, especially
suitable for simultaneous connections in
high-density environments
– Downlink MU-MIMO (multi-user, multiple
input, multiple output) to increase network
capacity
– Beamformee that improves signal quality
– Channel quality indication (CQI)
– DCM (dual carrier modulation) to improve
link robustness
– Spatial reuse to maximize parallel
transmissions
– Target wake time (TWT) that optimizes
power saving mechanisms
• Fully compatible with IEEE 802.11b/g/n protocol
– 20 MHz and 40 MHz bandwidth
– Data rate up to 150 Mbps
– Wi-Fi Multimedia (WMM)
– TX/RX A-MPDU, TX/RX A-MSDU
– Immediate Block ACK
– Fragmentation and defragmentation
– Transmit opportunity (TXOP)
– Automatic Beacon monitoring (hardware
TSF)
– 4 × virtual Wi-Fi interfaces
– Simultaneous support for Infrastructure BSS
in Station mode, SoftAP mode, Station +
SoftAP mode, and promiscuous mode
Note that when ESP32-C6 scans in Station
mode, the SoftAP channel will change along
with the Station channel
– Antenna diversity
– 802.11mc FTM
Bluetooth®
• Bluetooth LE: Bluetooth 5.3 certified
• Bluetooth mesh
• High power mode (20 dBm)
• Speed: 125 Kbps, 500 Kbps, 1 Mbps, 2 Mbps
• Advertising extensions
• Multiple advertisement sets
• Channel selection algorithm #2
• LE power control
• Internal co-existence mechanism between Wi-Fi
and Bluetooth to share the same antenna
IEEE 802.15.4
• Compliant with IEEE 802.15.4-2015 protocol
• OQPSK PHY in 2.4 GHz band
• Data rate: 250 Kbps
• Thread 1.3
• Zigbee 3.0
CPU and Memory
• HP RISC-V processor:
– Clock speed: up to 160 MHz
– Four stage pipeline
– CoreMark® score: 441.32 CoreMarkĠ2.76
CoreMark/MHz (160 MHz)
• LP RISC-V processor:
– Clock speed: up to 20 MHz
– Two stage pipeline
• L1 cache: 32 KB
• ROM: 320 KB
• HP SRAM: 512 KB
• LP SRAM: 16 KB
• Supported SPI protocols: SPI, Dual SPI, Quad
SPI, QPI interfaces that allow connection to flash,
and other SPI devices
• Flash controller with cache is supported
• Flash in-Circuit Programming (ICP) is supported
Advanced Peripheral Interfaces
• 30 × GPIOs (QFN40), or 22 × GPIOs (QFN32)
• Analog interfaces:
– 1 × 12-bit SAR ADC, up to 7 channels
– 1 × temperature sensor
• Digital interfaces:
– 2 × UART
– 1 × Low-power (LP) UART
– 2 × SPI ports for communication with flash
– 1 × General purpose SPI port
– 1 × I2C
– 1 × Low-power (LP) I2C
– 1 × I2S
– 1 × Pulse count controller
– 1 × USB Serial/JTAG controller
– 2 × TWAI® controller, compatible with ISO
11898-1 (CAN Specification 2.0)
– 1 × SDIO 2.0 slave controller
– LED PWM controller, up to 6 channels
– 1 × Motor Control PWM (MCPWM)
– 1 × Remote control peripheral (TX/RX)
– 1 × Parallel IO interface (PARLIO)
– General DMA controller, with 3 transmit
channels and 3 receive channels
– Event task matrix (ETM)
• Timers:
– 1 × 52-bit system timer
– 2 × 54-bit general-purpose timers
– 3 × digital watchdog timers
– 1 × analog watchdog timer
Power Management
• Fine-resolution power control through a selection
of clock frequency, duty cycle, Wi-Fi operating
modes, and individual power control of internal
components
• Four power modes designed for typical
scenarios: Active, Modem-sleep, Light-sleep,
Deep-sleep
• Power consumption in Deep-sleep mode is 7µA
• Low-power (LP) memory remains powered on in
Deep-sleep mode
Security
• Secure boot - permission control on accessing
internal and external memory
• Flash encryption - memory encryption and
decryption
• 4096-bit OTP, up to 1792 bits for users
• Trusted execution environment (TEE) controller
and access permission management (APM)
• Cryptographic hardware acceleration:
– AES-128/256 (FIPS PUB 197)
– ECC
– HMAC
– RSA
– SHA
– Digital signature
– Hash (FIPS PUB 180-4)
• External Memory Encryption and Decryption
(XTS_AES)
• Random Number Generator (RNG)
RF Module
• Antenna switches, RF balun, power amplifier,
low-noise receive amplifier
• Up to +21 dBm of power for an 802.11b
transmission
• Up to +19.5 dBm of power for an 802.11ax
transmission
• Up to -106 dBm of sensitivity for Bluetooth LE
receiver (125 Kbps)
Applications
With low power consumption, ESP32-C6 is an ideal choice for IoT devices in the following areas:
• Smart Home
• Industrial Automation
• Health Care
• Consumer Electronics
• Smart Agriculture
• POS machines
• Service robot
• Audio Devices
• Generic Low-power IoT Sensor Hubs
• Generic Low-power IoT Data Loggers
Contents
Contents
Product Overview 2
Features 3
Applications 5
1 ESP32-C6 Series Comparison 12
1.1 Nomenclature 12
1.2 Comparison 12
2 Pins 13
2.1 Pin Layout 13
2.2 Pin Overview 15
2.3 IO Pins 17
2.3.1 IO MUX and GPIO Pin Functions 17
2.3.2 LP IO MUX Functions 21
2.3.3 Analog Functions 21
2.3.4 Restrictions for GPIOs and LP GPIOs 23
2.4 Analog Pins 23
2.5 Power Supply 24
2.5.1 Power Pins 24
2.5.2 Power Scheme 24
2.5.3 Chip Power-up and Reset 25
2.6 Strapping Pins 26
2.6.1 SDIO Sampling and Driving Clock Edge Control 27
2.6.2 Chip Boot Mode Control 27
2.6.3 ROM Messages Printing Control 28
2.6.4 JTAG Signal Source Control 28
2.7 Pin Mapping Between Chip and Flash 30
3 Functional Description 31
3.1 CPU and Memory 31
3.1.1 HP CPU 31
3.1.2 LP CPU 31
3.1.3 Internal Memory 32
3.1.4 External Flash 32
3.1.5 Address Mapping Structure 33
3.1.6 Cache 33
3.1.7 TEE Controller 33
3.1.8 Access Permission Management (APM) 33
3.1.9 Timeout Protection 34
3.2 System Clocks 34
3.2.1 CPU Clock 34
3.2.2 Low-Power Clocks 34
3.3 Analog Peripherals 35
Contents
3.3.1 Analog-to-Digital Converter (ADC) 35
3.3.2 Temperature Sensor 35
3.4 Digital Peripherals 35
3.4.1 Universal Asynchronous Receiver Transmitter (UART) 35
3.4.2 Serial Peripheral Interface (SPI) 35
3.4.3 I2C Interface 36
3.4.4 I2S Interface 36
3.4.5 Pulse Count Controller (PCNT) 36
3.4.6 USB Serial/JTAG Controller 37
3.4.7 TWAI ® Controller 37
3.4.8 SDIO 2.0 Slave Controller 37
3.4.9 LED PWM Controller 38
3.4.10 Motor Control PWM (MCPWM) 38
3.4.11 Remote Control Peripheral 39
3.4.12 Parallel IO (PARLIO) Controller 39
3.4.13 General DMA Controller (GDMA) 39
3.4.14 Event Task Matrix (ETM) 39
3.5 Radio and Wi-Fi 40
3.5.1 2.4 GHz Receiver 40
3.5.2 2.4 GHz Transmitter 40
3.5.3 Clock Generator 40
3.5.4 Wi-Fi Radio and Baseband 41
3.5.5 Wi-Fi MAC 41
3.5.6 Networking Features 42
3.6 Bluetooth LE 42
3.6.1 Bluetooth LE Radio and PHY 42
3.6.2 Bluetooth LE Link Layer Controller 43
3.7 802.15.4 43
3.7.1 802.15.4 Radio and PHY 43
3.7.2 802.15.4 MAC 43
3.8 Low Power Management 44
3.9 Timers 44
3.9.1 System Timer 44
3.9.2 General Purpose Timers 44
3.9.3 Watchdog Timers 45
3.10 Cryptography/Security Components 46
3.10.1 AES Accelerator (AES) 46
3.10.2 ECC Accelerator (ECC) 46
3.10.3 HMAC Accelerator (HMAC) 47
3.10.4 RSA Accelerator (RSA) 47
3.10.5 SHA Accelerator (SHA) 47
3.10.6 Digital Signature (DS) 48
3.10.7 External Memory Encryption and Decryption (XTS_AES) 48
3.10.8 Random Number Generator (RNG) 48
3.11 Peripheral Pin Configurations 48
Contents
4 Electrical Characteristics 52
4.1 Absolute Maximum Ratings 52
4.2 Recommended Power Supply Characteristics 52
4.3 VDD_SPI Output Characteristics 53
4.4 DC Characteristics (3.3 V, 25 °C) 53
4.5 Current Consumption 54
4.5.1 RF Current Consumption in Active Mode 54
4.5.2 Current Consumption in Other Modes 55
5 RF Characteristics 56
5.1 Wi-Fi Radio 56
5.1.1 Wi-Fi RF Transmitter (TX) Characteristics 56
5.1.2 Wi-Fi RF Receiver (RX) Characteristics 57
5.2 Bluetooth LE Radio 59
5.2.1 Bluetooth LE RF Transmitter (TX) Characteristics 59
5.2.2 Bluetooth LE RF Receiver (RX) Characteristics 61
5.3 802.15.4 Radio 63
5.3.1 802.15.4 RF Transmitter (TX) Characteristics 63
5.3.2 802.15.4 RF Receiver (RX) Characteristics 63
6 Packaging 64
7 Related Documentation and Resources 65
Appendix A – ESP32-C6 Consolidated Pin Overview 66
Revision History 68
List of Tables
List of T ables
1-1 ESP32-C6 Series Comparison 12
2-1 QFN40 Pin Overview 16
2-2 QFN32 Pin Overview 17
2-3 QFN40 IO MUX Pin Functions 19
2-4 QFN32 IO MUX Pin Functions 19
2-5 LP IO MUX Functions 21
2-6 Analog Functions 22
2-7 Analog Pins 23
2-8 Power Pins 24
2-9 Voltage Regulators 24
2-10 Description of Timing Parameters for Power-up and Reset 26
2-11 Default Configuration of Strapping Pins 26
2-12 Description of Timing Parameters for the Strapping Pins 27
2-13 SDIO Input Sampling Edge/Output Driving Edge Control 27
2-14 Boot Mode Control 28
2-15 ROM Messages Printing Control 28
2-16 JTAG Signal Source Control 29
2-17 Pin Mapping Between QFN40 Chip and Off-package Flash 30
3-1 Peripheral Pin Configurations 49
4-1 Absolute Maximum Ratings 52
4-2 Recommended Power Characteristics 52
4-3 VDD_SPI Internal and Output Characteristics 53
4-4 DC Characteristics (3.3 V, 25 °C) 53
4-5 Current Consumption for Wi-Fi (2.4 GHz) in Active Mode 54
4-6 Current Consumption for Bluetooth LE in Active Mode 54
4-7 Current Consumption for 802.15.4 in Active Mode 54
4-8 Current Consumption in Modem-sleep Mode 55
4-9 Current Consumption in Low-Power Modes 55
5-1 Wi-Fi RF Characteristics 56
5-2 TX Power with Spectral Mask and EVM Meeting 802.11 Standards 56
5-3 TX EVM Test 56
5-4 RX Sensitivity 57
5-5 Maximum RX Level 58
5-6 RX Adjacent Channel Rejection 59
5-7 Bluetooth LE RF Characteristics 59
5-8 Bluetooth LE - Transmitter Characteristics - 1 Mbps 59
5-9 Bluetooth LE - Transmitter Characteristics - 2 Mbps 60
5-10 Bluetooth LE - Transmitter Characteristics - 125 Kbps 60
5-11 Bluetooth LE - Transmitter Characteristics - 500 Kbps 60
5-12 Bluetooth LE - Receiver Characteristics - 1 Mbps 61
5-13 Bluetooth LE - Receiver Characteristics - 2 Mbps 61
5-14 Bluetooth LE - Receiver Characteristics - 125 Kbps 62
5-15 Bluetooth LE - Receiver Characteristics - 500 Kbps 62
List of Tables
5-16 802.15.4 RF Characteristics 63
5-17 802.15.4 Transmitter Characteristics - 250 Kbps 63
5-18 802.15.4 Receiver Characteristics - 250 Kbps 63
7-1 QFN40 Pin Overview 66
7-2 QFN32 Pin Overview 67
List of Figures
List of Figures
1-1 ESP32-C6 Series Nomenclature 12
2-1 ESP32-C6 Pin Layout (QFN40, Top View) 13
2-2 ESP32-C6 Pin Layout (QFN32, Top View) 14
2-3 ESP32-C6 Power Scheme 25
2-4 Visualization of Timing Parameters for Power-up and Reset 25
2-5 Visualization of Timing Parameters for the Strapping Pins 27
3-1 Address Mapping Structure 33
6-1 QFN40 (5×5 mm) Package 64
6-2 QFN32 (5×5 mm) Package 64
1 ESP32-C6 Series Comparison
1 ESP32-C6 Series Comparison
1.1 Nomenclature
ESP32-C6
 F
 H1
 x
Chip series
In-package ﬂash
Flash temperature
H: High temperature
N: Normal temperature
Flash VL]H
0%

Figure 1-1. ESP32-C6 Series Nomenclature
1.2 Comparison
T able 1-1. ESP32-C6 Series Comparison
Ordering Code1 In-Package Flash Ambient T emp.2(°C) Package
ESP32-C6 — ⚶40 ∼ 105 QFN40 (5×5 mm)
ESP32-C6FH4 4 MB (Quad SPI)3 ⚶40 ∼ 105 QFN32 (5×5 mm)
1 For details on chip marking and packing, see Section6 Packaging.
2 Ambient temperature specifies the recommended temperature range of the envi-
ronment immediately outside an Espressif chip.
3 For details about SPI modes, see Section2.7 Pin Mapping Between Chip and
Flash.
2 Pins
2 Pins
2.1 Pin Layout
1
2
3
4
5
6
7
8
9
10
SPICS0
GPIO13
GPIO12
GPIO11
GPIO10
GPIO9
GPIO8
MTDO
MTCK
MTDI
MTMS
GPIO3
GPIO2
XTAL_32K_N
XTAL_32K_P
VDDPST1
CHIP_PU
VDDA3P3
VDDA3P3
ANT
VDDA2
XTAL_N
VDDA1
SDIO_DATA3
SDIO_DATA2
SDIO_DATA1
SDIO_DATA0
SDIO_CLK
SDIO_CMD
XTAL_P
U0RXD
U0TXD
VDDPST2
SPID
SPICLK
VDD_SPI
SPIWP
SPIQ
SPIHD
11
12
13
14
15
16
17
18
19
20
30
29
28
27
26
25
24
23
22
21
GPIO15
40
39
38
37
36
35
34
33
32
31
ESP32-C6
41 GND
Figure 2-1. ESP32-C6 Pin Layout (QFN40, T op View)
2 Pins
1
2
3
4
5
6
7
8
MTMS
GPIO3
GPIO12
GPIO9
GPIO8
MTDO
MTCK
MTDI
GPIO2
XTAL_32K_N
XTAL_32K_P
VDDPST1
CHIP_PU
VDDA3P3
VDDA3P3
ANT
VDDA2
XTAL_N
VDDA1
SDIO_DATA3
SDIO_DATA2
SDIO_DATA1
SDIO_DATA0
XTAL_P
U0RXD
U0TXD
VDDPST2
GPIO14
GPIO13
SDIO_CMD
SDIO_CLK
11
12
13
14
15
16
9
10
22
21
20
19
18
17
24
23
GPIO15
32
31
30
29
28
27
26
25
ESP32-C6
33 GND
Figure 2-2. ESP32-C6 Pin Layout (QFN32, T op View)
2 Pins
2.2 Pin Overview
The ESP32-C6 chip integrates multiple peripherals that require communication with the outside world. To keep
the chip package size reasonably small, the number of available pins has to be limited. So the only way to route
all the incoming and outgoing signals is through pin multiplexing. Pin muxing is controlled via software
programmable registers (seeESP32-C6 Technical Reference Manual > ChapterIO MUX and GPIO Matrix ).
All in all, the ESP32-C6 chip has the following types of pins:
• IO pins with the following predefined sets of functions to choose from:
– Each IO pin has predefinedIO MUX and GPIO functions – see Table2-3 IO MUX and GPIO Pin
Functions or Table2-4 IO MUX and GPIO Pin Functions
– Some IO pins have predefinedLP IO MUX functions – see Table2-5 LP IO MUX Functions
– Some IO pins have predefinedanalog functions – see Table2-6 Analog Functions
Predefined functions means that each IO pin has a set of direct connections to certain on-chip components.
During run-time, the user can configure which component from a predefined set to connect to a certain pin
at a certain time via memory mapped registers (see the TRM).
• Analog pins that have exclusively-dedicatedanalog functions – see Table2-7 Analog Pins
• Power pins supply power to the chip components and non-power pins – see Table2-8 Power Pins
Notes for T able 2-1 Pin Overview or T able2-2 Pin Overview (see below):
1. For more information, see respective sections below. Alternatively, seeAppendix A – ESP32-C6
Consolidated Pin Overview.
2. Bold marks the pin function set in which a pin has its default function in the default boot mode. See
Section 2.6.2 Chip Boot Mode Control.
3. In columnPin Providing Power, regarding pins powered by VDD_SPI:
• Power actually comes from the internal power rail supplying power to VDD_SPI. For details, see
Section 2.5.2 Power Scheme.
4. Except for GPIO12 and GPIO13 whose default drive strength is 40 mA, the default drive strength for all the
other pins is 20 mA.
5. Column Pin Settings shows predefined settings at reset and after reset with the following abbreviations:
• IE – input enabled
• WPU – internal weak pull-up resistor enabled
• WPD – internal weak pull-down resistor enabled
6. Depends on the value of EFUSE_DIS_PAD_JTAG
• 0 - default value. Input enabled, and internal weak pull-up resistor enabled (IE & WPU)
• 1 - input enabled (IE)
7. Output enabled
2 Pins
T able 2-1. QFN40 Pin Overview
Pin Pin Pin Pin Providing Pin Settings 6,7 Pin Function Sets 1,2
No. Name T ype1 Power 3-5 At Reset After Reset IO MUX LP IO MUX Analog
1 ANT Analog
2 VDDA3P3 Power
3 VDDA3P3 Power
4 CHIP_PU Analog VDDPST1
5 VDDPST1 Power
6 XTAL_32K_P IO VDDPST1 IO MUX LP IO MUX Analog
7 XTAL_32K_N IO VDDPST1 IO MUX LP IO MUX Analog
8 GPIO2 IO VDDPST1 IE IE IO MUX LP IO MUX Analog
9 GPIO3 IO VDDPST1 IE IE IO MUX LP IO MUX Analog
10 MTMS IO VDDPST1 IE IE IO MUX LP IO MUX Analog
11 MTDI IO VDDPST1 IE IE IO MUX LP IO MUX Analog
12 MTCK IO VDDPST1 IE, WPU6 IO MUX LP IO MUX Analog
13 MTDO IO VDDPST1 IE IO MUX LP IO MUX
14 GPIO8 IO VDDPST2 IE IE IO MUX
15 GPIO9 IO VDDPST2 IE, WPU IE, WPU IO MUX
16 GPIO10 IO VDDPST2 IE IO MUX
17 GPIO11 IO VDDPST2 IE IO MUX
18 GPIO12 IO VDDPST2 IE IO MUX Analog
19 GPIO13 IO VDDPST2 IE, WPU IO MUX Analog
20 SPICS0 IO VDD_SPI WPU IE, WPU IO MUX
21 SPIQ IO VDD_SPI WPU IE, WPU IO MUX
22 SPIWP IO VDD_SPI WPU IE, WPU IO MUX
23 VDD_SPI Power/IO — IO MUX Analog
24 SPIHD IO VDD_SPI WPU IE, WPU IO MUX
25 SPICLK IO VDD_SPI WPU IE, WPU IO MUX
26 SPID IO VDD_SPI WPU IE, WPU IO MUX
27 GPIO15 IO VDDPST2 IE IE IO MUX
28 VDDPST2 Power
29 U0TXD IO VDDPST2 WPU 7 IO MUX
30 U0RXD IO VDDPST2 IE, WPU IO MUX
31 SDIO_CMD IO VDDPST2 WPU IE IO MUX
32 SDIO_CLK IO VDDPST2 WPU IE IO MUX
33 SDIO_DATA0 IO VDDPST2 WPU IE IO MUX
34 SDIO_DATA1 IO VDDPST2 WPU IE IO MUX
35 SDIO_DATA2 IO VDDPST2 WPU IE IO MUX
36 SDIO_DATA3 IO VDDPST2 WPU IE IO MUX
37 VDDA1 Power
38 XTAL_N Analog
39 XTAL_P Analog
40 VDDA2 Power
41 GND Power
2 Pins
T able 2-2. QFN32 Pin Overview
Pin Pin Pin Pin Providing Pin Settings 6,7 Pin Function Sets 1,2
No. Name T ype1 Power 3-5 At Reset After Reset IO MUX LP IO MUX Analog
1 ANT Analog
2 VDDA3P3 Power
3 VDDA3P3 Power
4 CHIP_PU Analog VDDPST1
5 VDDPST1 Power
6 XTAL_32K_P IO VDDPST1 IO MUX LP IO MUX Analog
7 XTAL_32K_N IO VDDPST1 IO MUX LP IO MUX Analog
8 GPIO2 IO VDDPST1 IE IE IO MUX LP IO MUX Analog
9 GPIO3 IO VDDPST1 IE IE IO MUX LP IO MUX Analog
10 MTMS IO VDDPST1 IE IE IO MUX LP IO MUX Analog
11 MTDI IO VDDPST1 IE IE IO MUX LP IO MUX Analog
12 MTCK IO VDDPST1 IE, WPU6 IO MUX LP IO MUX Analog
13 MTDO IO VDDPST1 IE IO MUX LP IO MUX
14 GPIO8 IO VDDPST2 IE IE IO MUX
15 GPIO9 IO VDDPST2 IE, WPU IE, WPU IO MUX
16 GPIO12 IO VDDPST2 IE IO MUX Analog
17 GPIO13 IO VDDPST2 IE, WPU IO MUX Analog
18 GPIO14 IO VDDPST2 IE IO MUX
19 GPIO15 IO VDDPST2 IE IE IO MUX
20 VDDPST2 Power
21 U0TXD IO VDDPST2 WPU 7 IO MUX
22 U0RXD IO VDDPST2 IE, WPU IO MUX
23 SDIO_CMD IO VDDPST2 WPU IE IO MUX
24 SDIO_CLK IO VDDPST2 WPU IE IO MUX
25 SDIO_DATA0 IO VDDPST2 WPU IE IO MUX
26 SDIO_DATA1 IO VDDPST2 WPU IE IO MUX
27 SDIO_DATA2 IO VDDPST2 WPU IE IO MUX
28 SDIO_DATA3 IO VDDPST2 WPU IE IO MUX
29 VDDA1 Power
30 XTAL_N Analog
31 XTAL_P Analog
32 VDDA2 Power
33 GND Power
2.3 IO Pins
For details on configuring IO pins, seeuESP32-C6ඌҕॉ൭Ҩv > ChapterIO MUX and GPIO pins .
2.3.1 IO MUX and GPIO Pin Functions
The pins of ESP32-C6 can be assigned any function (F0-F2) from their respective sets of IO MUX functions as
listed in Table2-3 IO MUX and GPIO Pin Functions or Table2-4 IO MUX and GPIO Pin Functions .
Each set of the IO MUX functions has a general purpose input/output (GPIO0, GPIO1, etc.) function. If a pin is
assigned a GPIO function, this pin’s signal is routed via the GPIO matrix, which incorporates internal signal
2 Pins
routing circuitry for mapping signals programmatically. It gives the pin access to almost any IO MUX function.
However, the flexibility of programmatic mapping comes at a cost as it might affect speed and latency of routed
signals.
Notes for 2-3 IO MUX and GPIO Pin Functions or T able2-4 IO MUX and GPIO Pin Functions :
1. Bold marks the default pin functions in the default boot mode. See Section2.6.2 Chip Boot Mode Control.
2. Regarding highlighted cells, see Section2.3.4 Restrictions for GPIOs and LP GPIOs.
3. Each IO MUX function (Fn, n = 0 ~ 2) is associated with atype. The description oftype is as follows:
• I – input. O – output. T – high impedance.
• I1 – input; if the pin is assigned a function other than Fn, the input signal of Fn is always1.
• I0 – input; if the pin is assigned a function other than Fn, the input signal of Fn is always0.
4. Function names:
GPIO… General-purpose input/output with signals routed via the GPIO matrix. For
more details on the GPIO matrix, seeESP32-C6 Technical Reference Manual
> ChapterIO MUX and GPIO Matrix .
U…RXD
U…TXD
}
UART0/1 receive/transmit signals.
SDIO… SDIO interface signals.
5. Groups of functions (see the markings in the table):
a. JTAG interface for debugging.
b. UART interface for debugging.
c. SPI0/1 interface for connection to in-package or off-package flash via SPI bus. See also Section2.7 Pin
Mapping Between Chip and Flash.
d. SPI2 main interface for fast SPI connection. Among these pins, FSPICS0 is for input or output signals in
master or slave mode, whereas FSPICS1 ~ FSPICS5 are for output signals in master mode.
2 Pins
T able 2-3. QFN40 IO MUX Pin Functions
Pin IO MUX / IO MUX Function
No. GPIO
Name 0 T ype 1 T ype 2 T ype
6 GPIO0 GPIO0 I/O/T GPIO0 I/O/T
7 GPIO1 GPIO1 I/O/T GPIO1 I/O/T
8 GPIO2 GPIO2 I/O/T GPIO2 I/O/T FSPIQ I1/O/T
9 GPIO3 GPIO3 I/O/T GPIO3 I/O/T
10 GPIO4 MTMS I1 GPIO4 I/O/T FSPIHD I1/O/T
11 GPIO5 MTDI I1 GPIO5 I/O/T FSPIWP I1/O/T
12 GPIO6 MTCK I1 GPIO6 I/O/T FSPICLK I1/O/T
13 GPIO7 MTDO
5a
O/T GPIO7 I/O/T FSPID
5d
I1/O/T
14 GPIO8 GPIO8 I/O/T GPIO8 I/O/T
15 GPIO9 GPIO9 I/O/T GPIO9 I/O/T
16 GPIO10 GPIO10 I/O/T GPIO10 I/O/T
17 GPIO11 GPIO11 I/O/T GPIO11 I/O/T
18 GPIO12 GPIO12 I/O/T GPIO12 I/O/T
19 GPIO13 GPIO13 I/O/T GPIO13 I/O/T
20 GPIO24 SPICS0 O/T GPIO24 I/O/T
21 GPIO25 SPIQ I1/O/T GPIO25 I/O/T
22 GPIO26 SPIWP
5c
I1/O/T GPIO26 I/O/T
23 GPIO27 GPIO27 I/O/T GPIO27 I/O/T
24 GPIO28 SPIHD I1/O/T GPIO28 I/O/T
25 GPIO29 SPICLK O/T GPIO29 I/O/T
26 GPIO30 SPID
5c
I1/O/T GPIO30 I/O/T
27 GPIO15 GPIO15 I/O/T GPIO15 I/O/T
29 GPIO16 U0TXD O GPIO16 I/O/T FSPICS0 I1/O/T
30 GPIO17 U0RXD
5b
I1 GPIO17 I/O/T FSPICS1 O/T
31 GPIO18 SDIO_CMD I1/O/T GPIO18 I/O/T FSPICS2 O/T
32 GPIO19 SDIO_CLK I1 GPIO19 I/O/T FSPICS3 O/T
33 GPIO20 SDIO_DATA0 I1/O/T GPIO20 I/O/T FSPICS4 O/T
34 GPIO21 SDIO_DATA1 I1/O/T GPIO21 I/O/T FSPICS5
5d
O/T
35 GPIO22 SDIO_DATA2 I1/O/T GPIO22 I/O/T
36 GPIO23 SDIO_DATA3 I1/O/T GPIO23 I/O/T
T able 2-4. QFN32 IO MUX Pin Functions
Pin IO MUX / IO MUX Function
No. GPIO
Name 0 T ype 1 T ype 2 T ype
6 GPIO0 GPIO0 I/O/T GPIO0 I/O/T
7 GPIO1 GPIO1 I/O/T GPIO1 I/O/T
Cont’d on next page
2 Pins
T able 2-4 – cont’d from previous page
Pin IO MUX / IO MUX Function
No. GPIO
Name 0 T ype 1 T ype 2 T ype
8 GPIO2 GPIO2 I/O/T GPIO2 I/O/T FSPIQ I1/O/T
9 GPIO3 GPIO3 I/O/T GPIO3 I/O/T
10 GPIO4 MTMS I1 GPIO4 I/O/T FSPIHD I1/O/T
11 GPIO5 MTDI I1 GPIO5 I/O/T FSPIWP I1/O/T
12 GPIO6 MTCK I1 GPIO6 I/O/T FSPICLK I1/O/T
13 GPIO7 MTDO
5a
O/T GPIO7 I/O/T FSPID
5d
I1/O/T
14 GPIO8 GPIO8 I/O/T GPIO8 I/O/T
15 GPIO9 GPIO9 I/O/T GPIO9 I/O/T
16 GPIO12 GPIO12 I/O/T GPIO12 I/O/T
17 GPIO13 GPIO13 I/O/T GPIO13 I/O/T
18 GPIO14 GPIO14 I/O/T GPIO14 I/O/T
19 GPIO15 GPIO15 I/O/T GPIO15 I/O/T
21 GPIO16 U0TXD O GPIO16 I/O/T FSPICS0 I1/O/T
22 GPIO17 U0RXD
5b
I1 GPIO17 I/O/T FSPICS1 O/T
23 GPIO18 SDIO_CMD I1/O/T GPIO18 I/O/T FSPICS2 O/T
24 GPIO19 SDIO_CLK I1 GPIO19 I/O/T FSPICS3 O/T
25 GPIO20 SDIO_DATA0 I1/O/T GPIO20 I/O/T FSPICS4 O/T
26 GPIO21 SDIO_DATA1 I1/O/T GPIO21 I/O/T FSPICS5
5d
O/T
27 GPIO22 SDIO_DATA2 I1/O/T GPIO22 I/O/T
28 GPIO23 SDIO_DATA3 I1/O/T GPIO23 I/O/T
2 Pins
2.3.2 LP IO MUX Functions
LP IO MUX functions are activated if the HP digital system is turned off to save power. LP IO MUX functions and
data input/output are configured by the LP CPU.
Notes for T able 2-5 LP IO MUX Functions:
1. Bold marks the default pin functions in the default boot mode. See Section2.6.2 Chip Boot Mode Control.
2. Regarding highlighted cells, see Section2.3.4 Restrictions for GPIOs and LP GPIOs.
3. Function names:
LP_GPIO… General-purpose input/output configured by LP CPU.
LP_UART… LP UART functions.
LP_I2C… LP I2C functions.
T able 2-5. LP IO MUX Functions
Pin LP IO LP IO MUX Function
No. Name 0 1
6 LP_GPIO0 LP_GPIO0 LP_UART_DTRN
7 LP_GPIO1 LP_GPIO1 LP_UART_DSRN
8 LP_GPIO2 LP_GPIO2 LP_UART_RTSN
9 LP_GPIO3 LP_GPIO3 LP_UART_CTSN
10 LP_GPIO4 LP_GPIO4 LP_UART_RXD
11 LP_GPIO5 LP_GPIO5 LP_UART_TXD
12 LP_GPIO6 LP_GPIO6 LP_I2C_SDA
13 LP_GPIO7 LP_GPIO7 LP_I2C_SCL
2.3.3 Analog Functions
Analog functions can operate in any power mode.
Notes for T able 2-6 Analog Functions:
1. Bold marks the default pin functions in SPI Boot mode.
2. Regarding highlighted cells, see Section2.3.4 Restrictions for GPIOs and LP GPIOs.
3. Function names:
XTAL_32K_P
XTAL_32K_N
}
32 kHz external clock input/output connected to ESP32-C6’s oscillator.
P/N means differential clock positive/negative.
ADC1_CH… Analog to digital conversion channel for ADC1.
USB_D-
USB_D+
}
USB Serial/JTAG function. USB signal is a differential signal transmitted
over a pair of D+ and D- wires.
2 Pins
T able 2-6. Analog Functions
QFN40 QFN32 Analog Analog Function
Pin No. Pin No. IO Name 0 1
6 6 GPIO0 XTAL_32K_P ADC1_CH0
7 7 GPIO1 XTAL_32K_N ADC1_CH1
8 8 GPIO2 ADC1_CH2
9 9 GPIO3 ADC1_CH3
10 10 GPIO4 ADC1_CH4
11 11 GPIO5 ADC1_CH5
12 12 GPIO6 ADC1_CH6
18 16 GPIO12 USB_D-
19 17 GPIO13 USB_D+
23 — GPIO27 VDD_SPI
2 Pins
2.3.4 Restrictions for GPIOs and LP GPIOs
All IO pins of the ESP32-C6 have GPIO and some have LP GPIO pin functions. However, the IO pins are
multiplexed and have other important pin functions. This should be taken into account while certain pins are
chosen for general purpose input output.
In tables in Section2.3 IO Pins, some pin functions arehighlighted . The non-highlighted GPIO or LP GPIO pins
are recommended for use first. If more pins are needed, the highlighted GPIOs or LP GPIOs should be chosen
carefully to avoid conflicts with important pin functions.
The highlighted IO pins have the following important pin functions:
• GPIO – allocated for communication with flash and NOT recommended for other uses. For details, see
Section 2.7 Pin Mapping Between Chip and Flash.
• GPIO – have one of the following important functions:
– Strapping pins – need to be at certain logic levels at startup. See Section2.6 Strapping Pins.
– USB_D+/- – by default, connected to the USB Serial/JTAG Controller. To function as GPIOs, these
pins need to be reconfigured.
– JTAG interface– often used for debugging. See Table2-3 IO MUX and GPIO Pin Functions or Table
2-4 IO MUX and GPIO Pin Functions , note5a. To free these pins up, the pin functions USB_D+/- of the
USB Serial/JTAG Controller can be used instead. See also Section2.6.4 JTAG Signal Source Control.
– UART interface – often used for debugging. See Table2-3 IO MUX and GPIO Pin Functions or Table
2-4 IO MUX and GPIO Pin Functions , note5b.
See alsoAppendix A – ESP32-C6 Consolidated Pin Overview.
2.4 Analog Pins
T able 2-7. Analog Pins
QFN40 QFN32 Pin Pin Pin
Pin No. Pin No. Name T ype Function
1 1 ANT I/O RF input and output
4 4 CHIP_PU — High: on, enables the chip (Powered up).
Low: off, the chip powers off (powered down).
Note: Do not leave the CHIP_PU pin floating.
38 30 XTAL_N — External clock input/output connected to chip’s crystal or
oscillator. P/N means differential clock positive/negative.39 31 XTAL_P —
2 Pins
2.5 Power Supply
2.5.1 Power Pins
The chip is powered via the power pins described in Table2-8 Power Pins.
T able 2-8. Power Pins
QFN40 QFN32 Pin Power Supply 1,2
Pin No. Pin No. Name Direction Power Domain / Other IO Pins 4
2 2 VDDA3P3 Input Analog power domain
3 3 VDDA3P3 Input Analog power domain
5 5 VDDPST1 Input LP Digital and part of analog pin power domainsLP IO
23
—
VDD_SPI 3 Input In-package flash (backup power line)
Output In-package flash and off-package flash
28 20 VDDPST2 Input HP Digital power domain HP IO
37 29 VDDA1 Input Analog power domain
40 32 VDDA2 Input Analog power domain
41 33 GND — External ground connection
1 See in conjunction with Section2.5.2 Power Scheme.
2 Forrecommendedandmaximumvoltageandcurrent, seeSection 4.1AbsoluteMaximumRatings andSection
4.2 Recommended Power Supply Characteristics.
3 To configure VDD_SPI as input or output, seeESP32-C6 Technical Reference Manual > Chapter Low-power
Management.
4 LP IO pins are those powered by VDDPST1 and so on, as shown in Figure2-3 ESP32-C6 Power Scheme.
See also Table2-3 IO MUX and GPIO Pin Functions or Table2-4 IO MUX and GPIO Pin Functions > ColumnPin
Providing Power.
2.5.2 Power Scheme
The power scheme is shown in Figure2-3 ESP32-C6 Power Scheme.
The components on the chip are powered via voltage regulators.
T able 2-9. Voltage Regulators
Voltage Regulator Output Power Supply
HP 1.1 V HP power domain
LP 1.1 V LP power domain
2 Pins
LP
Voltage
Regulator
HP
Voltage
Regulator
LP System HP
System
VDD_PST1 VDD_PST2 VDDA1 VDDA2
Analog
VDD_SPI
LP IO HP IO
RSPI
Figure 2-3. ESP32-C6 Power Scheme
2.5.3 Chip Power-up and Reset
Once the power is supplied to the chip, its power rails need a short time to stabilize. After that, CHIP_PU – the
pin used for power-up and reset – is pulled high to activate the chip. For information on CHIP_PU as well as
power-up and reset timing, see Figure2-4 and Table2-10.
VIL_nRST
tST BL tRST
2.8 V
VDDA3P3,
VDDPST1,
VDDPST2,
VDDA1,
VDDA2
CHIP_PU
Figure 2-4. Visualization of Timing Parameters for Power-up and Reset
2 Pins
T able 2-10. Description of Timing Parameters for Power-up and Reset
Parameter Description Min (µs)
tST BL
Time reserved for the power rails of VDDA3P3, VDDPST1, VD-
DPST2, VDDA1 and VDDA2 to stabilize before the CHIP_PU pin
is pulled high to activate the chip
50
tRST
Time reserved for CHIP_PU to stay below VIL_nRST to reset the
chip (see Table4-4) 50
2.6 Strapping Pins
At each startup or reset, a chip requires some initial configuration parameters, such as in which boot mode to
load the chip, etc. These parameters are passed over via the strapping pins. After reset, the strapping pins
operate as regular IO pins.
The parameters controlled by the given strapping pins at chip reset are as follows:
• SDIO sampling and driving clock edge – MTMS and MTDI
• Chip boot mode – GPIO8 and GPIO9
• ROM code printing to UART – GPIO8
• JTAG signal source – GPIO15
GPIO9 is connected to the chip’s internal weak pull-up resistor at chip reset. This resistor determines the default
bit value of GPIO9. Also, the resistor determines the bit value if GPIO9 is connected to an external
high-impedance circuit.
T able 2-11. Default Configuration of Strapping Pins
Strapping Pin Default Configuration Bit Value
MTMS Floating –
MTDI Floating –
GPIO8 Floating –
GPIO9 Pull-up 1
GPIO15 Floating –
To change the bit values, the strapping pins should be connected to external pull-down/pull-up resistances. If the
ESP32-C6 is used as a device by a host MCU, the strapping pin voltage levels can also be controlled by the host
MCU.
All strapping pins have latches. At system reset, the latches sample the bit values of their respective strapping
pins and store them until the chip is powered down or shut down. The states of latches cannot be changed in
any other way. It makes the strapping pin values available during the entire chip operation, and the pins are freed
up to be used as regular IO pins after reset.
Regarding the timing requirements for the strapping pins, there are such parameters assetup time and hold time.
For more information, see Table2-12 and Figure2-5.
2 Pins
T able 2-12. Description of Timing Parameters for the Strapping Pins
Parameter Description Min (ms)
tSU
Setup time isthetimereservedforthepowerrailstostabilizebefore
the CHIP_PU pin is pulled high to activate the chip. 0
tH
Hold time is the time reserved for the chip to read the strapping pin
values after CHIP_PU is already high and before these pins start
operating as regular IO pins.
3
Strapping pin
VIL_nRST
VIH
tSU tH
CHIP_PU
Figure 2-5. Visualization of Timing Parameters for the Strapping Pins
2.6.1 SDIO Sampling and Driving Clock Edge Control
The strapping pin MTMS and MTDI can be used to decide on which clock edge to sample signals and drive
output lines. See Table2-13 SDIO Input Sampling Edge/Output Driving Edge Control.
T able 2-13. SDIO Input Sampling Edge/Output Driving Edge Control
MTMS MTDI Edge behavior
– (Floating) – (Floating) Default Configuration
0 0 Falling edge sampling, falling edge output
0 1 Falling edge sampling, rising edge output
1 0 Rising edge sampling, falling edge output
1 1 Rising edge sampling, rising edge output
2.6.2 Chip Boot Mode Control
GPIO8 and GPIO9 control the boot mode after the reset is released. See Table2-14 Boot Mode ControlBoot
Mode Control.
2 Pins
T able 2-14. Boot Mode Control
Boot Mode GPIO8 GPIO9
Default Configuration – (Floating) 1 (Pull-up)
SPI Boot (default) Any value 1
Download Boot 1 0
Invalid combination1 0 0
1 This combination triggers unexpected behavior
and should be avoided.
2.6.3 ROM Messages Printing Control
During boot process the messages by the ROM code can be printed to:
• USB Serial/JTAG controller. For this, EFUSE_DIS_USB_SERIAL_JTAG_ROM_PRINT should be 0 and
USB Serial/JTAG controller should be enabled.
• UART0. For this, set EFUSE_DIS_USB_SERIAL_JTAG_ROM_PRINT to 1. In this case,
EFUSE_UART_PRINT_CONTROL and GPIO8 control ROM messages printing as shown in Table2-15
ROM Messages Printing Control.
T able 2-15. ROM Messages Printing Control
eFuse1 GPIO8 ROM Code Printing
0 Ignored Always enabled
1
0 Enabled
1 Disabled
2
0 Disabled
1 Enabled
3 Ignored Always disabled
1 eFuse: EFUSE_UART_PRINT_CONTROL
2.6.4 JTAG Signal Source Control
The strapping pin GPIO15 can be used to control the source of JTAG signals during the early boot process. This
pin does not have any internal pull resistors and the strapping value must be controlled by the external circuit that
cannot be in a high impedance state.
As Table2-16 shows, GPIO15 is used in combination with EFUSE_DIS_PAD_JTAG, EFUSE_DIS_USB_JTAG,
and EFUSE_JTAG_SEL_ENABLE.
2 Pins
T able 2-16. JTAG Signal Source Control
eFuse 1 a eFuse 2 b eFuse 3 c GPIO15 JTAG Signal Source
0 0
0 Ignored USB Serial/JTAG Controller
1
0 JTAG pins MTDI, MTCK, MTMS, and MTDO
1 USB Serial/JTAG Controller
0 1 Ignored Ignored JTAG pins MTDI, MTCK, MTMS, and MTDO
1 0 Ignored Ignored USB Serial/JTAG Controller
1 1 Ignored Ignored JTAG is disabled
a eFuse 1: EFUSE_DIS_PAD_JTAG
b eFuse 2: EFUSE_DIS_USB_JTAG
c eFuse 3: EFUSE_JTAG_SEL_ENABLE
2 Pins
2.7 Pin Mapping Between Chip and Flash
Table2-17 lists the pin mapping between the chip and off-package flash for all SPI modes.
For chip variants with in-package flash (namely variants in QFN32 package, see Table1-1 Comparison), the pins
allocated for communication with in-package flash are not routed out, but you can take Table2-17 as a
reference.
For more information on SPI controllers, see also Section3.4.2 Serial Peripheral Interface (SPI).
Notice:
It is not recommended to use the pins connected to flash for any other purposes.
T able 2-17. Pin Mapping Between QFN40 Chip and Off-package Flash
QFN40 Pin Name Single SPI Dual SPI Quad SPI
Pin No. Flash Flash Flash
25 SPICLK CLK CLK CLK
20 SPICS0 CS# CS# CS#
26 SPID MOSI SIO0 SIO0
21 SPIQ MISO SIO1 SIO1
22 SPIWP WP# SIO2
24 SPIHD HOLD# SIO3
1 SIO: Serial Data Input and Output
3 Functional Description
3 Functional Description
This chapter describes the functions of ESP32-C6.
3.1 CPU and Memory
3.1.1 HP CPU
ESP32-C6 has a HP 32-bit RISC-V single-core processor with the following features:
• four-stage pipeline that supports a clock frequency of up to 160 MHz
• RV32IMAC ISA (instruction set architecture)
• 32-bit multiplier and 32-bit divider
• up to 28 vectored interrupts at 15 priority levels
• up to 4 hardware breakpoints/watchpoints
• up to 16 PMP/PMA regions
• JTAG for debugging
• compliant with RISC-V debug specification v0.13
• compliant with RISC-V Trace Specification v1.0
3.1.2 LP CPU
ESP32-C6 integrates a LP 32-bit RISC-V processor. This LP CPU is designed as a simplified, low-power
replacement of HP CPU in sleep modes. It can be also used to supplement the functions of the HP CPU in
normal working mode. The LP CPU and LP memory remain powered on in Deep-sleep mode. Hence, the
developer can store a program for the LP CPU in the LP memory to access LP IO, LP peripherals, and real-time
timers in Deep-sleep mode.
LP CPU has the following features:
• two-stage pipeline that supports a clock frequency of up to 20 MHz
• RV32IMAC ISA (instruction set architecture)
• 32-bit general-purpose registers
• 32-bit multiplier and divider
• support for interrupts
• up to 2 hardware breakpoints/watchpoints
• JTAG for debugging
• compliant with RISC-V debug specification v0.13
• boot by the CPU, its dedicated timer, or LP IO
3 Functional Description
3.1.3 Internal Memory
ESP32-C6’s internal memory includes:
• 320 KB of ROM: for booting and core functions
• HP memory: 512 KB of SRAM for data and instructions
• LP memory: 16 KB of SRAM that can be accessed by HP CPU or LP CPU. It can retain data in
Deep-sleep mode
• 4 Kbit of eFuse: 1792 bits are reserved for your data, such as encryption key and device ID
• In-package flash: See details in Chapter1 ESP32-C6 Series Comparison
3.1.4 External Flash
ESP32-C6 supports SPI, Dual SPI, Quad SPI, and QPI interfaces that allow connection to multiple external
flash.
CPU’s instruction memory space and read-only data memory space can map into external flash of ESP32-C6,
whose size can be 16 MB at most. ESP32-C6 supports hardware encryption/decryption based on XTS-AES to
protect developers’ programs and data in flash.
Through high-speed caches, ESP32-C6 can support at a time up to:
• 16 MB of instruction memory space which can map into flash as individual blocks of 64 KB. 32-bit fetch is
supported
• 16 MB of data memory space which can map into flash as individual blocks of 64 KB. 8-bit, 16-bit and
32-bit reads are supported
Note:
After ESP32-C6 is initialized, software can customize the mapping of external flash into the CPU address space.
3 Functional Description
3.1.5 Address Mapping Structure
Figure 3-1. Address Mapping Structure
3.1.6 Cache
ESP32-C6 has an four-way set associative cache. This cache is read-only and has the following features:
• size: 32 KB
• pre-load function
• lock function
• critical word first and early restart
3.1.7 TEE Controller
ESP32-C6 integrates a TEE (Trusted Execution Environment) controller to configure and extend security modes
for masters in the system. The TEE controller has the following features:
• up to 32 masters
• four security modes
• accessible by the main master in TEE security mode
3.1.8 Access Permission Management (APM)
ESP32-C6 integrates an APM module to manage access permissions. The module compares information
transmitted over the bus with predefined configurations and decides if to grant access. APM has the following
3 Functional Description
features:
• 16 regions with configurable addresses
• support for interrupts
• exception records
• accessible by the main master in TEE security mode
3.1.9 Timeout Protection
ESP32-C6 integrates a timeout protection module against bus being stuck. The module has the following
features:
• up to 65535 configurable timeout periods (3 timeout modules in CPU peripherals, APB peripherals and LP
peripherals)
• support for interrupts
• exception records
3.2 System Clocks
3.2.1 CPU Clock
The CPU clock has three possible sources:
• external main crystal clock
• internal fast RC oscillator clock (typically about 20 MHz, and adjustable)
• PLL clock
The application can select the clock source from the three clocks above. The selected clock source drives the
CPU clock directly, or after division, depending on the application. When the clock source is PLL clock, the clock
frequency should be no more than 160 MHz. Once the CPU is reset, the default clock source would be the
external main crystal clock divided by 1.
Note:
ESP32-C6 is unable to operate without an external main crystal clock.
3.2.2 Low-Power Clocks
The LP slow clock is used for RTC counter, RTC watchdog and the power management unit (PMU). It has four
possible sources:
• internal low-speed RC oscillator (typically about 32 kHz, and adjustable)
• internal slow RC oscillator (typically about 150 kHz, and adjustable)
• external low-speed (32 kHz) crystal clock
• external IO clock (external clock source connected with digital IO)
The LP fast clock is used for low-power peripherals and sensor controllers. It has two possible sources:
3 Functional Description
• external main crystal clock divided by 2
• internal fast RC oscillator clock (typically about 20 MHz, and adjustable)
3.3 Analog Peripherals
3.3.1 Analog-to-Digital Converter (ADC)
ESP32-C6 integrates a 12-bit SAR ADC and supports measurements on 7 channels (analog-enabled
pins).
For GPIOs assigned to ADC, please refer to Table3-1.
3.3.2 T emperature Sensor
The temperature sensor generates a voltage that varies with temperature. The voltage is internally converted via
an ADC into a digital value.
The temperature sensor has a range of⚶40 °C to 125 °C. It is designed primarily to sense the temperature
changes inside the chip. The temperature value depends on factors like microcontroller clock frequency or I/O
load. Generally, the chip’s internal temperature is higher than the ambient temperature.
3.4 Digital Peripherals
3.4.1 Universal Asynchronous Receiver Transmitter (UART)
ESP32-C6 has three UART interfaces, i.e. UART0, UART1 and LP UART. All the three interfaces provide
hardware flow control (CTS and RTS signals) and software flow control (XON and XOFF).
UART0 and UART 1 support IrDA and asynchronous communication (RS232 and RS485) at a speed of up to 5
Mbps. UART0 and UART1 connect to GDMA via UHCI0 interface (i.e. Universal Host Controller Interface), and
can be accessed by the GDMA controller or directly by the CPU.
LP UART only supports asynchronous communication (RS232) at a speed of up to 1.25 Mbps. LP UART can
only by accessed by the CPU.
For GPIOs assigned to UART, please refer to Table3-1.
3.4.2 Serial Peripheral Interface (SPI)
ESP32-C6 features three SPI interfaces (SPI0, SPI1, and SPI2). SPI0 and SPI1 can be configured to operate in
SPI memory mode, while SPI2 can be configured to operate in general-purpose SPI mode.
• SPI Memory mode
In SPI memory mode, SPI0 and SPI1 interface with external SPI memory. Data are transferred in unit of
byte. Up to four-line STR reads and writes are supported. The clock frequency is configurable to a
maximum of 120 MHz.
• SPI2 General-purpose SPI (GP-SPI) mode
SPI2 can operate in master and slave modes. SPI2 supports two-line full-duplex communication and
single-/two-/four-line half-duplex communication in both master and slave modes. The host’s clock
3 Functional Description
frequency is configurable. Data are transferred in unit of byte. The clock polarity (CPOL) and phase (CPHA)
are also configurable. The SPI2 interface can connect to GDMA.
– In master mode, the clock frequency is 80 MHz at most, and the four modes of SPI transfer format are
supported.
– In slave mode, the clock frequency is 60 MHz at most, and the four modes of SPI transfer format are
also supported.
For the recommended pin mapping between ESP32-C6 and external flash, please see Table2-17 Pin Mapping
Between Chip and Flash.
For GPIOs assigned to SPI, please refer to Table3-1.
3.4.3 I2C Interface
ESP32-C6 has an I2C and a LP I2C bus interfaces. I2C is used for I2C master mode or slave mode, depending
on your configuration, while LP I2C is always in master mode. Both interfaces support:
• standard mode (100 Kbit/s)
• fast mode (400 Kbit/s)
• up to 800 Kbit/s (constrained by SCL and SDA pull-up strength)
• 7-bit and 10-bit addressing mode
• double addressing mode
• 7-bit broadcast address
You can configure instruction registers to control the I2C interface for more flexibility.
For GPIOs assigned to I2C, please refer to Table3-1.
3.4.4 I2S Interface
ESP32-C6 includes a standard I2S interface. This interface can operate as a master or a slave in full-duplex
mode or half-duplex mode, and can be configured for 8-bit, 16-bit, 24-bit, or 32-bit serial communication. BCK
clock frequency, from 10 kHz up to 40 MHz, is supported.
The I2S interface supports TDM Philips, TDM MSB alignment, TDM PCM standard, PDM standard, and
PCM-to-PDM TX interface. It connects to the GDMA controller.
For GPIOs assigned to I2S, please refer to Table3-1.
3.4.5 Pulse Count Controller (PCNT)
The pulse count controller (PCNT) in ESP32-C6 captures pulses and counts pulse edges in seven modes. It has
the following features:
• four independent pulse counters (units) that count from 1 to 65535
• each unit consists of two independent channels sharing one pulse counter
• all channels have input pulse signals (e.g. sig_ch0_un) with their corresponding control signals (e.g.
ctrl_ch0_un)
3 Functional Description
• independently filter glitches of input pulse signals (sig_ch0_un and sig_ch1_un) and control signals
(ctrl_ch0_un and ctrl_ch1_un) on each unit
• each channel has the following parameters:
1. selection between counting on positive or negative edges of the input pulse signal
2. configuration to Increment, Decrement, or Disable counter mode for control signal’s high and low
states
• Maximum frequency of pulses: 40 MHz
For GPIOs assigned to PCNT, please refer to Table3-1.
3.4.6 USB Serial/JTAG Controller
ESP32-C6 integrates a USB Serial/JTAG controller. This controller has the following features:
• CDC-ACM virtual serial port and JTAG adapter functionality
• USB 2.0 full speed compliant, capable of up to 12 Mbit/s transfer speed (Note that this controller does not
support the faster 480 Mbit/s high-speed transfer mode)
• programming embedded/external flash
• CPU debugging with compact JTAG instructions
• a full-speed USB PHY integrated in the chip
For GPIOs assigned to USB Serial/JTAG, please refer to Table3-1.
3.4.7 TWAI® Controller
ESP32-C6 has two TWAI® controllers with the following features:
• compatible with ISO 11898-1 protocol (CAN Specification 2.0)
• standard frame format (11-bit ID) and extended frame format (29-bit ID)
• bit rates from 1 Kbit/s to 1 Mbit/s
• multiple modes of operation: Normal, Listen Only, and Self-Test (no acknowledgment required)
• 64-byte receive FIFO
• acceptance filter (single and dual filter modes)
• error detection and handling: error counters, configurable error warning limit, error code capture, arbitration
lost capture, automatic transceiver standby
For GPIOs assigned to TWAI®, please refer to Table3-1.
3.4.8 SDIO 2.0 Slave Controller
ESP32-C6 integrates an SD device interface that conforms to the industry-standard SDIO Specification Version
2.0, and allows a host controller to access the SoC, using the SDIO bus interface and protocol. The host can
access the registers of the SDIO interface directly and the shared memory via a DMA engine, thus maximizing
performance without engaging the processor cores.
The SDIO 2.0 Slave Controller supports the following features:
3 Functional Description
• clock range: 0 to 50 MHz
• SPI, 1-bit SDIO, and 4-bit SDIO transfer modes
• configurable sampling and driving clock edges
• special registers for direct access by host
• interrupting host to initiate data transfer
• automatic loading of SDIO bus data and automatic discarding of padding data
• block size of up to 512 bytes
• interrupt vectors between the host and the slave, allowing both to interrupt each other
• supports DMA for data transfer
For GPIOs assigned to SDIO, please refer to Table3-1.
3.4.9 LED PWM Controller
The LED PWM controller can generate independent digital waveform on six channels. The LED PWM
controller:
• can generate digital waveform with configurable periods and duty cycle. The resolution of duty cycle can
be up to 20 bits
• has multiple clock sources, including 80 MHz PLL clock, external main crystal clock, and internal fast RC
oscillator
• can operate when the CPU is in low-power mode (Light-sleep mode)
• supports gradual increase or decrease of duty cycle, which is useful for the LED RGB color-gradient
generator
• up to 16 duty cycle ranges for each PWM generator to generate gamma curve signals - each range can be
independently configured in terms of fading direction (increase or decrease), fading amount (the amount by
which the duty cycle increases or decreases each time), the number of fades (how many times the duty
cycle fades in one range), and fading frequency
For GPIOs assigned to LED PWM, please refer to Table3-1.
3.4.10 Motor Control PWM (MCPWM)
ESP32-C6 integrates a MCPWM that can be used to drive digital motors and smart light. This controller has a
clock divider (prescaler), three PWM timers, three PWM operators, and a dedicated capture submodule.
PWM timers are used to generate timing references. The PWM operators generate desired waveform based on
the timing references. By configuration, a PWM operator can use the timing reference of any PWM timer, and use
the same timing reference with other PwM operators. PWM operators can also use different PWM timers’ values
to produce independent PWM signals. PWM timers can be synchronized.
For GPIOs assigned to MCPWM, please refer to Table3-1.
3 Functional Description
3.4.11 Remote Control Peripheral
The Remote Control Peripheral (RMT) supports two channels of infrared remote transmission and two channels
of infrared remote reception. By controlling pulse waveform through software, it supports various infrared and
other single wire protocols. All four channels share a 192 × 32-bit memory block to store transmit or receive
waveform.
For GPIOs assigned to RMT, please refer to Table3-1.
3.4.12 Parallel IO (PARLIO) Controller
ESP32-C6 integrates a PARLIO controller for parallel data transfer. It has a transmitter and a receiver, connected
with the GDMA controller. In full-duplex mode the PARLIO controller supports up to 8-bit parallel data transfer,
while in half-duplex mode it supports up to 16-bit parallel data transfer.
The PARLIO controller has the following features:
• multiple clock sources and clock division, with clock frequency up to 40 MHz
• clock edge sampling
• 1/2/4/8/16-bit data transfer
• changeable sample sequence for data to be transmitted and received in 1-bitđ2-bit, and 4-bit mode
• support for multiple data sampling mode by the receiver
• support for multiple EOF signal generation modes by the receiver
• support for transmitter clock gating
For GPIOs assigned to PARLIO, please refer to Table3-1.
3.4.13 General DMA Controller (GDMA)
ESP32-C6 has a general DMA controller (GDMA) with six independent channels, i.e. three transmit channels and
three receive channels. These six channels are shared by peripherals with DMA feature. The GDMA controller
implements a fixed-priority scheme among these channels.
The GDMA controller controls data transfer using linked lists. It allows peripheral-to-memory and
memory-to-memory data transfer at a high speed. All channels can access internal RAM.
Peripherals on ESP32-C6 with DMA feature are SPI2, UHCI0, I2S, AES, SHA, ADC, and PARLIO.
3.4.14 Event T ask Matrix (ETM)
ESP32-C6 integrates a SOC ETM with multiple channels. Each input event on channels is mapped to an output
task. Events are generated by peripherals, while tasks are received by peripherals. The SOC ETM has the
following features:
• up to 50 mapping channels, each connected to an event and a task and controlled independently
• an event or a task can be mapped to any tasks or events in the matrix. That is to say, one event can be
mapped to different tasks via multiple channels, or different events can be mapped to the same task via
their individual channels
3 Functional Description
• peripherals supporting ETM include GPIO, LED PWM, general-purpose timers, RTC Watchdog Timer,
system timer, MCPWM, temperature sensor, ADC, I2S, LP CPU, GDMA, and PMU
3.5 Radio and Wi-Fi
The ESP32-C6 radio consists of the following blocks:
• 2.4 GHz receiver
• 2.4 GHz transmitter
• bias and regulators
• balun and transmit-receive switch
• clock generator
3.5.1 2.4 GHz Receiver
The 2.4 GHz receiver demodulates the 2.4 GHz RF signal to quadrature baseband signals and converts them to
the digital domain with two high-resolution, high-speed ADCs. To adapt to varying signal channel conditions,
ESP32-C6 integrates RF filters, Automatic Gain Control (AGC), DC offset cancelation circuits, and baseband
filters.
3.5.2 2.4 GHz Transmitter
The 2.4 GHz transmitter modulates the quadrature baseband signals to the 2.4 GHz RF signal, and drives the
antenna with a high-powered CMOS power amplifier. The use of digital calibration further improves the linearity of
the power amplifier.
Additional calibrations are integrated to cancel any radio imperfections, such as:
• carrier leakage
• I/Q amplitude/phase matching
• baseband nonlinearities
• RF nonlinearities
• antenna matching
These built-in calibration routines reduce the cost, time, and specialized equipment required for product
testing.
3.5.3 Clock Generator
The clock generator produces quadrature clock signals of 2.4 GHz for both the receiver and the transmitter. All
components of the clock generator are integrated into the chip, including inductors, varactors, filters, regulators
and dividers.
The clock generator has built-in calibration and self-test circuits. Quadrature clock phases and phase noise are
optimized on chip with patented calibration algorithms which ensure the best performance of the receiver and the
transmitter.
3 Functional Description
3.5.4 Wi-Fi Radio and Baseband
The ESP32-C6 Wi-Fi radio and baseband support the following features:
• compliant with IEEE 802.11b/g/n/ax
• 1T1R in 2.4 GHz band
• 802.11ax
– 20 MHz-only non-AP mode
– MCS0 ~MCS9
– uplink and downlink OFDMA
– downlink MU-MIMO (multi-user, multiple input, multiple output)
– longer OFDM symbol, with 0.8, 1.6, 3.2µs guard interval
– DCM (dual carrier modulation), up to 16-QAM
– single-user/multi-user beamformee
– channel quality indication (CQI)
– RX STBC (single spatial stream)
• 802.11b/g/n
– MCS0 ~MCS7 that supports 20 MHz and 40 MHz bandwidth
– MCS32
– data rate up to 150 Mbps
– 0.4 µs guard interval
• adjustable transmitting power
• antenna diversity
ESP32-C6 supports antenna diversity with an external RF switch. This switch is controlled by one or more
GPIOs, and used to select the best antenna to minimize the effects of channel imperfections.
3.5.5 Wi-Fi MAC
ESP32-C6 implements the full IEEE 802.11 b/g/n/ax Wi-Fi MAC protocol. It supports the Basic Service Set (BSS)
STA and SoftAP operations under the Distributed Control Function (DCF). Power management is handled
automatically with minimal host interaction to minimize the active duty period.
The ESP32-C6 Wi-Fi MAC applies the following low-level protocol functions automatically:
• 4 × virtual Wi-Fi interfaces
• infrastructure BSS in Station mode, SoftAP mode, Station + SoftAP mode, and promiscuous mode
• RTS protection, CTS protection, Immediate Block ACK
• fragmentation and defragmentation
• TX/RX A-MPDU, TX/RX A-MSDU
• transmit opportunity (TXOP)
3 Functional Description
• Wi-Fi multimedia (WMM)
• GCMP, CCMP, TKIP, WAPI, WEP, BIP, WPA2-PSK, and WPA3-PSK
• automatic beacon monitoring (hardware TSF)
• 802.11mc FTM
• 802.11ax supports:
– target wake time (TWT) requester
– multiple BSSIDs
– triggered response scheduling
– uplink power headroom
– operating mode
– buffer status report
– Multi-user Request-to-Send (MU-RTS), Multi-user Block ACK Request (MU-BAR), and Multi-STA
Block ACK (M-BA) frame
– intra-PPDU power saving mechanism
– two network allocation vectors (NAV)
– BSS coloring
– spatial reuse
– uplink power headroom
– operating mode control
– buffer status report
– TXOP duration RTS threshold
– UL-OFDMA random access (UORA)
3.5.6 Networking Features
Espressif provides libraries for TCP/IP networking, ESP-WIFI-MESH networking, and other networking protocols
over Wi-Fi. TLS 1.0, 1.1 and 1.2 is also supported.
3.6 Bluetooth LE
ESP32-C6 includes a Bluetooth Low Energy subsystem that integrates a hardware link layer controller, an
RF/modem block and a feature-rich software protocol stack. It supports the core features of Bluetooth 5 and
Bluetooth mesh.
3.6.1 Bluetooth LE Radio and PHY
Bluetooth Low Energy radio and PHY in ESP32-C6 support:
• 1 Mbps PHY
• 2 Mbps PHY for higher data rates
3 Functional Description
• coded PHY for longer range (125 Kbps and 500 Kbps)
• HW listen before talk (LBT)
3.6.2 Bluetooth LE Link Layer Controller
Bluetooth Low Energy Link Layer Controller in ESP32-C6 supports:
• LE advertising extensions, to enhance broadcasting capacity and broadcast more intelligent data
• multiple advertisement sets
• simultaneous advertising and scanning
• multiple connections in simultaneous central and peripheral roles
• adaptive frequency hopping and channel assessment
• LE channel selection algorithm #2
• LE power control
• connection parameter update
• high duty cycle non-connectable advertising
• LE privacy 1.2
• LE data packet length extension
• link layer extended scanner filter policies
• low duty cycle directed advertising
• link layer encryption
• LE Ping
3.7 802.15.4
ESP32-C6 includes an IEEE Standard 802.15.4 subsystem that integrates 2.4 GHz Radio, PHY and MAC layer.
It supports various software stacks including Thread, Zigbee, Matter, HomeKit, MQTT and so on.
3.7.1 802.15.4 Radio and PHY
• O-QPSK PHY in 2.4 GHz
• 250 Kbps data rate
• RSSI and LQI supported
3.7.2 802.15.4 MAC
ESP32-C6 supports most key features defined inIEEE Standard802.15.4-2015, including:
• CSMA/CA
• active scan and energy detect
• HW frame filter
3 Functional Description
• HW auto acknowledge
• HW auto frame pending
• coordinated sampled listening (CSL)
3.8 Low Power Management
With the use of advanced power-management technologies, ESP32-C6 can switch between different power
modes. ESP32-C6 supports:
• Active mode: CPU and chip radio are powered on. The chip can receive, transmit, or listen.
• Modem-sleep mode: The CPU is operational and the clock frequency can be reduced. Wi-Fi base band
and radio are disabled, but Wi-Fi connection can remain active.
• Light-sleep mode: The CPU is paused. Any wake-up events (wireless power management module, host,
RTC timer, or external interrupts) will wake up the chip. Wi-Fi base band and radio are disabled, but Wi-Fi
connection can remain active. Users can disable the CPU and most peripherals except SRAM and wireless
power management module (as shown inESP32-C6 Functional Block Diagram) to further reduce current
consumption.
• Deep-sleep mode: CPU, SRAM, and most peripherals are powered down. Only the LP memory is
powered on. LP peripheral states can be configured. Wi-Fi connection data are stored in the LP memory.
The LP CPU is operational.
3.9 Timers
3.9.1 System Timer
ESP32-C6 integrates a 52-bit system timer, which has two 52-bit counters and three comparators. The system
timer has the following features:
• counters with an average clock frequency of 16 MHz
• three types of independent interrupts generated according to alarm value
• two alarm modes: target mode and period mode
• 52-bit alarm values and 26-bit alarm periods
• automatic reload of counter value
• counters can be stalled if the CPU is stalled or in OCD mode
• real-time alarm events
3.9.2 General Purpose Timers
ESP32-C6 is embedded with two 54-bit general-purpose timers, which are based on 16-bit prescalers and
54-bit auto-reload-capable up/down-timers.
The timers’ features are summarized as follows:
• a 16-bit clock prescaler, from 2 to 65536
• a 54-bit time-base counter programmable to be incrementing or decrementing
3 Functional Description
• able to read real-time value of the time-base counter
• halting and resuming the time-base counter
• programmable alarm generation
• level interrupt generation
• real-time alarm events
• tasks in response to ETM, including enable and disable timers, enable alarms, read the timer’s real-time
values, reload the timer’s values
3.9.3 Watchdog Timers
Digital Watchdog Timers
The ESP32-C6 contains three digital watchdog timers: one in each of the two timer groups (called Main System
Watchdog Timers, or MWDT) and one in the low-power system (called the RTC Watchdog Timer, or
RWDT).
During the flash boot process, RWDT and the MWDT in timer group 0 (TIMG0) are enabled automatically in order
to detect and recover from booting errors.
Watchdog timers have the following features:
• four stages, each with a programmable timeout value. Each stage can be configured, enabled and
disabled separately
• interrupt, CPU reset, or core reset for MWDT upon expiry of each stage; interrupt, CPU reset, core reset, or
system reset for RWDT upon expiry of each stage
• 32-bit expiry counter
• write protection, to prevent RWDT and MWDT configuration from being altered inadvertently
• flash boot protection
If the boot process from an SPI flash does not complete within a predetermined period of time, the
watchdog will reboot the entire main system.
Analog Watchdog Timer
The ESP32-C6 also has one analog watchdog timer: RTC super watchdog timer (SWD). Super watchdog (SWD)
is an ultra-low-power circuit in analog domain that helps to prevent the system from operating in a sub-optimal
state and resets the system (system reset) if required. SWD contains a watchdog circuit that needs to be fed for
at least once during its timeout period, which is slightly less than one second. About 100 ms before watchdog
timeout, it will also send out a WD_INTR signal as a request to remind the system to feed the watchdog.
If the system does not respond to SWD feed request and watchdog finally times out, SWD will generate a system
level signal SWD_RSTB to reset whole digital circuits on the chip (system reset).
The source of the clock for SWD is constant and can not be selected.
SWD has the following features:
• ultra-low power
• interrupt to indicate that the SWD is about to time out
3 Functional Description
• various dedicated methods for software to feed SWD, which enables SWD to monitor the working state of
the whole operating system
3.10 Cryptography/Security Components
3.10.1 AES Accelerator (AES)
ESP32-C6 integrates an Advanced Encryption Standard (AES) accelerator, which is a hardware device that
speeds up computation using AES algorithm significantly, compared to AES algorithms implemented solely in
software. The AES accelerator integrated in ESP32-C6 has two working modes, which are Typical AES and
DMA-AES.
The following functionality is supported:
• typical AES working mode
– AES-128/AES-256 encryption and decryption
• DMA-AES working mode
– AES-128/AES-256 encryption and decryption
– Block cipher mode
* ECB (Electronic Codebook)
* CBC (Cipher Block Chaining)
* OFB (Output Feedback)
* CTR (Counter)
* CFB8 (8-bit Cipher Feedback)
* CFB128 (128-bit Cipher Feedback)
– interrupt on completion of computation
3.10.2 ECC Accelerator (ECC)
Elliptic Curve Cryptography (ECC) is an approach to public-key cryptography based on the algebraic structure of
elliptic curves. ECC allows smaller keys compared to RSA cryptography while providing equivalent
security.
ESP32-C6’s ECC Accelerator can complete various calculations based on different elliptic curves, thus
accelerating the ECC algorithm and ECC-derived algorithms (such as ECDSA).
ESP32-C6’s ECC Accelerator has the following features:
• two different elliptic curves, namely P-192 and P-256 defined inFIPS 186-3
• six working modes
• interrupt upon completion of calculation
3 Functional Description
3.10.3 HMAC Accelerator (HMAC)
The Hash-based Message Authentication Code (HMAC) module computes Message Authentication Codes
(MACs) using Hash algorithm SHA-256 and keys as described in RFC 2104. The 256-bit HMAC key is stored in
an eFuse key block and can be set as read-protected, i.e., the key is not accessible from outside the HMAC
accelerator.
Main features are as follows:
• standard HMAC-SHA-256 algorithm
• Hash result only accessible by configurable hardware peripheral (in downstream mode)
• compatibility with challenge-response authentication algorithm
• required keys for the Digital Signature (DS) peripheral (in downstream mode)
• re-enabled soft-disabled JTAG (in downstream mode)
3.10.4 RSA Accelerator (RSA)
The RSA accelerator provides hardware support for high-precision computation used in various RSA asymmetric
cipher algorithms, significantly improving their run time and reducing their software complexity. Compared with
RSA algorithms implemented solely in software, this hardware accelerator can speed up RSA algorithms
significantly. The RSA accelerator also supports operands of different lengths, which provides more flexibility
during the computation.
The following functionality is supported:
• large-number modular exponentiation with two optional acceleration options
• large-number modular multiplication
• large-number multiplication
• operands of different lengths
• interrupt on completion of computation
3.10.5 SHA Accelerator (SHA)
ESP32-C6 integrates an SHA accelerator, which is a hardware device that speeds up the SHA algorithm
significantly, compared to a SHA algorithm implemented solely in software. The SHA accelerator integrated in
ESP32-C6 has two working modes, which are Typical SHA and DMA-SHA.
The following functionality is supported:
• the following hash algorithms introduced inFIPS PUB 180-4 Spec
– SHA-1
– SHA-224
– SHA-256
• two working modes
– typical SHA
– DMA-SHA
3 Functional Description
• interleaved function when working in Typical SHA working mode
• interrupt function when working in DMA-SHA working mode
3.10.6 Digital Signature (DS)
A Digital Signature (DS) is used to verify the authenticity and integrity of a message using a cryptographic
algorithm. This can be used to validate a device’s identity to a server, or to check the integrity of a
message.
ESP32-C6 includes a Digital Signature (DS) module providing hardware acceleration of messages’ signatures
based on RSA. HMAC is used as the key derivation function to output the DS_KEY key using eFuse as the input
key. Subsequently, the DS module uses DS_KEY to decrypt the pre-encrypted parameters and calculate the
signature. Thewholeprocesshappensin hardwareso thatneitherthe decryptionkey fortheRSA parametersnor
the input key for the HMAC key derivation function can be seen by users while calculating the signature.
The following functionality is supported:
• RSA digital signatures with key length up to 3072 bits
• encrypted private key data, only decryptable by DS module
• SHA-256 digest to protect private key data against tampering by an attacker
3.10.7 External Memory Encryption and Decryption (XTS_AES)
The ESP32-C6 integrates an External Memory Encryption and Decryption module that complies with the
XTS_AES standard algorithm specified inIEEE Std 1619-2007, providing security for users’ application code and
data stored in the external memory (flash). Users can store proprietary firmware and sensitive data (e.g.,
credentials for gaining access to a private network) to the external flash.
The following functionality is supported:
• general XTS_AES algorithm, compliant with IEEE Std 1619-2007
• software-based manual encryption
• high-speed auto decryption without software’s participation
• encryption and decryption functions jointly enabled/disabled by registers configuration, eFuse parameters,
and boot mode
• configurable Anti-DPA
3.10.8 Random Number Generator (RNG)
The ESP32-C6 contains a true random number generator, which generates 32-bit random numbers that can be
used for cryptographical operations, among other things.
The random number generator in ESP32-C6 generates true random numbers, which means random numbers
generated from a physical process, rather than by means of an algorithm. No number generated within the
specified range is more or less likely to appear than any other number.
3.11 Peripheral Pin Configurations
3 Functional Description
T able 3-1. Peripheral Pin Configurations
Interface Signal Pin Function
ADC ADC1_CH0 XTAL_32K_P 12-bit SAR ADC
ADC1_CH1 XTAL_32K_N
ADC1_CH2 GPIO2
ADC1_CH3 GPIO3
ADC1_CH4 MTMS
ADC1_CH5 MTDI
ADC1_CH6 MTCK
JTAG MTDI MTDI JTAG for software debugging
MTCK MTCK
MTMS MTMS
MTDO MTDO
UART U0RXD_in Any GPIO pins Two UART channels with hardware flow control
and GDMAU0CTS_in
U0DSR_in
U0TXD_out
U0RTS_out
U0DTR_out
U1RXD_in
U1CTS_in
U1DSR_in
U1TXD_out
U1RTS_out
U1DTR_out
LP UART LP_UART_DTRN XTAL_32K_P One LP UART channel with hardware flow control
and GDMALP_UART_DSRN XTAL_32K_N
LP_UART_RTSN GPIO2
LP_UART_CTSN GPIO3
LP_UART_RXD MTMS
LP_UART_TXD MTDI
I2C I2CEXT0_SCL_in Any GPIO pins One I2C channel in slave or master mode
I2CEXT0_SDA_in
I2CEXT0_SCL_out
I2CEXT0_SDA_out
LP I2C LP_I2C_SDA MTCK One LP I2C channel in slave or master mode
LP_I2C_SCL MTDO
LED PWM ledc_ls_sig_out0~5 Any GPIO pins Six independent PWM channels
I2S I2S0O_BCK_in Any GPIO pins Stereo input and output from/to the audiocodec
I2S_MCLK_in
I2SO_WS_in
I2SI_SD_in
I2SI_BCK_in
I2SI_WS_in
I2SO_BCK_out
I2S_MCLK_out
3 Functional Description
Interface Signal Pin Function
I2SO_WS_out
I2SO_SD_out
I2SI_BCK_out
I2SI_WS_out
I2SO_SD1_out
Remote Control
Peripheral
RMT_SIG_IN0~1 Any GPIO pins Two channels for an IR transceiver of various
waveformsRMT_SIG_OUT0~1
SPI0/1 SPICLK_out_mux SPICLK Support Standard SPI, Dual SPI, Quad SPI, and
QPI that allow connection to external flashSPICS0_out SPICS0
SPICS1_out Any GPIO pins
SPID_in/_out SPID
SPIQ_in/_out SPIQ
SPIWP_in/_out SPIWP
SPIHD_in/_out SPIHD
SPI2 FSPICLK_in/_out_mux Any GPIO pins The following functionality is supported:
• Master mode and slave mode of SPI, Dual SPI,
Quad SPI, and QPI
• Connection to external flash, RAM and other SPI
devices
• Four modes of SPI transfer format
• Configurable SPI frequency
• 64-byte FIFO or GDMA buffer
FSPICS0_in/_out
FSPICS1~5_out
FSPID_in/_out
FSPIQ_in/_out
FSPIWP_in/_out
FSPIHD_in/_out
USB Serial/JTAG USB_D+ GPIO13 USB-to-serial converter, and USB-to-JTAG
converterUSB_D- GPIO12
TWAI® TWAI0_RX Any GPIO pins Compatible with ISO 11898-1 protocol
TWAI0_TX
TWAI0_BUS_OFF_ON
TWAI0_CLKOUT
TWAI0_STANDBY
TWAI1_RX
TWAI1_TX
TWAI1_BUS_OFF_ON
TWAI1_CLKOUT
TWAI1_STANDBY
Pulse Count Controller PCNT_SIG_CH0_in0~3 Any GPIO pins Captures pulses and counts pulse edges in
seven modesPCNT_SIG_CH1_in0~3
PCNT_CTRL_CH0_in0~3
PCNT_CTRL_CH1_in0~3
3 Functional Description
Interface Signal Pin Function
MCPWM PWM0_SYNC0~2_in Any GPIO pins One MCPWM to generate:
• differential PWM output signals
• fault input signals to be detected
• input signals to be captured
• external synchronization signals for PWM timers
PWM0_out0a
PWM0_out0b
PWM0_out1a
PWM0_F0~2_in
PWM0_out1b
PWM0_out2a
PWM0_out2b
PWM0_CAP0~2_in
PARLIO PARL_RX_DATA0~15 Any GPIO pins A module for parallel data transfer, with
• 16 pins to receive parallel data
• 16 pins to transmit parallel data
• 1 receiver clock pin (clock input)
• 2 transmitter clock pins (clock input and output)
PARL_TX_DATA0~15
PARL_RX_CLK_in
PARL_TX_CLK_in/_out
SDIO SDIO_CMD SDIO_CMD SDIO interface, conforming to the industry
standard SDIO Specification Version 2.0SDIO_CLK SDIO_CLK
SDIO_DATA0 SDIO_DATA0
SDIO_DATA1 SDIO_DATA1
SDIO_DATA2 SDIO_DATA2
SDIO_DATA3 SDIO_DATA3
4 Electrical Characteristics
4 Electrical Characteristics
Note:
The values presented in this section arepreliminaryand may change with the final release of this datasheet.
4.1 Absolute Maximum Ratings
Stresses above those listed in Table4-1 Absolute Maximum Ratings may cause permanent damage to the device.
These are stress ratings only and normal operation of the device at these or any other conditions beyond those
indicated in Section4.2 Recommended Power Supply Characteristicsis not implied. Exposure to
absolute-maximum-rated conditions for extended periods may affect device reliability.
T able 4-1. Absolute Maximum Ratings
Parameter Description Min Max Unit
Input power pins1 Allowed input voltage ⚶0.3 3.6 V
TST ORE Storage temperature ⚶40 150 °C
1 For more information on input power pins, see Section2.5.1 Power
Pins.
2 TheproductprovedtobefullyfunctionalafterallitsIOpinswerepulled
highwhilebeingconnectedtogroundfor24consecutivehoursatam-
bient temperature of 25 °C.
4.2 Recommended Power Supply Characteristics
For recommended ambient temperature, see Section1 ESP32-C6 Series Comparison.
T able 4-2. Recommended Power Characteristics
Parameter 1 Description Min T yp Max Unit
VDDA1, VDDA2, VDDA3P3 Recommended input voltage 3.0 3.3 3.6 V
VDDPST1 Recommended input voltage 3.0 3.3 3.6 V
VDD_SPI (as input) — 3.0 3.3 3.6 V
VDDPST2 2, 3 Recommended input voltage 3.0 3.3 3.6 V
IV DD Cumulative input current 0.5 — — A
1 See in conjunction with Section2.5 Power Supply.
2 If VDDPST2 is used to power VDD_SPI (see Section2.5.2Power Scheme), the voltage drop
on RSP I should be accounted for. See also Section4.3 VDD_SPI Output Characteristics.
3 IfwritingtoeFuses,thevoltageonVDDPST2shouldnotexceed3.3Vasthecircuitsrespon-
sible for burning eFuses are sensitive to higher voltages.
4 Electrical Characteristics
4.3 VDD_SPI Output Characteristics
T able 4-3. VDD_SPI Internal and Output Characteristics
Parameter Description 1 T yp Unit
RSP I
VDD_SPI powered by VDD3P3_RTC via RSP I
for 3.3 V flash2 7.5 Ω
1 See in conjunction with Section2.5.2 Power Scheme.
2 VDD3P3_RTC must be more thanVDD_flash_min + I_flash_max * R SP I;
where
• VDD_flash_min – minimum operating voltage of flash
• I_flash_max – maximum operating current of flash
4.4 DC Characteristics (3.3 V , 25 °C)
T able 4-4. DC Characteristics (3.3 V , 25 °C)
Parameter Description Min T yp Max Unit
CIN Pin capacitance — 2 — pF
VIH High-level input voltage 0.75 × VDD1 — VDD1+ 0.3 V
VIL Low-level input voltage ⚶0.3 — 0.25 × VDD1 V
IIH High-level input current — — 50 nA
IIL Low-level input current — — 50 nA
VOH2 High-level output voltage 0.8 × VDD1 — — V
VOL2 Low-level output voltage — — 0.1 × VDD1 V
IOH
High-level source current (VDD1= 3.3 V, VOH
>= 2.64 V, PAD_DRIVER = 3) — 40 — mA
IOL
Low-level sink current (VDD1= 3.3 V, VOL =
0.495 V, PAD_DRIVER = 3) — 28 — mA
RP U Internal weak pull-up resistor — 45 — kΩ
RP D Internal weak pull-down resistor — 45 — kΩ
VIH_nRST
Chip reset release voltage CHIP_PU voltage is
within the specified range) 0.75 × VDD1 — VDD1+ 0.3 V
VIL_nRST
Chip reset voltage (CHIP_PU voltage is within
the specified range) ⚶0.3 — 0.25 × VDD1 V
1 VDD – voltage from a power pin of a respective power domain.
2 VOH and VOL are measured using high-impedance load.
4 Electrical Characteristics
4.5 Current Consumption
4.5.1 RF Current Consumption in Active Mode
The current consumption measurements are taken with a 3.3 V supply at 25 °C ambient temperature.
TX current consumption is rated at a 100% duty cycle.
RX current consumption is rated when the peripherals are disabled and the CPU idle.
T able 4-5. Current Consumption for Wi-Fi (2.4 GHz) in Active Mode
Work Mode RF Condition Description Peak (mA)
Active (RF working)
TX
802.11b, 1 Mbps, DSSS @ 21.0 dBm 354
802.11g, 54 Mbps, OFDM @ 19.5 dBm 300
802.11n, HT20, MCS7 @ 18.5 dBm 280
802.11n, HT40, MCS7 @ 18.0 dBm 268
802.11ax, MCS9, @ 16.5 dBm 252
RX
802.11b/g/n, HT20 78
802.11n, HT40 82
802.11ax, HE20 78
T able 4-6. Current Consumption for Bluetooth LE in Active Mode
Work Mode RF Condition Description Peak (mA)
Active (RF working)
TX
Bluetooth LE @ 20.0 dBm 315
Bluetooth LE @ 9.0 dBm 190
Bluetooth LE @ 0 dBm 130
Bluetooth LE @⚶24.0 dBm 89
RX Bluetooth LE 71
T able 4-7. Current Consumption for 802.15.4 in Active Mode
Work Mode RF Condition Description Peak (mA)
Active (RF working)
TX
802.15.4 @ 20.0 dBm 305
802.15.4 @ 12.0 dBm 187
802.15.4 @ 0 dBm 119
802.15.4 @⚶24.0 dBm 82
RX 802.15.4 74
4 Electrical Characteristics
4.5.2 Current Consumption in Other Modes
T able 4-8. Current Consumption in Modem-sleep Mode
T yp (mA)
Mode
CPU Frequency
(MHz) Description All Peripherals
Clocks Disabled
All Peripherals
Clocks Enabled 1
Modem-sleep2,3
160
CPU is running 27 38
CPU is idle 17 28
80
CPU is running 19 30
CPU is idle 14 25
1 In practice, the current consumption might be different depending on which peripherals are
enabled.
2 In Modem-sleep mode, Wi-Fi is clock gated.
3 In Modem-sleep mode, the consumption might be higher when accessing flash.
T able 4-9. Current Consumption in Low-Power Modes
Mode Description T yp (µA)
Light-sleep
CPUandwirelesscommunicationmodulesarepowereddown,pe-
ripheral clocks are disabled, and all GPIOs are high-impedance 180
CPU, wireless communication modules and peripherals are pow-
ered down, and all GPIOs are high-impedance 35
Deep-sleep RTC timer and LP memory are powered on 7
Power off CHIP_PU is set to low level, the chip is powered off 1
5 RF Characteristics
5 RF Characteristics
This section contains tables with RF characteristics of the Espressif product.
The RF data is measured at the antenna port, where RF cable is connected, including the front-end loss. The
front-end circuit is a 0Ω resistor.
Devices should operate in the center frequency range allocated by regional regulatory authorities. The target
center frequency range and the target transmit power are configurable by software. SeeESP RF TestTooland
TestGuide for instructions.
Unless otherwise stated, the RF tests are conducted with a 3.3 V (±5%) supply at 25 ºC ambient
temperature.
5.1 Wi-Fi Radio
T able 5-1. Wi-Fi RF Characteristics
Name Description
Center frequency range of operating channel 2412 ~ 2484 MHz
Wi-Fi wireless standard IEEE 802.11b/g/n/ax
5.1.1 Wi-Fi RF Transmitter (TX) Characteristics
T able 5-2. TX Power with Spectral Mask and EVM Meeting 802.11 Standards
Min T yp Max
Rate (dBm) (dBm) (dBm)
802.11b, 1 Mbps, DSSS — 21.0 —
802.11b, 11 Mbps, CCK — 21.0 —
802.11g, 6 Mbps, OFDM — 20.5 —
802.11g, 54 Mbps, OFDM — 19.5 —
802.11n, HT20, MCS0 — 19.5 —
802.11n, HT20, MCS7 — 18.5 —
802.11n, HT40, MCS0 — 19.0 —
802.11n, HT40, MCS7 — 18.0 —
802.11ax, HE20, MCS0 — 19.5 —
802.11ax, HE20, MCS9 — 16.5 —
T able 5-3. TX EVM T est1
Min T yp Limit
Rate (dB) (dB) (dB)
802.11b, 1 Mbps, DSSS — ⚶25.5 ⚶10.0
Cont’d on next page
5 RF Characteristics
T able 5-3 – cont’d from previous page
Min T yp Limit
Rate (dB) (dB) (dB)
802.11b, 11 Mbps, CCK — ⚶25.5 ⚶10.0
802.11g, 6 Mbps, OFDM — ⚶26.5 ⚶5.0
802.11g, 54 Mbps, OFDM — ⚶29.0 ⚶25.0
802.11n, HT20, MCS0 — ⚶29.0 ⚶5.0
802.11n, HT20, MCS7 — ⚶30.0 ⚶27.0
802.11n, HT40, MCS0 — ⚶28.5 ⚶5.0
802.11n, HT40, MCS7 — ⚶29.5 ⚶27.0
802.11ax, HE20, MCS0 — ⚶29.0 ⚶5.0
802.11ax, HE20, MCS9 — ⚶34.0 ⚶32.0
1 EVM is measured at the corresponding typical TX power provided in
Table5-2 Wi-Fi RF Transmitter (TX) Characteristics above.
5.1.2 Wi-Fi RF Receiver (RX) Characteristics
For RX tests, the PER (packet error rate) limit is 8% for 802.11b, and 10% for 802.11g/n/ax.
T able 5-4. RX Sensitivity
Min T yp Max
Rate (dBm) (dBm) (dBm)
802.11b, 1 Mbps, DSSS — ⚶99.2 —
802.11b, 2 Mbps, DSSS — ⚶96.8 —
802.11b, 5.5 Mbps, CCK — ⚶93.8 —
802.11b, 11 Mbps, CCK — ⚶90.0 —
802.11g, 6 Mbps, OFDM — ⚶94.0 —
802.11g, 9 Mbps, OFDM — ⚶93.2 —
802.11g, 12 Mbps, OFDM — ⚶92.6 —
802.11g, 18 Mbps, OFDM — ⚶90.0 —
802.11g, 24 Mbps, OFDM — ⚶86.8 —
802.11g, 36 Mbps, OFDM — ⚶83.2 —
802.11g, 48 Mbps, OFDM — ⚶79.0 —
802.11g, 54 Mbps, OFDM — ⚶77.6 —
802.11n, HT20, MCS0 — ⚶93.6 —
802.11n, HT20, MCS1 — ⚶92.4 —
802.11n, HT20, MCS2 — ⚶89.6 —
802.11n, HT20, MCS3 — ⚶86.2 —
802.11n, HT20, MCS4 — ⚶82.8 —
802.11n, HT20, MCS5 — ⚶78.8 —
802.11n, HT20, MCS6 — ⚶77.2 —
Cont’d on next page
5 RF Characteristics
T able 5-4 – cont’d from previous page
Min T yp Max
Rate (dBm) (dBm) (dBm)
802.11n, HT20, MCS7 — ⚶75.6 —
802.11n, HT40, MCS0 — ⚶91.0 —
802.11n, HT40, MCS1 — ⚶90.0 —
802.11n, HT40, MCS2 — ⚶87.4 —
802.11n, HT40, MCS3 — ⚶83.8 —
802.11n, HT40, MCS4 — ⚶80.8 —
802.11n, HT40, MCS5 — ⚶76.6 —
802.11n, HT40, MCS6 — ⚶75.0 —
802.11n, HT40, MCS7 — ⚶73.4 —
802.11ax, HE20, MCS0 — ⚶93.8 —
802.11ax, HE20, MCS1 — ⚶91.2 —
802.11ax, HE20, MCS2 — ⚶88.4 —
802.11ax, HE20, MCS3 — ⚶85.6 —
802.11ax, HE20, MCS4 — ⚶82.2 —
802.11ax, HE20, MCS5 — ⚶78.4 —
802.11ax, HE20, MCS6 — ⚶76.6 —
802.11ax, HE20, MCS7 — ⚶74.8 —
802.11ax, HE20, MCS8 — ⚶71.0 —
802.11ax, HE20, MCS9 — ⚶69.0 —
T able 5-5. Maximum RX Level
Min T yp Max
Rate (dBm) (dBm) (dBm)
802.11b, 1 Mbps, DSSS — 5 —
802.11b, 11 Mbps, CCK — 5 —
802.11g, 6 Mbps, OFDM — 5 —
802.11g, 54 Mbps, OFDM — 0 —
802.11n, HT20, MCS0 — 5 —
802.11n, HT20, MCS7 — 0 —
802.11n, HT40, MCS0 — 5 —
802.11n, HT40, MCS7 — 0 —
802.11ax, HE20, MCS0 — 5 —
802.11ax, HE20, MCS9 — 0 —
5 RF Characteristics
T able 5-6. RX Adjacent Channel Rejection
Min T yp Max
Rate (dB) (dB) (dB)
802.11b, 1 Mbps, DSSS — 38 —
802.11b, 11 Mbps, CCK — 38 —
802.11g, 6 Mbps, OFDM — 31 —
802.11g, 54 Mbps, OFDM — 20 —
802.11n, HT20, MCS0 — 31 —
802.11n, HT20, MCS7 — 16 —
802.11n, HT40, MCS0 — 28 —
802.11n, HT40, MCS7 — 10 —
802.11ax, HE20, MCS0 — 25 —
802.11ax, HE20, MCS9 — 2 —
5.2 Bluetooth LE Radio
T able 5-7. Bluetooth LE RF Characteristics
Name Description
Center frequency range of operating channel 2402 ~ 2480 MHz
RF transmit power range ⚶24.0 ~ 20.0 dBm
5.2.1 Bluetooth LE RF Transmitter (TX) Characteristics
T able 5-8. Bluetooth LE - Transmitter Characteristics - 1 Mbps
Parameter Description Min T yp Max Unit
Carrier frequency offset and drift
Max. |fn|n=0, 1, 2, 3, ...k — 1.3 — kHz
Max. |f0 − fn|n=2, 3, 4, ...k — 1.5 — kHz
Max. |fn − fn−5|n=6, 7, 8, ...k — 0.9 — kHz
|f1 − f0| — 0.6 — kHz
Modulation characteristics
∆ F 1avg — 249.9 — kHz
Min. ∆ F 2max (for at least
99.9% of all∆ F 2max) — 212.1 — kHz
∆ F 2avg/∆ F 1avg — 0.88 — —
In-band emissions
± 2 MHz offset — ⚶29 — dBm
± 3 MHz offset — ⚶36 — dBm
> ± 3 MHz offset — ⚶39 — dBm
5 RF Characteristics
T able 5-9. Bluetooth LE - Transmitter Characteristics - 2 Mbps
Parameter Description Min T yp Max Unit
Carrier frequency offset and drift
Max. |fn|n=0, 1, 2, 3, ...k — 2.2 — kHz
Max. |f0 − fn|n=2, 3, 4, ...k — 1.1 — kHz
Max. |fn − fn−5|n=6, 7, 8, ...k — 1.1 — kHz
|f1 − f0| — 0.5 — kHz
Modulation characteristics
∆ F 1avg — 499.4 — kHz
Min. ∆ F 2max (for at least
99.9% of all∆ F 2max) — 443.5 — kHz
∆ F 2avg/∆ F 1avg — 0.95 — —
In-band emissions
± 4 MHz offset — ⚶40 — dBm
± 5 MHz offset — ⚶41 — dBm
> ± 5 MHz offset — ⚶42 — dBm
T able 5-10. Bluetooth LE - Transmitter Characteristics - 125 Kbps
Parameter Description Min T yp Max Unit
Carrier frequency offset and drift
Max. |fn|n=0, 1, 2, 3, ...k — 0.7 — kHz
Max. |f0 − fn|n=1, 2, 3, ...k — 0.3 — kHz
|f0 − f3| — 0.1 — kHz
Max. |fn − fn−3|n=7, 8, 9, ...k — 0.4 — kHz
Modulation characteristics
∆ F 1avg — 250.0 — kHz
Min. ∆ F 1max (for at least
99.9% of all∆ F 1max) — 238.0 — kHz
In-band emissions
± 2 MHz offset — ⚶29 — dBm
± 3 MHz offset — ⚶36 — dBm
> ± 3 MHz offset — ⚶39 — dBm
T able 5-11. Bluetooth LE - Transmitter Characteristics - 500 Kbps
Parameter Description Min T yp Max Unit
Carrier frequency offset and drift
Max. |fn|n=0, 1, 2, 3, ...k — 0.5 — kHz
Max. |f0 − fn|n=1, 2, 3, ...k — 0.3 — kHz
|f0 − f3| — 0.1 — kHz
Max. |fn − fn−3|n=7, 8, 9, ...k — 0.4 — kHz
Modulation characteristics
∆ F 2avg — 230.7 — kHz
Min. ∆ F 2max (for at least
99.9% of all∆ F 2max) — 217.6 — kHz
In-band emissions
± 2 MHz offset — ⚶28 — dBm
± 3 MHz offset — ⚶36 — dBm
> ± 3 MHz offset — ⚶39 — dBm
5 RF Characteristics
5.2.2 Bluetooth LE RF Receiver (RX) Characteristics
T able 5-12. Bluetooth LE - Receiver Characteristics - 1 Mbps
Parameter Description Min T yp Max Unit
Sensitivity @30.8% PER — — ⚶98.5 — dBm
Maximum received signal @30.8% PER — — 8 — dBm
C/I and receiver
selectivity performance
Co-channel F = F0 MHz — 7 — dB
Adjacent channel
F = F0 + 1 MHz — 4 — dB
F = F0 – 1 MHz — 3 — dB
F = F0 + 2 MHz — ⚶21 — dB
F = F0 – 2 MHz — ⚶22 — dB
F = F0 + 3 MHz — ⚶28 — dB
F = F0 – 3 MHz — ⚶36 — dB
F ≥ F0 + 4 MHz — ⚶27 — dB
F ≤ F0 – 4 MHz — ⚶36 — dB
Image frequency — — ⚶26 — dB
Adjacent channel to
image frequency
F = Fimage + 1 MHz — ⚶29 — dB
F = Fimage – 1 MHz — ⚶28 — dB
30 MHz~ 2000 MHz — ⚶16 — dBm
Out-of-band blocking performance 2003 MHz~ 2399 MHz — ⚶24 — dBm
2484 MHz~ 2997 MHz — ⚶16 — dBm
3000 MHz~ 12.75 GHz — ⚶1 — dBm
Intermodulation — — ⚶27 — dBm
T able 5-13. Bluetooth LE - Receiver Characteristics - 2 Mbps
Parameter Description Min T yp Max Unit
Sensitivity @30.8% PER — — ⚶95.5 — dBm
Maximum received signal @30.8% PER — — 8 — dBm
C/I and receiver
selectivity performance
Co-channel F = F0 MHz — 8 — dB
Adjacent channel
F = F0 + 2 MHz — 3 — dB
F = F0 – 2 MHz — 2 — dB
F = F0 + 4 MHz — ⚶23 — dB
F = F0 – 4 MHz — ⚶25 — dB
F = F0 + 6 MHz — ⚶31 — dB
F = F0 – 6 MHz — ⚶35 — dB
F ≥ F0 + 8 MHz — ⚶36 — dB
F ≤ F0 – 8 MHz — ⚶36 — dB
Image frequency — — ⚶23 — dB
Adjacent channel to
image frequency
F = Fimage + 2 MHz — ⚶30 — dB
F = Fimage – 2 MHz — 3 — dB
Cont’d on next page
5 RF Characteristics
T able 5-13 – cont’d from previous page
Parameter Description Min T yp Max Unit
30 MHz~ 2000 MHz — ⚶18 — dBm
Out-of-band blocking performance 2003 MHz~ 2399 MHz — ⚶28 — dBm
2484 MHz~ 2997 MHz — ⚶16 — dBm
3000 MHz~ 12.75 GHz — ⚶1 — dBm
Intermodulation — — ⚶29 — dBm
T able 5-14. Bluetooth LE - Receiver Characteristics - 125 Kbps
Parameter Description Min T yp Max Unit
Sensitivity @30.8% PER — — ⚶106.0 — dBm
Maximum received signal @30.8% PER — — 8 — dBm
C/I and receiver
selectivity performance
Co-channel F = F0 MHz — 2 — dB
Adjacent channel
F = F0 + 1 MHz — ⚶1 — dB
F = F0 – 1 MHz — ⚶3 — dB
F = F0 + 2 MHz — ⚶31 — dB
F = F0 – 2 MHz — ⚶27 — dB
F = F0 + 3 MHz — ⚶33 — dB
F = F0 – 3 MHz — ⚶42 — dB
F ≥ F0 + 4 MHz — ⚶31 — dB
F ≤ F0 – 4 MHz — ⚶48 — dB
Image frequency — — ⚶31 — dB
Adjacent channel to
image frequency
F = Fimage + 1 MHz — ⚶36 — dB
F = Fimage – 1 MHz — ⚶33 — dB
T able 5-15. Bluetooth LE - Receiver Characteristics - 500 Kbps
Parameter Description Min T yp Max Unit
Sensitivity @30.8% PER — — ⚶102.0 — dBm
Maximum received signal @30.8% PER — — 8 — dBm
C/I and receiver
selectivity performance
Co-channel F = F0 MHz — 4 — dB
Adjacent channel
F = F0 + 1 MHz — 1 — dB
F = F0 – 1 MHz — ⚶1 — dB
F = F0 + 2 MHz — ⚶23 — dB
F = F0 – 2 MHz — ⚶24 — dB
F = F0 + 3 MHz — ⚶33 — dB
F = F0 – 3 MHz — ⚶41 — dB
F ≥ F0 + 4 MHz — ⚶31 — dB
F ≤ F0 – 4 MHz — ⚶41 — dB
Image frequency — — ⚶30 — dB
Cont’d on next page
5 RF Characteristics
T able 5-15 – cont’d from previous page
Parameter Description Min T yp Max Unit
Adjacent channel to
image frequency
F = Fimage + 1 MHz — ⚶35 — dB
F = Fimage – 1 MHz — ⚶27 — dB
5.3 802.15.4 Radio
T able 5-16. 802.15.4 RF Characteristics
Name Description
Center frequency range of operating channel 2405 ~ 2480 MHz
1 Zigbee in the 2.4 GHz range supports 16 channels at 5 MHz spacing from
channel 11 to channel 26.
5.3.1 802.15.4 RF Transmitter (TX) Characteristics
T able 5-17. 802.15.4 Transmitter Characteristics - 250 Kbps
Parameter Min T yp Max Unit
RF transmit power range ⚶24.0 — 20.0 dBm
EVM — 13% — —
5.3.2 802.15.4 RF Receiver (RX) Characteristics
T able 5-18. 802.15.4 Receiver Characteristics - 250 Kbps
Parameter Description Min T yp Max Unit
Sensitivity @1% PER — — ⚶104.0 — dBm
Maximum received signal @1% PER — — 8 — dBm
Relative jamming level
Adjacent channel
F = F0 + 5 MHz — 27 — dB
F = F0 – 5 MHz — 32 — dB
Alternate channel
F = F0 + 10 MHz — 47 — dB
F = F0 – 10 MHz — 50 — dB
6 Packaging
6 Packaging
• For information about tape, reel, and chip marking, please refer toEspressif Chip Packaging Information.
• The pins of the chip are numbered in anti-clockwise order starting from Pin 1 in the top view. For pin
numbers and pin names, see also Figure2-1 ESP32-C6 Pin Layout (QFN40, Top View)and Figure2-2
ESP32-C6 Pin Layout (QFN32, Top View).
Figure 6-1. QFN40 (5×5 mm) Package
Figure 6-2. QFN32 (5×5 mm) Package
7 Related Documentation and Resources
7 Related Documentation and Resources
Related Documentation
• ESP32-C6TechnicalReferenceManual–DetailedinformationonhowtousetheESP32-C6memoryandperipherals.
• Certificates
https://espressif.com/en/support/documents/certificates
• Documentation Updates and Update Notification Subscription
https://espressif.com/en/support/download/documents
Developer Zone
• ESP-IDF and other development frameworks on GitHub.
https://github.com/espressif
• ESP32 BBS Forum – Engineer-to-Engineer (E2E) Community for Espressif products where you can post questions,
share knowledge, explore ideas, and help solve problems with fellow engineers.
https://esp32.com/
• The ESP Journal – Best Practices, Articles, and Notes from Espressif folks.
https://blog.espressif.com/
• See the tabsSDKs and Demos, Apps, Tools, AT Firmware.
https://espressif.com/en/support/download/sdks-demos
Products
• ESP32-C6 Series SoCs – Browse through all ESP32-C6 SoCs.
https://espressif.com/en/products/socs?id=ESP32-C6
• ESP32-C6 Series Modules – Browse through all ESP32-C6-based modules.
https://espressif.com/en/products/modules?id=ESP32-C6
• ESP32-C6 Series DevKits – Browse through all ESP32-C6-based devkits.
https://espressif.com/en/products/devkits?id=ESP32-C6
• ESP Product Selector – Find an Espressif hardware product suitable for your needs by comparing or applying filters.
https://products.espressif.com/#/product-selector?language=en
Contact Us
• See the tabsSales Questions, Technical Enquiries, Circuit Schematic & PCB Design Review , Get Samples
(Online stores),Become Our Supplier, Comments & Suggestions.
https://espressif.com/en/contact-us/sales-questions
Appendix A – ESP32-C6 Consolidated Pin Overview
Appendix A – ESP32-C6 Consolidated Pin Overview
T able 7-1. QFN40 Pin Overview
Pin Pin Pin Pin Providing Pin Settings Analog Function LP IO MUX Function IO MUX Function
No. Name T ype Power At Reset After Reset 0 1 0 1 0 T ype 1 T ype 2 T ype
1 ANT Analog
2 VDDA3P3 Power
3 VDDA3P3 Power
4 CHIP_PU Analog
5 VDDPST1 Power
6 XTAL_32K_P IO VDDPST1 XTAL_32K_P ADC1_CH0 LP_GPIO0 LP_UART_DTRN GPIO0 I/O/T GPIO0 I/O/T
7 XTAL_32K_N IO VDDPST1 XTAL_32K_N ADC1_CH1 LP_GPIO1 LP_UART_DSRN GPIO1 I/O/T GPIO1 I/O/T
8 GPIO2 IO VDDPST1 IE IE ADC1_CH2 LP_GPIO2 LP_UART_RTSN GPIO2 I/O/T GPIO2 I/O/T FSPIQ I1/O/T
9 GPIO3 IO VDDPST1 IE IE ADC1_CH3 LP_GPIO3 LP_UART_CTSN GPIO3 I/O/T GPIO3 I/O/T
10 MTMS IO VDDPST1 IE IE ADC1_CH4 LP_GPIO4 LP_UART_RXD MTMS I1 GPIO4 I/O/T FSPIHD I1/O/T
11 MTDI IO VDDPST1 IE IE ADC1_CH5 LP_GPIO5 LP_UART_TXD MTDI I1 GPIO5 I/O/T FSPIWP I1/O/T
12 MTCK IO VDDPST1 IE, WPU ADC1_CH6 LP_GPIO6 LP_I2C_SDA MTCK I1 GPIO6 I/O/T FSPICLK I1/O/T
13 MTDO IO VDDPST1 IE LP_GPIO7 LP_I2C_SCL MTDO O/T GPIO7 I/O/T FSPID I1/O/T
14 GPIO8 IO VDDPST2 IE IE GPIO8 I/O/T GPIO8 I/O/T
15 GPIO9 IO VDDPST2 IE, WPU IE, WPU GPIO9 I/O/T GPIO9 I/O/T
16 GPIO10 IO VDDPST2 IE GPIO10 I/O/T GPIO10 I/O/T
17 GPIO11 IO VDDPST2 IE GPIO11 I/O/T GPIO11 I/O/T
18 GPIO12 IO VDDPST2 IE USB_D- GPIO12 I/O/T GPIO12 I/O/T
19 GPIO13 IO VDDPST2 IE, WPU USB_D+ GPIO13 I/O/T GPIO13 I/O/T
20 SPICS0 IO VDD_SPI WPU IE, WPU SPICS0 O/T GPIO24 I/O/T
21 SPIQ IO VDD_SPI WPU IE, WPU SPIQ I1/O/T GPIO25 I/O/T
22 SPIWP IO VDD_SPI WPU IE, WPU SPIWP I1/O/T GPIO26 I/O/T
23 VDD_SPI Power/IO — VDD_SPI GPIO27 I/O/T GPIO27 I/O/T
24 SPIHD IO VDD_SPI WPU IE, WPU SPIHD I1/O/T GPIO28 I/O/T
25 SPICLK IO VDD_SPI WPU IE, WPU SPICLK O/T GPIO29 I/O/T
26 SPID IO VDD_SPI WPU IE, WPU SPID I1/O/T GPIO30 I/O/T
27 GPIO15 IO VDDPST2 IE IE GPIO15 I/O/T GPIO15 I/O/T
28 VDDPST2 Power
29 U0TXD IO VDDPST2 WPU U0TXD O GPIO16 I/O/T FSPICS0 I1/O/T
30 U0RXD IO VDDPST2 IE, WPU U0RXD I1 GPIO17 I/O/T FSPICS1 O/T
31 SDIO_CMD IO VDDPST2 WPU IE SDIO_CMD I1/O/T GPIO18 I/O/T FSPICS2 O/T
32 SDIO_CLK IO VDDPST2 WPU IE SDIO_CLK I1 GPIO19 I/O/T FSPICS3 O/T
33 SDIO_DATA0 IO VDDPST2 WPU IE SDIO_DATA0 I1/O/T GPIO20 I/O/T FSPICS4 O/T
34 SDIO_DATA1 IO VDDPST2 WPU IE SDIO_DATA1 I1/O/T GPIO21 I/O/T FSPICS5 O/T
35 SDIO_DATA2 IO VDDPST2 WPU IE SDIO_DATA2 I1/O/T GPIO22 I/O/T
36 SDIO_DATA3 IO VDDPST2 WPU IE SDIO_DATA3 I1/O/T GPIO23 I/O/T
37 VDDA1 Power
38 XTAL_N Analog
39 XTAL_P Analog
40 VDDA2 Power
41 GND Power
* For details, see Section2 Pins. Regarding highlighted cells, see Section2.3.4 Restrictions for GPIOs and LP GPIOs.
Appendix A – ESP32-C6 Consolidated Pin OverviewT able 7-2. QFN32 Pin Overview
Pin Pin Pin Pin Providing Pin Settings Analog Function LP IO MUX Function IO MUX Function
No. Name T ype Power At Reset After Reset 0 1 0 1 0 T ype 1 T ype 2 T ype
1 ANT Analog
2 VDDA3P3 Power
3 VDDA3P3 Power
4 CHIP_PU Analog
5 VDDPST1 Power
6 XTAL_32K_P IO VDDPST1 XTAL_32K_P ADC1_CH0 LP_GPIO0 LP_UART_DTRN GPIO0 I/O/T GPIO0 I/O/T
7 XTAL_32K_N IO VDDPST1 XTAL_32K_N ADC1_CH1 LP_GPIO1 LP_UART_DSRN GPIO1 I/O/T GPIO1 I/O/T
8 GPIO2 IO VDDPST1 IE IE ADC1_CH2 LP_GPIO2 LP_UART_RTSN GPIO2 I/O/T GPIO2 I/O/T FSPIQ I1/O/T
9 GPIO3 IO VDDPST1 IE IE ADC1_CH3 LP_GPIO3 LP_UART_CTSN GPIO3 I/O/T GPIO3 I/O/T
10 MTMS IO VDDPST1 IE IE ADC1_CH4 LP_GPIO4 LP_UART_RXD MTMS I1 GPIO4 I/O/T FSPIHD I1/O/T
11 MTDI IO VDDPST1 IE IE ADC1_CH5 LP_GPIO5 LP_UART_TXD MTDI I1 GPIO5 I/O/T FSPIWP I1/O/T
12 MTCK IO VDDPST1 IE, WPU ADC1_CH6 LP_GPIO6 LP_I2C_SDA MTCK I1 GPIO6 I/O/T FSPICLK I1/O/T
13 MTDO IO VDDPST1 IE LP_GPIO7 LP_I2C_SCL MTDO O/T GPIO7 I/O/T FSPID I1/O/T
14 GPIO8 IO VDDPST2 IE IE GPIO8 I/O/T GPIO8 I/O/T
15 GPIO9 IO VDDPST2 IE, WPU IE, WPU GPIO9 I/O/T GPIO9 I/O/T
16 GPIO12 IO VDDPST2 IE USB_D- GPIO12 I/O/T GPIO12 I/O/T
17 GPIO13 IO VDDPST2 IE, WPU USB_D+ GPIO13 I/O/T GPIO13 I/O/T
18 GPIO14 IO VDDPST2 IE GPIO14 I/O/T GPIO14 I/O/T
19 GPIO15 IO VDDPST2 IE IE GPIO15 I/O/T GPIO15 I/O/T
20 VDDPST2 Power
21 U0TXD IO VDDPST2 WPU U0TXD O GPIO16 I/O/T FSPICS0 I1/O/T
22 U0RXD IO VDDPST2 IE, WPU U0RXD I1 GPIO17 I/O/T FSPICS1 O/T
23 SDIO_CMD IO VDDPST2 WPU IE SDIO_CMD I1/O/T GPIO18 I/O/T FSPICS2 O/T
24 SDIO_CLK IO VDDPST2 WPU IE SDIO_CLK I1 GPIO19 I/O/T FSPICS3 O/T
25 SDIO_DATA0 IO VDDPST2 WPU IE SDIO_DATA0 I1/O/T GPIO20 I/O/T FSPICS4 O/T
26 SDIO_DATA1 IO VDDPST2 WPU IE SDIO_DATA1 I1/O/T GPIO21 I/O/T FSPICS5 O/T
27 SDIO_DATA2 IO VDDPST2 WPU IE SDIO_DATA2 I1/O/T GPIO22 I/O/T
28 SDIO_DATA3 IO VDDPST2 WPU IE SDIO_DATA3 I1/O/T GPIO23 I/O/T
29 VDDA1 Power
30 XTAL_N Analog
31 XTAL_P Analog
32 VDDA2 Power
33 GND Power
* For details, see Section2 Pins. Regarding highlighted cells, see Section2.3.4 Restrictions for GPIOs and LP GPIOs.
Revision History
Revision History
Date Version Release notes
2023-01-16 v0.5 Preliminary release
www.espressif.com
Disclaimer and Copyright Notice
Information in this document, including URL references, is subject to change without notice.
ALL THIRD PARTYnS INFORMATION IN THIS DOCUMENT IS PROVIDED AS IS WITH NO
WARRANTIES TO ITS AUTHENTICITY AND ACCURACY.
NO WARRANTY IS PROVIDED TO THIS DOCUMENT FOR ITS MERCHANTABILITY, NON-
INFRINGEMENT, FITNESS FOR ANY PARTICULAR PURPOSE, NOR DOES ANY WARRANTY
OTHERWISE ARISING OUT OF ANY PROPOSAL, SPECIFICATION OR SAMPLE.
All liability, including liability for infringement of any proprietary rights, relating to use of information
in this document is disclaimed. No licenses express or implied, by estoppel or otherwise, to any
intellectual property rights are granted herein.
The Wi-Fi Alliance Member logo is a trademark of the Wi-Fi Alliance. The Bluetooth logo is a
registered trademark of Bluetooth SIG.
All trade names, trademarks and registered trademarks mentioned in this document are property
of their respective owners, and are hereby acknowledged.
Copyright © 2023 Espressif Systems (Shanghai) Co., Ltd. All rights reserved.