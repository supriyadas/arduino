#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const int IN_1 = D5;
const int IN_2 = D6;
const int IN_3 = D7;
const int IN_4 = D8;
const int BI_LED_PIN = D4;

ESP8266WebServer server(80);
String command; 
String lastCommand;     

void setup() {
	Serial.begin(115200); 

	pinMode(IN_1, OUTPUT);
	pinMode(IN_2, OUTPUT);
	pinMode(IN_3, OUTPUT);
	pinMode(IN_4, OUTPUT); 
  pinMode(BI_LED_PIN, OUTPUT); 
  digitalWrite(BI_LED_PIN, LOW);
	/* Step 1: Configuring ssid and password */
	String ssid = "Supriya_Home";
	String pass = "apazyaea";
  
	/* Step 2: Creating soft access point */
	Serial.println("Connecting to ");
	Serial.println(ssid); 
	WiFi.begin(ssid, pass); 
	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");

  //Printing IP address
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

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
    digitalWrite(BI_LED_PIN, HIGH);
}

void goBack(){ 
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
    digitalWrite(BI_LED_PIN, HIGH);
}

void goRight(){ 
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
    digitalWrite(BI_LED_PIN, HIGH);
}

void goLeft(){ 
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
    digitalWrite(BI_LED_PIN, HIGH);
}

void stopRobot(){ 
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, LOW);
    digitalWrite(BI_LED_PIN, LOW);
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
