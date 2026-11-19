#include <Servo.h>

Servo myservo_0;
Servo myservo_1;
Servo myservo_2;
Servo myservo_3;

const int THRESHOLD = 400;
const int TAP_ANGLE = 35;
const int REST_ANGLE = 0;

void setup() {
  myservo_0.attach(11);
  myservo_1.attach(10);
  myservo_2.attach(9);
  myservo_3.attach(8);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  // Initialize all servos to resting position
  myservo_0.write(REST_ANGLE);
  myservo_1.write(REST_ANGLE);
  myservo_2.write(REST_ANGLE);
  myservo_3.write(REST_ANGLE);

  Serial.begin(9600);
}

void loop() {
  // Read sensor values (A1 and A0 are swapped to match physical stylus alignment)
  int light_0 = analogRead(A1);
  int light_1 = analogRead(A0);
  int light_2 = analogRead(A2);
  int light_3 = analogRead(A3);

  // SERVO 0
  if (light_0 > THRESHOLD) {
    myservo_0.write(TAP_ANGLE);
    delay(7);
    myservo_0.write(REST_ANGLE);
    delay(13);
  } else {
    myservo_0.write(REST_ANGLE);
  }

  // SERVO 1
  if (light_1 > THRESHOLD) {
    myservo_1.write(TAP_ANGLE);
    delay(7);
    myservo_1.write(REST_ANGLE);
    delay(13);
  } else {
    myservo_1.write(REST_ANGLE);
  }

  // SERVO 2
  if (light_2 > THRESHOLD) {
    myservo_2.write(TAP_ANGLE);
    delay(7);
    myservo_2.write(REST_ANGLE);
    delay(13);
  } else {
    myservo_2.write(REST_ANGLE);
  }

  // SERVO 3
  if (light_3 > THRESHOLD) {
    myservo_3.write(TAP_ANGLE);
    delay(7);
    myservo_3.write(REST_ANGLE);
    delay(13);
  } else {
    myservo_3.write(REST_ANGLE);
  }
}