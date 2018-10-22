/* 
 Controlling a servo position using a potentiometer (variable resistor) 
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h> 
 
Servo sX;  // create servo object to control a servo 
 Servo sY;

void setup() 
{ 
  sX.attach(2);  // attaches the servo on pin 9 to the servo object 
   sY.attach(5);  // attaches the servo on pin 9 to the servo object 
} 
 
void loop() 
{ 

  /*
  // val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180) 
  int rX = random(0,360);
  int rY = random(0,90); 
*/
for(int i = 0; i<360;i+=10){
int rX = i;
int rY = 45;
     setY(rY, setX(rX));
    delay(100);
}
  delay(2000);
} 


//takes 0  to 360 degree
//returns true if over 180 and Y needs to be inversed
bool setX(int x){
  
  if(x<180){
      sX.write(x);
      return false;
  }
  else{
    sX.write(x%180);
    return true;
    }
  
}

//takes 0  to 360 degree
//returns true if over 180 and Y needs to be inversed
void setY(int y, bool inversed){
  
  if(inversed){
      sY.write(180-y);
    
  }
  else{
    sY.write(y);
    
    }
  
}

