char data;
void setup() {
  Serial.begin(9600);
}
String serialString;

void loop() {

    serialString="";
    while(Serial.available()){
      delay(3);
      char c = Serial.read();
      serialString += c;
    }

   Serial.println(serialString);
}



