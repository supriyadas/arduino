#define BLYNK_PRINT Serial

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "eBlKOBl2XkLf98bsQMKGrQ9utwmJYqyW";
//char ssid[] = "Supriya_Home";
//char pass[] = "apazyaea";
char ssid[] = "iPhone";
char pass[] = "whatalovelyday";

SimpleTimer timer;

const int M1_IN_1 = D3;
const int M1_IN_2 = D4;
const int M2_IN_1 = D7;
const int M2_IN_2 = D8;

boolean front_clear = true;
boolean rear_clear  = true;
boolean autoDetecction  = true;

int firstVal, secondVal, thirdVal;

WidgetLED ledFront(V8);
WidgetLED ledRear(V9);

void setup() {
  initSetup();
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  ledFront.off();
  ledRear.off();
  timer.setInterval(1000,obstacleDetection); 
  timer.setInterval(1000,checkBlynkConnection);
}

void loop() {
  Blynk.run();
  timer.run();
}


BLYNK_WRITE(V10)
{
   firstVal = param.asInt();
   driveCar(); 
}

BLYNK_WRITE(V11)
{
   secondVal = param.asInt(); 
   driveCar();
}

BLYNK_WRITE(V12)
{
   thirdVal = param.asInt();
}
BLYNK_WRITE(V6)
{
   autoDetecction = param.asInt() == 0 ? false : true;
}
void driveCar() {
  if((firstVal == 2) && (secondVal == 2)) // for joystic normal position
  {
      stop_motors();  
  } 
  else if ( (firstVal == 2) && (secondVal > 3) ) // Forward direction
  {
      forward();
  }
  else if ( (firstVal == 2) && (secondVal < 1) ) // reverse direction
  {
      backward(); 
  }
  else if ( (firstVal > 3) && (secondVal == 2) ) // Right
  {
      turn_right(); 
  }
  else if ( (firstVal < 1) && (secondVal == 2) ) // left
  {
     turn_left();
  }
}

void initSetup() {
  pinMode(M1_IN_1, OUTPUT);
  pinMode(M1_IN_2, OUTPUT);
  pinMode(M2_IN_1, OUTPUT);
  pinMode(M2_IN_2, OUTPUT);
  stop_motors();

  Wire.begin(D1, D2);
}

void checkBlynkConnection(){
  if (!Blynk.connected()) {
    firstVal = 2;
    secondVal = 2;
    driveCar();
    Serial.println("Lost connection");
  }
}

void obstacleDetection(){
  if(autoDetecction){
    Wire.requestFrom(8, 2);
    int i = 0;
    while(Wire.available()){
      char c = Wire.read();
      if(i==0)  front_clear = c == '1' ? true : false;
      if(i==1)  rear_clear  = c == '1' ? true : false;
      i++;
    }
    //Serial.print(front_clear);
    //Serial.print(rear_clear);
    //Serial.println();
    if(front_clear){
      ledFront.off(); 
    }else{
      ledFront.on();
    }
    if(rear_clear){ 
      ledRear.off();
    }else{
      ledRear.on();
    }
    driveCar();
  }
}

void forward() {
  if(front_clear){
    digitalWrite(M1_IN_1, LOW);
    digitalWrite(M1_IN_2, HIGH);
    digitalWrite(M2_IN_1, LOW);
    digitalWrite(M2_IN_2, HIGH);
  }else{
    stop_motors();
  }
}

void backward() {
  if(rear_clear){
    digitalWrite(M1_IN_1, HIGH);
    digitalWrite(M1_IN_2, LOW);
    digitalWrite(M2_IN_1, HIGH);
    digitalWrite(M2_IN_2, LOW);
  }else{
    stop_motors();
  }
}

void turn_right() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M2_IN_1, HIGH);
  digitalWrite(M2_IN_2, LOW);
}

void turn_left() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
}

void stop_motors() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
}
