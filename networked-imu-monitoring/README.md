# Networked IMU Monitoring System

**Technologies:** C++, Qt 6, Raspberry Pi, Linux, I²C, TCP/IP, Apache, CGI, OpenGL

## Overview

A collaborative hardware–software project for monitoring and visualizing inertial measurement unit (IMU) data over a network.

The system uses a Raspberry Pi connected to an LSM6DS33 accelerometer and gyroscope. Sensor measurements are processed to estimate device orientation and transmitted to a desktop client for visualization.

## System Architecture

The system consists of three main components:

1. **Sensor and processing layer:** Reads accelerometer and gyroscope measurements over I²C and estimates device orientation using the Madgwick sensor fusion algorithm.
2. **Communication layer:** A C++ TCP server running on Raspberry Pi sends quaternion orientation data to the client application. A multithreaded implementation separates sensor reading, data processing, and communication.
3. **Visualization layer:** A Qt 6 desktop application receives quaternion data and displays the orientation using a 3D cube.

An additional Apache/CGI interface exposes quaternion measurements through a browser.

## Features

- Raspberry Pi and IMU sensor integration.
- C++ hardware abstraction layer for I²C communication.
- Quaternion-based orientation estimation.
- Multithreaded sensor processing using C++ threads, mutexes, and condition variables.
- TCP client-server communication.
- Qt 6 GUI with 3D orientation visualization.
- Web-based sensor data display using Apache and CGI.

## Development and Testing

The project involved configuring the Raspberry Pi environment, integrating IMU hardware, developing the sensor-processing components, and troubleshooting networking and hardware-access issues.

During development, the team investigated web-server configuration conflicts and I²C device permission errors.

## Project Context

Developed collaboratively as part of university Hardware/Software Co-Design coursework.

Source code and additional documentation will be included where publication is permitted.
