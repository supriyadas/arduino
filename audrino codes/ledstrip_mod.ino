

//pin definitions.  must be PWM-capable pins!
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int leftChannel = 0; 
int left,left1;
int state;
int color=1,i=0;

byte lineEnding = 0x0A; 

//maximum duty cycle to be used on each led for color balancing.  


void setup(){
  //set all three of our led pins to output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600); 

}


void loop(){
  if(Serial.available() > 12){ 
    state = Serial.read(); 
    //int bytesRead = Serial.readBytesUntil(lineEnding, (char*)state, 3);
    Serial.println(state);
  }
  
  
  left = analogRead(leftChannel); 
  //Serial.println(left);  
  left=left*3;
  
  
  if(left>0 && left<=80){
  
   
  analogWrite(redPin, 0);
  analogWrite(greenPin, left);
  analogWrite(bluePin, 0);
  
  
  }
  
  else if(left>110 && left<=140){
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, left);
  }
  
  else if(left>160 && left<=900){
    
    
  analogWrite(redPin, left);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  
  
  }
  else{
  }
  
  delay(5);
  
}
