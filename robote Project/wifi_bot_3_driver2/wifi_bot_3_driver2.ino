#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/*------------ MCU Pin-----Driver Pin */                   
const int IN_1  = D3;         //7
const int IN_2  = D4;         //8
const int PWM_1 = D5;         //5~

const int IN_3  = D6;         //4
const int IN_4  = D7;         //9
const int PWM_2 = D8;         //6~

/*
 * ---------------------------------------------------------
 * Driver pin A0, A1 conneted to +5v                      
 * Driver GND connected to GND of Node MCU
 * Driver +5V, GND connected to 5 volt power supply
 * --------------------------------------------------------
*/

ESP8266WebServer server(80);
String command; 
String lastCommand;     

void setup() {
  Serial.begin(115200);

  //Motor 1
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(PWM_1, OUTPUT);

  //Motor 2
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT); 
  pinMode(PWM_2, OUTPUT); 

  analogWrite(PWM_1, 255); 
  analogWrite(PWM_2, 255); 

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
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, LOW);
}

void HTTP_handleRoot(void) {

  if( server.hasArg("State") ){
         Serial.println(server.arg("State"));
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
