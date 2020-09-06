

int counter = 0;
int value = 0;
byte buffer[16] = { 
  0 };
int lastvalue = 0;
int avg = 0;
int freq = 0;
void setup() {
 pinMode(3, OUTPUT);
  Serial.begin(115200);
}

//Set's a single column value
//In my case the displays are rotated 90 degrees
//so in the code I'm setting rows instead of colums actualy
void Set(int index, int value)
{

  
     int device = index / 8; //calculate device
     int row = index - (device * 8); //calculate row
     int leds = map(value, 0, 255, 0, 9); //map value to number of leds.

     if(index == 0 || index == 1 || index == 2 || index == 3){
        avg = avg + value;
      }

     if(index == 15){
      analogWrite(3, value);
      //analogWrite(3, avg/4);
      avg = 0;
     }

}

void loop()
{
  if (Serial.available() >= 15)
  {
    value = Serial.read();
    Set(counter, value);
    counter++;
    if (counter > 15) counter = 0;
  }
}
