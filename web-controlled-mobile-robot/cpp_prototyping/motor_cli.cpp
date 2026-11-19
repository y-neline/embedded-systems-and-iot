#include <pigpio.h>
#include <iostream>
#include <algorithm>
#include <string>

class MotorL298N {
public:
    MotorL298N(int enPin, int pin1, int pin2)
        : en(enPin), in1(pin1), in2(pin2) {}

    void init() {
        gpioSetMode(en, PI_OUTPUT);
        gpioSetMode(in1, PI_OUTPUT);
        gpioSetMode(in2, PI_OUTPUT);

        gpioWrite(in1, 0);
        gpioWrite(in2, 0);

        gpioSetPWMfrequency(en, 1000);
        gpioSetPWMrange(en, 255);
        gpioPWM(en, 0);
    }

    void forward(int speedPercent) {
        gpioWrite(in1, 1);
        gpioWrite(in2, 0);
        gpioPWM(en, percentToPwm(speedPercent));
    }

    void reverse(int speedPercent) {
        gpioWrite(in1, 0);
        gpioWrite(in2, 1);
        gpioPWM(en, percentToPwm(speedPercent));
    }

    void stop() {
        gpioPWM(en, 0);
        gpioWrite(in1, 0);
        gpioWrite(in2, 0);
    }

private:
    int en, in1, in2;

    int percentToPwm(int speedPercent) {
        speedPercent = std::clamp(speedPercent, 0, 100);
        return speedPercent * 255 / 100;
    }
};

void robotForward(MotorL298N& motorA, MotorL298N& motorB, int speed) {
    motorA.forward(speed);
    motorB.forward(speed);
}

void robotReverse(MotorL298N& motorA, MotorL298N& motorB, int speed) {
    motorA.reverse(speed);
    motorB.reverse(speed);
}

void robotLeft(MotorL298N& motorA, MotorL298N& motorB, int speed) {
    motorA.reverse(speed);
    motorB.forward(speed);
}

void robotRight(MotorL298N& motorA, MotorL298N& motorB, int speed) {
    motorA.forward(speed);
    motorB.reverse(speed);
}

void robotStop(MotorL298N& motorA, MotorL298N& motorB) {
    motorA.stop();
    motorB.stop();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./motor_cli <forward|reverse|left|right|stop>\n";
        return 1;
    }

    std::string cmd = argv[1];

    if (gpioInitialise() < 0) {
        std::cerr << "pigpio init failed!\n";
        return 1;
    }

    MotorL298N motorA(12, 23, 24); // ENA, IN1, IN2
    MotorL298N motorB(13, 27, 22); // ENB, IN3, IN4

    motorA.init();
    motorB.init();

    int speed = 100;

    if (cmd == "forward") {
        robotForward(motorA, motorB, speed);
    } else if (cmd == "reverse") {
        robotReverse(motorA, motorB, speed);
    } else if (cmd == "left") {
        robotLeft(motorA, motorB, speed);
    } else if (cmd == "right") {
        robotRight(motorA, motorB, speed);
    } else if (cmd == "stop") {
        robotStop(motorA, motorB);
    } else {
        std::cerr << "Unknown command: " << cmd << "\n";
        gpioTerminate();
        return 1;
    }

    gpioTerminate();
    return 0;
}