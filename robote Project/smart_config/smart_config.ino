#include <ESP8266WiFi.h>
#include <EEPROM.h>

const int wifiResetBtn = D1;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512); 
  pinMode(wifiResetBtn, INPUT);

  /* Clearing EEPROM data */
  
  if (digitalRead(wifiResetBtn) == HIGH) {
     Serial.println("Clearing EPROM data....");
     for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
     }
     EEPROM.commit();
     Serial.println("EPROM Cleared successfully.");
  }

  /* Reading EEPROM ssid */
  
  Serial.println("Reading EEPROM ssid");
  String esid = "";
  for (int i = 0; i < EEPROM.read(99); ++i){
    esid += char(EEPROM.read(i));
  }

  /* Reading EEPROM password */
  Serial.println("Reading EEPROM pass");
  String epass = "";
  for (int i = 32; i <  32+EEPROM.read(100); ++i) {
    epass += char(EEPROM.read(i));
  }
  
  /* Verifying ssid exist or not */
  boolean credentialsPresent = false;
  if(esid.length()>0)
  {
    credentialsPresent = true;
    Serial.println("Credentials Found");
    Serial.println();
    Serial.print("SSID: ");
    Serial.println(esid);
    Serial.print("PASS: ");
    Serial.println(epass);
  }

  if(credentialsPresent){

    /* Connecting to wifi with saved credentials */
    
    Serial.println("Connecting with WiFi with saved credentials");
    WiFi.begin(esid, epass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi connection Successful");
    Serial.println("IP Address: ");
    Serial.print(WiFi.localIP());
    
  }else{
  
    /* Connecting to wifi with smart config */
    
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
  
    /* Fetching current wifi credentials */
    
    String qsid = WiFi.SSID().c_str();
    String qpass = WiFi.psk().c_str();
    
    Serial.print( "\n\tREADING NVR\n");
    Serial.printf( "\tSSID = %s\n"   , WiFi.SSID().c_str());
    Serial.printf( "\tPSK  = %s\n\n" , WiFi.psk().c_str());
    
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
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
  }
  
}
void loop() {
}
