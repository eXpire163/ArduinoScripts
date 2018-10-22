//#include <Adafruit_NeoPixel.h>



//Programed for the Arduino MEGA 2560!!!

// Define Variables:
int chA=6;  //Constant variables relating to pin locations


int chOut1 =0;  //Varibles to store and display the values of each channel
//asdf


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);//115200
  // Set input pins
  pinMode(chA, INPUT);
  Serial.print ("Ch1:");  //Display text string on Serial Monitor to distinguish variables
 
  
}

//Main Program
void loop() {
  // read the input channels
  chOut1 = pulseIn (chA,HIGH);  //Read and store channel 1
  Serial.print ("Ch1:");  //Display text string on Serial Monitor to distinguish variables
  Serial.print (chOut1);     //Print in the value of channel 1
  Serial.print ("\n");  //Display text string on Serial Monitor to distinguish variables
 
  delay(500);
}




