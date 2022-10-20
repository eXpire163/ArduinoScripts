#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 40

#define DATA_PIN 4

uint8_t patternCounter = 0;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup()
{
  delay(500);
  // Uncomment/edit one of the following lines for your leds arrangement.
  // ## Clockless types ##
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed
  FastLED.setBrightness(20);
}

void loop()
{

  switch (patternCounter)
  {
  case 0:
    movingDots();
    break;
  case 1:
    rainbowBeat();
    break;
  case 2:
    redWhiteBlue();
    break;
  }

  // Turn the LED on, then pause
  //EVERY_N_MILLISECONDS(200){
  //  leds[random8(0,NUM_LEDS -1)] = CRGB::Red;
  //}
  EVERY_N_SECONDS(5)
  {
    nextPattern();
  }

  //fadeToBlackBy(leds, NUM_LEDS, 1);
  FastLED.show();
}

void nextPattern()
{
  patternCounter = (patternCounter + 1) % 3;
}

//------- Put your patterns below -------//

void movingDots()
{

  uint16_t posBeat = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t posBeat2 = beatsin16(60, 0, NUM_LEDS - 1, 0, 0);

  uint16_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 32767);
  uint16_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 32767);

  // Wave for LED color
  uint8_t colBeat = beatsin8(45, 0, 255, 0, 0);

  leds[(posBeat + posBeat2) / 2] = CHSV(colBeat, 255, 255);
  leds[(posBeat3 + posBeat4) / 2] = CHSV(colBeat, 255, 255);

  fadeToBlackBy(leds, NUM_LEDS, 10);
}

void rainbowBeat()
{

  uint16_t beatA = beatsin16(30, 0, 255);
  uint16_t beatB = beatsin16(20, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA + beatB) / 2, 8);
}

void redWhiteBlue()
{

  uint16_t sinBeat = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t sinBeat2 = beatsin16(30, 0, NUM_LEDS - 1, 0, 21845);
  uint16_t sinBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 43690);

  leds[sinBeat] = CRGB::Blue;
  leds[sinBeat2] = CRGB::Red;
  leds[sinBeat3] = CRGB::BlueViolet;

  fadeToBlackBy(leds, NUM_LEDS, 10);
}
