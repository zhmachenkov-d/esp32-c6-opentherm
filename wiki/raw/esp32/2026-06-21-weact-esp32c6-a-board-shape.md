# WeAct ESP32-C6-A Board Shape (外形)

> Source: https://github.com/WeActStudio/WeActStudio.ESP32-C6-A/blob/main/Hardware/ESP32C6-A%20Board%20Shape%20%E5%A4%96%E5%BD%A2.pdf
> Collected: 2026-06-21
> Published: 2023-07-17 (PDF creation date)

WeActStudio — mechanical board-outline drawing for the ESP32-C6-A development board (`Hardware/ESP32C6-A Board Shape 外形.pdf`).

Binary copy: [weact-esp32c6-a-board-shape.pdf](weact-esp32c6-a-board-shape.pdf)

## Document properties

| Property | Value |
|----------|-------|
| Pages | 1 |
| Format | Vector drawing (jsPDF export) |
| Canvas size | 107.48 × 174.83 mm (jsPDF page units) |
| Content | Board outline, connector positions, pin silkscreen labels |

No numeric dimension callouts are embedded as extractable text; use the PDF drawing directly (or measure a physical board) for enclosure and keep-out design.

## Pin silkscreen labels (from drawing)

Power and control:

- **3V3**, **5V**, **G** (ground, multiple)
- **RST**, **RESET**, **BOOT**
- **USB** (USB connector, shown at both board ends in drawing)
- **UART**, **TX**, **RX**
- **NC** (no connect)
- **RGB** (addressable LED)

GPIO numbers shown on header pins:

- **0**, **1**, **2**, **3**, **4**, **5**, **6**, **7**, **8**, **9**, **10**, **11**, **12**, **13**, **15**, **18**, **19**, **20**, **21**, **22**, **23**

Pin numbering matches the ESP32-C6-DevKitC-1 style header layout (P2P replacement board).

## Usage

Reference for:

- Enclosure / mounting hole placement
- Connector and header keep-out zones
- Confirming physical pin positions when routing off-board circuits (e.g. OpenTherm interface)

Pair with [weact-esp32-c6-a-schematic](2026-06-21-weact-esp32-c6-a-schematic.md) for electrical pin function.
