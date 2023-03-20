// about: clock
#include <Adafruit_NeoPixel.h>
#include <Time.h>
#include <EEPROM.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

int numPixel = 23;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixel, 2, NEO_GRB + NEO_KHZ800);

int speed = 50;
time_t time;

uint32_t h = strip.Color(255, 0, 0);
uint32_t m = strip.Color(0, 255, 0);
uint32_t s = strip.Color(0, 0, 255);

void setup()
{

  setTime(3, 6, 9, 1, 1, 1);

  Serial.begin(9600);
  strip.begin();
  // strip.setBrightness(64);
  strip.show(); // Initialize all pixels to 'off'
  time = now();
}

void loop()
{
  // Some example procedures showing how to display to the pixels:
  for (int i = 0; i < numPixel; i++)
  {
    setPixel(i, strip.Color(0, 0, 0));
  }
  // if(EEPROM.read(0)==123){
  //  s = strip.Color(255,255,255);

  // }
  // if(millis()>30000){
  //      s = strip.Color(0,0,0);
  // EEPROM.write(0,123);

  //   }

  setPixel(hour() % numPixel, h);
  setPixel(minute() % numPixel, m);
  setPixel(second() % numPixel, s);

  strip.show();
  delay(1000);
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
