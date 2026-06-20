# esp32-c6-opentherm

OpenTherm boiler controller firmware for ESP32-C6 hardware.

## Language

**Dev container**:
A reproducible Linux development environment for this repo, built from Espressif's official ESP-IDF Docker image.
_Avoid_: Docker setup, dev environment, container config

**ESP-IDF**:
Espressif's IoT firmware framework used to build, flash, and monitor firmware for this project. Version 5.4 in the dev container.
_Avoid_: SDK, IDF (alone without version context)

**Target**:
The ESP chip family an ESP-IDF build is compiled for. This project uses **esp32c6**.
_Avoid_: Board, chip, platform (when meaning the IDF target)

**WeAct ESP32-C6-A**:
The physical development board for this project; pin-compatible with Espressif ESP32-C6-DevKitC-1.
_Avoid_: DevKit, board, ESP32-C6 (when meaning this specific hardware)
