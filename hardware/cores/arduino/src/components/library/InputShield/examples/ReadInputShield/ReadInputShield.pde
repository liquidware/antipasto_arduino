//*********************************************
// Example: ReadInputShield
//
// Description: Prints the InputShield's state 
//              to the PC's terminal 
//
// Christopher Ladden @ www.liquidware.org
//*********************************************

#include <InputShield.h>

InputShield input = InputShield(0);

void setup() {
   /* Start hardware serial */
   Serial.begin(9600);
}

//*********************************************
// Print the InputShield's state to the PC 
void loop() {
   
   Serial.print("Lat: "); 
   Serial.print(input.getJoyLat());

   Serial.print("Vert: "); 
   Serial.print(input.getJoyVert());   
   
   Serial.print("Button A: ");
   Serial.print(input.getButtonA());
   
   Serial.print("Button B: ");
   Serial.print(input.getButtonB());
   
   Serial.print("Button Joy: ");
   Serial.print(input.getButtonJoy());
   
   Serial.print("\n");
   
   delay(50);
}
