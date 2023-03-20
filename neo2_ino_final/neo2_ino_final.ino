// about: play with neo led lib final
#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, 1, NEO_GRB + NEO_KHZ800);
String inString;
void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}

void loop()
{
  // Some example procedures showing how to display to the pixels:
  int kctime = 100;
  int inChar;

  // Read serial input:
  if (Serial.available() > 0)
  {
    inChar = Serial.read();
  }

  if (inChar != '.' && inString.length() < 12)
  {
    inString += (char)inChar;
  }

  // if you get a comma, convert to a number,
  // set the appropriate color, and increment
  // the color counter:

  else
  {

    // do something different for each value of currentColor:
    // Serial.write("boooooom\n");

    int p = inString.substring(1, 3).toInt();
    int r = inString.substring(4, 6).toInt();
    int g = inString.substring(7, 9).toInt();
    int b = inString.substring(10, 12).toInt();

    setPixel(p, strip.Color(r, g, b));
    inString = "";
  }
}

void setPixel(int number, uint32_t c)
{
  if (number < strip.numPixels())
  {
    strip.setPixelColor(number, c);
    strip.show();
  }
}
