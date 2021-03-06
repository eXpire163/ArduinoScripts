/*
 * Test code for reading a PPM receiver
 *
 * \note The Teensy 3.1 supports 5V input signals and can be plugged
 *       directly into a receiver. 
 */


/**
 * \brief parameters
 */
#define IDLE_TIME_THRESHOLD  4000
#define ULONG_MAX 0xffffffff


/** 
 * pins correspond to the digital IO pins. Change these values if your
 * setup is different.
 */
#define NUM_CHANNELS 6
#define PPM_CHANNEL 2

/**
 * \brief globals relevant to capturing the channel times
 * \note values are modified withing an ISR and must be volatile.
 */
struct rxVals{
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


void setup()
{
    Serial.begin(115200);   // baud rate is irrelevant for Teensy
    delay(2000);

    initPPM_RX();
}

/**
 * \brief PrettyPrints the raw channel microsecond count
 */
void loop()
{
    prettyPrintAll();
    delay(50);
    //Serial.println(digitalRead(PPM_CHANNEL));
}

/**
 * \brief initialize PPM receiver-related variables and enter state machine.
 */
void initPPM_RX()
{
  Serial.println("init");
    pinMode(PPM_CHANNEL, INPUT_PULLUP);
    // Reset all channel values
    for (uint8_t channelIter= 0; channelIter < NUM_CHANNELS; ++channelIter)
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

    Serial.println("initdone");
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

  Serial.println("start");
    detachInterrupt(PPM_CHANNEL);
    PPM_RX_Vals.startTime = micros();

    Serial.println(PPM_RX_Vals.startTime);
    attachInterrupt(PPM_CHANNEL, stopTimer, FALLING);
    
}


/**
 * \brief callback fn that logs the microsecond count of a channel and
 *        reattaches the rising-edge interrupt for that channel.
 */
void stopTimer()
{
  Serial.println("stop");
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
    Serial.println();

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
