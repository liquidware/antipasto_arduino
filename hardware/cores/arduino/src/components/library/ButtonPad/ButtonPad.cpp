/*
  ButtonPad.h - Liquidware ButtonPad library
  Copyright (c) 2009 Mike Gionfriddo & Christopher Ladden.  All right reserved.

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

#include <inttypes.h>
#include "wiring.h"
#include "ButtonPad.h"

/******************************************************************************
 * Constructors
 ******************************************************************************/


/**
 * ButtonPad Constructor. 
 *  
 * Initializes the Arduino pins for input and power to the 
 * ButtonPad. 
 * 
 * @author christopher.ladden (7/13/2009)
 */
ButtonPad::ButtonPad() {

    //ButtonPad pins 
    pinList[0] = 64;
    pinList[1] = 65;
    pinList[2] = 66;
    pinList[3] = 67;
    pinList[4] = 68;
    pinList[5] = 69;    
    
#if 1
	//Arduino LED pins
	//*	order changed by MLS Jan 2, 2009
	ledList[0]	=	21;
	ledList[1]	=	18;
	ledList[2]	=	19;
	ledList[3]	=	20;
	ledList[4]	=	16;
	ledList[5]	=	17;
#else
	//Arduino LED pins
	ledList[0]	=	16;
	ledList[1]	=	17;
	ledList[2]	=	18;
	ledList[3]	=	19;
	ledList[4]	=	20;
	ledList[5]	=	21;
#endif

    //Initialize Power to the ButtonPad through Arduino MEGA Pins 62 and 63
    pinMode(62,OUTPUT);
    pinMode(63,OUTPUT);
    digitalWrite(62,HIGH);
    digitalWrite(63,LOW);

    for (uint8_t pin=0; pin<6; pin++) 
        {
        pinMode(pinList[pin],INPUT);     //pin as input
        digitalWrite(pinList[pin],HIGH); //enable internal pullup
        }

    for (uint8_t pin=0; pin<6; pin++) 
        {
        pinMode(ledList[pin],OUTPUT);     //pin as output
        digitalWrite(ledList[pin],LOW); //enable internal pulldown
        }
}

/******************************************************************************
 * Global Functions
 ******************************************************************************/

/**
 * Read the Buttons 
 * 
 * @author christopher.ladden (7/13/2009)
 * 
 * @return int Returns 0 if nothing is being pressed. Otherwise,
 *         consult the ButtonPad datasheet for Button ID
 */
int ButtonPad::readButtons() {

    uint8_t button = 0;

    for (uint8_t pin=0; pin<6; pin++) {
        button |= digitalRead(pinList[pin]) << pin;
    }

    return button;
}

/**
 * 
 * 
 * @author christopher.ladden (7/13/2009)
 * 
 * @param ledPin The ButtonPad LED to turn on 0 through 5
 * @param state  ON or OFF
 */
void ButtonPad::ledWrite(uint8_t ledPin, uint8_t state)
{
    digitalWrite(ledList[ledPin],state);

}

