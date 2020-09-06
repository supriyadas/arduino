const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  
    pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
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
