---
type: Reference
title: Zigbee Cluster Library
description: ZCL Rev 8 overview — cluster model, foundation commands, HVAC chapter.
tags: [zigbee, zcl]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md
---

The [Zigbee Cluster Library (ZCL)](https://zigbeealliance.org/wp-content/uploads/2021/10/07-5123-08-Zigbee-Cluster-Library.pdf) (document 07-5123 Rev 8) defines interoperable **clusters** — attributes and commands — for Zigbee devices.

## Cluster model

```
Node
 └── Endpoint (0–255)
      ├── Server clusters (input) — expose attributes
      └── Client clusters (output) — send commands remotely
```

Foundation commands (Chapter 2): Read/Write Attributes, Configure Reporting, Discover Attributes.

## HVAC clusters (Chapter 6)

| Cluster ID | Name |
|------------|------|
| 0x0201 | **Thermostat** — primary HVAC control |
| 0x0202 | Fan Control |
| 0x0200 | Pump Configuration |

This firmware implements a **Thermostat server** on an End Device — see [Thermostat Cluster](/zigbee/thermostat-cluster.md).

# Citations

[1] [wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md](wiki/raw/zigbee/2019-12-zigbee-cluster-library-rev8.md)
