/*
  LibCompass - A Compass Library for the Arduino.

  Supported compass modules:
    HMC6352 Compass Module - http://moderndevice.com/products/hmc6352-breakout-compass-sensor

  Created by Christopher Ladden from Modern Device on December 2009.

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

#ifndef LIB_COMPASS_H
#define LIB_COMPASS_H

#include <inttypes.h>

//Commpass Address and Commands from the HMC6352 datasheet
typedef enum {
    hmc6352_Address      = 0x21,
    hmc6352_WriteEEPROM  = 'w',
    hmc6352_ReadEEPROM   = 'r',
    hmc6352_WriteRAM     = 'G',
    hmc6352_ReadRAM      = 'g',
    hmc6352_Sleep        = 'S',
    hmc6352_Wakeup       = 'W',
    hmc6352_BridgeOffset = 'O',
    hmc6352_EnterCal     = 'C',
    hmc6352_ExitCal      = 'E',
    hmc6352_SaveOPMode   = 'L',
    hmc6352_GetData      = 'A',
} HMC6352_SENSOR_T;

class LibCompass
{
  private:
    /* Nothing */

  public:
    LibCompass(uint8_t CompassType);
    float GetHeading(void);
    bool Calibrate(void);
    void Wake(void);
    void Sleep(void);
};

#endif
