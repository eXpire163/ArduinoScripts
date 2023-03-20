// about: PPM to light 2
#include <Adafruit_NeoPixel.h>

/*
 * Test code for reading a 6-channel rc receiver
 *
 * \note The Teensy 3.1 supports 5V input signals and can be plugged
 *       directly into a receiver.
 */

/**
 * pins correspond to the digital IO pins. Change these values if your
 * setup is different.
 */
int totalLEDs = 14;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(totalLEDs, 2, NEO_GRB + NEO_KHZ800);

#define NUM_CHANNELS 6
#define CH0 3
#define CH1 4
#define CH2 5
#define CH3 6
#define CH4 7
#define CH5 8

/**
 * global array of channels for iterating much more conveniently.
 * Do not change.
 */
const uint8_t rcChannels[] = {CH0, CH1, CH2, CH3, CH4, CH5};

/**
 * \brief globals relevant to capturing the channel times
 * \note values are modified withing an ISR and must be volatile.
 */
struct rxVals
{
  unsigned long channelTimes[5];

  unsigned long startTime;
  unsigned int currChannel;
  unsigned int channelIndex;
} volatile myRX_Vals;

/// Prototypes:
// void startTime();
// void stopTime();
// void prettyPrintChannel(unsigned int channel);
// void setup();
// void loop();

void setup()
{
  setupIO();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(115200); // baud rate is irrelevant for Teensy

  for (unsigned int i = 0; i < NUM_CHANNELS; ++i)
    attachInterrupt(rcChannels[i], startTime, RISING);

  Serial.println(" PPM Receiver test");
}

/**
 * \brief prints the raw channel data microsecond count
 */
void loop()
{

  // switch ch2

  long rcswitch = myRX_Vals.channelTimes[2];
  int kctime = 10;

  if (rcswitch < 1500)
  {

    toSides(strip.Color(0, 255, 0), strip.Color(255, 0, 0), 100);
  }
  else
  {
    colorWipe(strip.Color(0, 255, 0), kctime); // green
    colorWipe(strip.Color(0, 0, 0), kctime);   // off
  }

  delay(50);

  Serial.println("PPM Values");
  for (int i = 0; i < NUM_CHANNELS; ++i)
  {
    Serial.print("Ch");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(myRX_Vals.channelTimes[i]);
    prettyPrintChannel(i);
    Serial.println();
  }
  Serial.println();
}
void setupIO()
{
  pinMode(CH0, INPUT);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
}

/**
 * \brief callback fn for evaluating receiver. Starts the timer and
 *        attaches a falling-edge interrupt to the channel that
 *        triggered this interrupt.
 */
void startTime()
{
  myRX_Vals.startTime = micros();
  /// Find out which pin triggered the interrupt and set it to trigger
  /// on the next falling edge
  for (unsigned int i = 0; i < NUM_CHANNELS; ++i)
  {
    boolean val = digitalRead(rcChannels[i]);
    if (val)
    {
      attachInterrupt(rcChannels[i], stopTime, FALLING);
      myRX_Vals.currChannel = rcChannels[i];
      myRX_Vals.channelIndex = i;
      break;
    }
    /// else.. this shouldn't happen.
  }
}

/**
 * \brief callback fn that logs the microsecond count of a channel and
 *        reattaches the rising-edge interrupt for that channel.
 */
void stopTime()
{
  myRX_Vals.channelTimes[myRX_Vals.channelIndex] = micros() -
                                                   myRX_Vals.startTime;
  attachInterrupt(myRX_Vals.currChannel, startTime, RISING);
}

/*
 * \brief prints bars proportional to the raw microsecond count of
 *        a channel.
 */
void prettyPrintChannel(unsigned int myChannel)
{
  unsigned int numEquals;
  if (myRX_Vals.channelTimes[myChannel] < 3000)
    numEquals = myRX_Vals.channelTimes[myChannel] / 100;
  else
  {
    Serial.print("x");
    return;
  }
  for (unsigned int i = 0; i < numEquals; ++i)
    Serial.print("=");
}

// LED
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

void toSides(uint32_t left, uint32_t right, uint8_t wait)
{

  // totalLEDs

  uint16_t mid = strip.numPixels() / 2;
  for (uint16_t i = mid; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, left);
    strip.setPixelColor(mid - 1 - (i - mid), right);
    strip.show();
    delay(wait);
  }

  for (uint16_t i = mid; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 255));
    strip.setPixelColor(mid - 1 - (i - mid), strip.Color(0, 0, 255));
    strip.show();
    delay(wait);
  }
}
