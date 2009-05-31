#include <ButtonShield.h>

/* Setup for Mode A */
ButtonShield buttons = ButtonShield(0);

void setup() {
  
  Serial.begin(9600);
  
}

void loop() {
    
    Serial.println(buttons.readButtons());
}
