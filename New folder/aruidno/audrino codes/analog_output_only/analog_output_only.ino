void setup()

{
  pinMode(3, OUTPUT);
  analogWrite(3,150);
  Serial.begin(9600);
}

void loop()

{
  
  Serial.println(analogRead(0));
  delay(500);
  
}
