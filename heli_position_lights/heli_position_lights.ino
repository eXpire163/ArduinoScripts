// Simple demonstration on using an input device to trigger changes on your
// NeoPixels. Wire a momentary push button to connect from ground to a
// digital IO pin. When the button is pressed it will change to a new pixel
// animation. Initial state has all pixels off -- press the button once to
// start the first animation. As written, the button does not interrupt an
// animation in-progress, it works only when idle.


#include <Adafruit_NeoPixel.h>
// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 2;


// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.


#define PIXEL_PIN 4     //13 on white ghost Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 16  // Number of NeoPixels
#define DELAYVAL 30
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


void setup() {


  delay(1000);
  strip.begin();  // Initialize NeoPixel strip object (REQUIRED)
  strip.show();   // Initialize all pixels to 'off'
}

void loop() {


  devel();
}



void devel() {

  strip.clear();  // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for (int thisPos = 0; thisPos < PIXEL_COUNT; thisPos++) {  // For each pixel...


    if (thisPos < PIXEL_COUNT / 2) {

      strip.setPixelColor(thisPos, strip.Color(0, 230, 0));
    } else {

      strip.setPixelColor(thisPos, strip.Color(230, 0, 0));
    }
  }

  strip.show();  // Send the updated pixel colors to the hardware.
  delay(2000);  
  flash();

}


void flash() {
  uint8_t power = 230;
  strip.setPixelColor(7, strip.Color(power, power, power));
  strip.setPixelColor(8, strip.Color(power, power, power));
  strip.show();
  delay(30);

  //strip.clear();
}


