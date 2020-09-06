#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(250, PIN, NEO_GRB + NEO_KHZ800);
int j=1;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
nflag(500);
//rainbowCycleNew(5);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// The colours are a transition b - r - g 
void nflag(uint8_t wait) {
int i,k;
for(i=0; i<strip.numPixels(); i++) {
     strip.setPixelColor(i, strip.Color(0, 0, 0)) ;
}
for(i=0; i<50; i++) {
   strip.setPixelColor(i, strip.Color(255, 255, 255));
}
for(i=50; i<100; i++) {
   strip.setPixelColor(i, strip.Color(0, 255, 0));
}
for(i=100; i<150; i++) {
   strip.setPixelColor(i, strip.Color(210, 0, 63));
}
//for(i=150; i<200; i++) {
//   strip.setPixelColor(i, strip.Color(100, 240, 138));
//}
//for(i=200; i<250; i++) {
//   strip.setPixelColor(i, strip.Color(140, 240, 138));
//}
  strip.show();
   delay(wait);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
       strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
void rainbowCycleNew(uint8_t wait) {
  uint16_t i, j;

  for(j=1; j<=256*5; j++) { // 5 cycles of all colors on wheel
    
    for(i=1; i<= 19; i++) {
      if(i>=1&&i<=7){
        setHex(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      
      if(i>=8&&i<=13){
        setTriA(i-7, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }

      if(i>=14&&i<=19){
        setTriB(i-13, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
    }
    
    strip.show();
    delay(wait);
  }
}

void setHex(int i, uint32_t c) {

switch(i){
  
 case 1:
  strip.setPixelColor(0, c);
  strip.setPixelColor(1, c);
  strip.setPixelColor(2, c);
 break; 

case 2:
  strip.setPixelColor(3, c);
  strip.setPixelColor(4, c);
  strip.setPixelColor(5, c);
 break; 


 case 3:
  strip.setPixelColor(15, c);
  strip.setPixelColor(16, c);
  strip.setPixelColor(17, c);
 break; 

 case 4:
  strip.setPixelColor(18, c);
  strip.setPixelColor(19, c);
  strip.setPixelColor(20, c);
 break; 
 
 case 5:
  strip.setPixelColor(6, c);
  strip.setPixelColor(7, c);
  strip.setPixelColor(8, c);
 break; 
 

 case 6:
  strip.setPixelColor(12, c);
  strip.setPixelColor(13, c);
  strip.setPixelColor(14, c);
 break; 

  case 7:
  strip.setPixelColor(9, c);
  strip.setPixelColor(10, c);
  strip.setPixelColor(11, c);
 break; 


 }

}
 void setTriA(int i, uint32_t c) {

    switch(i){
      case 1:
       strip.setPixelColor(21, c);
      break; 

      case 2:
       strip.setPixelColor(24, c);
      break; 

      case 3:
       strip.setPixelColor(31, c);
      break; 

      case 4:
       strip.setPixelColor(32, c);
      break; 

      case 5:
       strip.setPixelColor(29, c);
      break; 

      case 6:
       strip.setPixelColor(22, c);
      break; 
    }
  }

   void setTriB(int i, uint32_t c) {

    switch(i){
      case 1:
       strip.setPixelColor(23, c);
      break; 

      case 2:
       strip.setPixelColor(26, c);
      break; 

      case 3:
       strip.setPixelColor(28, c);
      break; 

      case 4:
       strip.setPixelColor(30, c);
      break; 

      case 5:
       strip.setPixelColor(27, c);
      break; 

      case 6:
       strip.setPixelColor(25, c);
      break; 
    }

  }
