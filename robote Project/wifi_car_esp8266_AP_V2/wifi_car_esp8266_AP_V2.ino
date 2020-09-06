#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "ROBO ROB - bot1";
const char *password = "123456789";

IPAddress ip(192, 168, 4, 1);
IPAddress netmask(255, 255, 255, 0);
const int port = 8080; // Port
ESP8266WebServer server(port);

const int M1_IN_1 = D3;
const int M1_IN_2 = D4;
const int M2_IN_1 = D7;
const int M2_IN_2 = D8;

char speed = '2';

boolean front_clear = true;
boolean rear_clear  = true;
boolean botMoving  = false;

boolean autoDetecction  = true;

String direction = "";

void setup() {
  Wire.begin(D1, D2);
  Serial.begin(115200); 
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, netmask);
  WiFi.softAP(ssid, password);

  pinMode(M1_IN_1, OUTPUT);
  pinMode(M1_IN_2, OUTPUT);
  pinMode(M2_IN_1, OUTPUT);
  pinMode(M2_IN_2, OUTPUT);

  server.on("/move", HTTP_GET, handleMoveRequest);
  server.on("/action", HTTP_GET, handleActionRequest);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
  
  if(wifi_softap_get_station_num()==0){
     stop_motors();
  }
  Wire.beginTransmission(8);
  Wire.write(speed);
  Wire.endTransmission(); 
  if(autoDetecction){
    Wire.requestFrom(8, 13);
    int i = 0;
    while(Wire.available()){
      char c = Wire.read();
      if(i==0)  front_clear = c == '1' ? true : false;
      if(i==1)  rear_clear  = c == '1' ? true : false;
      i++;
    }
    if (direction.equals("F")) {
      forward();
    }else if (direction.equals("B")) {
      backward();
    }else  if (direction.equals("L")) {
      turn_left();
    }else  if (direction.equals("R")) {
      turn_right();
    }
  }
}

void handleMoveRequest() {
  if (!server.hasArg("dir")) {
    server.send(404, "text / plain", "Move: undefined");
    return;
  }
  direction = server.arg("dir");
  Serial.println(direction);
  if (direction.equals("F")) {
    forward();
    server.send(200, "text / plain", "Move: forward");
  }
  else if (direction.equals("B")) {
    backward();
    server.send(200, "text / plain", "Move: backward");
  }
  else  if (direction.equals("S")) {
    stop_motors();
    server.send(200, "text / plain", "Move: stop");
  }
  else  if (direction.equals("L")) {
    turn_left();
    server.send(200, "text / plain", "Turn: Left");
  }
  else  if (direction.equals("R")) {
    turn_right();
    server.send(200, "text / plain", "Turn: Right");
  }
  else {
    server.send(404, "text / plain", "Move: undefined");
  }
}


void handleActionRequest() {
  if (!server.hasArg("type")) {
    server.send(404, "text / plain", "Action: undefined");
    return;
  }
  String type = server.arg("type");
  Serial.println(type);
  if (type.equals("1")) {
    autoDetecction = true;
    server.send(200, "text / plain", "Action 1");
  }
  else if (type.equals("2")) {
    autoDetecction = false;
    front_clear = true;
    rear_clear = true;
    server.send(200, "text / plain", "Action 2");
  }
  else if (type.equals("3")) {
    // TODO : Action 3
    server.send(200, "text / plain", "Action 3");
  }
  else if (type.equals("4")) {
    // TODO : Action 4
    server.send(200, "text / plain", "Action 4");
  }
  else if (type.equals("5")) {
    speed = '1';
    server.send(200, "text / plain", "Action 5");
  }
  else if (type.equals("6")) {
    speed = '2';
    server.send(200, "text / plain", "Action 6");
  }
  else if (type.equals("7")) {
    // TODO : Action 7
    server.send(200, "text / plain", "Action 7");
  }
  else if (type.equals("8")) {
    // TODO : Action 8
    server.send(200, "text / plain", "Action 8");
  }
  else {
    server.send(404, "text / plain", "Action: undefined");
  }
}

void handleNotFound() {
  server.send(404, "text / plain", "404: Not found");
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

void turn_left() {
  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, HIGH);
}

void stop_motors() {
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M2_IN_1, LOW);
  digitalWrite(M2_IN_2, LOW);
  botMoving = false;
}
