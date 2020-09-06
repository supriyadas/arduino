
#include <Stepper.h>
#include <EEPROM.h>

#define STEPS 2038 // the number of steps in one revolution of your motor (28BYJ-48)

Stepper stepper1(STEPS, 8, 10, 9, 11);
Stepper stepper2(STEPS, 4, 6, 5, 7);
int steps = 200;
int start = 1;
int perStape = 10;
boolean setupMode = false;
int setupMotor = 0;
int direc = 0;
int addr1 = 0;
int addr2 = 1;
boolean firstLoop = true;
String readString;

void setup() {
  stepper1.setSpeed(10);
  stepper2.setSpeed(10);
  Serial.begin(9600);
  if(EEPROM.read(addr1)!=0){
    start = EEPROM.read(addr1);
  }
  if(EEPROM.read(addr2)!=0){
    direc = EEPROM.read(addr2);
  }
}

void loop() {
  while(Serial.available()){ 
    delay(3);
    char c = Serial.read();
    readString += c;
  }
  if(readString.length() > 0){
    
    //Operation type
    if(readString == "adj"){
      setupMode = true;
    }else if(readString == "rest"){
      setupMode = true;
    }else if(readString == "play"){
      setupMode = false;
    }

    //Selected Motor
    if(readString == "m1"){
      setupMotor = 1;
    }else if(readString == "m2"){
      setupMotor = 2;
    }else if(readString == "m3"){
      setupMotor = 3;
    }else if(readString == "m4"){
      setupMotor = 4;
    }
    Serial.println(readString);
  }
  if(!setupMode){
    if(!firstLoop){
      direc = 0;
      start = 1;
        
    }
    firstLoop = false;
    for(int i=start;i<=steps*2;i++){
      
      if(direc==0){
        stepper1.step(perStape);
        stepper2.step(-perStape);
      }else{
        stepper1.step(-perStape);
        stepper2.step(perStape);
      }
  
      if(i==steps){
        direc = 1;
      }
      EEPROM.update(addr1, i);
      EEPROM.update(addr2, direc);
    }
  }

  readString="";
}
