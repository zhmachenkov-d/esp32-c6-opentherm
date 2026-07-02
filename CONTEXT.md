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
