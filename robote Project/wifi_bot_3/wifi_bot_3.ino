#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const int IN_1 = D5;
const int IN_2 = D6;
const int IN_3 = D7;
const int IN_4 = D8;

ESP8266WebServer server(80);
String command; 
String lastCommand;     

void setup() {
  Serial.begin(115200); 

  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT); 

  /* Step 1: Configuring ssid and password */
  String esid = "robo_rob1";
  String epass = "123456789";
  
  /* Step 2: Creating soft access point */
  boolean ssidPresent = setupWifiAP(esid, epass);

  /* Step 3: Starting WEB-server  */
   server.on ( "/move", HTTP_handleRoot );
   server.onNotFound ( HTTP_handleRoot );
   server.begin();    
}

void loop() {

  server.handleClient();
  command = server.arg("dir");

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
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, LOW);
}

void HTTP_handleRoot(void) {

  if( server.hasArg("dir") ){
         Serial.println(server.arg("dir"));
    }
    server.send ( 200, "text/html", "" );
    delay(1);
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
