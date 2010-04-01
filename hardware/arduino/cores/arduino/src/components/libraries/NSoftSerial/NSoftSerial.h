/*
  Name:
        NSoftSerial.cpp - Concurrent Software Serial library.
 
  Description:
        An Arduino serial library that supports N Software serial
        instances where N <= 3. 
 
  Copyright (c) 2009 Christopher Ladden  All right reserved.

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

#ifndef NSoftSerial_h
#define NSoftSerial_h

#include <inttypes.h>
#include "Print.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

#define NSS_MAX_RX_BUFF 16
#define NSS_MAX_UARTS   3

/******************************************************************************
 * Structures
 ******************************************************************************/

typedef struct {
    uint8_t enabled;
    uint8_t tx_pinNum;
    uint8_t rx_pinNum;
    int     bitDelay;
    int     head;
    int     tail;
    char    rx_buffer[NSS_MAX_RX_BUFF];
} UART_T;

/******************************************************************************
 * Global Functions and Class Definitions
 ******************************************************************************/

uint16_t whackDelay2(uint16_t delay);
static void recv(void);

char* serial_getNewRxBufferPtr(uint8_t rxPinNum);
char* serial_getRxBufferPtr(uint8_t rxPinNum);
unsigned char* serial_getRxBufferIndex(uint8_t rxPinNum);
int serial_getBitDelay(uint8_t rxPinNum);



class NSoftSerial : public Print
{
  private:
    volatile UART_T* _uart;

    void printNumber(unsigned long, uint8_t);

  public:
    NSoftSerial(uint8_t, uint8_t);
    void setTX(uint8_t tx);
    void setRX(uint8_t rx);
    void begin(long);
    uint8_t available(void);
    int read();
    void flush(void);
    virtual void write(uint8_t);
};

#endif

