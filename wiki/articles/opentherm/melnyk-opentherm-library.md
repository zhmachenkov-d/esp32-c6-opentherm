# Melnyk OpenTherm Library

> Sources: Ihor Melnyk, 2024-02-08
> Raw: [2024-02-08-melnyk-opentherm-library](../../raw/opentherm/2024-02-08-melnyk-opentherm-library.md)

## Overview

The [OpenTherm Library](https://github.com/ihormelnyk/opentherm_library) by Ihor Melnyk is a widely used **OpenTherm v2.2 master** implementation for Arduino, ESP8266, and ESP32. Version **1.1.5** (MIT license). It uses **GPIO interrupts** for Manchester frame timing rather than delay-based bit banging, improving stability with boilers.

This project targets ESP32-C6 with ESP-IDF; this library is the primary reference for protocol logic and API design, even if the final code is native ESP-IDF rather than Arduino.

## Requirements

| Item | Detail |
|------|--------|
| Protocol | OpenTherm v2.2 OT/+ |
| Hardware | [OpenTherm adapter](http://ihormelnyk.com/opentherm_adapter) — MCU GPIO cannot drive 7–18 V / mA bus directly |
| Pins | 2 GPIOs: **in** (interrupt-capable) and **out**, cross-wired to adapter |
| Timing | Master must communicate **≥ every 1 s** (library examples use `delay(1000)`) |

Default constructor pins: `inPin=4`, `outPin=5` (override for ESP32-C6 board routing).

## Setup pattern

```cpp
#include <OpenTherm.h>

const int inPin = 4;
const int outPin = 5;
OpenTherm ot(inPin, outPin);

void IRAM_ATTR handleInterrupt() {
    ot.handleInterrupt();
}

void setup() {
    ot.begin(handleInterrupt);
}

void loop() {
    ot.setBoilerStatus(true, true, false);  // CH, DHW, cooling
    ot.setBoilerTemperature(64.0f);
    float t = ot.getBoilerTemperature();
    delay(1000);
}
```

On ESP8266/ESP32, mark the interrupt handler with `ICACHE_RAM_ATTR` / `IRAM_ATTR`.

## API surface

### High-level helpers

| Method | Maps to |
|--------|---------|
| `setBoilerStatus(ch, dhw, cooling, …)` | ID 0 status + control flags |
| `setBoilerTemperature(°C)` | ID 1 TSet (WRITE) |
| `getBoilerTemperature()` | ID 25 Tboiler (READ) |
| `getReturnTemperature()` | ID 28 Tret |
| `getDHWTemperature()` | ID 26 Tdhw |
| `getModulation()` | ID 17 RelModLevel |
| `getPressure()` | ID 18 CHPressure |
| `getFault()` | OEM fault from status response |

### Low-level / advanced

| Method | Purpose |
|--------|---------|
| `buildRequest(type, id, data)` | Construct 32-bit frame |
| `sendRequest(request)` | Blocking request/response |
| `sendRequestAsync(request)` | Non-blocking send |
| `process()` | Drive async state machine in `loop()` |
| `getLastResponseStatus()` | SUCCESS, INVALID, TIMEOUT, NONE |

`OpenThermMessageID` enum in `OpenTherm.h` lists all spec data IDs (0–127) with types — use for advanced reads/writes beyond the convenience methods.

### Response parsing (static)

`isFlameOn`, `isCentralHeatingActive`, `isHotWaterActive`, `isFault`, `isCoolingActive`, `getFloat`, `temperatureToData` — decode raw 32-bit response frames.

## ESP-IDF integration

The repo includes `CMakeLists.txt` registering the library as an ESP-IDF component:

```cmake
idf_component_register(
  SRCS "src/OpenTherm.cpp"
  INCLUDE_DIRS "." "src"
  PRIV_REQUIRES arduino
)
```

Usable with **Arduino-as-ESP-IDF-component** workflow. A pure ESP-IDF port would reuse `OpenTherm.cpp` logic but replace `Arduino.h` GPIO/timing with `driver/gpio` and ESP-IDF interrupt APIs.

## Installation

- **Arduino IDE:** Library Manager → search "OpenTherm Library" by Ihor Melnyk
- **PlatformIO / manual:** clone [github.com/ihormelnyk/opentherm_library](https://github.com/ihormelnyk/opentherm_library)
- **Docs / examples:** [ihormelnyk.com/opentherm_library](http://ihormelnyk.com/opentherm_library)

## Relevance to esp32-c6-opentherm

| Aspect | How this library helps |
|--------|------------------------|
| Frame encode/decode | Reference implementation of Manchester OT/+ |
| Message IDs | `OpenThermMessageID` enum matches spec |
| Master loop | 1 s polling pattern in examples |
| Adapter hardware | Schematic linked from README |

Options for this project: (1) embed as Arduino component under ESP-IDF, (2) port core to native ESP-IDF, or (3) reimplement using this source as spec checklist.

## See Also

- [OpenTherm Protocol v2.2](opentherm-protocol-v2-2.md) — specification this library implements
- [WeAct ESP32-C6-A Dev Board](../esp32/weact-esp32-c6-a-dev-board.md) — target hardware
- [ESP-IDF Get Started (ESP32-C6)](../esp-idf/esp32-c6-get-started.md) — toolchain setup
- GitHub: [ihormelnyk/opentherm_library](https://github.com/ihormelnyk/opentherm_library)
