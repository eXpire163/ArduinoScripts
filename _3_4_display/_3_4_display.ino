// about: Display something on a selfmade (snakestyle) 3x4 matrix
#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 1, NEO_GRB + NEO_KHZ800);

int speed = 250;

void setup()
{
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
  // Some example procedures showing how to display to the pixels:

  uint32_t c = strip.Color(55, 0, 0);
  uint32_t c2 = strip.Color(0, 0, 0);
  showRows(speed, c, c2);
  showLines(speed, c, c2);
  showRowsRew(speed, c, c2);
  showLinesRew(speed, c, c2);
}

void setPixel(int number, uint32_t c)
{
  if (number < strip.numPixels())
  {
    strip.setPixelColor(number, c);
  }
}

void show(boolean wait)
{

  if (!wait)
  {
    boolean next = false;
    while (!next)
    {

      if (Serial.available() > 0)
      {
        // read incoming serial data:
        char inChar = Serial.read();
        // Type the next ASCII value from what you received:
        next = true;
      }
    }
  }
  strip.show();
}

void showRows(int speed, uint32_t c, uint32_t backColor)
{
  for (int i = 0; i < 3; i++)
  {
    setRow(i, c, true);

    delay(speed);
    setRow(i, backColor, false);
  }
}
void showRowsRew(int speed, uint32_t c, uint32_t backColor)
{
  for (int i = 2; i >= 0; i--)
  {
    setRow(i, c, true);

    delay(speed);
    setRow(i, backColor, false);
  }
}

void showLines(int speed, uint32_t c, uint32_t backColor)
{
  for (int i = 0; i < 3; i++)
  {
    setLine(i, c, true);

    //  delay(speed);
    setLine(i, backColor, false);
  }
}
void showLinesRew(int speed, uint32_t c, uint32_t backColor)
{
  for (int i = 2; i >= 0; i--)
  {
    setLine(i, c, true);

    //  delay(speed);
    setLine(i, backColor, false);
  }
}

void setLine(int linenum, uint32_t c, boolean wait)
{

  setPixel(0 + (3 * linenum), c);
  setPixel(1 + (3 * linenum), c);
  setPixel(2 + (3 * linenum), c);
  show(wait);
}
void setRow(int linenum, uint32_t c, boolean wait)
{

  setPixel(0 + (linenum), c);
  setPixel(5 - (linenum), c);
  setPixel(6 + (linenum), c);
  show(wait);
}
