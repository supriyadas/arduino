const int BUTTON_PIN = D3;
const int LONG_PRESS_TIME  = 3000;

// Variables will change:
int lastState = LOW;
unsigned long pressedTime  = 0;
bool isPressing = false;
bool isLongDetected = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  longPressDetector();
}
void longPressCallbck(){
  Serial.println("A long press is detected");
}

void longPressDetector (){
 
  // read the state of the switch/button:
  int currentState = digitalRead(BUTTON_PIN);

  if(lastState == HIGH && currentState == LOW) {        // button is pressed
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;
  } 
  if(isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - pressedTime;

    if( pressDuration > LONG_PRESS_TIME ) {
      longPressCallbck();
      isLongDetected = true;
    }
  }
  // save the the last state
  lastState = currentState;
  
}
