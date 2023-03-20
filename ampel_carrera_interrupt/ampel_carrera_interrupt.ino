// about: same with added lightgates for start check and lapcount
#include <Adafruit_NeoPixel.h>

int numPixel = 12; // 60;

int ticker = 1;
bool isOn = true;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixel, 1, NEO_GRB + NEO_KHZ800);

const int startButton = 4;
const int sensorLane1 = 5;
const int sensorLane2 = 6;

const int ledPin = 13;

uint32_t rot;
uint32_t gruen;
uint32_t schwarz;
uint32_t weis;
uint32_t blau;
uint32_t turkies;

int totalRounds = 10;

bool wasPressed;

bool countdownRunning;
bool fehlstartLane1;
bool fehlstartLane2;

bool readyLane1;
bool readyLane2;

int countLane1;
int countLane2;

int lastRound1;
int bestRound1;
int lastRound2;
int bestRound2;

elapsedMillis roundTimeLane1;
elapsedMillis roundTimeLane2;

void setup()
{
  wasPressed = false;
  Serial.begin(9600); // USB is always 12 Mbit/sec
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  rot = strip.Color(255, 0, 0);
  gruen = strip.Color(0, 255, 0);
  schwarz = strip.Color(0, 0, 0);
  weis = strip.Color(255, 255, 255);
  blau = strip.Color(0, 0, 255);
  turkies = strip.Color(0, 255, 255);

  strip.setBrightness(110);
  pinMode(startButton, INPUT_PULLUP);

  pinMode(sensorLane1, INPUT_PULLUP);
  pinMode(sensorLane2, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT);

  attachInterrupt(startButton, setStart, FALLING); // interrrupt 1 is data ready

  setStartVars();
}

void setStartVars()
{
  Serial.println("setStartVars");
  countdownRunning = false;

  fehlstartLane1 = false;
  fehlstartLane2 = false;

  readyLane1 = false;
  readyLane2 = false;

  countLane1 = 0;
  countLane2 = 0;

  lastRound1 = -1;
  bestRound1 = -1;
  lastRound2 = -1;
  bestRound2 = -1;
}

bool newRun = false;
void loop()
{
  // buttonState = digitalRead(startButton);
  // Serial.println(analogRead(startButton));
  if (wasPressed)
  {
    setStartVars();

    countdown();

    if (fehlstartLane1 || fehlstartLane2)
    {
      showFehlstart();
    }
    else
    {
      startRace();
    }
    wasPressed = false;
  }

  if (countLane1 >= totalRounds || countLane2 >= totalRounds)
  {
    endRace();
    showWinner();
  }
}

void startRace()
{
  Serial.println("race start");
  attachInterrupt(sensorLane1, endRound1, FALLING); // fehlstartcheck
  attachInterrupt(sensorLane2, endRound2, FALLING); // fehlstartcheck

  roundTimeLane1 = 0;
  roundTimeLane2 = 0;
}

void endRace()
{
  Serial.println("race ended");
  detachInterrupt(sensorLane1);
  detachInterrupt(sensorLane2);
}

void showFehlstart()
{
  if (fehlstartLane1)
  {
    setCol1(rot);
  }
  if (fehlstartLane2)
  {
    setCol2(rot);
  }
}

// watermark generates this interrupt
void setStart()
{
  cli();
  Serial.println("Start was Pressed");
  wasPressed = true;
  sei();
}

// rundenzäler
void endRound1()
{
  cli();
  countLane1++;
  lastRound1 = roundTimeLane1;
  roundTimeLane1 = 0;
  if (lastRound1 < bestRound1)
  {
    bestRound1 = lastRound1;
  }
  String outputtext = "lane1;" + String(countLane1) + ";" + String(lastRound1);
  Serial.println(outputtext);
  sei();
}

// rundenzäler
void endRound2()
{
  cli();
  countLane2++;
  lastRound2 = roundTimeLane2;
  roundTimeLane2 = 0;
  if (lastRound2 < bestRound2)
  {
    bestRound2 = lastRound2;
  }
  String outputtext = "lane2;" + String(countLane2) + ";" + String(lastRound2);
  Serial.println(outputtext);
  sei();
}

// fehlstarts interupts
void fehlstartcheck1()
{
  cli();
  Serial.println("fehlstart;1");
  fehlstartLane1 = true;
  sei();
}

void fehlstartcheck2()
{
  cli();
  Serial.println("fehlstart;2");
  fehlstartLane2 = true;
  sei();
}

void countdown()
{
  // erst low adnn high
  while (digitalRead(sensorLane1) == HIGH || digitalRead(sensorLane2) == HIGH)
  {
    Serial.println("lan1 ready" + digitalRead(sensorLane1));
    Serial.println("lan2 ready" + digitalRead(sensorLane2));
    if (digitalRead(sensorLane1) == HIGH)
    {
      setCol1(blau);
    }
    else
    {
      setCol1(turkies);
    }
    if (digitalRead(sensorLane2) == HIGH)
    {
      setCol2(blau);
    }
    else
    {
      setCol2(turkies);
    }

    delay(50);
  }

  attachInterrupt(sensorLane1, fehlstartcheck1, RISING); // fehlstartcheck
  attachInterrupt(sensorLane2, fehlstartcheck2, RISING); // fehlstartcheck

  countdownRunning = true;

  setRow1(schwarz);
  setRow2(schwarz);
  setRow3(schwarz);

  Serial.println("Start in;3");
  setRow1(rot);
  delay(1000);
  setRow1(schwarz);

  Serial.println("Start in;2");
  setRow2(rot);
  delay(1000);
  setRow2(schwarz);

  Serial.println("Start in;1");
  setRow3(rot);
  delay(1000);

  detachInterrupt(sensorLane1);
  detachInterrupt(sensorLane2);

  Serial.println("Start in;0");
  setRow1(gruen);
  setRow2(gruen);
  setRow3(gruen);
  delay(3000);

  setRow1(schwarz);
  setRow2(schwarz);
  setRow3(schwarz);

  countdownRunning = false;
}

void showWinner()
{

  int winnerpos = 0;
  if (countLane2 > countLane1)
  {
    winnerpos = 6;
  }

  while (!wasPressed)
  {
    zielflagge(winnerpos);
  }
}

void zielflagge(int winnerpos)
{

  setPixel(winnerpos + 0, weis);
  setPixel(winnerpos + 1, schwarz);
  setPixel(winnerpos + 2, weis);
  setPixel(winnerpos + 3, schwarz);
  setPixel(winnerpos + 4, weis);
  setPixel(winnerpos + 5, schwarz);

  strip.show();

  delay(200);

  setPixel(winnerpos + 1, weis);
  setPixel(winnerpos + 2, schwarz);
  setPixel(winnerpos + 3, weis);
  setPixel(winnerpos + 4, schwarz);
  setPixel(winnerpos + 5, weis);
  setPixel(winnerpos + 0, schwarz);

  strip.show();

  delay(200);
}

// Display Helper -> Ampel
void setCol1(uint32_t farbe)
{
  setPixel(0, farbe);
  setPixel(1, farbe);
  setPixel(2, farbe);

  strip.show();
}
void setCol2(uint32_t farbe)
{
  setPixel(9, farbe);
  setPixel(10, farbe);
  setPixel(11, farbe);

  strip.show();
}

void setRow1(uint32_t farbe)
{
  setPixel(0, farbe);
  setPixel(5, farbe);
  setPixel(6, farbe);
  setPixel(11, farbe);
  strip.show();
}
void setRow2(uint32_t farbe)
{
  setPixel(0 + 1, farbe);
  setPixel(3 + 1, farbe);
  setPixel(6 + 1, farbe);
  setPixel(9 + 1, farbe);
  strip.show();
}
void setRow3(uint32_t farbe)
{
  setPixel(2, farbe);
  setPixel(3, farbe);
  setPixel(8, farbe);
  setPixel(9, farbe);
  strip.show();
}

void setPixel(int number, uint32_t c)
{
  if (number < strip.numPixels())
  {
    strip.setPixelColor(number, c);
  }
}
