int motorrc = 9;
int motorra = 8;
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
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW); 
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
      //Serial.println(gkey);
      
    
    }else{
    
      //Serial.println(gkey);
    }
  }
  
  ldr_value = analogRead(0);   
  
  if(ldr_value<10){
    s=0;
  
  }else{
    s=1;
  } 
  
  if(f!=s){
    f=s;
    i++;
  }  
  
  
 if(gkey=="B"){
  digitalWrite(motorrc, HIGH);
  digitalWrite(motorra, LOW);  
 }
 
  if(gkey=="F"){
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, HIGH);  
 }
 
  if(gkey=="S"){
  
  if(i>0){  
  Serial.println(i);
  i=0;
  }
  
  digitalWrite(motorrc, LOW);
  digitalWrite(motorra, LOW);   
  }
  
 
 
}

