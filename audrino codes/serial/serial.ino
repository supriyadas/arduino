const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int leftChannel = 2;
int left,left1;

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
    left = analogRead(leftChannel); 
    Serial.println(left);  
    
    delay(500);
    /*
  // print the string when a newline arrives:
  if (stringComplete) {
    //Serial.println(inputString);

    
    
    //Serial.println(r);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  */
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
