#include <SendOnlySoftwareSerial.h>

SendOnlySoftwareSerial mySerial(6);  // Tx pin

void setup()
{
  mySerial.begin(9600);
}

int i;

void loop()
{
  mySerial.print ("test: ");
  mySerial.println (i++);
  delay (100);
}
