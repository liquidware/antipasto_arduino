/*
  ButtonShield.h - Liquidware ButtonShield library
  Copyright (c) 2009 Christopher Ladden.  All right reserved.

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

#ifndef INPUTSHIELD_H
#define INPUTSHIELD_H

#include <inttypes.h>

/* The Inputshield's pin description */ 
typedef struct
{
  uint8_t  JoyLateral;
  uint8_t  JoyVertical;
  uint8_t  ButtonA;
  uint8_t  ButtonB;
  uint8_t  ButtonJoy;
} INPUTSHIELD_DESC_T;

/* The InputShield Class */
class InputShield
{
  private:
    INPUTSHIELD_DESC_T _pins;
    uint8_t _averages;
    int sampleAnalogPin(uint8_t pin);

  public:
    InputShield(uint8_t mode);      /* For Mode A mode=0 , For Mode B mode=1 */ 

    int getJoyLat(void);           /* Returns 0 - 1023 for the lateral position. */
    int getJoyVert(void);          /* Returns 0 - 1023 for the vertical position. */
    int getButtonA(void);          /* Returns 1 if button pressed */
    int getButtonB(void);          /* Returns 1 if button pressed */
    int getButtonJoy(void);        /* Returns 1 if button pressed */

    void setMode(uint8_t mode);        /* Mode A = 0, Mode B = 1*/ 
    void setSampleAverages(uint8_t sampleAverages);    /* Sets the joystick ADC sampling average */   
};

#endif
