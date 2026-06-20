# OpenTherm Arduino/ESP8266/ESP32 Library

> Source: https://github.com/ihormelnyk/opentherm_library
> Collected: 2026-06-20
> Published: 2024-02-08

## README

This library provides implementation of OpenTherm protocol.

OpenTherm Library is based on OpenTherm protocol specification v2.2 and works with all OpenTherm compatible boilers. Library can be easily installed into Arduino IDE and compiled for Arduino, ESP8266/ESP32 and other similar controllers.

OpenTherm protocol requires simple low voltage twowire connection to boiler, but voltage levels (7..15V) still much higher than Arduino/ESP8266 levels, which requires OpenTherm Adapter (http://ihormelnyk.com/opentherm_adapter).

This version of library uses interrupts to achieve better stability and synchronization with boiler.

### Using OpenTherm Library you will be able:

- control your boiler remotely (get status, switch on/off heating/water heating, set water temperature and much more)
- make custom thermostat

### Configuration and Usage

```c
#include <OpenTherm.h>
```

Choose 2 GPIO pins connected to OpenTherm Adapter. Input pin must support interrupts. Controller output pin connects to adapter input and vice versa.

```c
const int inPin = 2;
const int outPin = 3;

OpenTherm ot(inPin, outPin);

void handleInterrupt() {
	ot.handleInterrupt();
}

void setup()
{
    ot.begin(handleInterrupt);
}

void loop()
{
    ot.setBoilerStatus(enableCentralHeating, enableHotWater, enableCooling);
    ot.setBoilerTemperature(64);
    float temperature = ot.getBoilerTemperature();
    delay(1000);
}
```

According to OpenTherm Protocol specification master (controller) must communicate at least every 1 sec.

Detailed documentation: http://ihormelnyk.com/opentherm_library

### OpenTherm Adapter Schematic

http://ihormelnyk.com/Content/Pages/opentherm_adapter/opentherm_adapter_schematic.png

### Arduino UNO Connection

http://ihormelnyk.com/Content/Pages/opentherm_adapter/opentherm_adapter_arduino_connection.png

### License

Copyright (c) 2018 Ihor Melnyk. Licensed under the MIT license.

## library.properties

```
name=OpenTherm Library
version=1.1.5
author=Ihor Melnyk <ihor.melnyk@gmail.com>
maintainer=Ihor Melnyk <ihor.melnyk@gmail.com>
sentence=OpenTherm Library for HVAC system control communication using Arduino and ESP8266/ESP32 hardware.
paragraph=OpenTherm Library is based on OpenTherm protocol specification v2.2 and works with all OpenTherm compatible boilers.
category=Communication
url=https://github.com/ihormelnyk/opentherm_library
architectures=*
includes=OpenTherm.h
```

## Repository layout

- `src/OpenTherm.h`, `src/OpenTherm.cpp` — core library
- `examples/` — demo sketches
- `CMakeLists.txt` — ESP-IDF component registration (`PRIV_REQUIRES arduino`)

## OpenTherm.h — key API (excerpt)

Frame structure comment: P | MSG-TYPE | SPARE | DATA-ID | DATA-VALUE

**OpenThermMessageType:** READ_DATA, WRITE_DATA, INVALID_DATA, READ_ACK, WRITE_ACK, DATA_INVALID, UNKNOWN_DATA_ID

**OpenThermMessageID (selected):** Status=0, TSet=1, SConfigSMemberIDcode=3, Tboiler=25, Tdhw=26, Toutside=27, Tret=28, RelModLevel=17, OpenThermVersionMaster=124, OpenThermVersionSlave=125

**OpenThermResponseStatus:** NONE, SUCCESS, INVALID, TIMEOUT

**Class OpenTherm:** constructor(inPin, outPin, isSlave=false); begin(handleInterrupt); sendRequest; sendRequestAsync; buildRequest; process (async); setBoilerStatus; setBoilerTemperature; getBoilerTemperature; getReturnTemperature; getDHWTemperature; getModulation; getPressure; getFault; static parsers isFlameOn, isCentralHeatingActive, etc.

Default pins in header: inPin=4, outPin=5. ESP8266 uses ICACHE_RAM_ATTR on interrupt handler.
