---
type: Protocol
title: OpenTherm Protocol
description: Point-to-point master/slave HVAC bus; OT/+ digital and OT/- analogue levels.
tags: [opentherm]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md
---

OpenTherm v2.2 defines communication between a **room unit (master)** and a **boiler (slave)** over a two-wire bus. This project implements the master on ESP32-C6; the boiler is the slave.

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

## Related concepts

- Frame structure and timing: [OpenTherm Frame Format](/opentherm/opentherm-frame-format.md)
- Temperature encoding: [OpenTherm Data Encoding](/opentherm/opentherm-data-encoding.md)
- Reference implementation: [Melnyk OpenTherm Library](/opentherm/melnyk-opentherm-library.md)

# Citations

[1] [wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md](wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md)
[2] [OpenTherm Protocol v2.2 PDF](https://ihormelnyk.com/Content/Pages/opentherm_library/Opentherm%20Protocol%20v2-2.pdf)
