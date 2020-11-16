#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EasyButton.h>
#include <WebSocketsServer.h>


const char* ssid = "Supriya_Home";
const char* password = "apazyaea";

const int M1_IN_1 = D5;
const int M1_IN_2 = D6;
const int M2_IN_1 = D7;
const int M2_IN_2 = D8;
const int BI_LED_PIN = D4;
const int BUTTON_PIN = D3;

const int record_len = 100;

char control[record_len];
int duration[record_len];
boolean record_enable = false;
int ri=0;

unsigned long lastReceived;

char direc = 'S';


WebSocketsServer webSocket = WebSocketsServer(81);
EasyButton button(BUTTON_PIN);

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
  button.begin();
  button.onPressed(setupRecordMode);
  /* =====UDP WebSocket setup ======= End ====== */
}

void loop() {
    webSocket.loop();
    button.read();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  
  if (type == WStype_TEXT){
  
    /* =====Fetch data from APP ======= Start ===== */
    
    for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
    Serial.println();

    char msgType  = (char) payload[0];
    
    if(msgType == 'D'){ 
      direc  = (char) payload[2];

      if(record_enable && ri<record_len-2){
        control[ri]=direc;
        if(ri>0){
          duration[ri-1] = millis()-lastReceived;
        }
        ri++;
        updateLastReceivedTime();
      }
      driveCar();
    }
    if(msgType == 'P'){
      Serial.println("Record play Start");
      for (int i=0; i<ri; i++ ) {
       direc=control[i];
       Serial.print("D-");
       Serial.println(direc);
       driveCar();
       delay(duration[i]);
      }
      Serial.println("Record play End");
    }
       

    /* =====Fetch data from APP ======= End ====== */
   }
}

void setupRecordMode() {
  if(!record_enable){
    for (int i=0; i<record_len; i++ ) {
       control[i]=0;
       duration[i]=0;
    }
    record_enable = true;
    ri = 0;
    Serial.println("Record Enabled");
  }else{
    record_enable = false;
    
     Serial.println("Record Complete");
     for (int i=0; i<ri; i++ ) {
       Serial.print(control[i]);
       Serial.print("=>");
       Serial.println(duration[i]);
    }
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
  pinMode(BI_LED_PIN, OUTPUT);
  stop_motors();              //Keep the motor stop  
  lastReceived = millis();    //Set initial time  
}

void driveCar() {
  if(direc=='S') stop_motors();  
  else if (direc=='F') forward();
  else if (direc=='B') backward();
  else if (direc=='R') turn_right();
  else if (direc=='L') turn_left();
}

void forward() {
    digitalWrite(M1_IN_1, LOW);
    digitalWrite(M1_IN_2, HIGH);
    digitalWrite(M2_IN_1, LOW);
    digitalWrite(M2_IN_2, HIGH);
    digitalWrite(BI_LED_PIN,HIGH);
}

void backward() {
    digitalWrite(M1_IN_1, HIGH);
    digitalWrite(M1_IN_2, LOW);
    digitalWrite(M2_IN_1, HIGH);
    digitalWrite(M2_IN_2, LOW);
    digitalWrite(BI_LED_PIN,HIGH);
}

void turn_right() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M2_IN_1, HIGH);
  digitalWrite(M2_IN_2, LOW);
  digitalWrite(BI_LED_PIN,HIGH);
}

void turn_left() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
  digitalWrite(BI_LED_PIN,HIGH);
}

void stop_motors() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
  digitalWrite(BI_LED_PIN,LOW);
}
