#include <AccelStepper.h>
#define HALFSTEP 8  //Half-step mode (8 step control signal sequence)

// Motor pin definitions
#define mtrPin1  2     // IN1 on the ULN2003 driver 1
#define mtrPin2  3     // IN2 on the ULN2003 driver 1
#define mtrPin3  4     // IN3 on the ULN2003 driver 1
#define mtrPin4  5     // IN4 on the ULN2003 driver 1

#define mtr2Pin1  8      // IN1 on the ULN2003 driver2 1
#define mtr2Pin2  9      // IN2 on the ULN2003 driver2 1
#define mtr2Pin3  10     // IN3 on the ULN2003 driver2 1
#define mtr2Pin4  11     // IN4 on the ULN2003 driver2 1

int n=5,i=2,j=2;

AccelStepper stepper1(HALFSTEP, mtrPin1, mtrPin3, mtrPin2, mtrPin4);
AccelStepper stepper2(HALFSTEP, mtr2Pin1, mtr2Pin3, mtr2Pin2, mtr2Pin4);


void setup() {
  Serial.begin(9600);
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);  //Make the acc quick
  stepper1.setSpeed(1000);
  stepper1.moveTo(4096*5); 

  stepper2.setMaxSpeed(3000.0);
  stepper2.setAcceleration(500.0);  //Make the acc quick
  stepper2.setSpeed(500);
  stepper2.moveTo(4096); 
}

void loop() {

//Change direction when the stepper reaches the target position
// if (stepper1.distanceToGo() == 0) {
//   //stepper1.moveTo(stepper1.currentPosition());  //Reverse motor
//   if(i<=n){
//    stepper1.moveTo(4096*i);
//    stepper2.moveTo(4096*i);
//    i++;
//   }
//}
// if (stepper2.distanceToGo() == 0) {
//   //stepper1.moveTo(stepper1.currentPosition());  //Reverse motor
//   if(j<=n){
//    stepper2.moveTo(4096*j);
//    j++;
//   }
//}
 stepper1.run(); 
 //stepper2.run();
  //Start stepper3.run();

  //Serial.println(stepper1.distanceToGo());
}
