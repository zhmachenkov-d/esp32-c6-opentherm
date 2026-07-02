# esp32-c6-opentherm

OpenTherm-to-Zigbee bridge firmware for ESP32-C6 hardware: OpenTherm master to a boiler, Zigbee Thermostat cluster for Home Assistant.

## Language

**Bridge**:
Firmware role that translates Zigbee Thermostat cluster commands to OpenTherm master frames and exposes boiler readings on Zigbee.
_Avoid_: Gateway, adapter (when meaning this firmware)

**Boiler**:
The OpenTherm slave on the two-wire OT/+ bus; the heating appliance being controlled.
_Avoid_: Furnace, heater (generic)

**Dev container**:
A reproducible Linux development environment for this repo, built from Espressif's official ESP-IDF Docker image.
_Avoid_: Docker setup, dev environment, container config

**Dev secrets file** (`.env`):
A gitignored, local-only file at the repo root holding secrets consumed by the dev container (currently `GH_TOKEN` for GitHub CLI). The file must exist before a dev container rebuild; secret values inside it are optional. Non-secret dev settings belong in committed config, not here. Changes take effect only after a full dev container rebuild. Git authentication uses host SSH keys, not this file.
_Avoid_: Environment config, local overrides (when meaning non-secrets)

**Dev secrets template** (`.env.example`):
The committed schema for the dev secrets file: every secret key the dev container reads must appear here with a comment. Onboarding copies it to `.env`.
_Avoid_: Example config, env sample

**ESP-IDF**:
Espressif's IoT firmware framework used to build, flash, and monitor firmware for this project. Version 5.4 in the dev container.
_Avoid_: SDK, IDF (alone without version context)

**Local temperature (device)**:
Boiler flow water temperature from OpenTherm data ID 25 (Tboiler), exposed as the Zigbee Thermostat `LocalTemperature` attribute in v1.
_Avoid_: Room temperature, indoor temp (when meaning this value)

**Target**:
The ESP chip family an ESP-IDF build is compiled for. This project uses **esp32c6**.
_Avoid_: Board, chip, platform (when meaning the IDF target)

**Water setpoint**:
The canonical meaning of Zigbee `OccupiedHeatingSetpoint` in v1: boiler water temperature target mapped to OpenTherm ID 1 (TSet), not room air setpoint.
_Avoid_: Room setpoint, target temperature (without qualifier)

**Standard cluster mapping**:
An OpenTherm Data ID exposed on a Zigbee cluster from the ZCL specification (e.g. Thermostat `LocalTemperature` for ID 25), rather than on the spillover cluster.
_Avoid_: Native mapping, ZCL attribute map

**Spillover cluster**:
The manufacturer-specific Zigbee cluster that exposes OpenTherm Data IDs with no standard cluster mapping. Each ID appears here at most once.
_Avoid_: Custom cluster, register cluster (when meaning this cluster)

**Available Data ID**:
An OpenTherm Data ID the boiler supports: discovery READ returns `READ-ACK` or `DATA_INVALID`, but not `UNKNOWN-DATAID`.
_Avoid_: Supported ID, valid ID (when meaning discovery result)

**Poll tier**:
A scheduling class for polling available Data IDs — fast (~1 s) for live control and sensors, slow (~30–60 s) for config and counters.
_Avoid_: Priority, poll rate

**Adaptive promotion**:
A Data ID temporarily moves to the fast poll tier after a Zigbee write or a detected value change, then decays back to its default tier.
_Avoid_: Dynamic polling, boost mode

**Master-writable Data ID**:
An available Data ID the OpenTherm spec allows the master to change via `WRITE-DATA`. Zigbee writes are accepted only for these IDs (plus existing Thermostat paths for ID 0 status bits).
_Avoid_: Writable register, output ID

**Discovery-driven endpoint**:
A Zigbee endpoint created when discovery finds an available Data ID that has a standard cluster mapping.
_Avoid_: Dynamic endpoint, sensor endpoint

**Spillover endpoint**:
The fixed Zigbee endpoint that hosts the spillover cluster for available Data IDs with no standard cluster mapping.
_Avoid_: Register endpoint, custom endpoint

**Cached discovery catalog**:
The NVS-persisted set of available Data IDs from the last successful discovery scan, used to register endpoints before Zigbee join on subsequent boots.
_Avoid_: ID cache, register map

**Catalog rescan**:
A forced full discovery scan (IDs 0–127) that replaces the cached discovery catalog, updates endpoint topology, and may add or remove discovery-driven endpoints.
_Avoid_: Rediscovery, refresh

**Catalog clear**:
Erasing the cached discovery catalog from NVS so the next boot performs first-boot discovery. Triggered via the spillover cluster `ClearCatalog` manufacturer command.
_Avoid_: Cache reset, factory reset (when meaning catalog only)

**Invalid reading**:
An OpenTherm `DATA_INVALID` response for an available Data ID. Exposed on Zigbee as that attribute type's invalid sentinel (e.g. `0x8000` for temperatures), not a converted value.
_Avoid_: No data, stale value

**Catalog validation**:
Re-probing cached Data IDs to detect boiler capability changes. Runs on boot and on manual `RescanCatalog` only — not on a periodic schedule.
_Avoid_: Background scan, auto rediscovery

**WeAct ESP32-C6-A**:
The physical development board for this project; pin-compatible with Espressif ESP32-C6-DevKitC-1.
_Avoid_: DevKit, board, ESP32-C6 (when meaning this specific hardware)

## Knowledge system

**Concept**:
A single unit of knowledge in the project's compiled knowledge bundle, represented as one markdown file with YAML frontmatter and a markdown body.
_Avoid_: Page, note, article (when the OKF identity matters)

**Knowledge bundle**:
The compiled, agent-readable directory tree under `knowledge/` that contains concepts plus `index.md` and `log.md`.
_Avoid_: Wiki, docs folder, vault

**Raw source**:
An immutable input document under `wiki/raw/` that the agent reads and compiles into one or more **Concepts**.
_Avoid_: Concept, bundle entry, article

**Playbook**:
A project-owned **Concept** in `knowledge/bridge/` that documents firmware behavior, mappings, or procedures synthesized from reference concepts.
_Avoid_: SOP, runbook (when meaning this OKF type)
