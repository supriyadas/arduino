#define tpin 3
#define echo 2
#define led 8


void setup(){

Serial.begin(9600);
pinMode(tpin, OUTPUT);
pinMode(echo, INPUT);
pinMode(led, OUTPUT);
}


void loop(){

int duration, distance;

digitalWrite(tpin, HIGH);
delayMicroseconds(1000);
digitalWrite(tpin, LOW);

duration=pulseIn(echo,HIGH);
distance=(duration/2)/29.1;

if(distance<10){
  digitalWrite(led, HIGH);
}else{
  digitalWrite(led, LOW);
}


Serial.print(distance);
Serial.print("cm");
Serial.println("");
delay(0);
}

