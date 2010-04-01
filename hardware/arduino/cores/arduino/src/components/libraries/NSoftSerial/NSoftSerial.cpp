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

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <avr/interrupt.h>
#include "WConstants.h"
#include "NSoftSerial.h"

/******************************************************************************
 * Statics
 ******************************************************************************/
static volatile UART_T* gRxBufferPtr;
static volatile uint8_t gUARTIndex = 0;

volatile UART_T gUART[NSS_MAX_UARTS];

#if (F_CPU == 16000000)
void whackDelay(uint16_t delay) { 
  uint8_t tmp=0;

  asm volatile("sbiw    %0, 0x01 \n\t"
	       "ldi %1, 0xFF \n\t"
	       "cpi %A0, 0xFF \n\t"
	       "cpc %B0, %1 \n\t"
	       "brne .-10 \n\t"
	       : "+r" (delay), "+a" (tmp)
	       : "0" (delay)
	       );
}
#endif

/******************************************************************************
 * Interrupts
 ******************************************************************************/

SIGNAL(SIG_PIN_CHANGE0) {
	cli();
    recv();
	sei();
}

SIGNAL(SIG_PIN_CHANGE2)
{
	cli();
    recv();
	sei();
}


void recv(void) { 
  char i, d = 0; 
  volatile uint8_t b = 1;
  volatile int index;

  if (gUART[0].enabled && (digitalRead(gUART[0].rx_pinNum) == LOW) ) {  
      gRxBufferPtr = &gUART[0];

	  //dummy delay
	  whackDelay(21);
	  asm("nop");
	  asm("nop");
	  asm("nop");

  } 
  else if (gUART[1].enabled && (digitalRead(gUART[1].rx_pinNum) == LOW) ) {
      gRxBufferPtr = &gUART[1];

	  //dummy delay
	  whackDelay(8);
	  asm("nop");
	  asm("nop");
	  asm("nop");

  }
  else if (gUART[2].enabled && (digitalRead(gUART[2].rx_pinNum) == LOW) ) {
      gRxBufferPtr = &gUART[2];
  }
  else {
	  return;
  }

  /* Error checking */
  if (digitalRead(gRxBufferPtr->rx_pinNum)) 
    return;       // not ready! 

  /* Delay 1/2 bit time */
  whackDelay(gRxBufferPtr->bitDelay - 38);

    for (i=0; i<8; i++) {

        whackDelay((gRxBufferPtr->bitDelay*2)-3);  // digitalread takes some time

        if (digitalRead(gRxBufferPtr->rx_pinNum)) {

            /* Set the bit */
            d |= b; 

        } else {

            /* Balance out the if statement with delays */
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
        }

        /* Shift the bit */
        b = b << 1;
    }

  whackDelay(gRxBufferPtr->bitDelay*2);

  //Store the character
  index = (gRxBufferPtr->head + 1) % NSS_MAX_RX_BUFF;

    if (index != gRxBufferPtr->tail) {
        gRxBufferPtr->rx_buffer[gRxBufferPtr->head] = d;
        gRxBufferPtr->head = index;
    }
} 
  


/******************************************************************************
 * Constructors
 ******************************************************************************/

NSoftSerial::NSoftSerial(uint8_t receivePin, uint8_t transmitPin)
{
    /* Get a new uart */
    _uart = &gUART[gUARTIndex];
    _uart->rx_pinNum = receivePin;
    _uart->tx_pinNum = transmitPin;

    gUARTIndex++; //incremement for the uart for the next software serial instance
  
}

void NSoftSerial::setTX(uint8_t tx) {
    _uart->tx_pinNum = tx;
}
void NSoftSerial::setRX(uint8_t rx) {
    _uart->rx_pinNum = rx;
}

/******************************************************************************
 * User API
 ******************************************************************************/

void NSoftSerial::begin(long speed)
{
  pinMode(_uart->tx_pinNum, OUTPUT);
  digitalWrite(_uart->tx_pinNum, HIGH);

  pinMode(_uart->rx_pinNum, INPUT); 
  digitalWrite(_uart->rx_pinNum, HIGH);  // pullup!

  switch (speed) {
  case 115200: // For xmit -only-!
    _uart->bitDelay = 4; break;
  case 57600:  // For xmit -only-!
    _uart->bitDelay = 14; break;
  case 38400:  // For xmit -only-!
    _uart->bitDelay = 24; break;
  case 31250:  // For xmit -only-!
    _uart->bitDelay = 31; break;
  case 19200:
    _uart->bitDelay = 52; break;
  case 9600:
    _uart->bitDelay = 112; break;
  case 4800:
    _uart->bitDelay = 230; break;
  case 2400:
    _uart->bitDelay = 468; break;
  default:
    _uart->bitDelay = 0;
  }    

   if (_uart->rx_pinNum < 8) {
     // a PIND pin, PCINT16-23
     PCMSK2 |= _BV(_uart->rx_pinNum);
     PCICR |= _BV(2);
  } else if (_uart->rx_pinNum <= 13) {
    // a PINB pin, PCINT0-5
    PCICR |= _BV(0);    
    PCMSK0 |= _BV(_uart->rx_pinNum-8);
  } 

  whackDelay(_uart->bitDelay*2); // if we were low this establishes the end

  _uart->enabled = true;
}

int NSoftSerial::read(void)
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (_uart->head == _uart->tail) {
		return -1;
	} else {
		unsigned char c = _uart->rx_buffer[_uart->tail];
		_uart->tail = (_uart->tail + 1) % NSS_MAX_RX_BUFF;
		return c;
	}
}

uint8_t NSoftSerial::available(void) {

    return (NSS_MAX_RX_BUFF + _uart->head - _uart->tail) % NSS_MAX_RX_BUFF;

}

void NSoftSerial::write(uint8_t c) {
    byte mask;

    if (!_uart->enabled)
        return;

    cli();  // turn off interrupts for a clean txmit

    digitalWrite(_uart->tx_pinNum, LOW);  // startbit
    whackDelay(_uart->bitDelay*2);

    for (mask = 0x01; mask; mask <<= 1) {
        if (c & mask) { // choose bit
            digitalWrite(_uart->tx_pinNum,HIGH); // send 1
        } else {
            digitalWrite(_uart->tx_pinNum,LOW); // send 0
            asm("nop"); //set/clear timing balance
            asm("nop"); //set/clear timing balance
            asm("nop"); //set/clear timing balance
        }
        whackDelay(_uart->bitDelay*2);
    }

    digitalWrite(_uart->tx_pinNum, HIGH);
    sei();  // turn interrupts back on. hooray!
    whackDelay(_uart->bitDelay*2);
}
