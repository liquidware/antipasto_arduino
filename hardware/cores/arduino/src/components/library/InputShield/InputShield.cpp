/*
  InputShield - Liquidware InputShield library
  Copyright (c) 2010 Christopher Ladden.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <avr/io.h>
#include <inttypes.h>

#include "wiring.h"
#include "AFSoftSerial.h"
#include "InputShield.h"

/******************************************************************************
 * Constructors
 ******************************************************************************/

InputShield::InputShield(uint8_t mode) {
    SetMode(mode);
    _averages = 1;
}

/**
 * Initialize a new InputShield with a Software Serial
 * Connection
 *
 * @param mode The inputshield hardware switch mode (0 = mode A,
 *             1 = mode B)
 * @param serial A pointer to an software serial instance
 */
InputShield::InputShield(uint8_t mode, AFSoftSerial & mySerial) {
    SetMode(mode);
    _averages = 1;
    mySerial.begin(9600);
}

/******************************************************************************
 * Global Functions
 ******************************************************************************/

//******************************************
// Setup the pins depending on the user
// selected mode.
// mode A = 0; mode B = 1
//******************************************
void InputShield::SetMode(uint8_t mode) {

    /* Which mode? */
    if (mode == INPUTSHIELD_MODE_A ) {
        //Mode A
        _pins.JoyLateral = 4;
        _pins.JoyVertical = 5;
        _pins.ButtonA = 4;
        _pins.ButtonB = 5;
        _pins.ButtonJoy = 6;
        _pins.Vibrate = 7;
    } else {
        //Mode B
        _pins.JoyLateral = 2;
        _pins.JoyVertical = 3;
        _pins.ButtonA = 8;
        _pins.ButtonB = 9;
        _pins.ButtonJoy = 10;
        _pins.Vibrate = 11;
    }

    // Configure the Arduino pins
    pinMode(_pins.ButtonA, INPUT);     //pin as input
    digitalWrite(_pins.ButtonA, HIGH); //enable internal pullup

    pinMode(_pins.ButtonB, INPUT);     //pin as input
    digitalWrite(_pins.ButtonB, HIGH); //enable internal pullup

    pinMode(_pins.ButtonJoy, INPUT);     //pin as input
    digitalWrite(_pins.ButtonJoy, HIGH); //enable internal pullup

    pinMode(_pins.Vibrate, OUTPUT);     //pin as output
    digitalWrite(_pins.Vibrate, HIGH); //enable internal pullup

    //Store the mode
    iMode = mode;
}

//******************************************
// Return the Analog value of
// the Joystick's lateral location
//******************************************
int InputShield::GetJoyLat(void) {
    return sampleAnalogPin(_pins.JoyLateral);
}

//******************************************
// Return the Analog value of
// the Joystick's vertical location
//******************************************
int InputShield::GetJoyVert(void) {
    return sampleAnalogPin(_pins.JoyVertical);
}

//******************************************
// Read the button
//******************************************
int InputShield::GetButtonA(void) {
    return digitalRead(_pins.ButtonA);
}

//******************************************
// Read the button
//******************************************
int InputShield::GetButtonB(void) {
    return digitalRead(_pins.ButtonB);
}

//******************************************
// Read the button
//******************************************
int InputShield::GetButtonJoy(void) {
    return digitalRead(_pins.ButtonJoy);
}

//******************************************
// Set the analog pin sample averages
//******************************************
void InputShield::SetSampleAverages(uint8_t sampleAverages) {
    /* Zero Check */
    if (sampleAverages > 0) {
        _averages = sampleAverages;
    } else {
        _averages = 1;
    }
}

//******************************************
// Vibrate the InputShield's Motor
//******************************************
void InputShield::Vibrate(uint16_t duration) {
    digitalWrite(_pins.Vibrate, LOW);
    delay(duration);
    digitalWrite(_pins.Vibrate, HIGH);
}

//*****************************************************************
//* Build and send InputShield State
//* Packet via Software Serial
//*****************************************************************
void InputShield::SendHardwareState(AFSoftSerial & mySerial) {
    uint8_t buttonValue = 0;
    uint16_t joyValueLat = 0;
    uint16_t joyValueVert = 0;

    // Get the Inputshield's entire state
    joyValueLat  = GetJoyLat();
    joyValueVert = GetJoyVert();
    buttonValue = (GetButtonA()   << 0) |
                  (GetButtonB()   << 1) |
                  (GetButtonJoy() << 2);

    // Send an InputShield data packet
    // Packet Format:
    //   -------------------
    //  | Escape Char       |
    //  | Data Size         |
    //  | The Mode ID       |
    //  | joyLat Low Byte   |
    //  | joyLat High Byte  |
    //  | joyVert Low Byte  |
    //  | joyVert High Byte |
    //  | Button Bit Field  |
    //  | Escape Char       |
    //   -------------------

    /* Send escape character */
    mySerial.print('S');

    /* Send the size of the message */
    mySerial.print((unsigned char)6);

    // Send the state via serial
    if (iMode == INPUTSHIELD_MODE_A) {
        sendData((unsigned char*)"A", 1, mySerial);
    } else {
        sendData((unsigned char*)"B", 1, mySerial);
    }
    sendData((unsigned char*)&joyValueLat, sizeof(joyValueLat), mySerial);
    sendData((unsigned char*)&joyValueVert,sizeof(joyValueVert), mySerial);
    sendData((unsigned char*)&buttonValue, sizeof(buttonValue), mySerial);

    /* Send escape character */
    mySerial.print('S');
}

/******************************************************************************
 * Private Functions
 ******************************************************************************/

//*****************************************************************
// Sample the analog pin
//*****************************************************************
int InputShield::sampleAnalogPin(uint8_t pin) {
    uint32_t average = 0;

    for (int x=0; x < _averages; x++) {
        average+= analogRead(pin);
    }

    average /= _averages;

    return average;
}

//*****************************************************************
//* Send serial data using a SLIP style packet
//*    Notes:
//*       ESC = 'S'
//*       TESC = 'T'
//*****************************************************************
void InputShield::sendData(unsigned char *data, unsigned char size, AFSoftSerial & mySerial)
{
    unsigned char ii;

    /* Send all the data */
    for (ii=0; ii < size; ii++) {
        delay(2);
        if ((char)data[ii] == 'S') {
            mySerial.print('T');  //escape the escape character
        }
        mySerial.print(data[ii]);
    }
}


