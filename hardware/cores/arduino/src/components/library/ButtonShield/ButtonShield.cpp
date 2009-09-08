#include <inttypes.h>
#include "wiring.h"
#include "ButtonShield.h"

/******************************************************************************
 * Constructors
 ******************************************************************************/

ButtonShield::ButtonShield(uint8_t mode) {
    setMode(mode);
}

/******************************************************************************
 * Global Functions
 ******************************************************************************/

void ButtonShield::setMode(uint8_t mode) {

    /* Which mode? */
    if (mode == 0 ) {
        //Mode A 
        pinList[0] = 4;
        pinList[1] = 5;
        pinList[2] = 6;
        pinList[3] = 7;
        pinList[4] = 18;
        pinList[5] = 19;
    } else {
        //Mode B
        pinList[0] = 8;
        pinList[1] = 9;
        pinList[2] = 10;
        pinList[3] = 11;
        pinList[4] = 16;
        pinList[5] = 17;
    }

    for (uint8_t pin=0; pin<6; pin++) {
        pinMode(pinList[pin],INPUT);     //pin as input
        digitalWrite(pinList[pin],HIGH); //enable internal pullup
    }
}


int ButtonShield::readButtons() {

    uint8_t button = 0;

    for (uint8_t pin=0; pin<6; pin++) {
        button |= digitalRead(pinList[pin]) << pin;
    }

    return button;
}

