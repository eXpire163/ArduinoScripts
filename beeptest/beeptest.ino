// about: test audio stuff
/* Blink without Delay

 Turns on and off a light emitting diode(LED) connected to a digital
 pin, without using the delay() function.  This means that other code
 can run at the same time without being interrupted by the LED code.

 The circuit:
 * LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 that's attached to pin 13, so no hardware is needed for this example.


 created 2005
 by David A. Mellis
 modified 8 Feb 2010
 by Paul Stoffregen

 This example code is in the public domain.


 http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 */

// constants won't change. Used here to set pin numbers:
// Pin 13: Arduino has an LED connected on pin 13
// Pin 11: Teensy 2.0 has the LED on pin 11
// Pin  6: Teensy++ 2.0 has the LED on pin 6
// Pin 13: Teensy 3.0 has the LED on pin 13
const int ledPin = 1; // the number of the LED pin

// Variables will change:
int ledState = LOW;      // ledState used to set the LED
long previousMillis = 0; // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000; // interval at which to blink (milliseconds)

void setup()
{
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  float ff = 150.0;
  while (true)
  {
    digitalWrite(ledPin, HIGH);
    delay(int(1000.0 / ff));
    digitalWrite(ledPin, LOW);
    delay(int(1000.0 / ff));
  }
}
