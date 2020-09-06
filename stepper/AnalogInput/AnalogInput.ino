#include <EEPROM.h>

int m = 1;
int part = 0;

int motor1[2] = {5,4};//done
int motor2[2] = {6,7};
int motor3[2] = {3,2};//done
int motor4[2] = {8,9};

int sensorPin1 = A0;
int sensorPin2 = A1;
int sensorPin3 = A2;
int sensorPin4 = A3; 

int sensor1 = 0;
int sensor2 = 0;
int sensor3 = 0;
int sensor4 = 0;

int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
int l1 = 0, l2 = 0, l3 = 0, l4 = 0;
char d1, d2, d3, d4;

void setup() {
  Serial.begin(9600);
  
  set_motor_pins();
  motor_default ();
  set_default_last_value();
}

void loop() {
  
  sensor1 = analogRead(sensorPin1) > 500 ? 0 : 1;
  sensor2 = analogRead(sensorPin2) > 500 ? 0 : 1;
  sensor3 = analogRead(sensorPin3) > 500 ? 0 : 1;
  sensor4 = analogRead(sensorPin4) > 500 ? 0 : 1;
  
  set_motors('S','S','S','S');

  Serial.println(sensor1);

  if(l1!=sensor1){
    if(l1 == 1 && sensor1 == 0){ c1++; }
    l1=sensor1;
  }

  if(l2!=sensor2){
    if(l2 == 1 && sensor2 == 0){ c2++; }
    l2=sensor2;
  }

  if(l3!=sensor3){
    if(l3 == 1 && sensor3 == 0){ c3++; }
    l3=sensor3;
  }

  if(l4!=sensor4){
    if(l4 == 1 && sensor4 == 0){ c4++; }
    l4=sensor4;
  }
}

void set_motors(char a1, char a2, char a3, char a4){

  move_motor(1, a1);
  move_motor(2, a2);
  move_motor(3, a3);
  move_motor(4, a4);

}

void set_next_part(int b1, int b2, int b3, int b4, int p){

  if(c1==v(b1)) move_motor(1, 'S');
  if(c2==v(b2)) move_motor(2, 'S');
  if(c3==v(b3)) move_motor(3, 'S');
  if(c4==v(b4)) move_motor(4, 'S');

  if(c1==v(b1) && c2==v(b2) && c3==v(b3) && c4==v(b4)){
    part = p;
  }

}

void move_motor(int motor_no, char direc) {
  switch (direc) {
  case 'S':
    switch (motor_no) {
    case 1:
      digitalWrite(motor1[0], LOW);
      digitalWrite(motor1[1], LOW);
      break;
    case 2:
      digitalWrite(motor2[0], LOW);
      digitalWrite(motor2[1], LOW);
      break;
    case 3:
      digitalWrite(motor3[0], LOW);
      digitalWrite(motor3[1], LOW);
      break;
    case 4:
      digitalWrite(motor4[0], LOW);
      digitalWrite(motor4[1], LOW);
      break;
    }
    break;
  case 'U':
    switch (motor_no) {
    case 1:
      digitalWrite(motor1[0], LOW);
      digitalWrite(motor1[1], HIGH);
      break;
    case 2:
      digitalWrite(motor2[0], LOW);
      digitalWrite(motor2[1], HIGH);
      break;
    case 3:
      digitalWrite(motor3[0], LOW);
      digitalWrite(motor3[1], HIGH);
      break;
    case 4:
      digitalWrite(motor4[0], LOW);
      digitalWrite(motor4[1], HIGH);
      break;
    }
    break;
  case 'D':
    switch (motor_no) {
    case 1:
      digitalWrite(motor1[0], HIGH);
      digitalWrite(motor1[1], LOW);
      break;
    case 2:
      digitalWrite(motor2[0], HIGH);
      digitalWrite(motor2[1], LOW);
      break;
    case 3:
      digitalWrite(motor3[0], HIGH);
      digitalWrite(motor3[1], LOW);
      break;
    case 4:
      digitalWrite(motor4[0], HIGH);
      digitalWrite(motor4[1], LOW);
      break;
    }
    break;
  }

}
void set_motor_pins () {
  pinMode(motor1[0], OUTPUT);
  pinMode(motor1[1], OUTPUT);
  pinMode(motor2[0], OUTPUT);
  pinMode(motor2[1], OUTPUT);
  pinMode(motor3[0], OUTPUT);
  pinMode(motor3[1], OUTPUT);
  pinMode(motor4[0], OUTPUT);
  pinMode(motor4[1], OUTPUT);
}

void motor_default () {
   digitalWrite(motor1[0], LOW);
   digitalWrite(motor1[1], LOW);

   digitalWrite(motor2[0], LOW);
   digitalWrite(motor2[1], LOW);

   digitalWrite(motor3[0], LOW);
   digitalWrite(motor3[1], LOW);

   digitalWrite(motor4[0], LOW);
   digitalWrite(motor4[1], LOW);
}

void set_default_last_value () {
  l1 = analogRead(sensorPin1) > 500 ? 0 : 1;
  l2 = analogRead(sensorPin2) > 500 ? 0 : 1;
  l3 = analogRead(sensorPin3) > 500 ? 0 : 1;
  l4 = analogRead(sensorPin4) > 500 ? 0 : 1;
}

int v (int a) {
  return a*m;
}


