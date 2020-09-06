int motorrc = 9;
int motorra = 8;
int ledPin=5;

String readString;
String gkey="";

int f=0;
int s=0;
int i=0;
int ldr_value = 0;  


void setup()

{

  pinMode(motorrc, OUTPUT);
  pinMode(motorra, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);
  analogWrite(ledPin, 200); 
  Serial.begin(9600);
}



void loop()

{

  readString="";
  while(Serial.available()){ // the code here is for bluetooth
  delay(3);
  char c = Serial.read();
  readString += c;
  }
  
    if(readString!=""){
    if(gkey!=readString){
      gkey=readString;
      Serial.println(gkey);
      
    
    }
  }
  
    ldr_value = analogRead(0);   
  
 delay(10);
   if(ldr_value>0){ 
       //Serial.println("p");
 //Serial.println(ldr_value);
  

 
  if(ldr_value<200){
    s=0;
  
  }else{
    s=1;
  } 
 
  if(f!=s){
    f=s;
    i++;
    Serial.println("s");
    Serial.println(i);
  }  
  
   }

  if(gkey=="1"){
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, HIGH);  
 }
   if(gkey=="2"){
  digitalWrite(motorrc, HIGH);
  digitalWrite(motorra, LOW);  
 }
   if(gkey=="3"){
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);  
 }
 
 
 
}

