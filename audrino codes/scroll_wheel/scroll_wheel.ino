int sensorValue;
int value,c;
int i=0;

 void setup() { 

   Serial.begin (9600);
 } 

 void loop() { 
 sensorValue = analogRead(1); 
 
 if(sensorValue>200){
   value=1;
 }else{
   value=0;
 }
 
 if(c!=value){
       i++;
      Serial.println (i);
 }


   
   c=value;

 
 } 
