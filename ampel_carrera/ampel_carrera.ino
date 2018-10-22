#include <Adafruit_NeoPixel.h>

int numPixel =12;//60;

int ticker = 1;
bool isOn = true;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixel, 1, NEO_GRB + NEO_KHZ800);

const int buttonPin = 4; 
const int ledPin =  13;    

int buttonState = 0; 
int buttonLastState = 0;


uint32_t rot;
uint32_t gruen;
uint32_t schwarz;

void setup()
{
 Serial.begin(9600); // USB is always 12 Mbit/sec
 strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  rot = strip.Color(255,0,0);
  gruen = strip.Color(0,255,0);
  schwarz = strip.Color(0,0,0);
strip.setBrightness(110);
pinMode(buttonPin, INPUT_PULLUP);
pinMode(ledPin, OUTPUT);      

  delay(100);
}





bool newRun = false;
void loop(){
  buttonState = digitalRead(buttonPin);
  //Serial.println(analogRead(buttonPin));
   if (buttonState == LOW) {     
    // turn LED on:    
    //Serial.println("low");
    countdown();
    //digitalWrite(ledPin, HIGH);  
  } 
  else{
    //Serial.println("high");
    //digitalWrite(ledPin, LOW);  
   
    }
 delay(300);
   // countdown();

}

int start1=0;
int start2=0;
int start3=0;

void countdown(){

 setRow1(schwarz);
  setRow2(schwarz);
  setRow3(schwarz);
  
  Serial.println(rot);
  setRow1(rot);
  delay(1000); 
 setRow1(schwarz);

    Serial.println("Hello World..2.");
  setRow2(rot);
  delay(1000);  
setRow2(schwarz);

   Serial.println("Hello World..3.");
  setRow3(rot);
  delay(1000);  

   Serial.println("Hello World..4.");
  setRow1(gruen);
  setRow2(gruen);
  setRow3(gruen);
  delay(3000);  

   setRow1(schwarz);
  setRow2(schwarz);
  setRow3(schwarz);
}

void setRow1(uint32_t farbe){
  setPixel(0,farbe);
  setPixel(5,farbe);
  setPixel(6,farbe);
  setPixel(11,farbe);
   strip.show();
}
void setRow2(uint32_t farbe){
  setPixel(0+1,farbe);
  setPixel(3+1,farbe);
  setPixel(6+1,farbe);
  setPixel(9+1,farbe);
   strip.show();
}
void setRow3(uint32_t farbe){
  setPixel(2,farbe);
  setPixel(3,farbe);
  setPixel(8,farbe);
  setPixel(9,farbe);
   strip.show();
}


void startWhite(int startspeed){
  
 

while(start3<255){
  
  for(int i = 0;i<=numPixel;i++){
      setPixel(i , strip.Color(start1,start2,start3));
    
    }
      delay(startspeed);
      start3++;
  strip.show();
}

while(start2<255){
  for(int i = 0;i<=numPixel;i++){
       setPixel(i , strip.Color(start1,start2,start3));
        
    }
      delay(startspeed);
      start2++;
  strip.show();
}

while(start1<255){
  for(int i = 0;i<=numPixel;i++){
   setPixel(i , strip.Color(start1,start2,start3));
       
    }
      delay(startspeed);
      start1++;
   strip.show();
  }

  
  
  
  }


void rainbow(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}





void setPixel(int number, uint32_t c){
  if(number<strip.numPixels()){
    strip.setPixelColor(number, c);
     
  }
}
/*

void loop2() {
//  Serial.println("in");
  //waitForGap(1000);
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if(results.value == 0xFFFFFFFF){}
     else if(results.value == 0x20DF10EF){
        toggleOnOff();
   
     }

      else if(results.value == 0x20DF40BF){
        heller();
        Serial.println("heller");
      }
      else if(results.value == 0x20DFC03F){
        dunkler();
        Serial.println("dunkler");
      }else {
        Serial.println(results.value, HEX);
      }
      
      irrecv.resume(); // Receive the next value
  }//end recive data
 else{
   //updateStrip();
   
  }
 // delay(100);
   // Serial.println("tick");
}

//Befehle
void heller(){
if(brightness<=200){
  brightness= brightness+50;
   strip.setBrightness(brightness);
   Serial.println(brightness);
  }
}

void dunkler(){
if(brightness>=50){
  brightness= brightness-50;
   strip.setBrightness(brightness);
   Serial.println( brightness);
  }
}

void toggleOnOff(){

  Serial.println("On/Off");
  if(isOn){
    for(int i = 0; i < numPixel ;i++){
      setPixel(i, strip.Color(0,0,0));
    }
  }
  else{
    int brightness = 255;
    for(int i = 0; i < numPixel ;i++){
      setPixel(i, strip.Color(brightness,brightness,brightness));
    }
 
  }//end else
  strip.show();
  isOn = !isOn;
}*/
  
  

