// about: room lights
#include <Adafruit_NeoPixel.h>
#include <IRremote.h>
#include <EEPROM.h>

int numPixel = 60; // 60;
int brightness = 255;

int ticker = 1;
bool isOn = true;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixel, 2, NEO_GRB + NEO_KHZ800);
const int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;

byte state;
void setup()
{
  Serial.begin(9600);
  // irrecv.enableIRIn(); // Start the receiver
  // irrecv.blink13(true);

  strip.begin();
  // strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'

  state = EEPROM.read(1);
  if (state == 0)
  {
    EEPROM.write(1, 1);
  }
  else
  {
    EEPROM.write(1, 0);
  }

  delay(100);
}

void loop()
{

  if (state == 0)
  {
    startWhite(3);
  }
  else
  {
    rainbow(555);
  }
}

int start1 = 0;
int start2 = 0;
int start3 = 0;

void startWhite(int startspeed)
{

  while (start3 < 255)
  {

    for (int i = 0; i <= numPixel; i++)
    {
      setPixel(i, strip.Color(start1, start2, start3));
    }
    delay(startspeed);
    start3++;
    strip.show();
  }

  while (start2 < 255)
  {
    for (int i = 0; i <= numPixel; i++)
    {
      setPixel(i, strip.Color(start1, start2, start3));
    }
    delay(startspeed);
    start2++;
    strip.show();
  }

  while (start1 < 255)
  {
    for (int i = 0; i <= numPixel; i++)
    {
      setPixel(i, strip.Color(start1, start2, start3));
    }
    delay(startspeed);
    start1++;
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

void setPixel(int number, uint32_t c)
{
  if (number < strip.numPixels())
  {
    strip.setPixelColor(number, c);
  }
}

void loop2()
{
  //  Serial.println("in");
  // waitForGap(1000);
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    if (results.value == 0xFFFFFFFF)
    {
    }
    else if (results.value == 0x20DF10EF)
    {
      toggleOnOff();
    }

    else if (results.value == 0x20DF40BF)
    {
      heller();
      Serial.println("heller");
    }
    else if (results.value == 0x20DFC03F)
    {
      dunkler();
      Serial.println("dunkler");
    }
    else
    {
      Serial.println(results.value, HEX);
    }

    irrecv.resume(); // Receive the next value
  }                  // end recive data
  else
  {
    // updateStrip();
  }
  // delay(100);
  // Serial.println("tick");
}

// Befehle
void heller()
{
  if (brightness <= 200)
  {
    brightness = brightness + 50;
    strip.setBrightness(brightness);
    Serial.println(brightness);
  }
}

void dunkler()
{
  if (brightness >= 50)
  {
    brightness = brightness - 50;
    strip.setBrightness(brightness);
    Serial.println(brightness);
  }
}

void toggleOnOff()
{

  Serial.println("On/Off");
  if (isOn)
  {
    for (int i = 0; i < numPixel; i++)
    {
      setPixel(i, strip.Color(0, 0, 0));
    }
  }
  else
  {
    int brightness = 255;
    for (int i = 0; i < numPixel; i++)
    {
      setPixel(i, strip.Color(brightness, brightness, brightness));
    }

  } // end else
  strip.show();
  isOn = !isOn;
}

// IR HELPER

// Wait for the gap between tests, to synchronize with
// the sender.
// Specifically, wait for a signal followed by a gap of at last gap ms.
void waitForGap(int gap)
{
  Serial.println("Waiting for gap");
  while (1)
  {
    while (digitalRead(RECV_PIN) == LOW)
    {
    }
    unsigned long time = millis();
    while (digitalRead(RECV_PIN) == HIGH)
    {
      if (millis() - time > gap)
      {
        return;
      }
    }
  }
}
