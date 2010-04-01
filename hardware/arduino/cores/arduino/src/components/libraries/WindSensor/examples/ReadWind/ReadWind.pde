/////////////////////////////////////////////////////////////////
//Wind Monitor Example
//This example uses an Arduino Mega and a Wind Sensor from http://moderndevice.com/
//mike.gionfriddo@gmail.com
/////////////////////////////////////////////////////////////////

#include <WindSensor.h>

#define WIND_SIGNAL 15// Arduino Mega Analog pin 15

WindSensor myWind = WindSensor(WIND_SIGNAL);// Arduino Mega Analog pin 15

void setup(){
  
  Serial.begin(9600);//create a serial port from the Core to the PC at 9600 baud

}


void loop(){
  
  Serial.print("Air Flow: ");//print the word "Air Flow: " to the serial monitor
  Serial.println(myWind.getAirFlow());// print to the magnatude of air flowing through the sensor
 
}
