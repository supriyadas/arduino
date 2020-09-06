#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const int wifiResetBtn = D1;
const int IN_1 = D5;
const int IN_2 = D6;
const int IN_3 = D0;
const int IN_4 = D1;

ESP8266WebServer server(80);
String command; 
String lastCommand;     

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512); 
  pinMode(wifiResetBtn, INPUT);

  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
   
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, HIGH);

  /* Step 1: Configuring ssid and password */
  String esid = "robo_rob1";
  String epass = "123456789";
  
  /* Step 2: Creating soft access point */
  boolean ssidPresent = setupWifiAP(esid, epass);

  /* Step 3: Starting WEB-server  */
   server.on ( "/", HTTP_handleRoot );
   server.onNotFound ( HTTP_handleRoot );
   server.begin();    
}

void loop() {

  server.handleClient();
  command = server.arg("State");

  if (command == "F") goAhead();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "S") stopRobot();
  
}

void goAhead(){ 
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
}

void goBack(){ 
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
}

void goRight(){ 
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
}

void goLeft(){ 
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
}

void stopRobot(){ 
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, HIGH);
}

void HTTP_handleRoot(void) {

  if( server.hasArg("State") ){
         Serial.println(server.arg("State"));
    }
    server.send ( 200, "text/html", "" );
    delay(1);
}

void clearEPROM(){
  if (digitalRead(wifiResetBtn) == HIGH) {
     Serial.println("Clearing EPROM data....");
     for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
     }
     EEPROM.commit();
     Serial.println("EPROM Cleared successfully.");
  }
} 


/* WiFi connection realted functions 
==============START============= */

boolean setupWifiAP(String sid, String pass){

  IPAddress apIP(192, 168, 0, 1);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(sid, pass);
  IPAddress myIP = WiFi.softAPIP();
}
/* WiFi connection realted functions 
===============END============== */
