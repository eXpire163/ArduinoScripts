// about: PPM to light
/**
 * @brief rc controlled leds for rc planes with ppm input
 *
 * Test code for reading a PPM receiver
 *
 * \note The Teensy 3.1 supports 5V input signals and can be plugged
 *       directly into a receiver.
 *
 * thanks to https://github.com/Poofjunior/teensyReadRC_Receiver
 */
#include <Adafruit_NeoPixel.h>

/**
 * \brief parameters
 */
#define IDLE_TIME_THRESHOLD 4000
#define ULONG_MAX 0xffffffff

/**
 * pins correspond to the digital IO pins. Change these values if your
 * setup is different.
 */
#define NUM_CHANNELS 8
#define PPM_CHANNEL 6

/**
 * setup leds
 */
#define TOTAL_LEDS 11

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN 4

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LEDS, PIN, NEO_GRB + NEO_KHZ800);

/**
 * setup modes
 *
 */
#define totalModes 2
#define rayStepSize 4

/**
 * \brief globals relevant to capturing the channel times
 * \note values are modified withing an ISR and must be volatile.
 */
struct rxVals
{
    unsigned long channelTimes[NUM_CHANNELS];
    unsigned long startTime;
    unsigned long finTime;
    unsigned long diffTime;
    unsigned int channelIndex;
} volatile PPM_RX_Vals;

/// Prototypes:
void initPPM_RX();
void startInitialTimer();
void startTimer();
void stopTimer();
void prettyPrintAll();
void prettyPrintChannel(unsigned int channel);
void setup();
void loop();

int current_step = 0;

int max_color = 150;

uint32_t off = strip.Color(0, 0, 20);
uint32_t white = strip.Color(max_color, max_color, max_color);
uint32_t blue = strip.Color(0, 0, max_color);
uint32_t cyan = strip.Color(0, max_color, max_color);
uint32_t red = strip.Color(max_color, 0, 0);
uint32_t green = strip.Color(0, max_color, 0);
uint32_t orange = strip.Color(max_color, max_color / 2, 0);

int speed = 100;
int currentMode = 0;
uint32_t[] afterBurnerColors;

void setup()
{
    Serial.begin(115200); // baud rate is irrelevant for Teensy
    delay(2000);

    initPPM_RX();
    initLED();
}

/**
 * \brief PrettyPrints the raw channel microsecond count
 */
void loop()
{
    updateMode();

    prettyPrintAll();

    updateLEDs();

    delay(speed);
}

/**
 * basic led loop
 */
void updateLEDs()
{
    currentMode = 0;
    switch (currentMode)
    {
    case 1:
        updateRay();
        speed = map(PPM_RX_Vals.channelTimes[0], 1000, 2000, 500, 50);
        break;

    default:
        updateBase();
        speed = 100;
        break;
    }

    strip.show();
    current_step++;
}

/*********************
 *
 * MODES
 *
 **************/

void updateBase()
{

    int left1 = 0;
    int left2 = 1;
    int right1 = TOTAL_LEDS - 1;
    int right2 = TOTAL_LEDS - 2;
    int mid = TOTAL_LEDS / 2;

    for (unsigned int i = 2; i < TOTAL_LEDS - 2; i++)
    {
        strip.setPixelColor(i, white);
    }

    strip.setPixelColor(left1, green);
    strip.setPixelColor(left2, green);
    strip.setPixelColor(right1, red);
    strip.setPixelColor(right2, red);

    if (current_step % 10 == 0)
    {
        strip.setPixelColor(mid, cyan);
        current_step = 0;
    }
    else
    {
        strip.setPixelColor(mid, off);
    }
}

void updateRay()
{

    for (unsigned int i = 0; i < TOTAL_LEDS; i++)
    {
        if (i % (rayStepSize - 1) == 0 + current_step)
        {
            strip.setPixelColor(i, red);
        }
        else
        {
            strip.setPixelColor(i, cyan);
        }
    }
    if (current_step >= rayStepSize)
    {
        current_step = -1;
    }
}

void updateAfterburner()
{
    int speed = PPM_RX_Vals.channelTimes[0];
    int ledsToShow = map(speed, 1000, 2000, 0, totalLEDs) + random(-2, 2);

    for (int i = 0; i < totalLEDs; i++)
    {
        if (i < ledsToShow)
        {
            strip.setPixelColor(i, afterBurnerColors[i]);
        }
        else
        {
            strip.setPixelColor(i, off);
        }
    }
}

void updateMode()
{

    currentMode = map(PPM_RX_Vals.channelTimes[4], 1000, 2000, 0, totalModes);
}

/**
 *  init leds
 */
void initLED()
{

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

    // init afterBurnerColors
    afterBurnerColors = new uint32_t[totalLEDs];

    for (int i = 0; i < totalLEDs; i++)
    {
        afterBurnerColors[i] = Wheel(map(i, 0, totalLEDs, 0, 244))
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    if (WheelPos < 122)
    {
        // orange 255, 215, 50
        // red 255,0,0
        // blue 51, 92, 255
        int r = map(WheelPos, 0, 120, 255, 255);
        int g = map(WheelPos, 0, 120, 215, 0);
        int b = map(WheelPos, 0, 120, 50, 0);
        return strip.Color(r, g, b);
    }
    else
    {
        WheelPos -= 122;

        int r = map(WheelPos, 0, 120, 255, 51);
        int g = map(WheelPos, 0, 120, 0, 92);
        int b = map(WheelPos, 0, 120, 0, 255);
        return strip.Color(r, g, b);
    }
}

/**
 * \brief initialize PPM receiver-related variables and enter state machine.
 */
void initPPM_RX()
{
    pinMode(PPM_CHANNEL, INPUT);
    // Reset all channel values
    for (uint8_t channelIter = 0; channelIter < NUM_CHANNELS; ++channelIter)
    {
        PPM_RX_Vals.channelTimes[channelIter] = 0;
    }

    // Init channel index outside of range such that garbage data isn't
    // detected until long delay is detected.
    PPM_RX_Vals.channelIndex = NUM_CHANNELS;
    // initialize startTime to 'reset' val
    PPM_RX_Vals.startTime = micros();

    detachInterrupt(PPM_CHANNEL);
    attachInterrupt(PPM_CHANNEL, stopTimer, FALLING);
}

/**
 * \brief callback fn for initializing FSM to a known start state by
 *        waiting for the start of the next packet.
 */
void startInitialTimer()
{
    PPM_RX_Vals.startTime = micros();
}

/**
 * \brief callback fn for evaluating receiver. Starts the timer and
 *        attaches a falling-edge interrupt to the channel that
 *        triggered this interrupt.
 */
void startTimer()
{
    detachInterrupt(PPM_CHANNEL);
    PPM_RX_Vals.startTime = micros();
    attachInterrupt(PPM_CHANNEL, stopTimer, FALLING);
}

/**
 * \brief callback fn that logs the microsecond count of a channel and
 *        reattaches the rising-edge interrupt for that channel.
 */
void stopTimer()
{
    detachInterrupt(PPM_CHANNEL);
    PPM_RX_Vals.finTime = micros();
    PPM_RX_Vals.diffTime = PPM_RX_Vals.finTime - PPM_RX_Vals.startTime;
    // Check for (and handle) overflow in unsigned values.
    if (PPM_RX_Vals.finTime < PPM_RX_Vals.startTime)
        PPM_RX_Vals.diffTime = (ULONG_MAX - PPM_RX_Vals.diffTime) + 1;

    // Write to the channel
    if (PPM_RX_Vals.channelIndex < NUM_CHANNELS)
    {
        PPM_RX_Vals.channelTimes[PPM_RX_Vals.channelIndex] =
            PPM_RX_Vals.diffTime;
    }
    attachInterrupt(PPM_CHANNEL, startTimer, RISING);

    // iterate to next channel and stay within bounds.
    ++PPM_RX_Vals.channelIndex;

    // Reset to channel if long delay detected in signal.
    if (PPM_RX_Vals.diffTime > IDLE_TIME_THRESHOLD)
        PPM_RX_Vals.channelIndex = 0;
}

void prettyPrintAll()
{
    Serial.println("PPM Values");
    for (int i = 0; i < NUM_CHANNELS; ++i)
    {
        Serial.print("Ch");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(PPM_RX_Vals.channelTimes[i]);
        prettyPrintChannel(i);
        Serial.println();
    }
    Serial.println(digitalRead(6));
}

/*
 * \brief prints bars proportional to the raw microsecond count of
 *        a channel.
 */
void prettyPrintChannel(unsigned int myChannel)
{
    unsigned int numEquals;
    if (PPM_RX_Vals.channelTimes[myChannel] < 3000)
        numEquals = PPM_RX_Vals.channelTimes[myChannel] / 100;
    else
    {
        Serial.print("x");
        return;
    }
    for (unsigned int i = 0; i < numEquals; ++i)
        Serial.print("=");
}
