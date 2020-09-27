#include <Wire.h>
#include <NewPing.h>
#include <SendOnlySoftwareSerial.h>

#define SONAR_NUM 2      // Number of sensors.
#define MAX_DISTANCE 20 // Maximum distance (in cm) to ping.

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
const int ledPinRare  = 12;

SendOnlySoftwareSerial mySerial(6);  // Tx pin

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(3, 2, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(5, 4, MAX_DISTANCE)
};

long sensors[SONAR_NUM]; //array to store sensor distances

boolean front_clear = true;
boolean rear_clear  = true;

int motorSpeed = 255;
long threshold = 15;
long duration, distance, frontSensor, backSensor;

void setup() {
  Serial.begin(9600);

  Wire.begin(8);               
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  pinMode(ledPinFront,  OUTPUT);
  pinMode(ledPinRare,   OUTPUT);
  pinMode(motorSpeedPin,OUTPUT);
  
  analogWrite(motorSpeedPin, motorSpeed);
  mySerial.begin(9600);
}

void loop() {

  /* -----------------------------------------------------*
   *  Calculating distance using ultrasonic               *
   *  Wait 50ms between pings (about 20 pings/sec).       *
   *  29ms should be the shortest delay between pings     *
   *  ----------------------------------------------------*
  */
  
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    delay(50);
    sensors[i] = sonar[i].ping_cm();
  }
  frontSensor = sensors[0];
  backSensor  = sensors[1];
  updateSensorDataToCode();
  printUltrasonicData();
  /* == Setting up motor speed ==*/
  
  
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
      sendObsData();
  }
  if(backSensor!=0 && backSensor<threshold){
      rear_clear = false;
      digitalWrite(ledPinRare, HIGH);
      sendObsData();
  }
}

void setMotorSpeed(char c) {
  if(c=='1') motorSpeed = 60;
  else if(c=='2') motorSpeed = 110;
  else if(c=='3') motorSpeed = 160;
  else if(c=='4') motorSpeed = 210;
  else if(c=='5') motorSpeed = 255;
  analogWrite(motorSpeedPin, motorSpeed);
  Serial.println(motorSpeed);
}

void stopMotorInstantly(){
  analogWrite(motorSpeedPin, 0);
}

void releaseMotor(){
  analogWrite(motorSpeedPin, motorSpeed);
}

void sendObsData(){
  char us_status[2] = {front_clear ? '1':'0', rear_clear ? '1':'0'};
  mySerial.print (us_status);
  delay (100);
}

void printUltrasonicData(){
  Serial.print(frontSensor);
  Serial.print(" - ");
  Serial.print(backSensor);
  Serial.println("");
}

