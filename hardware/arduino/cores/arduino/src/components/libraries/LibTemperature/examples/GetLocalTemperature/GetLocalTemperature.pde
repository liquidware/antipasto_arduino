/****************************
 * GetLocalTemperature
 *  An example sketch that prints the
 *  local temperature to the PC's serial port
 *
 *  Tested with the TMP421-Breakout
 *  Temperature Sensor from Modern Device
 *****************************/
#include "Wire.h"
#include <LibTemperature.h>

LibTemperature temp = LibTemperature(0);

void setup() {
  Serial.begin(9600);
}

void loop() {
 Serial.print("Temp: ");
 Serial.print(temp.GetTemperature());
 Serial.println(" degC");
 delay(100);
}
