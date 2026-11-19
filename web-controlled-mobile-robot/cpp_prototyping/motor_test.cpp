#include <pigpio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

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

void robotStop(MotorL298N& motorA, MotorL298N& motorB) {
    motorA.stop();
    motorB.stop();
}

void robotLeft(MotorL298N& motorA, MotorL298N& motorB, int speed) {
    motorA.reverse(speed);
    motorB.forward(speed);
}

void robotRight(MotorL298N& motorA, MotorL298N& motorB, int speed) {
    motorA.forward(speed);
    motorB.reverse(speed);
}

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio init failed!" << std::endl;
        return 1;
    }

    MotorL298N motorA(12, 23, 24); // ENA, IN1, IN2
    MotorL298N motorB(13, 27, 22); // ENB, IN3, IN4

    motorA.init();
    motorB.init();

    std::cout << "Robot forward at 70%" << std::endl;
    robotForward(motorA, motorB, 70);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Robot forward at 100%" << std::endl;
    robotForward(motorA, motorB, 100);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Stop" << std::endl;
    robotStop(motorA, motorB);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Robot reverse at 70%" << std::endl;
    robotReverse(motorA, motorB, 70);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Stop" << std::endl;
    robotStop(motorA, motorB);




/// LEFT and RIGHT
    std::cout << "Turn left at 80%" << std::endl;
    robotLeft(motorA, motorB, 80);
    std::this_thread::sleep_for(std::chrono::seconds(4));

    std::cout << "Stop" << std::endl;
    robotStop(motorA, motorB);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Turn right at 80%" << std::endl;
    robotRight(motorA, motorB, 80);
    std::this_thread::sleep_for(std::chrono::seconds(4));

    std::cout << "Stop" << std::endl;
    robotStop(motorA, motorB);
    std::this_thread::sleep_for(std::chrono::seconds(1));


    gpioTerminate();
    return 0;
}
