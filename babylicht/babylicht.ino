#include <Adafruit_NeoPixel.h>
// include the library code:
//#include <LiquidCrystalFast.h>


int numPixel = 12; //60;

int ticker = 1;
bool isOn = true;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixel, 20, NEO_GRB + NEO_KHZ800);
//LiquidCrystalFast lcd(2, 15, 3, 17, 4, 19, 5);


const int startButton = 14;

const int ledPin =  13;

int helligkeit = 155;

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

  delay(200);

  wasPressed = false;

  Serial.begin(9600); // USB is always 12 Mbit/sec
  strip.begin();
  setAll(strip.Color(0, 0, 0));
  strip.show(); // Initialize all pixels to 'off'

  //  lcd.begin(16, 2);
  //  lcd.print("Carrera Profi - Laptime");


  setColors();

  // strip.setBrightness(110);
  pinMode(startButton, INPUT_PULLUP);
  delay(200);

}






int mode = 0;
bool lastRun = false;
void loop() {

  if ((digitalRead(startButton)) == LOW && !lastRun) {

    mode++;
    mode = mode % 3;



    Serial.println(mode);
    if (mode == 0) {

      setPixel(5,strip.Color(helligkeit, 0, 0));
    }
    else if (mode == 1) {
      setPixel(5,strip.Color(0, helligkeit, 0));
    }
    else if ( mode == 2) {
      setPixel(5, strip.Color(0, 0,helligkeit));
    }
     strip.show();
  }
  lastRun =  (digitalRead(startButton) == LOW);


  delay(20);

}



void setAll(uint32_t farbe) {
  Serial.println(farbe);
  for (int i = 0; i <= numPixel; i++) {
    setPixel(i, farbe);
    
  }
  strip.show();
}


//Display Helper -> Ampel
void setCol1(uint32_t farbe) {
  setPixel(0, farbe);
  setPixel(1, farbe);
  setPixel(2, farbe);

  strip.show();
}
void setCol2(uint32_t farbe) {
  setPixel(9, farbe);
  setPixel(10, farbe);
  setPixel(11, farbe);

  strip.show();
}


void setRow1(uint32_t farbe) {
  setPixel(0, farbe);
  setPixel(5, farbe);
  setPixel(6, farbe);
  setPixel(11, farbe);
  strip.show();
}
void setRow2(uint32_t farbe) {
  setPixel(0 + 1, farbe);
  setPixel(3 + 1, farbe);
  setPixel(6 + 1, farbe);
  setPixel(9 + 1, farbe);
  strip.show();
}
void setRow3(uint32_t farbe) {
  setPixel(2, farbe);
  setPixel(3, farbe);
  setPixel(8, farbe);
  setPixel(9, farbe);
  strip.show();
}



void setPixel(int number, uint32_t c) {
  if (number < strip.numPixels()) {
    strip.setPixelColor(number, c);

  }
}

void setColors() {


  rot = strip.Color(helligkeit, 0, 0);
  gruen = strip.Color(0, helligkeit, 0);
  schwarz = strip.Color(0, 0, 0);
  weis = strip.Color(helligkeit, helligkeit, helligkeit);
  blau = strip.Color(0, 0, helligkeit);
  turkies = strip.Color(0, helligkeit, helligkeit);
  orange = strip.Color(helligkeit, helligkeit, 0);

}



