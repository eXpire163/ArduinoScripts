// about: play with neo led lib
#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(28, 2, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
  // Some example procedures showing how to display to the pixels:
  int kctime = 10;
  colorWipe(strip.Color(0, 0, 0), kctime);   // off
  colorWipe(strip.Color(255, 0, 0), kctime); // Red
                                             // colorWipe(strip.Color(0, 0, 0), kctime); // off
  // colorWipe(strip.Color(0, 255, 0), kctime); // Green
  // colorWipe(strip.Color(0, 0, 0), kctime); // off
  // colorWipe(strip.Color(0, 0, 255), kctime); // Blue
  // colorWipe(strip.Color(0, 0, 0), kctime); // off
  // rainbowCycle(kctime);
  // rainbowCycle(kctime);
  // rainbow(kctime);
  // rainbow(kctime);
  // rainbow(kctime);
  // colorWipe(strip.Color(0, 0, 0), kctime); // off

  // rainbowCycle(kctime);
  //  colorWipe(strip.Color(0, 0, 0), kctime); // off
  // firePixel(strip.Color(255, 0, 0),strip.Color(0, 0, 0),kctime);
  //  firePixel(strip.Color(0, 0, 255),strip.Color(0, 0, 0),kctime);
  //  firePixel(strip.Color(255, 255, 255),strip.Color(0, 0, 0),kctime);
  //   firePixel(strip.Color(255, 0, 0),strip.Color(0, 0, 0),kctime);
  //  firePixel(strip.Color(0, 0, 255),strip.Color(0, 0, 0),kctime);
  //  firePixel(strip.Color(255, 255, 255),strip.Color(0, 0, 0),kctime);
  //   firePixel(strip.Color(255, 0, 0),strip.Color(0, 0, 0),kctime);
  //  firePixel(strip.Color(0, 0, 255),strip.Color(0, 0, 0),kctime);
  //  firePixel(strip.Color(255, 255, 255),strip.Color(0, 0, 0),kctime);
  // setPixel(20,strip.Color(0,0,55));
}

void setPixel(int number, uint32_t c)
{
  if (number < strip.numPixels())
  {
    strip.setPixelColor(number, c);
    strip.show();
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// pixel color to shoot out, backcolor, wait
void firePixel(uint32_t c, uint32_t b, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    strip.setPixelColor(i, b);
    strip.show();
  }
}

void rainbow(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    if (wait != -1)
    {
      // delay(wait);
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
