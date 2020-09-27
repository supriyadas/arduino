#include <AccelStepper.h>

#define HALFSTEP 4  //Half-step mode (8 step control signal sequence)

// Motor pin definitions
#define mtr1Pin1  2     // IN1 on the ULN2003 driver 1
#define mtr1Pin2  3     // IN2 on the ULN2003 driver 1
#define mtr1Pin3  4     // IN3 on the ULN2003 driver 1
#define mtr1Pin4  5     // IN4 on the ULN2003 driver 1

#define mtr2Pin1  6      // IN1 on the ULN2003 driver2 1
#define mtr2Pin2  7      // IN2 on the ULN2003 driver2 1
#define mtr2Pin3  8      // IN3 on the ULN2003 driver2 1
#define mtr2Pin4  9      // IN4 on the ULN2003 driver2 1


AccelStepper stepper1(HALFSTEP, mtr1Pin1, mtr1Pin3, mtr1Pin2, mtr1Pin4);
AccelStepper stepper2(HALFSTEP, mtr2Pin1, mtr2Pin3, mtr2Pin2, mtr2Pin4);



void setup() {
  Serial.begin(9600);

  stepper1.setMaxSpeed(500);
  stepper1.setAcceleration(500);  //Make the acc quick
  stepper1.setSpeed(1000);

  stepper2.setMaxSpeed(500);
  stepper2.setAcceleration(500);  //Make the acc quick
  stepper2.setSpeed(1000);
}

void loop() {

 stepper1.moveTo(5000); 
 stepper2.moveTo(-5000); 

 stepper1.run(); 
 stepper2.run();

if(stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0) {
  delay(2000);;
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
 }

}

