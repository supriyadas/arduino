//Serial RGB led controller
//Author: Trevor Shannon
//see http://trevorshp.com/creations/android_led.htm

//pin definitions.  must be PWM-capable pins!
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int leftChannel = 0; 
int left,left1;
int color=1,i=0;

//maximum duty cycle to be used on each led for color balancing.  
//if "white" (R=255, G=255, B=255) doesn't look white, reduce the red, green, or blue max value.
const int max_red = 255;
const int max_green = 90;
const int max_blue = 100;



void setup(){
  //set all three of our led pins to output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
   Serial.begin(9600); 

}


void loop(){
  left = analogRead(leftChannel); 
  Serial.println(left);  
  left=left*3;
  
  
  if(left>0 && left<=70){
  analogWrite(redPin, 0);
  analogWrite(greenPin, left*2);
  analogWrite(bluePin, 0);
  }
  
  else if(left>70 && left<=140){
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, left);
  }
  
  else if(left>140 && left<=900){
  analogWrite(redPin, left);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  }
  else{
  }
  delay(5);
  
}
