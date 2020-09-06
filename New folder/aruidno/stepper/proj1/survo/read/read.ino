#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
  Serial.println(myservo.read());
  delay(100);      
}

