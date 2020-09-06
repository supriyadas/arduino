const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int whitePin = 3;


const int leftChannel = 0;
int left,left1;
int color=1,i=0;
String md="s";

int whval=0;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


const int max_red = 255;
const int max_green = 90;
const int max_blue = 100;



void setup() {
  
    pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(whitePin, OUTPUT);

  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    md="b";
    analogWrite(whitePin, 0);
    
    if(md=="b"){
    
    Serial.println(inputString);

    int commaIndex1 = inputString.indexOf(',');
    int commaIndex2 = inputString.indexOf(',', commaIndex1+1);
    int commaIndex3 = inputString.indexOf(',', commaIndex2+1);
     
    String firstValue = inputString.substring(0, commaIndex1);
    String secondValue = inputString.substring(commaIndex1+1, commaIndex2);
    String thirdValue = inputString.substring(commaIndex2+1, commaIndex3);
    
    int r = (firstValue.toInt())*1.4;
    int g = (secondValue.toInt())*1.4;
    int b = (thirdValue.toInt())*1.4;
    
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
    
    
    
    //Serial.println(r);
    // clear the string:
    inputString = "";
    stringComplete = false;
    }
    
    
  }else{
  
    if(md=="s"){
        
   
        
        left = analogRead(leftChannel); 
        Serial.println(left);  
        left=left*3;
        
        
        if(left>0 && left<=70){
          
        analogWrite(redPin, 0);
        analogWrite(greenPin, left*2);
        analogWrite(bluePin, 0);
        //analogWrite(whitePin, 0);
        //whval=left;
        }
        
        else if(left>70 && left<=140){
        analogWrite(redPin, 0);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, left);
        //analogWrite(whitePin, 0);
        
        if(left>100){
        whval=left;
        }
        }
        
        else if(left>140 && left<=900){
        analogWrite(redPin, left);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 0);
        
        //analogWrite(whitePin, left);
        //digitalWrite(whitePin, HIGH);
        //analogWrite(whitePin, 255);
        whval=left;
       
        
        
        }
        else{
        }
        
        if(whval<0){
        whval=0;
        }
        analogWrite(whitePin, whval);
        
        if(whval<70){
        whval-=1;
        }else{
        whval-=2;
        }
        
        
        
        delay(5);
  
    
    }
  
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    
    
  }
}
