#include <Wire.h>

// Ultrasonic Front Pins
const int trigPin1 = 3;
const int echoPin1 = 2;

// Ultrasonic Rear Pins
const int trigPin2 = 5;
const int echoPin2 = 4;

// Motor Speed Pin
const int motorSpeedPin = 9;

// Ultrasolic LED indicator
const int ledPinFront = 13;
const int ledPinRare = 12;


boolean front_clear, rear_clear;

int motorSpeed = 255;

long threshold = 20;
long duration, distance, frontSensor, backSensor;

void setup() {

  Wire.begin(8);               
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
 
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(ledPinFront, OUTPUT);
  pinMode(ledPinRare, OUTPUT);

  front_clear = true;
  rear_clear = true;
  
  pinMode(motorSpeedPin, OUTPUT);
  analogWrite(motorSpeedPin, motorSpeed);
  
  Serial.begin(9600);
}

void loop() {

  /* == Calculating distance using ultrasonic ==*/
  
  SonarSensor(trigPin1, echoPin1);
  frontSensor = distance;
  SonarSensor(trigPin2, echoPin2);
  backSensor = distance;
  updateSensorDataToCode();
  //printUltrasonicData();
  
  /* == Setting up motor speed ==*/
  
  analogWrite(motorSpeedPin, motorSpeed);

}

void SonarSensor(int trigPin,int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
 int i = 0;
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    if(i==0){
     setMotorSpeed(c);
     i++;
    }
    //Serial.print(c);           /* print the character */
  }
  //Serial.println();             /* to newline */
}



void requestEvent() {
  char us_status[2] = {front_clear ? '1':'0', rear_clear ? '1':'0'};
  Wire.write(us_status);
}


void updateSensorDataToCode(){
  
  if(frontSensor!=0 && frontSensor>=threshold){
    front_clear = true;
    digitalWrite(ledPinFront, LOW);
  }
  if(backSensor!=0 && backSensor>=threshold){
    rear_clear = true;
    digitalWrite(ledPinRare, LOW);
  }
    
  if(frontSensor!=0 && frontSensor<threshold){
      front_clear = false;
      digitalWrite(ledPinFront, HIGH);
  }
  if(backSensor!=0 && backSensor<threshold){
      rear_clear = false;
      digitalWrite(ledPinRare, HIGH);
  }
  
}

void setMotorSpeed(char c) {
  if(c=='1') motorSpeed = 60;
  else if(c=='2') motorSpeed = 110;
  else if(c=='3') motorSpeed = 160;
  else if(c=='4') motorSpeed = 210;
  else if(c=='5') motorSpeed = 255;
  Serial.println(motorSpeed);
}

void printUltrasonicData(){
  Serial.println(frontSensor);
  Serial.print(" - ");
  Serial.print(frontSensor);
  Serial.print(" - ");
}

