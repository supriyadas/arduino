#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>

const int BI_LED_PIN = D4;

const char* ssid = "Supriya_Home";
const char* password = "apazyaea";

const int M1_IN_1 = D5;
const int M1_IN_2 = D6;

const int M2_IN_1 = D7;
const int M2_IN_2 = D8;

WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
  
  initSetup();

  /* =====WI-FI connection setup ==== Start ===== */

  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
    

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  /* =====WI-FI connection setup ==== End ====== */

  /* =====UDP WebSocket setup ======= Start ===== */
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  /* =====UDP WebSocket setup ======= End ====== */
}

void loop() {
    webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  
  if (type == WStype_TEXT){
  
    /* =====Fetch data from APP ======= Start ===== */
    
    for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
    Serial.println();
    char stChar  = (char) payload[0];
    tick();
    if(stChar=='A'){
      forward();
    }else if (stChar=='B'){
      forward();
    }else if (stChar=='L'){
      turn_front_right();
    }else if (stChar=='R'){
      turn_front_left();
    }else if (stChar=='S'){
      stop_motors();
      digitalWrite(BI_LED_PIN, LOW);
    }else if (stChar=='X'){
      
      char botSpeed = botSpeed  = (char) payload[1];
      Wire.beginTransmission(8);
      Wire.write(botSpeed);
      Wire.endTransmission();
      
    }else{
      stop_motors();
    }

    /* =====Fetch data from APP ======= End ====== */
   }
}


void tick(){
  //toggle LED
  int state = digitalRead(BI_LED_PIN); 
  digitalWrite(BI_LED_PIN, !state);
}


void initSetup() {
  Serial.begin(115200);
  
  pinMode(M1_IN_1, OUTPUT);   //Left motor
  pinMode(M1_IN_2, OUTPUT);   //Left motor
  pinMode(M2_IN_1, OUTPUT);   //Right motor
  pinMode(M2_IN_2, OUTPUT);   //Right motor
  Wire.begin(D1, D2);         //Start I2C with Arduino
  stop_motors();              //Keep the motor stop  
}


void forward() {
    digitalWrite(M1_IN_1, LOW);
    digitalWrite(M1_IN_2, HIGH);
    digitalWrite(M2_IN_1, LOW);
    digitalWrite(M2_IN_2, HIGH);
}

void backward() {
    digitalWrite(M1_IN_1, HIGH);
    digitalWrite(M1_IN_2, LOW);
    digitalWrite(M2_IN_1, HIGH);
    digitalWrite(M2_IN_2, LOW);
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
