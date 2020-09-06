int f=0;
int s=0;
int i=0;
int ldr_value = 0;  

void setup() {

    Serial.begin(9600); 
}

void loop()
{

  ldr_value = analogRead(0);   
  //Serial.println(ldr_value);
  
  if(ldr_value<10){
    s=0;
  
  }else{
    s=1;
  } 
  
  if(f!=s){
    f=s;
    i++;
    Serial.println(i);
  }
  
               
}
