#define FASTLED_ESP8266_RAW_PIN_ORDER
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FastLED.h>
#include <EEPROM.h>

#define LED_PIN     D8
#define NUM_LEDS    64
#define LEAF_COUNT  9
#define LED_PER_LEAF  6
#define BRIGHTNESS  100
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 50

char auth[] = "sFNM3CARqImQ-91cprti9p0o5C2qsEHz";
char ssid[] = "Link XXX3";
char pass[] = "whatalovelyday";


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
    delay( 3000 ); // power-up safety delay
    EEPROM.begin(512);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    Serial.begin(9600);
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    Blynk.begin(auth, ssid, pass);
}

int operationMode = 1;
int selectedLeaf = 1;
int eeAddress = 0;
BLYNK_WRITE(V1)
{
  operationMode = param.asInt();
}
BLYNK_WRITE(V5)
{
  operationMode = 1;
  ChangePalettePeriodically(param.asInt());
}
BLYNK_WRITE(V3)
{
  operationMode = 3;
  if(param.asInt()==10)
  {
    for( int i = 0; i < LEAF_COUNT; i++) {
      int c_red = leds[i*LED_PER_LEAF].red;
      int c_green = leds[i*LED_PER_LEAF].green;
      int c_blue = leds[i*LED_PER_LEAF].blue;

      EEPROM.put(eeAddress++, c_red);
      EEPROM.put(eeAddress++, c_green);
      EEPROM.put(eeAddress++, c_blue);
    }
    EEPROM.commit();
  }
  if(param.asInt()==11)
  {
    eeAddress = 0;
    for( int i = 0; i < LEAF_COUNT; i++) {
      
      int R = 0; EEPROM.get(eeAddress++, R);
      int G = 0; EEPROM.get(eeAddress++, G);
      int B = 0; EEPROM.get(eeAddress++, B);
      
      for( int j = i*LED_PER_LEAF; j < LED_PER_LEAF*(i+1); j++) {
        leds[j].setRGB( R, G, B);
      }
      FastLED.show();
    }
  }
  if(param.asInt()>=1 && param.asInt()<=9)
  {
    selectedLeaf = param.asInt();
  }
}
BLYNK_WRITE(V2)
{
  int R = param[0].asInt();
  int G = param[1].asInt();
  int B = param[2].asInt();
//  Serial.println(R);
//  Serial.println(G);
//  Serial.println(B);
  if(operationMode == 2){
    for(int i=0;i<NUM_LEDS;i++){
      leds[i].setRGB( R, G, B);
    }
    FastLED.show();
  }
  if(operationMode == 3){
    for(int i=(selectedLeaf-1)*LED_PER_LEAF; i<(selectedLeaf-1)*LED_PER_LEAF+LED_PER_LEAF; i++){
      leds[i].setRGB( R, G, B);
    }
    FastLED.show();
  }
}

void loop()
{
    Blynk.run();

    if(operationMode == 1){
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1; /* motion speed */
      FillLEDsFromPaletteColors( startIndex);
      
      FastLED.show();
      FastLED.delay(1000 / UPDATES_PER_SECOND);
    }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < LEAF_COUNT; i++) {
      for( int j = i*LED_PER_LEAF; j < LED_PER_LEAF*(i+1); j++) {
        leds[j] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      }
      colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically(int design)
{
        if( design ==  1)  { currentPalette = RainbowColors_p;          currentBlending = LINEARBLEND; }
        if( design == 2)  { currentPalette = RainbowStripeColors_p;     currentBlending = NOBLEND;  }
        if( design == 3)  { currentPalette = RainbowStripeColors_p;     currentBlending = LINEARBLEND; }
        if( design == 4)  { SetupPurpleAndGreenPalette();               currentBlending = LINEARBLEND; }
        if( design == 5)  { SetupTotallyRandomPalette();                currentBlending = LINEARBLEND; }
        if( design == 6)  { SetupBlackAndWhiteStripedPalette();         currentBlending = NOBLEND; }
        if( design == 7)  { SetupBlackAndWhiteStripedPalette();         currentBlending = LINEARBLEND; }
        if( design == 8)  { currentPalette = CloudColors_p;             currentBlending = LINEARBLEND; }
        if( design == 9)  { currentPalette = PartyColors_p;             currentBlending = LINEARBLEND; }
        if( design == 10)  { currentPalette = myRedWhiteBluePalette_p;  currentBlending = NOBLEND;  }
        if( design == 11)  { currentPalette = myRedWhiteBluePalette_p;  currentBlending = LINEARBLEND; }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
