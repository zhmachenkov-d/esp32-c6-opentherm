---
type: Protocol
title: OpenTherm Frame Format
description: OT/+ 32-bit frame layout, message types, and master/slave timing rules.
tags: [opentherm, timing]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md
---

Each OT/+ message is a **32-bit frame** with start/stop bits:

```
[P][MSG-TYPE:3][SPARE:4][DATA-ID:8][DATA-VALUE:16]
```

## Message types

| Master sends | Code | Slave replies | Code |
|--------------|------|---------------|------|
| READ-DATA | 000 | READ-ACK | 100 |
| WRITE-DATA | 001 | WRITE-ACK | 101 |
| INVALID-DATA | 010 | DATA-INVALID | 110 |
| — | — | UNKNOWN-DATAID | 111 |

## Timing rules

- Master initiates every conversation; exactly one request + one response
- Slave response window: **20–800 ms** after master frame
- Minimum **100 ms** gap before next master conversation
- Master must send at least one message every **1 s** (max **1.15 s** with tolerance)

Violating the 1 s rule causes boilers to fault or fall back to OT/-.

## Mandatory data IDs

Every OT/+ device must support:

| ID | Name | Direction | Notes |
|----|------|-----------|-------|
| **0** | Status | Master READ | Master status in HB, slave status in LB of response |
| **1** | Control setpoint | Master WRITE | CH water temp setpoint 0..100°C |
| **3** | Slave configuration | Master READ | DHW present, modulating/on-off, cooling, etc. |

See [Data ID 0 Status](/opentherm/data-id-0-status.md) and [Data ID 1 TSet](/opentherm/data-id-1-tset.md).

# Citations

[1] [wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md](wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md)
