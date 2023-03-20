// about: same with added lightgates for start check and lapcount WIP
#include <Adafruit_NeoPixel.h>
// include the library code:
#include <LiquidCrystalFast.h>

int numPixel = 12; // 60;

int ticker = 1;
bool isOn = true;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixel, 20, NEO_GRB + NEO_KHZ800);
LiquidCrystalFast lcd(2, 15, 3, 17, 4, 19, 5);

const int startButton = 14;
const int sensorLane1 = 16;
const int sensorLane2 = 18;

const int ledPin = 13;

int helligkeit = 255;

uint32_t rot;
uint32_t gruen;
uint32_t schwarz;
uint32_t weis;
uint32_t blau;
uint32_t turkies;
uint32_t orange;

int totalRounds = 6;

bool racing = false;

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

  lcd.begin(16, 2);
  lcd.print("Carrera Profi - Laptime");

  setColors();

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
  racing = false;

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

  setRow1(schwarz);
  setRow2(schwarz);
  setRow3(schwarz);
}

void loop()
{

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

  if (racing)
  {
    updateStatus();
    if (countLane1 >= totalRounds || countLane2 >= totalRounds)
    {
      endRace();
      showWinner();
    }
  }
  delay(5);
}

void startRace()
{
  Serial.println("race start");
  racing = true;
  attachInterrupt(sensorLane1, endRound1, RISING);
  attachInterrupt(sensorLane2, endRound2, RISING);

  roundTimeLane1 = 0;
  roundTimeLane2 = 0;
}

void endRace()
{
  racing = false;
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

// startknopf pressed
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

  lcd.clear();

  int countOK = 0;
  while ((digitalRead(sensorLane1) == LOW || digitalRead(sensorLane2) == LOW) || countOK < 200)
  {

    if (digitalRead(sensorLane1) == HIGH && digitalRead(sensorLane2) == HIGH)
    {
      countOK++;
    }

    if (digitalRead(sensorLane1) == LOW)
    {
      setCol1(blau);
      write1("1: Zum Start");
    }
    else
    {
      setCol1(turkies);

      write1("1: Bereit");
    }
    if (digitalRead(sensorLane2) == LOW)
    {
      setCol2(blau);
      write2("2: Zum Start");
    }
    else
    {
      setCol2(turkies);
      lcd.setCursor(0, 1);
      write2("2: Bereit");
    }

    delay(5);
  }
  lcd.clear();

  attachInterrupt(sensorLane1, fehlstartcheck1, RISING); // fehlstartcheck
  attachInterrupt(sensorLane2, fehlstartcheck2, RISING); // fehlstartcheck

  countdownRunning = true;

  setRow1(schwarz);
  setRow2(schwarz);
  setRow3(schwarz);

  lcd.setCursor(9, 0);
  lcd.print("3");

  Serial.println("Start in;3");
  setRow1(rot);
  delay(1000);
  setRow1(schwarz);

  lcd.setCursor(9, 0);
  lcd.print("2");
  Serial.println("Start in;2");
  setRow2(rot);
  delay(1000);
  setRow2(schwarz);

  lcd.setCursor(9, 0);
  lcd.print("1");
  Serial.println("Start in;1");

  setRow3(rot);
  delay(1000);

  detachInterrupt(sensorLane1);
  detachInterrupt(sensorLane2);

  lcd.setCursor(7, 0);
  lcd.print("Start");
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

void updateStatus()
{

  String last1 = " L:" + (lastRound1 / 1000);
  String best1 = " B:" + (bestRound1 / 1000);
  String curr1 = " C:" + (roundTimeLane1 / 1000);
  write1("1:" + String(countLane1));
  Serial.println("1:" + countLane1);
  showRounds();

  String last2 = " L:" + (lastRound2 / 1000);
  String best2 = " B:" + (bestRound2 / 1000);
  String curr2 = " C:" + (roundTimeLane2 / 1000);
  write2("2:" + String(countLane2));
  Serial.println("2:" + countLane2);
  showRounds();
}

// lane 1 = , lane 2 = 6
void showRounds()
{

  for (int i = 0; i <= (countLane1 - 1); i++)
  {
    setPixel(i, orange);
  }

  for (int i = 0; i <= (countLane2 - 1); i++)
  {
    setPixel(i + 6, orange);
  }

  strip.show();
}

String txt1 = "";
String txt2 = "";
void write1(String txt)
{
  if (txt != txt1)
  {
    txt1 = txt;
    while (txt.length() < 20)
    {
      txt = txt + " ";
    }
    lcd.setCursor(0, 0);
    lcd.print(txt);
  }
}
void write2(String txt)
{
  if (txt != txt2)
  {
    txt2 = txt;
    while (txt.length() < 20)
    {
      txt = txt + " ";
    }
    lcd.setCursor(0, 1);
    lcd.print(txt);
  }
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

void setColors()
{

  rot = strip.Color(helligkeit, 0, 0);
  gruen = strip.Color(0, helligkeit, 0);
  schwarz = strip.Color(0, 0, 0);
  weis = strip.Color(helligkeit, helligkeit, helligkeit);
  blau = strip.Color(0, 0, helligkeit);
  turkies = strip.Color(0, helligkeit, helligkeit);
  orange = strip.Color(helligkeit, helligkeit, 0);
}
