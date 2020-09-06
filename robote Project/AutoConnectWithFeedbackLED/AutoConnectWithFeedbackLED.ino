#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Ticker.h>
#include <EasyButton.h>

const int BUTTON_PIN = D3;
const int LONG_PRESS_TIME  = 3000;

// Variables will change:
int lastState = LOW;
unsigned long pressedTime  = 0;
bool isPressing = false;
bool isLongDetected = false;

Ticker ticker;
EasyButton button(BUTTON_PIN);
WiFiManager wifiManager;
LiquidCrystal_I2C lcd(0x23, 16, 2);

void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Config mode");
  lcd.setCursor(0,1);
  lcd.print(myWiFiManager->getConfigPortalSSID());
  
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // initialize the LCD
  lcd.begin();
  lcd.backlight();

  displayTitle();
  
  
  //set led pin as output
  pinMode(BUILTIN_LED, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  wifiManager.autoConnect("ConfigRob");
  
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed to connect");
    lcd.setCursor(0,1);
    lcd.print("Resterting...");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, LOW);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("C-");
  lcd.setCursor(2,0);
  lcd.print(WiFi.SSID());
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());

  button.begin();
  button.onPressed(onPressed);
}

void loop() {
  longPressDetector();
  button.read();
}

void onPressed()
{
  Serial.println("Button pressed");
  tick();
  delay(200);
  tick();
}

void longPressCallbck(){
  Serial.println("A long press is detected");
  ticker.attach(0.1, tick);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Resetting..");
  delay(2000);
  wifiManager.resetSettings();
  ESP.reset();
  delay(1000);
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
      longPressCallbck();
      isLongDetected = true;
    }
  }
  // save the the last state
  lastState = currentState;
}


/*------------------LCD methos----------------*/
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
}
