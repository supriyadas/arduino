
int LaserPin = 3; 
int motorrc = 9;
int motorra = 8;
int stopflag=1;
int led1=10;
int led2=11;
int led3=12;
int led4=13;
int i=0;
int flag_gr=1;

String readString;
String gkey="";

void setup() {     // presenting pins as outputs
  
  pinMode(motorrc, OUTPUT);
  pinMode(motorra, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led4, LOW);
  
  Serial.begin(9600);
}

void loop() {
  
  if(stopflag==0){
    digitalWrite(led1, HIGH);
  }else{
    digitalWrite(led1, LOW);
  }
  
  
  readString="";
  while(Serial.available()){ // the code here is for bluetooth
  delay(3);
  char c = Serial.read();
  readString += c;
  }
  
  if(readString!=""){
    if(gkey!=readString){
      gkey=readString;
      //Serial.println(gkey);
      
    
    }else{
    
      //Serial.println(gkey);
    }
  }
 
  
 
  if(gkey=="L1"){
  analogWrite(LaserPin, 90);   
  }
  if(gkey=="L2"){
  analogWrite(LaserPin, 180);   
  }
  if(gkey=="L3"){
  analogWrite(LaserPin, 255);   
  }
  if(gkey=="LF"){
  analogWrite(LaserPin, 0);   
  }
  
  
  if(gkey=="F"){
  digitalWrite(motorrc, HIGH);
  digitalWrite(motorra, LOW);   
  stopflag=1;
  }
  if(gkey=="B"){
    if(stopflag==1){
      digitalWrite(motorrc, LOW);
      digitalWrite(motorra, HIGH);
    }  
  }
  if(gkey=="S"){
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);   
  }
  

  if(gkey=="R"){
    
  analogWrite(LaserPin, 0);    
  if(analogRead(1)>500){
    stopflag=0;
    flag_gr=1;
  }  
  
  if(stopflag==1){
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, HIGH);
  digitalWrite(led2, LOW);
  }
  if(stopflag==0){
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);
  }

  }
  
  
  
  if(gkey=="ST"){
  analogWrite(LaserPin, 200);
  stopflag=1;
  
  if(flag_gr==1){  
  
  digitalWrite(led4, HIGH);  
  i++;
  digitalWrite(motorrc, HIGH);
  digitalWrite(motorra, LOW);   
  delay(10);
  //Serial.println(i);
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);
  delay(10);
  
  if(analogRead(0)>300){
    flag_gr=0;
    digitalWrite(motorrc, LOW);
    digitalWrite(motorra, LOW);
    Serial.println(i);
    digitalWrite(led2, HIGH);
    digitalWrite(led4, LOW);
    i=0;
  } 
  
  
  
  }
  
  }
  

}
