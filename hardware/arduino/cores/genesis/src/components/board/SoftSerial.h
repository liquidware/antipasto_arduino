/*
  SoftwareSerial.h - Software serial library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

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

#ifndef SOFTSERIAL_H
#define SOFTSERIAL_H

#include <inttypes.h>
#include "Print.h"

#define SS_MAX_RX_BUFF 64

uint16_t whackDelay2(uint16_t delay);
static void recv(void);

class SoftSerial : public Print
{
       
  public:
    SoftSerial(uint8_t, uint8_t);
    int available(void);
    void begin(unsigned long);
    void flush(void);
    int read();
    virtual void write(uint8_t);
    
};



/*
typedef union
{
    typedef void (SoftSerial::* recv)(void);
    void (*f)();
} SOFTSERIAL_MFP_T;
*/
//extern SoftSerial Serial;

#endif

