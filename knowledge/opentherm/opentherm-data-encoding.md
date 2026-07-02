---
type: Reference
title: OpenTherm Data Encoding
description: f8.8 temperature encoding and flag8 status bit packing.
tags: [opentherm, encoding]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md
---

## f8.8 temperatures

Temperatures and setpoints use **f8.8** fixed point: 16-bit signed, 8 fractional bits (divide by 256 for °C).

Example: 21.5°C → `0x1580` (5504 / 256 = 21.5).

Used by [Data ID 1 TSet](/opentherm/data-id-1-tset.md) and [Data ID 25 Tboiler](/opentherm/data-id-25-tboiler.md).

## flag8 status fields

Status and configuration fields use **flag8** (8 single-bit flags in one byte). See [Data ID 0 Status](/opentherm/data-id-0-status.md).

# Citations

[1] [wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md](wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md)
