# esp32-c6-opentherm

OpenTherm-to-Zigbee bridge for the WeAct ESP32-C6-A. The firmware joins an existing Zigbee network as an End Device with a Thermostat cluster server and drives a boiler as an OpenTherm master.

## Hardware

- [WeAct ESP32-C6-A](knowledge/esp32/weact-esp32-c6-a.md) (or ESP32-C6-DevKitC-1)
- OpenTherm adapter wired to **GPIO12** (in) and **GPIO13** (out) by default
- OpenTherm two-wire connection to a compatible boiler

## Dev container secrets

Before the first dev container rebuild (or when setting up a new clone):

1. `cp .env.example .env` (file required; secret values optional)
2. Set `GH_TOKEN` in `.env` if you use the GitHub CLI (personal access token with repo scope)
3. Rebuild the dev container (**Dev Containers: Rebuild Container**)
4. Verify: `gh api user -q .login` (when `GH_TOKEN` is set); `git push` uses SSH

Rebuild after any `.env` change. The file is gitignored; never commit secrets.

## Build

From the dev container (ESP-IDF 5.4):

```bash
idf.py set-target esp32c6
idf.py build
```

Optional: override OpenTherm GPIOs in `idf.py menuconfig` under **OpenTherm Bridge**.

## Flash and monitor

First flash on a board used for Zigbee before:

```bash
idf.py -p /dev/ttyACM0 erase-flash
idf.py -p /dev/ttyACM0 flash monitor
```

Subsequent flashes:

```bash
idf.py -p /dev/ttyACM0 flash monitor
```

## Zigbee join test

1. Enable permit-join on your Zigbee coordinator (ZHA, Zigbee2MQTT, etc.).
2. Power or reset the ESP32-C6 board.
3. Confirm serial log shows `Joined network successfully`.
4. Confirm the coordinator discovers a Thermostat device (`esp32-c6-opentherm` / `OT-ZB-Bridge-v1`).

## OpenTherm test

With the adapter and boiler connected:

- Serial log shows ~1 Hz OpenTherm traffic (`Tboiler`, status keepalive).
- Changing heating setpoint or mode from Home Assistant updates OpenTherm TSet and CH enable.

**Note:** v1 reports boiler **water** temperature and accepts **water** setpoints on the Thermostat cluster, not room air values.

## Docs

- [CONTEXT.md](CONTEXT.md) — domain glossary
- [knowledge/](knowledge/index.md) — OKF knowledge bundle (OpenTherm, Zigbee, ESP32, bridge playbooks)
- [docs/adr/0001-okf-knowledge-system.md](docs/adr/0001-okf-knowledge-system.md) — knowledge system architecture
