#include <avr/io.h>
#include <inttypes.h>

#include "InputShield.h"
#include "wiring.h"

/******************************************************************************
 * Constructors
 ******************************************************************************/

InputShield::InputShield(uint8_t mode) {
    setMode(mode);
    _averages = 1;
}

/******************************************************************************
 * Global Functions
 ******************************************************************************/

//******************************************
// Setup the pins depending on the user
// selected mode.
// mode A = 0; mode B = 1
void InputShield::setMode(uint8_t mode) {

    /* Which mode? */
    if (mode == 0 ) {
        //Mode A 
        _pins.JoyLateral = 5;
        _pins.JoyVertical = 4;
        _pins.ButtonA = 4;
        _pins.ButtonB = 5;
        _pins.ButtonJoy = 6;
    } else {
        //Mode B
        _pins.JoyLateral = 3;
        _pins.JoyVertical = 2;
        _pins.ButtonA = 8;
        _pins.ButtonB = 9;
        _pins.ButtonJoy = 10;
    }

        pinMode(_pins.ButtonA, INPUT);     //pin as input
        digitalWrite(_pins.ButtonA, HIGH); //enable internal pullup

        pinMode(_pins.ButtonA, INPUT);     //pin as input
        digitalWrite(_pins.ButtonA, HIGH); //enable internal pullup

        pinMode(_pins.ButtonA, INPUT);     //pin as input
        digitalWrite(_pins.ButtonA, HIGH); //enable internal pullup

        

}

//********************************
// Return the Analog value of
// the Joystick's lateral location 
int InputShield::getJoyLat(void) {
    
    return sampleAnalogPin(_pins.JoyLateral);
}

//********************************
// Return the Analog value of
// the Joystick's vertical location 
int InputShield::getJoyVert(void) {
    
    return sampleAnalogPin(_pins.JoyVertical);
}

//********************************
// Read the button
int InputShield::getButtonA(void) {
    
    return digitalRead(_pins.ButtonA);
}

//********************************
// Read the button
int InputShield::getButtonB(void) {
    
    return digitalRead(_pins.ButtonB);
}

//********************************
// Read the button
int InputShield::getButtonJoy(void) {
    
    return digitalRead(_pins.ButtonJoy);
}

//************************************
// Set the analog pin sample averages
void InputShield::setSampleAverages(uint8_t sampleAverages) {
    /* Zero Check */
    if (sampleAverages > 0) {
        _averages = sampleAverages;
    } else
        _averages = 1;
    }
    
}

/******************************************************************************
 * Private Functions
 ******************************************************************************/

//*****************************************
// Sample the analog pin
int InputShield::sampleAnalogPin(uint8_t pin) {
    uint32_t average = 0;

    for (int x=0; x < _averages; x++) {
        average+= analogRead(pin);
    }
    
    average /= _averages;

    return average;
}


