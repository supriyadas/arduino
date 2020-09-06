#include <AccelStepper.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <SPI.h>

#define HALFSTEP 8  //Half-step mode (8 step control signal sequence)

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 2);

EthernetServer server(80);
String readString; 
int value1=0, value2=0;

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
  
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);  //Make the acc quick
  stepper1.setSpeed(1000);

  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(500);  //Make the acc quick
  stepper2.setSpeed(1000);
  

}

void loop() {

  EthernetClient client = server.available();
  if (client) {
   // Serial.println("new client");

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
          if (readString.length() < 100) {
            readString += c; 
          }

        if (c == '\n') {
          
          client.println("Get String");

          value1= 100*(readString[6] - '0')+10*(readString[7] - '0')+(readString[8] - '0');
          value2= 100*(readString[9] - '0')+10*(readString[10] - '0')+(readString[11] - '0');
          
          client.println(readString);


            if((value1>=0&&value1<=256)&&(value2>=0&&value2<=256)){

              client.print("Moving motor1: ");
              client.print(stepper1.currentPosition ()/256);
              client.print(" --> ");
              client.println(value1);
    
              client.print("Moving motor2: ");
              client.print(stepper2.currentPosition ()/256);
              client.print(" --> ");
              client.println(value2);

          
              Serial.println("-----Value Form PC START-------");
              Serial.println(value1);
              Serial.println(value2);
              Serial.println("-----Value Form PC END-------");
              haveData = true;
            }
           
           readString="";
          client.stop();
        }
      }
    }
  }
  

  if (haveData &&  (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0)) {

    Serial.println("Entering forword mode:");
    
    Serial.println(stepper1.currentPosition ()/256);
    Serial.println(stepper2.currentPosition ()/256);
    Serial.println(value1);
    Serial.println(value2);
    
    stepper1.moveTo(256*value1); 
    stepper2.moveTo(256*value2); 

    haveData = false;
    
  }

 stepper1.run(); 
 stepper2.run();

}
