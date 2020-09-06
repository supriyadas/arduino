#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 250
#define DATA_PIN 3
#define FORWARD 0
#define BACKWARD 1
#define SLOW 250
#define MEDIUM 50
#define FAST 5

CRGB leds[NUM_LEDS];

boolean direction = FORWARD;

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop() {
  static uint8_t hue = 0;
  CRGB colorx = CHSV( 100,255, 255);
  leds[0] = colorx;
  leds[1] = colorx;
  leds[2] = colorx;
  leds[3] = colorx;
  leds[4] = colorx;
  leds[5] = colorx;

colorx = CHSV( 80,255, 255);
  leds[6] = colorx;
  leds[7] = colorx;
  leds[8] = colorx;
  leds[9] = colorx;
  leds[10] = colorx;
  leds[11] = colorx;

  colorx = CHSV( 140,255, 255);
  leds[12] = colorx;
  leds[13] = colorx;
  leds[14] = colorx;
  leds[15] = colorx;
  leds[16] = colorx;
  leds[17] = colorx;

  colorx = CHSV( 80,255, 255);
  leds[18] = colorx;
  leds[19] = colorx;
  leds[20] = colorx;
  leds[21] = colorx;
  leds[22] = colorx;
  leds[23] = colorx;

    colorx = CHSV( 140,255, 255);
  leds[24] = colorx;
  leds[25] = colorx;
  leds[26] = colorx;
  leds[27] = colorx;
  leds[28] = colorx;
  leds[29] = colorx;

   colorx = CHSV( 100,255, 255);
  leds[30] = colorx;
  leds[31] = colorx;
  leds[32] = colorx;
  leds[33] = colorx;
  leds[34] = colorx;
  leds[35] = colorx;

  colorx = CHSV( 80,255, 255);
  leds[36] = colorx;
  leds[37] = colorx;
  leds[38] = colorx;
  leds[39] = colorx;
  leds[40] = colorx;
  leds[41] = colorx;

  colorx = CHSV( 140,255, 255);
  leds[42] = colorx;
  leds[43] = colorx;
  leds[44] = colorx;
  leds[45] = colorx;
  leds[46] = colorx;
  leds[47] = colorx;

  colorx = CHSV( 80,255, 255);
  leds[48] = colorx;
  leds[49] = colorx;
  leds[50] = colorx;
  leds[51] = colorx;
  leds[52] = colorx;
  leds[53] = colorx;
  
  FastLED.show();
  delay(100);
}
