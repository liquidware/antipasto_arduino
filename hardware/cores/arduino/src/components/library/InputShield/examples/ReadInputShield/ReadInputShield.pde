//*********************************************
// Example: ReadInputShield
//
// Description: Prints the InputShield's state
//              to the PC's terminal
//
// Christopher Ladden @ www.liquidware.org
//*********************************************

#include <InputShield.h>

InputShield inputShield = InputShield(0);  // create a new inputShield in Mode A
                                           // Modes allow multiple inputshields on one Arduino. Changing the
                                           // mode changes the Arduino pins the inputshield uses.
                                           // Modes are changed by changing the mode switch on the inputShield.


void setup() {
   /* Start hardware serial */
   Serial.begin(9600);
}

//*********************************************
// Print the InputShield's state to the PC
//*********************************************
void loop() {
   inputShield.Vibrate(250);

   Serial.print(" Lat: ");
   Serial.print(inputShield.GetJoyLat());

   Serial.print(" Vert: ");
   Serial.print(inputShield.GetJoyVert());

   Serial.print(" Button A: ");
   Serial.print(inputShield.GetButtonA());

   Serial.print(" Button B: ");
   Serial.print(inputShield.GetButtonB());

   Serial.print(" Button Joy: ");
   Serial.print(inputShield.GetButtonJoy());

   Serial.print("\n");

   delay(500);
}
