#include <QMC5883L.h>
#include <Wire.h>

const int M1_IN_1 = D5;
const int M1_IN_2 = D6;
const int M2_IN_1 = D7;
const int M2_IN_2 = D8;

int motorSpeed = 100;

QMC5883L compass;
int baseHeading = 0;
boolean baseHeadingSet = false;
boolean tiltLeft  = false;
boolean tiltRight = false;

void setup()
{
	Wire.begin(D6, D5);

	compass.init();
	compass.setSamplingRate(10);

	Serial.begin(9600);
	Serial.println("QMC5883L Compass Demo");
	Serial.println("Turn compass in all directions to calibrate....");
}

void loop()
{
	int heading = compass.readHeading()/30;
  
	if(heading==0) {
		/* Still calibrating, so measure but don't print */
   Serial.println("calibrating");
	} else {
    
    if(!baseHeadingSet){
      baseHeading = heading;
      baseHeadingSet = true;
    }

    tiltLeft  = heading<baseHeading ?  true : false;
    tiltRight = heading>baseHeading ?  true : false;

    //driveCar();
    //Serial.print(compass.readHeading());
   // Serial.print("  ");
		Serial.println(heading);
    Serial.print("L:");
    Serial.print(tiltLeft);
    Serial.print(" R:");
    Serial.println(tiltRight);
	}
}

/*
|===============================================================|
|------------------MOTOR control methods----START---------------|
|===============================================================|
*/

void driveCar() {
  if(!tiltLeft && !tiltRight) forward();  
  else if (tiltLeft && !tiltRight) turn_front_right();
  else if (!tiltLeft && tiltRight) turn_front_left();
}

void forward() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
}

void backward() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, HIGH);
  digitalWrite(M2_IN_2, LOW);
}

void turn_right() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M2_IN_1, HIGH);
  digitalWrite(M2_IN_2, LOW);
}

void turn_front_right() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
}

void turn_back_right() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
}

void turn_left() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
}

void turn_front_left() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
}

void turn_back_left() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, HIGH);
  digitalWrite(M2_IN_2, LOW);
}

void stop_motors() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
}

/*
|===============================================================|
|------------------MOTOR control methods----END-----------------|
|===============================================================|
*/
