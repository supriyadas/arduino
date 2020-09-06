int motorrc = 9;
int motorra = 8;
int pin1 = 0;  
int pin2 = 0;  

void setup()
{
  pinMode(motorrc, OUTPUT);
  pinMode(motorra, OUTPUT);
}

void loop()
{
  if(analogRead(4)>100){
  pin1=1;
  }else{
  pin1=0;
  }
  
  if(analogRead(5)>100){
  pin2=1;
  }else{
  pin2=0;
  }

 if(pin1==1&&pin2==0){
  digitalWrite(motorrc, HIGH);
  digitalWrite(motorra, LOW);  
  delay(5);
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);  
  delay(20);
 }
 
 if(pin1==0&&pin2==1){ 
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, HIGH);  
  delay(5);
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);  
  delay(20);
 }

 if(pin1==0&&pin2==0){ 
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);  
 }
  
               
}
