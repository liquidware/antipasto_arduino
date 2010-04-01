/*
  ArduinoReader - Arduino Serial Read Library for use with the TouchShield from Liquidware
            TouchShield Slide : http://www.liquidware.com/shop/show/TSL/TouchShield+Slide
            TouchShield Stealth : http://www.liquidware.com/shop/show/TS/TouchShield+Stealth
            Arduino Duemilanove : http://www.liquidware.com/shop/show/ARD/Arduino+Duemilanove

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

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>
#include <string.h>
#include "wiring.h"
#include "HardwareSerial.h"
#include "ArduinoReader.h"

/******************************************************************************
 * Statics
 ******************************************************************************/

/* none */

/******************************************************************************
 * Constructors
 ******************************************************************************/

ArduinoReader::ArduinoReader(void ) {
    Serial.begin(9600);
}

/******************************************************************************
 * User API
 ******************************************************************************/

int ArduinoReader::Read(char * buffer) {
int readLen;

    /* Get the serial data */
    readLen = readDataSLIP(buffer);

    return readLen;
}

/******************************************************************************
 * Private Methods
 ******************************************************************************/

//***************************************
//* Reads characters from serial
//* using a SLIP packet format.
//*    Notes:
//*       ESC = 'S'
//*       TESC = 'T'
//***************************************
int ArduinoReader::readDataSLIP(char * buffer)
{
  int ii=0;

   /* Wait for at least 2 bytes */
   if (serialAvailable())
   {
     char inChar = Serial.read();

     /* Start of frame */
     if (inChar == 'S')
     {

     /* Keep reading bytes */
     while(1)
     {

       while(!serialAvailable()) { ; }
       inChar = Serial.read();

         if (inChar == 'S')
         {
           break;
         }

        if (inChar == 'T')
        {
                 while(!serialAvailable()) { ; }
                 inChar = Serial.read();
        }

          buffer[ii++] = inChar;
     }
     }
  }

  return ii;
}
