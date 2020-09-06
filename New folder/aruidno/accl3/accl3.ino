#include <AccelStepper.h>
#include <EEPROM.h>

#define HALFSTEP 8  //Half-step mode (8 step control signal sequence)

// Motor pin definitions
#define mtr1Pin1  2     // IN1 on the ULN2003 driver 1
#define mtr1Pin2  3     // IN2 on the ULN2003 driver 1
#define mtr1Pin3  4     // IN3 on the ULN2003 driver 1
#define mtr1Pin4  5     // IN4 on the ULN2003 driver 1

#define mtr2Pin1  6      // IN1 on the ULN2003 driver2 1
#define mtr2Pin2  7      // IN2 on the ULN2003 driver2 1
#define mtr2Pin3  8      // IN3 on the ULN2003 driver2 1
#define mtr2Pin4  9      // IN4 on the ULN2003 driver2 1

int addr1 = 0;
int addr2 = 1;
boolean reset = false;
boolean haveData = true;

int n=5,p[2];


AccelStepper stepper1(HALFSTEP, mtr1Pin1, mtr1Pin3, mtr1Pin2, mtr1Pin4);
AccelStepper stepper2(HALFSTEP, mtr2Pin1, mtr2Pin3, mtr2Pin2, mtr2Pin4);



void setup() {
  Serial.begin(9600);

  p[0] = 30;
  p[1] = 30;
  EEPROM.write(addr1, p[0]);
  EEPROM.write(addr2, p[1]);
 
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);  //Make the acc quick
  stepper1.setSpeed(1000);
  stepper1.moveTo(256*p[0]); 

  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(500);  //Make the acc quick
  stepper2.setSpeed(1000);
  

}

void loop() {

  if(!reset && ((int)EEPROM.read(addr1)>0 || (int)EEPROM.read(addr2)>0)){

    Serial.println("Entering reverse mode:");
    Serial.println((int)EEPROM.read(addr1));
    Serial.println((int)EEPROM.read(addr2));

    stepper1.moveTo(-256*(int)EEPROM.read(addr1)); 
    stepper2.moveTo(-256*(int)EEPROM.read(addr2)); 
    reset = true;
  }
  
  if (haveData &&  (stepper1.distanceToGo() == 0 && stepper1.distanceToGo() == 0)) {

    Serial.println("Entering forword mode:");
    Serial.println(p[0]);
    Serial.println(p[1]);
    
    stepper1.moveTo(256*p[0]); 
    stepper2.moveTo(256*p[1]); 
    EEPROM.write(addr1, p[0]);
    EEPROM.write(addr2, p[1]);

    haveData = false;
    
  }

 stepper1.run(); 
 stepper2.run();
}
