#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Ticker.h>
#include <EasyButton.h>

/*====== System Pin Configuration ======*/

const int M1_IN_1 = D5;
const int M1_IN_2 = D6;
const int M2_IN_1 = D7;
const int M2_IN_2 = D8;

const int BI_LED_PIN = D4;
const int BUTTON_PIN = D3;

/*====== System Glaboal variables ======*/

//Bot driving veriable

boolean front_clear = true;
boolean rear_clear  = true;
boolean autoDetecction  = true;
unsigned long lastReceived;
char direc = 'S';
char botSpeed = '5';

//Long press detcetion veriables 

int lastState = LOW;
bool isPressing = false;
bool isLongDetected = false;
unsigned long pressedTime  = 0;
const int LONG_PRESS_TIME  = 5000;

//Diplay lcd veriable

int displayMode = 1;
int loadingCount = 1;

//Initializing services
Ticker ticker,ticker1;
SimpleTimer timer;
EasyButton button(BUTTON_PIN);
WiFiManager wifiManager;
LiquidCrystal_I2C lcd(0x23, 16, 2);
WebSocketsServer webSocket = WebSocketsServer(81);


void setup() {
  
  initSetup();    
  displayTitle();

  /* =====WI-FI connection setup ==== Start ===== */

  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("RobRobSetup");
  
  //Prevent system goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {

    displayFailedToConnect();
    delay(2000);
    Serial.println("failed to connect and hit timeout");
    ESP.restart();
    delay(1000);
    
  }

  //System succesfully connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  ticker1.detach();
  digitalWrite(BI_LED_PIN, LOW);
  displayConnectedToWiFi();
  
  /* =====WI-FI connection setup ==== End ====== */

  /* =====UDP WebSocket setup ======= Start ===== */
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  timer.setInterval(1000,mainLoop); 
  
  /* =====UDP WebSocket setup ======= End ====== */

  button.begin();
  button.onPressed(onBtnPressed);
}

void loop() {
    webSocket.loop();
    timer.run();
    button.read();
    longPressDetector();
}

/*
|===============================================================|
|--------------------Callback methods----START------------------|
|===============================================================|
*/

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  
  if (type == WStype_TEXT){
  
    /* =====Fetch data from APP ======= Start ===== */
    
    for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
    Serial.println();

    char msgType  = (char) payload[0];
    
    if(msgType == 'D'){         // Set bot direction
       
      direc  = (char) payload[2];
      driveCar();
      displayUpdateDir();
      
    }else if(msgType == 'O'){   // Obstacle detection 
      
      char obsOpt  = (char) payload[2];
      autoDetecction = obsOpt == 'E' ? true : false;
      if(!autoDetecction){
        front_clear = true;
        rear_clear = true;
      }
      driveCar();
      displayUpdateObs();
      
    }else if(msgType == 'S'){   // Set motor speed 
      
      botSpeed  = (char) payload[2];
      Wire.beginTransmission(8);
      Wire.write(botSpeed);
      Wire.endTransmission(); 
      driveCar();
      displayUpdateSpeed();
    }
    updateLastReceivedTime();   // Updating last msg receive time

    /* =====Fetch data from APP ======= End ====== */
   }
}



void mainLoop(){

  if(autoDetecction){

    //Fetch Obstacle details from Arduino using I2C
    
    Wire.requestFrom(8, 2);
    int i = 0;
    boolean front_clear_last = front_clear;
    boolean rear_clear_last = rear_clear;
    
    while(Wire.available()){
      char c = Wire.read();
      if(i==0)  front_clear = c == '1' ? true : false;
      if(i==1)  rear_clear  = c == '1' ? true : false;
      i++;
    }
    driveCar();
    if(front_clear_last!=front_clear || rear_clear_last!=rear_clear){
      displayUpdateObs();
    }
    
    //Send status to APP when Obstacle Detection enabled
    
    char obsStatus[4] = {'O',front_clear ? '0' : '1',rear_clear ? '0' : '1','E'}; 
    webSocket.broadcastTXT(obsStatus, sizeof(obsStatus));
    
  }else{
    
    //Send status to APP when Obstacle Detection disabled
    
    char obsStatus[4] = {'O', '0', '0', 'D'}; 
    webSocket.broadcastTXT(obsStatus, sizeof(obsStatus));
  }
  
  //Check if connection lost from APP
  
  if(millis() - lastReceived>4000){
      stop_motors();
      direc = 'S';
     Serial.println("Disconnect");
  }
}

void onBtnPressed()
{
  Serial.println("Button pressed");
  tick();
  delay(200);
  tick();
  displayMode = (displayMode == 1 ? 2 : (displayMode == 2 ? 3 : (displayMode == 3 ? 1 : displayMode)));
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


/*
|===============================================================|
|--------------------Callback methods----END--------------------|
|===============================================================|

|===============================================================|
|--------------------Utility methods----START-------------------|
|===============================================================|
*/

void updateLastReceivedTime() {
  lastReceived = millis();
}

void initSetup() {
  Serial.begin(115200);
  
  pinMode(M1_IN_1, OUTPUT);   //Left motor
  pinMode(M1_IN_2, OUTPUT);   //Left motor
  pinMode(M2_IN_1, OUTPUT);   //Right motor
  pinMode(M2_IN_2, OUTPUT);   //Right motor
  pinMode(BI_LED_PIN, OUTPUT);//LED ind poi
  
  stop_motors();              //Keep the motor stop  
  Wire.begin(D1, D2);         //Start I2C with Arduino
  ticker.attach(0.6, tick);   //Start blinking LED
  lastReceived = millis();    //Set initial time  
  lcd.begin();                //Initialize LCD
  lcd.backlight();            //Turn LCD backlight On
}


/*
|===============================================================|
|--------------------Utility methods----END---------------------|
|===============================================================|

|===============================================================|
|------------------MOTOR control methods----START---------------|
|===============================================================|
*/

void driveCar() {
  if(direc=='S') stop_motors();  
  else if (direc=='F') forward();
  else if (direc=='B') backward();
  else if (direc=='R') turn_right();
  else if (direc=='L') turn_left();
  else if (direc=='W') turn_front_right();
  else if (direc=='X') turn_front_left();
  else if (direc=='Y') turn_back_left();
  else if (direc=='Z') turn_back_right();
}

void forward() {
  if(front_clear){
    digitalWrite(M1_IN_1, LOW);
    digitalWrite(M1_IN_2, HIGH);
    digitalWrite(M2_IN_1, LOW);
    digitalWrite(M2_IN_2, HIGH);
  }else{
    stop_motors();
  }
}

void backward() {
  if(rear_clear){
    digitalWrite(M1_IN_1, HIGH);
    digitalWrite(M1_IN_2, LOW);
    digitalWrite(M2_IN_1, HIGH);
    digitalWrite(M2_IN_2, LOW);
  }else{
    stop_motors();
  }
}

void turn_right() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M2_IN_1, HIGH);
  digitalWrite(M2_IN_2, LOW);
}

void turn_front_right() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
}

void turn_back_right() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
}

void turn_left() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
}

void turn_front_left() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
}

void turn_back_left() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, HIGH);
  digitalWrite(M2_IN_2, LOW);
}

void stop_motors() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
}

/*
|===============================================================|
|------------------MOTOR control methods----END-----------------|
|===============================================================|


|===============================================================|
|-----------------------LCD methods----START--------------------|
|===============================================================|
*/

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

byte dir_s[]={B00000,B00000,B01111,B01111,B01111,B01111,B00000,B00000}; 
byte dir_f[]={B00000,B00100,B01110,B11111,B00100,B00100,B00000,B00000};
byte dir_b[]={B00000,B00100,B00100,B11111,B01110,B00100,B00000,B00000};
byte dir_l[]={B00000,B00100,B01100,B11111,B01100,B00100,B00000,B00000};
byte dir_r[]={B00000,B00100,B00110,B11111,B00110,B00100,B00000,B00000};
byte dir_w[]={B00000,B01111,B00111,B00111,B01001,B10000,B00000,B00000};
byte dir_x[]={B00000,B11110,B11100,B11100,B10010,B00001,B00000,B00000};
byte dir_y[]={B00000,B00000,B00001,B10010,B11100,B11100,B11110,B00000};
byte dir_z[]={B00000,B00000,B10000,B01001,B00111,B00111,B01111,B00000};
byte dir_f_na[] = {B11111,B00100,B01110,B11111,B00100,B00100,B00100,B00000};
byte dir_b_na[] = {B00000,B00100,B00100,B00100,B11111,B01110,B00100,B11111};

byte small_i[] = {B01110,B00100,B00100,B00100,B01110,B00000,B00000,B00000};
byte small_n[] = {B01110,B01010,B01010,B01010,B01010,B00000,B00000,B00000};
byte small_f[] = {B01110,B01000,B01100,B01000,B01000,B00000,B00000,B00000};
byte small_o[] = {B01110,B01010,B01010,B01010,B01110,B00000,B00000,B00000};

void displayControlTemplate(){
  
  lcd.clear();
  lcd.createChar(1, small_i);
  lcd.createChar(2, small_n);
  lcd.createChar(3, small_f);
  lcd.createChar(4, small_o);

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

  lcd.setCursor(6,1);
  lcd.print("S:");

  lcd.setCursor(12,1);
  lcd.print("O:");
  
}  
void displayUpdateDir(){
  
  if(displayMode==3){
    
    if(direc=='S') lcd.createChar(6, dir_s);  
    else if (direc=='F') lcd.createChar(6, dir_f);
    else if (direc=='B') lcd.createChar(6, dir_b);
    else if (direc=='R') lcd.createChar(6, dir_r);
    else if (direc=='L') lcd.createChar(6, dir_l);
    else if (direc=='W') lcd.createChar(6, dir_w);
    else if (direc=='X') lcd.createChar(6, dir_x);
    else if (direc=='Y') lcd.createChar(6, dir_y);
    else if (direc=='Z') lcd.createChar(6, dir_z);
  
    lcd.setCursor(2,1);
    lcd.write(6);
  }
}

void displayUpdateSpeed(){
  if(displayMode==3){
    lcd.setCursor(8,1);
    lcd.print(botSpeed);
  }
}

void displayUpdateObs(){
  if(displayMode==3){
    lcd.createChar(7, autoDetecction ? front_clear ? dir_f : dir_f_na : dir_s);
    lcd.createChar(8, autoDetecction ? rear_clear  ? dir_b : dir_b_na : dir_s);
    
    lcd.setCursor(14,1);
    lcd.write(7);
    lcd.setCursor(15,1);
    lcd.write(8);
  }
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
  displayControlTemplate();
  displayMode = 3;
  displayUpdateDir();
  displayUpdateSpeed();
  displayUpdateObs();
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
      displayControlTemplate();
      displayUpdateDir();
      displayUpdateSpeed();
      displayUpdateObs();
  }
}

/*
|===============================================================|
|-----------------------LCD methods----END----------------------|
|===============================================================|
*/
