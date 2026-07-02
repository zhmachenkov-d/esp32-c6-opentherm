# Layered OpenTherm-to-Zigbee mapping (v2)

v2 exposes every **available Data ID** from the boiler on Zigbee using a three-layer model: standard ZCL clusters first (Thermostat, Analog Input/Output, Multistate, etc.), a fixed **spillover cluster** for OEM/diagnostic IDs with no ZCL analogue, and no duplicate attributes across layers. Discovery probes IDs 0–127 (`READ-ACK` or `DATA_INVALID` = available); the result is cached in NVS for fast subsequent boots. Polling is tiered (fast ~1 s / slow ~60 s) with adaptive promotion after writes or value changes. Zigbee writes are OT-spec strict (master-writable IDs only). Catalog validation runs on boot and via `RescanCatalog` / `ClearCatalog` manufacturer commands — not on a periodic schedule.

**Considered options:** custom cluster only (rejected — breaks HA Thermostat semantics); full mirror in spillover (rejected — dual-write/sync bugs); flat round-robin polling (rejected — bus latency); try-write on all IDs (rejected — invalid OT frames); join-first discovery (rejected — stale endpoint topology on coordinators).

**Consequences:**

- Endpoint 10 stays the Thermostat cluster (v1 water setpoint, local temperature, system mode preserved).
- Discovery-driven endpoints are created per available ID with a standard cluster mapping; spillover endpoint is fixed.
- `OpenThermMessageID` enum, spec table (`ot_data_catalog`), discovery engine, poll scheduler, and `zb_ot_bridge` refactor are required before feature-complete v2.
- Home Assistant needs a small quirk or script for `RescanCatalog` / `ClearCatalog`; native entities work for standard-mapped IDs.
- Domain terms live in `CONTEXT.md`; implementation steps in `.plans/v2-layered-ot-zigbee-mapping.md`.
