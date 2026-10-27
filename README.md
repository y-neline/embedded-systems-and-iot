# Embedded Systems & IoT Projects

A portfolio of academic projects involving embedded systems, hardware–software integration, C/C++, Qt, wireless networking, and IoT applications.

These projects were developed collaboratively during my Computer Science studies at Nazarbayev University.

## Projects

### 1. Networked IMU Monitoring System

**Technologies:** C++, Qt 6, Raspberry Pi, Linux, I²C, TCP/IP, OpenGL

A sensor-monitoring system that processes inertial measurement unit (IMU) data on a Raspberry Pi and transmits orientation information to a desktop application.

**Key features:**
- I²C communication with accelerometer and gyroscope sensors.
- C++ hardware abstraction layer for sensor interfacing.
- Quaternion-based orientation estimation using the Madgwick algorithm.
- Multithreaded TCP server for sensor processing and communication.
- Qt desktop application for real-time 3D orientation visualization.

### 2. Web-Controlled Mobile Robot

**Technologies:** C++, Python, Raspberry Pi, Flask, GPIO, PWM, HTML, JavaScript

A mobile robot controlled remotely through a browser-based interface.

**Key features:**
- DC motor control using an L298N motor driver.
- PWM-based speed and direction control.
- Browser interface for forward, reverse, left, right, and stop commands.
- Live video streaming from a connected USB camera.
- Hardware integration and debugging.

### 3. LoRa-Based Time Dissemination System

**Technologies:** ESP32, MicroPython, LoRa, GPS, UART, SPI, I²C

A wireless communication prototype for transmitting GPS-derived time information between LoRa devices.

**Key features:**
- GPS time acquisition using a GPS module.
- Wireless time-data transmission over LoRa.
- Receiver-side time adjustment based on estimated delay.
- OLED display for received information.
- Telegram bot integration for remote monitoring.

### 4. Arduino-Based Automated Piano Tiles Robot

**Technologies:** Arduino Uno, C++, Photoresistors, Servomotors

A robotic system designed to detect black tiles in a mobile game and automatically press them using motor-driven styluses.

**Key features:**
- Analog light-sensor readings.
- Threshold-based tile detection.
- Servo motor control using Arduino.
- Hardware calibration and response-time adjustments.

## Technical Skills Demonstrated

**Programming:** C, C++, Python, MicroPython

**Embedded Platforms:** Raspberry Pi, Arduino, ESP32

**Frameworks and Tools:** Qt 6, Flask, Linux, Git

**Hardware Interfaces:** GPIO, I²C, SPI, UART, PWM

**Networking:** TCP/IP, ESP-NOW, LoRa, MQTT

## Collaboration and Attribution

The projects presented here were completed as part of university coursework in collaboration with other students. Project descriptions summarize the team's implementations, and individual contributions are identified where applicable.

Source code and additional technical documentation may be shared where permitted by project ownership and university policies.
