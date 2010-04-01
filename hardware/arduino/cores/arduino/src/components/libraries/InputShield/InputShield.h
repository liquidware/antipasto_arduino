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

#ifndef INPUTSHIELD_H
#define INPUTSHIELD_H

#include <inttypes.h>
#include "AFSoftSerial.h"

#define INPUTSHIELD_MODE_A  0
#define INPUTSHIELD_MODE_B  1

/* The Inputshield's pin description */
typedef struct
{
  uint8_t  JoyLateral;
  uint8_t  JoyVertical;
  uint8_t  ButtonA;
  uint8_t  ButtonB;
  uint8_t  ButtonJoy;
  uint8_t  Vibrate;
} INPUTSHIELD_DESC_T;

/* The InputShield Class */
class InputShield
{
  private:
    INPUTSHIELD_DESC_T _pins;
    uint8_t _averages;
    int sampleAnalogPin(uint8_t pin);
    void sendData(unsigned char *data, unsigned char size, AFSoftSerial & mySerial);
    uint8_t iMode;

  public:
    InputShield(uint8_t mode);      /* For Mode A mode=0 , For Mode B mode=1 */
    InputShield(uint8_t mode, AFSoftSerial & serial);

    int GetJoyLat(void);           /* Returns 0 - 1023 for the lateral position. */
    int GetJoyVert(void);          /* Returns 0 - 1023 for the vertical position. */
    int GetButtonA(void);          /* Returns 1 if button pressed */
    int GetButtonB(void);          /* Returns 1 if button pressed */
    int GetButtonJoy(void);        /* Returns 1 if button pressed */

    void SetMode(uint8_t mode);        /* Mode A = 0, Mode B = 1*/
    void SetSampleAverages(uint8_t sampleAverages);    /* Sets the joystick ADC sampling average */
    void SendHardwareState(AFSoftSerial & mySerial);
    void Vibrate(uint16_t duration);
};

#endif
