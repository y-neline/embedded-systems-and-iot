#include <Servo.h>

Servo myservo_0;
Servo myservo_1;
Servo myservo_2;
Servo myservo_3;

void setup() {
  myservo_0.attach(11);
  myservo_1.attach(10);
  myservo_2.attach(9);
  myservo_3.attach(8);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  Serial.begin(9600);
}

void loop() {
  int light_0 = analogRead(A1);
  int light_1 = analogRead(A0);
  int light_2 = analogRead(A2);
  int light_3 = analogRead(A3);

  // Print sensor values to Serial Monitor for threshold tuning
  Serial.print("L0: "); Serial.print(light_0);
  Serial.print(" | L1: "); Serial.print(light_1);
  Serial.print(" | L2: "); Serial.print(light_2);
  Serial.print(" | L3: "); Serial.println(light_3);

  if (light_0 > 400) {
    myservo_0.write(35);
    delay(7);
    myservo_0.write(0);
    delay(13);
  } else {
    myservo_0.write(0);
  }

  if (light_1 > 400) {
    myservo_1.write(35);
    delay(7);
    myservo_1.write(0);
    delay(13);
  } else {
    myservo_1.write(0);
  }

  if (light_2 > 500) {
    myservo_2.write(35);
    delay(7);
    myservo_2.write(0);
    delay(13);
  } else {
    myservo_2.write(0);
  }

  if (light_3 > 400) {
    myservo_3.write(35);
    delay(7);
    myservo_3.write(0);
    delay(13);
  } else {
    myservo_3.write(0);
  }
}