int motorlc = 13; // left motor clockwise rotation
int motorla = 12; // left motor anticlockwise rotation
int i=0;
void setup() {     // presenting pins as outputs
  
  pinMode(motorlc, OUTPUT);
  pinMode(motorla, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  i++;
  Serial.println(i);
    digitalWrite(motorlc, LOW);
    digitalWrite(motorla, HIGH);
    delay(10);
    digitalWrite(motorlc, LOW);
    digitalWrite(motorla, LOW);
    delay(100);

}
