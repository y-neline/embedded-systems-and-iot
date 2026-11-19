# LoRa-Based Time Dissemination System (NTP-like Synchronization)

An IoT time-synchronization system built on **TTGO LoRa32 (ESP32)** microcontrollers using **MicroPython**. A transmitter node reads UTC/local time from a **NEO-6MV2 GPS** module and broadcasts it over LoRa to a receiver node, which calculates transmission delay, adjusts its local clock, displays status on an **SSD1306 OLED**, and logs telemetry via a **Telegram Bot**.

## Hardware & Software
- **Microcontrollers:** 2x TTGO LoRa32 (ESP32)
- **GPS Module:** u-blox NEO-6MV2 (UART, 9600 baud)
- **Display:** Built-in SSD1306 128x64 I2C OLED
- **Firmware / IDE:** MicroPython via Thonny IDE

## Project Structure
- `src/transmitter_code.py` — Parses `$GPRMC` NMEA sentences from GPS, converts time to seconds, transmits over LoRa, and sends Telegram status logs.
- `src/receiver_code.py` — Listens for LoRa packets, measures transmission delay (~963 ms observed), updates OLED display, and pushes logs to Telegram.
- `src/lora.py` & `src/ssd1306.py` — MicroPython SX127x LoRa radio and SSD1306 OLED drivers.
- `docs/` — Project presentation and technical report.

## Setup & Usage
1. Flash MicroPython onto both TTGO boards and upload `lora.py` and `ssd1306.py` to the root filesystem of each device.
2. Update `wifi_ssid`, `wifi_password`, `telegram_bot_token`, and `telegram_chat_id` in both node scripts.
3. Upload `transmitter_code.py` (as `main.py`) to the GPS-equipped board and `receiver_code.py` (as `main.py`) to the receiver board.
