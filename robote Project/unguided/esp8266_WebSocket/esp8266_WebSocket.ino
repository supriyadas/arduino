#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>

const boolean CONN_TYPE_WIFI_AP = false;

const char* ssid = "Supriya_Home";
const char* password = "apazyaea";

const int M1_IN_1 = D3;
const int M1_IN_2 = D4;
const int M2_IN_1 = D7;
const int M2_IN_2 = D8;

boolean front_clear = true;
boolean rear_clear  = true;
boolean autoDetecction  = false;
unsigned long lastReceived;

char direc = 'S';
char botSpeed = '5';

WebSocketsServer webSocket = WebSocketsServer(81);
SimpleTimer timer;

void setup() {
  
  initSetup();

  /* =====WI-FI connection setup ==== Start ===== */
  
  if(CONN_TYPE_WIFI_AP){
    
    WiFi.mode(WIFI_AP);
    IPAddress ip(192, 168, 4, 1);
    IPAddress netmask(255, 255, 255, 0);
    WiFi.softAPConfig(ip, ip, netmask);
    WiFi.softAP(ssid, password);
    
  }else{

    WiFi.begin(ssid, password);
    Serial.println("");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
     }
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  /* =====WI-FI connection setup ==== End ====== */

  /* =====UDP WebSocket setup ======= Start ===== */
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  timer.setInterval(1000,mainLoop); 
  
  /* =====UDP WebSocket setup ======= End ====== */
}

void loop() {
    webSocket.loop();
    timer.run();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  
  if (type == WStype_TEXT){
  
    /* =====Fetch data from APP ======= Start ===== */
    
    for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
    Serial.println();

    char msgType  = (char) payload[0];
    
    if(msgType == 'D'){         // Set bot direction
       
      direc  = (char) payload[2];
      driveCar();
      
    }else if(msgType == 'O'){   // Obstacle detection 
      
      char obsOpt  = (char) payload[2];
      autoDetecction = obsOpt == 'E' ? true : false;
      if(!autoDetecction){
        front_clear = true;
        rear_clear = true;
      }
      driveCar();
      
    }else if(msgType == 'S'){   // Set motor speed 
      
      botSpeed  = (char) payload[2];
      Wire.beginTransmission(8);
      Wire.write(botSpeed);
      Wire.endTransmission(); 
      driveCar();
      
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
    while(Wire.available()){
      char c = Wire.read();
      if(i==0)  front_clear = c == '1' ? true : false;
      if(i==1)  rear_clear  = c == '1' ? true : false;
      i++;
    }
    driveCar();

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

void updateLastReceivedTime() {
  lastReceived = millis();
}

void initSetup() {
  Serial.begin(115200);
  
  pinMode(M1_IN_1, OUTPUT);   //Left motor
  pinMode(M1_IN_2, OUTPUT);   //Left motor
  pinMode(M2_IN_1, OUTPUT);   //Right motor
  pinMode(M2_IN_2, OUTPUT);   //Right motor
  
  stop_motors();              //Keep the motor stop  
  Wire.begin(D1, D2);         //Start I2C with Arduino
  lastReceived = millis();    //Set initial time  
}

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
