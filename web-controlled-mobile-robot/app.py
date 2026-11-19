from flask import Flask, send_file, Response
import pigpio
import cv2

app = Flask(__name__)

# Pins
ENA, IN1, IN2 = 12, 23, 24
ENB, IN3, IN4 = 13, 27, 22

pi = pigpio.pi()

if not pi.connected:
    raise RuntimeError("Could not connect to pigpio daemon")

camera = cv2.VideoCapture(0, cv2.CAP_V4L2)
camera.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
camera.set(cv2.CAP_PROP_FPS, 20)

if not camera.isOpened():
    raise RuntimeError("Could not open USB camera")

def percent_to_pwm(speed_percent):
    speed_percent = max(0, min(100, speed_percent))
    return speed_percent * 255 // 100

def init_motor(en, pin1, pin2):
    pi.set_mode(en, pigpio.OUTPUT)
    pi.set_mode(pin1, pigpio.OUTPUT)
    pi.set_mode(pin2, pigpio.OUTPUT)

    pi.write(pin1, 0)
    pi.write(pin2, 0)

    pi.set_PWM_frequency(en, 1000)
    pi.set_PWM_range(en, 255)
    pi.set_PWM_dutycycle(en, 0)

def motor_forward(en, pin1, pin2, speed):
    pi.write(pin1, 1)
    pi.write(pin2, 0)
    pi.set_PWM_dutycycle(en, percent_to_pwm(speed))

def motor_reverse(en, pin1, pin2, speed):
    pi.write(pin1, 0)
    pi.write(pin2, 1)
    pi.set_PWM_dutycycle(en, percent_to_pwm(speed))

def motor_stop(en, pin1, pin2):
    pi.set_PWM_dutycycle(en, 0)
    pi.write(pin1, 0)
    pi.write(pin2, 0)

def robot_forward(speed=100):
    motor_forward(ENA, IN1, IN2, speed)
    motor_forward(ENB, IN3, IN4, speed)

def robot_reverse(speed=100):
    motor_reverse(ENA, IN1, IN2, speed)
    motor_reverse(ENB, IN3, IN4, speed)

def robot_left(speed=100):
    motor_reverse(ENA, IN1, IN2, speed)
    motor_forward(ENB, IN3, IN4, speed)

def robot_right(speed=100):
    motor_forward(ENA, IN1, IN2, speed)
    motor_reverse(ENB, IN3, IN4, speed)

def robot_stop():
    motor_stop(ENA, IN1, IN2)
    motor_stop(ENB, IN3, IN4)

def generate_frames():
    while True:
        success, frame = camera.read()
        if not success:
            continue

        ret, buffer = cv2.imencode('.jpg', frame)
        if not ret:
            continue

        frame_bytes = buffer.tobytes()

        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame_bytes + b'\r\n')

init_motor(ENA, IN1, IN2)
init_motor(ENB, IN3, IN4)
robot_stop()

@app.route("/")
def index():
    return send_file("index.html")

@app.route("/video_feed")
def video_feed():
    return Response(generate_frames(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route("/forward")
def forward():
    robot_forward(100)
    return "Forward"

@app.route("/reverse")
def reverse():
    robot_reverse(100)
    return "Reverse"

@app.route("/left")
def left():
    robot_left(100)
    return "Left"

@app.route("/right")
def right():
    robot_right(100)
    return "Right"

@app.route("/stop")
def stop():
    robot_stop()
    return "Stop"

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=18080, threaded=True)