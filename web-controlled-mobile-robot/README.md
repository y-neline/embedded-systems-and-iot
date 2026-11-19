# Web-Controlled Mobile Robot

A mobile robot powered by a Raspberry Pi and controlled remotely via a browser-based interface with live USB camera streaming.

## Features
- **Motor Control:** Dual DC motor actuation via an L298N driver using hardware PWM for speed and directional control.
- **Web Interface:** Responsive HTML/CSS/JS control pad sending asynchronous `fetch()` commands to a Python Flask backend.
- **Live Video Stream:** Real-time MJPEG streaming from a USB webcam (`/dev/video0`) using OpenCV.
- **C++ Prototyping:** Initial GPIO/PWM motor driver classes and CLI controller built in C++ with `pigpio`.

## Hardware Pinout (BCM)
| L298N Pin | Raspberry Pi GPIO | Function |
| :--- | :--- | :--- |
| ENA | GPIO 12 | Motor A PWM Speed |
| IN1 / IN2 | GPIO 23 / 24 | Motor A Direction |
| ENB | GPIO 13 | Motor B PWM Speed |
| IN3 / IN4 | GPIO 27 / 22 | Motor B Direction |

## How to Run
1. Start the `pigpio` daemon:
   ```bash
   sudo pigpiod
   ```
2. Install Python dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Run the Flask server:
   ```bash
   python3 app.py
   ```
4. Open `http://<raspberry-pi-ip>:18080/` in any browser on the same network.
