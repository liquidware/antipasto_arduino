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
#ifndef ARDUINO_READER_H
#define ARDUINO_READER_H

#include <inttypes.h>

#define MAX_READ_BUFF_SIZE 10

class ArduinoReader
{
    private:
       int readDataSLIP(char * buffer);

    public:
       ArduinoReader(void);
       int Read(char * buffer);
};

#endif
