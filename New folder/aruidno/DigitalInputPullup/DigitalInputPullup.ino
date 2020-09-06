

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
 int sensorValue = analogRead(A0);
 Serial.println(sensorValue);
 delay(50);
}
