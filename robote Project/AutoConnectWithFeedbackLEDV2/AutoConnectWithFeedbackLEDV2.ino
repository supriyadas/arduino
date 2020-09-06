#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Ticker.h>
#include <EasyButton.h>


const int BI_LED_PIN = D4;

//Long press detcetion veriables 
const int BUTTON_PIN = D3;
const int LONG_PRESS_TIME  = 5000;
int lastState = LOW;
unsigned long pressedTime  = 0;
bool isPressing = false;
bool isLongDetected = false;


int displayMode = 1;
int loadingCount = 1;

Ticker ticker,ticker1;
EasyButton button(BUTTON_PIN);
WiFiManager wifiManager;
LiquidCrystal_I2C lcd(0x23, 16, 2);


//gets called when WiFiManager enters configuration mode


void setup() {
  
  Serial.begin(115200);

  //set led pin as output
  pinMode(BI_LED_PIN, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);
 
  
  /* ===== Initialize System ======= Start ===== */
  
  initLCD();
  displayTitle();
  
  /* ===== Initialize System ======= End ======= */
  
  /* =====WI-FI connection setup ==== Start ===== */
  
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("RobRobSetup");
  
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {

    displayFailedToConnect();
    delay(2000);
    Serial.println("failed to connect and hit timeout");
    ESP.restart();
    delay(1000);
    
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  ticker1.detach();
  digitalWrite(BI_LED_PIN, LOW);
  displayConnectedToWiFi();
  
  /* =====WI-FI connection setup ==== End ====== */

  button.begin();
  button.onPressed(onBtnPressed);
}

void loop() {
  button.read();
  longPressDetector();
}

void onBtnPressed()
{
  Serial.println("Button pressed");
  tick();
  delay(200);
  tick();
  displayMode = (displayMode == 1 ? 2 : (displayMode == 2 ? 3 : (displayMode == 3 ? 1 : displayMode
  )));
  upadteDisplay();
}

void configModeCallback (WiFiManager *myWiFiManager) {

  displayConfigMode();
  loadingCount = 1;
  ticker1.attach(.2, displayLoading);
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  
  ticker.attach(0.2, tick);
  
}

void resetSystem(){
  Serial.println("Reset request detected.");
  ticker.attach(0.1, tick);
  displayResetSystem();
  delay(2000);
  wifiManager.resetSettings();
  ESP.reset();
  delay(1000);
}

void tick(){
  //toggle LED
  int state = digitalRead(BI_LED_PIN); 
  digitalWrite(BI_LED_PIN, !state);
}

void longPressDetector (){
 
  // read the state of the switch/button:
  int currentState = digitalRead(BUTTON_PIN);

  if(lastState == HIGH && currentState == LOW) {        // button is pressed
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;
  } 
  if(lastState == LOW && currentState == HIGH) {    // button is released
    isPressing = false;
  }
  if(isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - pressedTime;

    if( pressDuration > LONG_PRESS_TIME ) {
      resetSystem(); //Long press callbsck
      isLongDetected = true;
    }
  }
  // save the the last state
  lastState = currentState;
}

/*------------------LCD methos----START----------------*/

void initLCD(){
  lcd.begin();
  lcd.backlight();  
}

void displayTitle(){

  byte smallO0[] = {B00000,B00000,B00000,B01110,B11011,B11011,B11011,B01110};
  byte smallB0[] = {B00000,B11000,B11000,B11110,B11011,B11011,B11011,B11110};
  byte smallR0[] = {B00000,B00000,B00000,B01101,B01111,B01100,B01100,B01100};

  lcd.clear();
  lcd.createChar(1, smallO0);
  lcd.createChar(2, smallB0);
  lcd.createChar(3, smallR0);

  lcd.setCursor(3,0);
  lcd.write(3);
  lcd.setCursor(4,0);
  lcd.write(1);
  lcd.setCursor(5,0);
  lcd.write(2);
  lcd.setCursor(6,0);
  lcd.write(1);
  
  lcd.setCursor(8,0);
  lcd.write(3);
  lcd.setCursor(9,0);
  lcd.write(1);
  lcd.setCursor(10,0);
  lcd.write(2);
  lcd.setCursor(2,1);
  lcd.print("smart robot");
  delay(1000);
}

void displayConfigMode(){
  byte settings0[] = {B00000,B00000,B00100,B01100,B10100,B10100,B10011,B01000};
  byte settings1[] = {B00000,B00000,B01000,B01100,B01010,B01010,B10010,B00100};
  byte settings2[] = {B00100,B00100,B00100,B00100,B11111,B11111,B00000,B00000};
  byte settings3[] = {B01000,B01000,B01000,B01000,B11110,B11110,B00000,B00000};

  lcd.clear();
  lcd.createChar(1, settings0);
  lcd.createChar(2, settings1);
  lcd.createChar(3, settings2);
  lcd.createChar(4, settings3);
  
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.write(2);
  lcd.setCursor(0,1);
  lcd.write(3);
  lcd.setCursor(1,1);
  lcd.write(4);

  lcd.setCursor(4,0);
  lcd.print("config mode");
}

void displayIP(){
  
  byte small_I[] = {B00000,B01110,B00100,B00100,B00100,B01110,B00000,B00000};
  byte small_P[] = {B00000,B01110,B01010,B01110,B01000,B01000,B00000,B00000};

  lcd.clear();
  lcd.createChar(1, small_I);
  lcd.createChar(2, small_P);

  lcd.setCursor(0,0);
  lcd.print("-------");
  lcd.setCursor(7,0);
  lcd.write(1);
  lcd.setCursor(8,0);
  lcd.write(2);
  lcd.setCursor(9,0);
  lcd.print("-------");
  
  String ip = WiFi.localIP().toString();
  lcd.setCursor((16%ip.length())/2,1);
  lcd.print(ip);
}

void displayAP(){

  byte small_A[] = {B00000,B01110,B01010,B01110,B01010,B01010,B00000,B00000};
  byte small_P[] = {B00000,B01110,B01010,B01110,B01000,B01000,B00000,B00000};

  lcd.clear();
  lcd.createChar(1, small_A);
  lcd.createChar(2, small_P);

  lcd.setCursor(0,0);
  lcd.print("-------");
  lcd.setCursor(7,0);
  lcd.write(1);
  lcd.setCursor(8,0);
  lcd.write(2);
  lcd.setCursor(9,0);
  lcd.print("-------");
  String ap = WiFi.SSID();
  lcd.setCursor(ap.length() > 16 ?  0 : (16%ap.length())/2,1);
  lcd.print(ap.substring(0, 16));
}



void displayControl(){

  byte dir_f[]={B00000,B00100,B01110,B11111,B00100,B00100,B00000,B00000};
  byte dir_b[]={B00000,B00100,B00100,B11111,B01110,B00100,B00000,B00000};
  byte dir_l[]={B00000,B00100,B01100,B11111,B01100,B00100,B00000,B00000};
  byte dir_r[]={B00000,B00100,B00110,B11111,B00110,B00100,B00000,B00000};
  byte dir_f_na[] = {B11111,B00100,B01110,B11111,B00100,B00100,B00100,B00000};
  byte dir_b_na[] = {B00000,B00100,B00100,B00100,B11111,B01110,B00100,B11111};
  
  byte small_i[] = {B01110,B00100,B00100,B00100,B01110,B00000,B00000,B00000};
  byte small_n[] = {B01110,B01010,B01010,B01010,B01010,B00000,B00000,B00000};
  byte small_f[] = {B01110,B01000,B01100,B01000,B01000,B00000,B00000,B00000};
  byte small_o[] = {B01110,B01010,B01010,B01010,B01110,B00000,B00000,B00000};
  
  lcd.clear();
  lcd.createChar(1, small_i);
  lcd.createChar(2, small_n);
  lcd.createChar(3, small_f);
  lcd.createChar(4, small_o);
  
  lcd.createChar(6, dir_f);
  lcd.createChar(7, dir_f_na);
  lcd.createChar(8, dir_b_na);

  lcd.setCursor(6,0);
  lcd.write(1);
  lcd.setCursor(7,0);
  lcd.write(2);
  lcd.setCursor(8,0);
  lcd.write(3);
  lcd.setCursor(9,0);
  lcd.write(4);
  lcd.setCursor(0,0);
  lcd.print("------");
  lcd.setCursor(10,0);
  lcd.print("------");
  
  lcd.setCursor(0,1);
  lcd.print("D:");
  lcd.setCursor(2,1);
  lcd.write(6);
  
  lcd.setCursor(6,1);
  lcd.print("S:");
  lcd.setCursor(8,1);
  lcd.print("5");

  lcd.setCursor(12,1);
  lcd.print("O:");
  lcd.setCursor(14,1);
  lcd.write(7);
  lcd.setCursor(15,1);
  lcd.write(8);
}

void displayLoading(){

  byte lodingBox[]  = {B00000,B11111,B11111, B11111, B11111,B11111,B00000,B00000};
  lcd.createChar(5, lodingBox);

  loadingCount = loadingCount>18 ? 1 : loadingCount;
                      
  if(loadingCount<10){
    int i = loadingCount+4;
    if(i>5){
      lcd.setCursor(i-2,1);
      lcd.print(" ");
    }
    lcd.setCursor(i,1);
    lcd.write(5);
    
  }else{
    int i = 19 - loadingCount+4;
    if(i<13){
      lcd.setCursor(i+2,1);
      lcd.print(" ");
    }
    
    lcd.setCursor(i,1);
    lcd.write(5);
  }
  loadingCount++;
}

void displayConnectedToWiFi(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connected...");
  delay(1000);
  displayIP();
  delay(2000);
  displayAP();
  delay(2000);
  displayControl();
}

void displayResetSystem(){
  
  byte reset0[] = {B00011,B11111,B11000,B01010,B01010,B01010,B01000,B01111};
  byte reset1[] = {B11000,B11111,B00011,B10010,B10010,B10010,B00010,B11110};

  lcd.clear();
  lcd.createChar(1, reset0);
  lcd.createChar(2, reset1);

  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.write(2);

  lcd.setCursor(4,0);
  lcd.print("Resetting...");
  
}


void displayFailedToConnect(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Failed to connect");
  lcd.setCursor(0,1);
  lcd.print("Resterting...");
}

void upadteDisplay(){
  if(displayMode == 1){
      displayIP();
  }
  if(displayMode == 2){
      displayAP();
  }
  if(displayMode == 3){
      displayControl();
  }
}

/*------------------LCD methos----END----------------*/
