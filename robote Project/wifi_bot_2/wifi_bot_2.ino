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

  /* Step 1: Clearing EEPROM data if RESET is HIGH */
  clearEPROM();

  /* Step 2: Reading EEPROM ssid and password */
  String esid = readSID();
  String epass = readPASS();
  
  /* Step 3: Verifying ssid exist or not */
  boolean ssidPresent = verifyCredentials(esid, epass);

  /* Step 4: Connecting to WiFi with saved credentials if preset else connect with smart config*/
  if(ssidPresent){
    connectWiFiWithCredentials(esid, epass);
  }else{
    connectWiFiWithSmartConfig();
  }

  /* Step 5: Starting WEB-server  */
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
//    digitalWrite(IN_1, LOW);
//    digitalWrite(IN_2, LOW);
//    digitalWrite(IN_3, LOW);
//    digitalWrite(IN_4, LOW);
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
//  if (digitalRead(wifiResetBtn) == HIGH) {
//     Serial.println("Clearing EPROM data....");
//     for (int i = 0 ; i < EEPROM.length() ; i++) {
//        EEPROM.write(i, 0);
//     }
//     EEPROM.commit();
//     Serial.println("EPROM Cleared successfully.");
//  }
} 


/* WiFi connection realted functions 
==============START============= */

String readSID(){
  Serial.println("Reading EEPROM ssid...");
  String sid = "";
  for (int i = 0; i < EEPROM.read(99); ++i){
    sid += char(EEPROM.read(i));
  }
  if(sid.length()>0){
    Serial.print("SSID: ");
    Serial.println(sid);
  }else{
    Serial.println("No SSID found!");
  }
  return sid;
}

String readPASS(){
  Serial.println("Reading EEPROM pass...");
  String pass = "";
  for (int i = 32; i <  32+EEPROM.read(100); ++i) {
    pass += char(EEPROM.read(i));
  }
  
  if(pass.length()>0){
    Serial.print("Password: ");
    Serial.println(pass);
  }else{
    Serial.println("No Password found!");
  }
  
  return pass;
}

boolean verifyCredentials(String sid, String pass){

  boolean credentialsPresent = false;
  
  if(sid.length()>0) {
    credentialsPresent = true;
    Serial.println("Credentials Found");
  }else{
    Serial.println("Credentials Not Found!");
  }
  return credentialsPresent;
}

void connectWiFiWithCredentials(String esid, String epass){
  
    Serial.println("Connecting with WiFi with saved credentials");
    WiFi.begin(esid, epass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi connection Successful");
    Serial.println("IP Address: ");
    Serial.print(WiFi.localIP());
}

void connectWiFiWithSmartConfig(){

    /* Set ESP8266 to WiFi Station mode */
    WiFi.mode(WIFI_STA);
    /* start SmartConfig */
    WiFi.beginSmartConfig();
  
    /* Wait for SmartConfig packet from mobile */
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("SmartConfig done.");
  
    /* Wait for WiFi to connect to AP */
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi connection Successful");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    /* Fetching current wifi credentials */
    
    String qsid = WiFi.SSID().c_str();
    String qpass = WiFi.psk().c_str();

    Serial.print( "Reading current config...");
    Serial.printf( "SSID = %s\n"   , WiFi.SSID().c_str());
    Serial.printf( "PSK  = %s\n\n" , WiFi.psk().c_str());

    saveConfigToEPROM(qsid, qpass);
}

void saveConfigToEPROM(String qsid, String qpass){
  
   Serial.println("Writing eeprom ssid:");
    for (int i = 0; i < qsid.length(); ++i){
      EEPROM.write(i, qsid[i]);
    }
    EEPROM.write(99, qsid.length());
    
    Serial.println("Writing eeprom pass:");
    for (int i = 0; i < qpass.length(); ++i)
    {
      EEPROM.write(32 + i, qpass[i]);
    }
    EEPROM.write(100, qpass.length());
    
    EEPROM.commit();
}

/* WiFi connection realted functions 
===============END============== */
