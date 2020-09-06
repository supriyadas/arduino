/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "sFNM3CARqImQ-91cprti9p0o5C2qsEHz";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Realme 3";
char pass[] = "123456789";

int current = 0;
int previous = 1;

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(D2, OUTPUT);
  Blynk.begin(auth, ssid, pass);
   
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}
int buttonState;
BLYNK_WRITE(V5)
{
  buttonState = param.asInt();
}


void switchstate(){
  current = buttonState;
  if (current != previous){
    previous = current;
    if (current == 0)
          {
              digitalWrite(D2, LOW);
          }
          else
         {
              digitalWrite(D2, HIGH);
         }          
  }
}

void loop()
{
  Blynk.run();
 switchstate();
//  if (buttonState){
//    Serial.println("on");
//  }else{
//     Serial.println("off");
//  }

}


