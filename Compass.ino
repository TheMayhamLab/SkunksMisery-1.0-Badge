/*
 * Name:      Compass
 * Purpose:   Skunks Misery Badge 1.0 - Compass
 * By:        Micael Vieau & Kevin Bong
 * Created:   2019.4.16
 * Modified:  2019.5.20
 * Rev Level  1.0
 */

#include <Wire.h> //Wire Librarey for I2C communication
#include <HMC5883L.h>
#include <MechaQMC5883.h> //QMC5883 Library is added since mine is QMC583 and not HMC5883

MechaQMC5883 qmc; //Create an object name for the sensor, I have named it as qmc

char led_count = 7; // Total number of LEDs - 1
int startcount = 0; // Counter used for the startup sequence
int degreevariant = 2; // This is the number of degrees on each side of the direction we want to allow for the LED to be on.
//int degreevariant = 22.5;
// ****************** Shift register stuff ******************
int latchPin = 1; // Pin 6 (PB1) on the ATtiny85
int clockPin = 4; // Pin 3 (PB4) on the ATtiny85
int dataPin = 3;  // Pin 2 (PB3) on the ATtiny85
byte leds = B00000000; // LEDS off by default
int heading=0;

void setup() {
  Wire.begin(); //Begin I2C communication 
  qmc.init(); //Initialise the QMC5883 Sensor 

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}

void loop() {

  // Run startup funciton
  if (startcount < 2)
  {
    startup();
    AllOff();
  }
  
  int x,y,z;
  qmc.read(&x,&y,&z); //Get the values of X,Y and Z from sensor 
  int heading=atan2(x, y)/0.0174532925; //Calculate the degree using X and Y parameters with this formulae 
  
  //Convert result into 0 to 360
  if(heading < 0)
  {
    heading+=360;
  }
  heading = 360-heading;

  //Based on the value of heading light up the correct LED(s).
  
  // North
  if (heading > (360-degreevariant) || heading < (0+degreevariant))
  {
    OneOn(0);
  }
  // North by North-East
  if (heading > (22-degreevariant) && heading < (22+degreevariant))
  {
    TwoOn(7,0);
  }
  // North-East
  if (heading > (45-degreevariant) && heading < (45+degreevariant))
  {
    OneOn(7);
  }
  // North-East by East
  if (heading > (67-degreevariant) && heading < (67+degreevariant))
  {
    TwoOn(7,6);
  }
  // East
  if (heading > (90-degreevariant) && heading < (90+degreevariant))
  {
    OneOn(6);
  }
  // East by South-East
  if (heading > (113-degreevariant) && heading < (113+degreevariant))
  {
    TwoOn(6,5);
  }
  // South-East
  if (heading > (135-degreevariant) && heading < (135+degreevariant))
  {
    OneOn(5);
  }
  // South-East by South
  if (heading > (157-degreevariant) && heading < (157+degreevariant))
  {
    TwoOn(5,4);
  }
  // South
  if (heading > (180-degreevariant) && heading < (180+degreevariant))
  {
    OneOn(4);
  }
  // South by South-West
  if (heading > (203-degreevariant) && heading < (203+degreevariant))
  {
   TwoOn(4,3);
  }
  // South-West
  if (heading > (225-degreevariant) && heading < (225+degreevariant))
  {
    OneOn(3);
  }
  // South-West by West
  if (heading > (247-degreevariant) && heading < (247+degreevariant))
  {
    TwoOn(3,2);
  }
  // West
  if (heading > (270-degreevariant) && heading < (270+degreevariant))
  {
    OneOn(2);
  }
  // West by North-West
  if (heading > (293-degreevariant) && heading < (293+degreevariant))
  {
   TwoOn(2,1);
  }
  // North-West
  if (heading > (315-degreevariant) && heading < (315+degreevariant))
  {
    OneOn(1);
  }
  // North-West by North
  if (heading > (337-degreevariant) && heading < (337+degreevariant))
  {
    TwoOn(1,0);
  }
  else
  {
    AllOff();
  }
}

void AllOff()
{
  // Turns off all the LEDs
  delay(5);
  
  for (int i = 0; i < 8; i++)
  {
    bitClear(leds, i);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, leds);
    digitalWrite(latchPin, LOW);
  }
  delay(5);
}

void OneOn(int whichLED)
{
  // Turn on one LED when pointed in the correct direction
  leds = B00000000;
  delay(45);
    bitSet(leds, whichLED);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, leds);
    digitalWrite(latchPin, HIGH);
  //delay(5);
}

void TwoOn(int whichLED1, int whichLED2)
{
  // Turn on two LEDs when between two directions
  leds = B00000000;
  delay(5);
    bitSet(leds, whichLED1);
    bitSet(leds, whichLED2);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, leds);
    digitalWrite(latchPin, HIGH);
  delay(5);
}

void startup() 
{
  // Function to cycle through the LEDs when the board turns on
  leds = B00000000;
  for (int i = 0; i < 8; i++)
  {
    OneOn(i);
    delay(50);
  }
  startcount++;  
}
