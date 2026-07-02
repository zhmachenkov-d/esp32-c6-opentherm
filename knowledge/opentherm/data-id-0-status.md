---
type: Data ID
title: Data ID 0 Status
description: Mandatory status exchange; master CH/DHW flags and slave fault/activity bits.
tags: [opentherm, status]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md
---

ID **0** is the mandatory status exchange. Initiate with `READ-DATA(id=0, master_status, 0x0000)` and expect `READ-ACK` with slave status. Do not use WRITE for ID 0.

## Master status (HB)

| Bit | Meaning |
|-----|---------|
| 0 | CH enable |
| 1 | DHW enable |
| 2 | Cooling enable |
| 3 | OTC active |
| 4 | CH2 enable |

## Slave status (LB)

| Bit | Meaning |
|-----|---------|
| 0 | Fault |
| 1 | CH active |
| 2 | DHW active |
| 3 | Flame on |
| 4 | Cooling active |
| 5 | CH2 active |
| 6 | Diagnostic event |

Status bits use **flag8** encoding — see [OpenTherm Data Encoding](/opentherm/opentherm-data-encoding.md).

# Citations

[1] [wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md](wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md)
