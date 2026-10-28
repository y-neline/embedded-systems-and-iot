# Networked IMU Monitoring System

**C++ | Qt 6 | Raspberry Pi | Linux | I²C | TCP/IP | Multithreading**

A hardware–software system for acquiring, processing, and visualizing inertial measurement unit (IMU) data over a network.

Developed as a collaborative university project in Hardware/Software Co-Design.

## Overview

This project implements an IMU monitoring system using a Raspberry Pi connected to an LSM6DS33 accelerometer and gyroscope.

The Raspberry Pi reads sensor measurements through I²C, processes the data using the Madgwick sensor fusion algorithm, and estimates the device's orientation as a quaternion.

The processed orientation data can be accessed through two interfaces:

- **Qt 6 desktop application:** Receives orientation data over TCP and visualizes it using a rotating 3D cube.
- **Web interface:** Displays quaternion measurements in a browser using Apache and a C++ CGI application.

## System Architecture

```text
             LSM6DS33 IMU Sensor
                      |
                      | I²C
                      v
                 Raspberry Pi
                      |
             C++ Sensor Interface
                      |
              Madgwick Algorithm
                      |
              Quaternion (w,x,y,z)
                      |
          +-----------+-----------+
          |                       |
          v                       v
      TCP Server              Apache / CGI
          |                       |
          | TCP/IP                | HTTP
          v                       v
    Qt 6 Desktop App         Web Browser
          |
          v
    3D Visualization
```

The system separates hardware communication, sensor processing, network communication, and visualization into different components.

### Sensor Interface

The C++ hardware abstraction layer consists of:

- `I2CDevice.h` — Handles low-level Linux I²C communication, including device initialization and register access.
- `IMUSensor.h` — Configures the LSM6DS33 sensor and retrieves accelerometer and gyroscope measurements.
- `MadgwickIMU.h` — Implements IMU sensor fusion to estimate orientation using quaternions.

### TCP Server

Two versions of the server were developed:

- `imu_server.cpp` — Single-threaded implementation for sensor processing and TCP transmission.
- `imu_server4.cpp` — Multithreaded implementation separating sensor acquisition, orientation estimation, and network communication.

The multithreaded version uses three C++ threads:

1. **Sensor thread:** Reads accelerometer and gyroscope measurements.
2. **Processing thread:** Applies sensor fusion and computes normalized quaternions.
3. **Communication thread:** Sends orientation data to a connected TCP client.

Mutexes and condition variables coordinate data exchange between threads.

### Qt Desktop Client

The Qt 6 application uses:

- `QTcpSocket` for network communication.
- Qt Widgets for the user interface.
- `QQuaternion` for representing orientation.
- `QOpenGLWidget` and `QPainter` for 3D cube visualization.

The client receives quaternion values in a comma-separated, newline-delimited format:

```text
0.999992,0.003363,0.001644,-0.001339
```

The received measurements are displayed numerically and used to update the cube's orientation.

### Web Interface

A separate C++ CGI application runs through Apache on the Raspberry Pi.

It reads IMU measurements, processes them using the sensor fusion algorithm, and generates an HTML page displaying quaternion values.

Unlike the continuously updating TCP client, the CGI implementation produces measurements when the page is requested.

## Technologies

| Category | Technologies |
|---|---|
| Programming | C++17 |
| Hardware | Raspberry Pi, LSM6DS33 IMU |
| Operating System | Linux / Raspberry Pi OS |
| Hardware Communication | I²C |
| Networking | TCP/IP, POSIX sockets, HTTP |
| Desktop GUI | Qt 6, Qt Widgets, Qt Network |
| Visualization | QOpenGLWidget, QPainter |
| Concurrency | std::thread, std::mutex, std::condition_variable |
| Web Server | Apache, CGI |
| Build Tools | CMake (Qt client), g++ |

## Project Structure

```text
networked-imu-monitoring/
├── README.md
├── sensor/
│   ├── I2CDevice.h
│   ├── IMUSensor.h
│   └── MadgwickIMU.h
├── server/
│   ├── imu_server.cpp
│   └── imu_server4.cpp
├── qt-client/
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── mainwindow.ui
│   ├── cubewidget.cpp
│   └── cubewidget.h
├── web-cgi/
│   └── imu_cgi.cpp
└── images/
```

The server and CGI components depend on the headers in `sensor/`. The Qt desktop client is a separate application.

## Development and Testing

The project was developed and tested using a Raspberry Pi connected to an IMU sensor and a separate computer running the Qt client.

Development included:

- Configuring Raspberry Pi OS and I²C communication.
- Reading and converting raw accelerometer and gyroscope measurements.
- Implementing quaternion-based orientation estimation.
- Establishing TCP communication between the Raspberry Pi and desktop client.
- Developing a Qt interface for displaying sensor orientation.
- Implementing a multithreaded server.
- Configuring Apache to expose IMU measurements through CGI.

### Challenges Encountered

**Web server configuration:** Initial attempts using Nginx encountered configuration issues. Apache was subsequently used for the CGI interface.

**Hardware access permissions:** The Apache service initially lacked permission to access `/dev/i2c-1`. The issue was resolved by configuring the service user's group permissions.

**Thread synchronization:** The multithreaded server uses mutexes and condition variables to coordinate sensor acquisition, processing, and transmission.

## Limitations and Possible Improvements

This project is an academic prototype rather than a production-ready monitoring system.

Potential improvements include:

- More comprehensive socket error handling and reconnection logic.
- Graceful shutdown and cleanup of worker threads.
- Improved sensor calibration and orientation accuracy testing.
- Automated testing of sensor-processing and networking components.
- Configurable connection settings and logging.

## Project Attribution

Developed collaboratively during the Hardware/Software Co-Design course at Nazarbayev University.

The implementation was developed and tested by a student team, with AI-assisted coding used during development. The project involved collaborative integration, debugging, and hardware testing.

The Madgwick sensor fusion algorithm is based on the work of Sebastian Madgwick.

This repository documents the academic prototype and its implementation.
