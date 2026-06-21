# ESP32-C6 Series Hardware Design Guidelines

> Source: https://github.com/WeActStudio/WeActStudio.ESP32-C6-A/blob/main/Doc/esp32-c6_hardware_design_guidelines_en.pdf
> Collected: 2026-06-21
> Published: 2023

Espressif Systems — Version 1.0. Upstream: https://espressif.com/sites/default/files/documentation/esp32-c6_hardware_design_guidelines_en.pdf

Binary copy: [esp32-c6_hardware_design_guidelines_en.pdf](esp32-c6_hardware_design_guidelines_en.pdf)

## Introduction

Hardware design guidelines give advice on how to integrate ESP32-C6 into other products. ESP32-C6 is a series of ultra-low-power SoCs with support for 2.4 GHz Wi-Fi 6 (802.11 ax), Bluetooth 5 (LE), Zigbee and Thread (802.15.4). These guidelines will help to ensure optimal performance of your product with respect to technical accuracy and conformity to Espressif's standards.

## 1 Overview

ESP32-C6 series is a low-power MCU-based SoC solution that supports 2.4 GHz Wi-Fi 6 (802.11 ax), Bluetooth 5 (LE), Zigbee 3.0 and Thread 1.3 (802.15.4). With its state-of-the-art power and RF performance, this SoC is an ideal choice for a wide variety of application scenarios relating to Internet of Things (IoT), smart home, industrial automation, health care, and consumer electronics.

ESP32-C6 has a high-performance (HP) 32-bit RISC-V processor and a low-power (LP) 32-bit RISC-V processor, operating at up to 160 MHz and 20 MHz respectively. The chip supports application development to operate without the need for a host MCU.

ESP32-C6 series provides a highly-integrated way to implement wireless communication technologies using a complete RF subsystem, including an antenna switch, RF balun, power amplifier, low noise amplifier (LNA), filter, power management unit, calibration circuits, etc. As a result, PCB size has been greatly reduced.

With its advanced calibration circuitry, ESP32-C6 series can dynamically adjust itself to remove external circuit imperfections or adapt to changes in external conditions. As such, the mass production of ESP32-C6 series does not require expensive and specialized Wi-Fi test equipment.

## 2 Schematic Checklist

The core circuitry of ESP32-C6 requires only 20 electrical components (resistors, capacitors, and inductors), one crystal and one SPI flash memory chip (optional for the QFN32 package). The high integration of ESP32-C6 allows for simple peripheral circuit design.

ESP32-C6 consists of variants in two packages, namely the QFN40 package and the QFN32 package. The main difference between these two packages is whether the flash is integrated into the chip's package.

Unless otherwise specified, "ESP32-C6" used in this document refers to the QFN40 variant of ESP32-C6.

Any basic ESP32-C6 circuit design may be broken down into 11 major sections: power supply; power-on sequence and system reset; flash; clock source; RF; UART; strapping pins; GPIO; ADC; USB; SDIO.

### 2.1 Power Supply

#### 2.1.1 Digital Power Supply

ESP32-C6 has pin5 VDDPST1 and pin28 VDDPST2 that supply power to LP digital pins/part of analog pins and HP digital pins respectively, in a voltage range of 3.0 V ~ 3.6 V. It is recommended to add a 0.1 µF filter capacitor close to each digital power supply pin.

Pin23 VDD_SPI can serve as the power supply for the external device at 3.3 V (typical value), provided by VDDPST2 via RSP I. Therefore, there will be a voltage drop on VDD_SPI to VDDPST2. It is recommended to add a 0.1 µF and a 1 µF filter capacitor close to VDD_SPI.

VDD_SPI can be connected to and powered by an external power supply. When not serving as a power supply pin, VDD_SPI can be used as GPIO27.

When using VDD_SPI as the power supply pin for the in-package flash or external 3.3 V flash, the supply voltage should be 3.0 V or above, so as to meet the requirements of flash's working voltage.

#### 2.1.2 Analog Power Supply

Pin2 VDDA3P3, pin3 VDDA3P3, pin37 VDDA1, and pin40 VDDA2 are the analog power supply pins, working at 3.0 V ~ 3.6 V.

Please be noted that the sudden increase in current draw, when ESP32-C6 is transmitting signals, may cause a power rail collapse. Therefore, it is highly recommended to add a 10 µF capacitor to the power pin2 and pin3 VDDA3P3, which can work in conjunction with the 1 µF capacitor. In addition, a CLC filter circuit needs to be added near VDDA3P3 pins so as to suppress high-frequency harmonics. The recommended rated current of the inductor is 500 mA or above. Place the appropriate decoupling capacitor near each analog power pin.

Notice:
- The recommended power supply voltage for ESP32-C6 is 3.3 V and the output current is no less than 500 mA.
- It is suggested to add another 10 µF capacitor at the power entrance. If the power entrance is close to pin2 and pin3, two 10 µF capacitors could be merged into one.
- It is suggested to add an ESD protection diode at the power entrance.

### 2.2 Power-up Timing and System Reset

#### 2.2.1 Power-up Timing

When ESP32-C6 uses a 3.3 V system power supply, the power rails need some time to stabilize before CHIP_PU is pulled up and the chip is activated. Therefore, CHIP_PU needs to be powered up after the 3.3 V rails have been brought up.

To ensure the correct power-up timing, it is advised to add an RC delay circuit at the CHIP_PU pin. The recommended setting for the RC delay circuit is usually R = 10 kΩ and C = 1 µF. However, specific parameters should be adjusted based on the characteristics of the actual power supply and the power-up and reset sequence timing of the chip.

#### 2.2.2 System Reset

CHIP_PU serves as the reset pin of ESP32-C6. The reset voltage (VIL_nRST) should be in the range of (–0.3 ~ 0.25 × VDDPST1) V. To avoid reboots caused by external interferences, make the CHIP_PU trace as short as possible. Also, add a pull-up resistor as well as a capacitor to the ground whenever possible.

Notice: CHIP_PU pin must not be left floating.

#### 2.2.3 Power-up and Reset Timing

| Parameter | Description | Min (µs) |
|-----------|-------------|----------|
| tSTBL | Time reserved for the power rails of VDDA3P3, VDDPST1, VDDPST2, VDDA1 and VDDA2 to stabilize before the CHIP_PU pin is pulled high to activate the chip | 50 |
| tRST | Time reserved for CHIP_PU to stay below VIL_nRST to reset the chip | 50 |

Notice: If your device needs to be frequently powered on and off, the power supply ramps up slowly, or the power supply such as solar photovoltaic is not stable, adding a single RC circuit might not meet the requirements for power-up and reset timing, and consequently the chip will not boot correctly. In this case, it is advised to take other approaches, such as adding an external reset chip or a watchdog timer IC. For chips with a 3.3 V flash, the reset threshold is suggested to be around 3.0 V.

### 2.3 Flash

ESP32-C6 series in QFN40 package can support up to 16 MB external flash, powered by VDD_SPI. It is recommended to add a zero-ohm series resistor on the SPI lines to lower the driving current, reduce interference to RF, adjust timing, and better shield from interference.

ESP32-C6 series of chips in QFN32 package have in-package SPI flash. The pins for flash are not bonded out.

### 2.4 Clock Source

ESP32-C6 has two clock sources: external clock source (compulsory) and RTC clock source (optional).

#### 2.4.1 External Clock Source (compulsory)

Currently, the ESP32-C6 firmware only supports 40 MHz crystal.

The initial values of external capacitors C1 and C4 can be determined according to the formula:

CL = (C1 × C4) / (C1 + C4) + Cstray

where the value of CL (Load Capacitance) can be found in the crystal's datasheet, and the value of Cstray refers to the PCB's stray capacitance. The values of C1 and C4 need to be further adjusted after an overall test.

In order to reduce the impact of high-frequency crystal harmonics on RF performance, please add a series component (resistor or inductor) on the XTAL_P clock trace. Initially it is suggested to use an inductor of 24 nH, and the value should be adjusted after an overall test. Note that the accuracy of the selected crystal should be within ±10 ppm.

Notice: Defects in the manufacturing of crystal and oscillators (for example, large frequency deviation of more than ±10 ppm) will affect the RF performance.

#### 2.4.2 RTC (optional)

ESP32-C6 supports main crystal division, an external 32.768 kHz crystal, or an external signal (e.g. an oscillator) to act as the RTC sleep clock, suitable for applications that require high-precision RTC clocks such as a scenario which requires the Bluetooth wake-up function.

Notice for 32.768 kHz crystal:
- Equivalent series resistance (ESR) ≤ 70 kΩ.
- Load capacitance at both ends should be configured according to the crystal's specification.
- The parallel resistor R is used for biasing the crystal circuit (5 MΩ < R ≤ 10 MΩ). In general, you do not need to populate the resistor.
- If the RTC source is not required, then the pins for the external 32.768 kHz crystal can be used as GPIOs.

External signal requirements (input to XTAL's P end through a DC blocking capacitor ~20 pF; XTAL's N end can be floating):
- Sine wave or square wave
- 0.6 < Vpp < VDD

### 2.5 RF

The RF circuit of the ESP32-C6 series of chips is mainly composed of three parts: the RF traces on the PCB board, the chip matching circuit, the antenna and the antenna matching circuit.

For the RF traces on the PCB board, 50 Ω impedance control is required. Chip matching circuit must be placed close to the chip. It is mainly used to adjust the impedance point and suppress harmonics. The CLC structure is preferred, and a set of LC can be added if space permits.

For the antenna and the antenna matching circuit, to ensure the radiation performance, the antenna's characteristic impedance must be around 50 Ω. Adding a CLC matching circuit near the antenna is recommended to adjust the antenna. However, if the available space is limited and the antenna impedance point can be guaranteed to be 50 Ω by simulation, then there is no need to add a matching circuit near the antenna.

RF tuning of PCB board depends greatly on the antenna and PCB layout. The initial value of the resistor can be 0 Ω. For ESP32-C6 series of chips, it is recommended to set the S11 parameter to 30+j0 Ω and the center frequency is 2442 MHz.

If the RF function is not required, the RF pin can be left floating.

Notice: The matching parameters vary with board, so the ones used in Espressif modules could not be applied directly.

### 2.6 UART

It is recommended to connect a 499 Ω series resistor to the U0TXD line in order to suppress the 80 MHz harmonics.

Usually UART0 is used as the serial port for download and log printing, and UART0 pins are fixed. Other UART interfaces can be mapped to any available GPIO by software configurations. For these interfaces, it is also recommended to add a series resistor to the TX line to suppress harmonics.

### 2.7 Strapping Pins

At each startup or reset, a chip requires some initial configuration parameters, such as in which boot mode to load the chip, etc. These parameters are passed over via the strapping pins. After reset, the strapping pins operate as regular IO pins.

GPIO8 and GPIO9 control the boot mode after the reset is released.

| Boot Mode | GPIO8 | GPIO9 |
|-----------|-------|-------|
| Default Configuration | – (Floating) | 1 (Pull-up) |
| SPI Boot (default) | Any value | 1 |
| Download Boot | 1 | 0 |
| Invalid combination | 0 | 0 |

The invalid combination (GPIO8=0, GPIO9=0) triggers unexpected behavior and should be avoided.

Strapping pin timing:

| Parameter | Description | Min (ms) |
|-----------|-------------|----------|
| tSU | Setup time — power rails stabilize before CHIP_PU is pulled high | 0 |
| tH | Hold time — chip reads strapping pin values after CHIP_PU is high | 3 |

Notice: Please do not add high-value capacitors at GPIO9, otherwise the chip might enter Download Boot mode when booted for the first time.

### 2.8 GPIO

The pins of ESP32-C6 series can be configured via IO MUX or GPIO matrix. IO MUX provides the default pin configurations, whereas the GPIO matrix is used to route signals from peripherals to GPIO pins.

When using GPIOs:
- Pay attention to their default configurations after reset. It is recommended to add a pull-up or pull-down to pins in high-impedance state.
- Avoid using the pins already occupied by flash.
- Pay attention to the states of strapping pins during power-up.

Pin settings abbreviations: IE — input enabled; WPU — internal weak pull-up enabled; WPD — internal weak pull-down enabled.

(Full pin overview table in original PDF — see datasheet for peripheral pin configurations.)

### 2.9 ADC

Please add a 0.1 µF filter capacitor between pins and ground when using the ADC function to improve accuracy.

### 2.10 USB

ESP32-C6 has a USB Serial/JTAG controller. GPIO12 and GPIO13 can be used as D- and D+ of USB respectively. It is recommended to reserve a series resistor and a capacitor to ground on each trace, and place them close to the chip side.

ESP32-C6 supports download and log printing over USB.

### 2.11 SDIO

ESP32-C6 series has only one SDIO slave controller that conforms to the industry-standard SDIO Specification Version 2.0. SDIO should be connected to specific GPIOs: SDIO_CMD, SDIO_CLK, SDIO_DATA0, SDIO_DATA1, SDIO_DATA2, and SDIO_DATA3. Please add a pull-up resistor to these GPIOs, and preferably reserve a series resistor on each trace.

## 3 PCB Layout Design

This chapter introduces the key points of how to design an ESP32-C6 PCB layout using the ESP32-C6-WROOM-1 module as an example.

### 3.1 General Principles of PCB Layout

Recommended four-layer PCB design:
- Layer 1 (TOP): Signal traces and components
- Layer 2 (GND): No signal traces — complete GND plane
- Layer 3 (POWER): GND plane under RF and crystal; route power and few signal traces
- Layer 4 (BOTTOM): Route power traces; no components

Two-layer PCB design is also possible:
- Layer 1 (TOP): Signal traces and components
- Layer 2 (BOTTOM): No components; minimal traces; complete GND plane for chip, RF and crystal

### 3.2 Positioning a Module on a Base Board

Minimize interference of the base board on the module's antenna performance. Place the module's on-board PCB antenna outside the base board when possible, with the feed point closest to the board edge.

If PCB antenna cannot be placed outside the board, ensure a clearance of at least 15 mm around antenna area (no copper, routing, components). Cut away base board under antenna area when possible.

When designing an end product, account for housing interference on the antenna and carry out RF verification. Test throughput and communication signal range of the whole product.

### 3.3 Power Supply

- Four-layer PCB preferred; power traces on inner third layer; at least two vias when main power traces cross layers
- Main 3.3 V power trace width ≥ 25 mil; VDDA3P3 traces ≥ 20 mil; other power traces ≥ 10 mil
- ESD protection diode next to power port; 10 µF capacitor before chip; star-shape topology for power branches
- All decoupling capacitors close to corresponding power pin; ground vias close to capacitor ground pad
- CLC filter capacitor near VDDA3P3 connected to fourth layer through via with keep-out on other layers
- VDDA3P3 surrounded by grounding copper, isolated from RF and GPIO traces
- Chip bottom ground pad: at least nine ground vias

### 3.4 Crystal

- Crystal far from clock pin (gap ≥ 2.4 mm); ground vias around clock trace
- No vias on clock input/output traces (no layer crossing)
- Series components close to chip side
- Matching capacitors on two sides of crystal, at end of clock trace
- No high-frequency digital signal traces under crystal
- No magnetic components nearby

### 3.5 RF

- RF trace: 50 Ω characteristic impedance; reference plane second layer; π-type matching close to chip
- RF trace on outer layer without vias; 135° bends or circular arcs
- Stub to ground at first matching capacitor ground pad (15 mil length, ~100 Ω ± 10% impedance) to suppress second harmonics
- Complete ground plane on adjacent layer; no traces under RF trace
- Keep USB, UART, crystals, DDR, high-frequency clocks away from RF antenna

Example four-layer stack-up for 50 Ω RF trace: width 12.6 mil, gap 12.2 mil (see original PDF Figure 21).

### 3.6 Flash

Place zero-ohm series resistors on SPI lines close to chip. Route SPI traces on inner layer; ground copper and vias around clock and data traces.

### 3.7 UART

Series resistor on U0TXD close to chip and away from crystal. U0TXD/U0RXD traces short, surrounded by ground copper and vias.

### 3.8 USB

RC circuit on USB traces closer to chip. Differential pairs, equal length, complete reference ground plane.

### 3.9 SDIO

Control parasitic capacitance. SDIO_CMD and SDIO_DATA0–3 trace length 3 mil longer or shorter than SDIO_CLK. Surround SDIO_CLK with ground. Path from SDIO GPIOs to master ≤ 2500 mil (preferably ≤ 2000 mil). Do not route SDIO traces across planes.

### 3.10 Typical Layout Problems and Solutions

**Q: Current ripple not large, but RF TX performance poor.**
Peak-to-peak ripple should be <80 mV (MCS7@11n) and <120 mV (11m@11b) when tested in normal working mode. Solution: add 10 µF filter capacitor close to analog power pin branch.

**Q: Power ripple small, but RF TX performance poor.**
Crystal quality, frequency offset, or interference from high-speed signals under crystal. Solution: re-layout per Section 3.4.

**Q: TX power much higher/lower than target, poor EVM.**
Impedance mismatch on RF-to-antenna line. Solution: match antenna with π-type circuit on RF trace.

**Q: TX OK but RX sensitivity low.**
External coupling to antenna (crystal harmonics, UART crossing RF). Solution: keep antenna away from crystals and high-frequency traces per Section 3.5.

## 4 Hardware Development

### 4.3 Download Instructions

Download firmware via UART or USB.

**UART download:**
1. Set Download Boot mode: IO8 high, IO9 low (IO9 pulled up by default)
2. Power on; verify Download Boot mode via UART0
3. Download firmware using Flash Download Tool via UART
4. After download, keep IO9 floating or pull high for SPI Boot mode
5. Power on again to execute new firmware

**USB download:**
1. If flash empty: set Download Boot mode (IO8 high, IO9 low)
2. Power on; verify Download Boot mode via USB
3. Download firmware using Flash Download Tool via USB
4. Pull IO9 high or floating for SPI Boot mode
5. Power on again
6. If flash not empty, start from step 3

Notice:
- Check chip entered Download Boot mode before downloading
- Serial tools cannot be used simultaneously with Flash Download Tool
- If USB GPIOs reconfigured or USB disabled in firmware, must enter Download Boot mode first for next USB download
- Reserve UART0 connector as alternative download path

## Related Documentation

- ESP32-C6 Series Datasheet
- ESP32-C6 Technical Reference Manual
