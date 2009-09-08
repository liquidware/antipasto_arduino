/*
  SoftwareSerial.cpp - Software serial library
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
  
  - hacked by ladyada 
  - hacked by Chris Ladden
  
*/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <avr/interrupt.h>
#include "wiring.h"
#include "SoftSerial.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Global Variables
 ******************************************************************************/
static uint8_t _receivePin;
static uint8_t _transmitPin;
volatile int _bitDelay;
volatile char _receive_buffer[SS_MAX_RX_BUFF]; 
volatile uint8_t _receive_buffer_index;
volatile int _rx_buffer_head = 0;
volatile int _rx_buffer_tail = 0;
unsigned long _baudRate;

/******************************************************************************
 * Interrupts
 ******************************************************************************/
#if (F_CPU == 16000000)
void whackDelay(uint16_t delay)
{
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


/* ===========================================================================
*  FUNCTION: SIG_PIN_CHANGE0
*
*  DESIGN DESCRIPTION:
*     Pin Change Interrupt handler
*
*  PARAMETER LIST:
*     none
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*===========================================================================*/
SIGNAL(SIG_PIN_CHANGE0) 
{
    /* Handle the pin change */
    recv();
}

/* ===========================================================================
*  FUNCTION: recv
*
*  DESIGN DESCRIPTION:
*     Handles the bit banging receive.
*
*  PARAMETER LIST:
*     none
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*===========================================================================*/
void recv(void)
{
    char bit, d = 0; 

    /* Error checking */
    if (digitalRead(_receivePin))
        return;       // not ready! 

    /* Delay */
    whackDelay(_bitDelay-5);

    /* Read the incoming waveform */
    for (bit=0; bit<8; bit++)
    {
        whackDelay(_bitDelay*2);  // digitalread takes some time
        if (digitalRead(_receivePin))
            d |= (1 << bit);
    }
    
    /* Stop bit delay */ 
    whackDelay(_bitDelay*2);

    int index = (_rx_buffer_head + 1) % SS_MAX_RX_BUFF;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (index != _rx_buffer_tail) {
		_receive_buffer[_rx_buffer_head] = d;
		_rx_buffer_head = index;

	}

} 


/******************************************************************************
 * Constructors
 ******************************************************************************/

/* ===========================================================================
*  FUNCTION: SoftSerial
*
*  DESIGN DESCRIPTION:
*     SoftSerial Constructor
*
*  PARAMETER LIST:
*     none
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*     Important: only works for illuminato pins 0-7
* 
*===========================================================================*/
SoftSerial::SoftSerial(uint8_t receivePin, uint8_t transmitPin)
{
    _receivePin = receivePin;
    _transmitPin = transmitPin;
    _baudRate = 0;
}

/******************************************************************************
 * User API
 ******************************************************************************/
 
/* ===========================================================================
*  FUNCTION: begin
*
*  DESIGN DESCRIPTION:
*     Setup the pins and baud rate for the connection
*
*  PARAMETER LIST:
*     speed : The baud rate of our connection. See case statement 
              below for supported baud rates.
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*===========================================================================*/
void SoftSerial::begin(unsigned long speed)
{

    pinMode(_transmitPin, OUTPUT);
    digitalWrite(_transmitPin, HIGH);

    pinMode(_receivePin, INPUT); 
    digitalWrite(_receivePin, HIGH);  // pullup!

    _baudRate = speed;
    switch (_baudRate)
    {
    case 115200: // For xmit -only-!
        _bitDelay = 5; break;
    case 57600:
        _bitDelay = 14; break;
    case 38400:
        _bitDelay = 24; break;
    case 31250:
        _bitDelay = 31; break;
    case 19200:
        _bitDelay = 54; break;
    case 9600:
        _bitDelay = 114; break;
    case 4800:
        _bitDelay = 232; break;
    case 2400:
        _bitDelay = 470; break;
    default:
        _bitDelay = 0;
    }    
    

    if (_receivePin < 8)
    {
        SETBIT(PCMSK0,_receivePin);      //Set the pin bit
        SETBIT(EIMSK,PCIE0);     //Enable PCINT7..0 interrupts
        sei(); //enable interrupts
    }

    whackDelay(_bitDelay*2); // if we were low this establishes the end
}

/* ===========================================================================
*  FUNCTION: read
*
*  DESIGN DESCRIPTION:
*     Read a byte off the buffer.
*
*  PARAMETER LIST:
*     none
*
*  RETURNED:
*     -1 : if we fail
*      c : the next char available in the buffer
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*===========================================================================*/
int SoftSerial::read(void)
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (_rx_buffer_head == _rx_buffer_tail) {
		return -1;
	} else {
		unsigned char c = _receive_buffer[_rx_buffer_tail];
		_rx_buffer_tail = (_rx_buffer_tail + 1) % SS_MAX_RX_BUFF;
		return c;
	}
}

/* ===========================================================================
*  FUNCTION: available
*
*  DESIGN DESCRIPTION:
*     Check to see if a byte is available in the buffer
*
*  PARAMETER LIST:
*     none
*
*  RETURNED:
*     the number of available chars
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*===========================================================================*/
int SoftSerial::available(void)
{
    return (SS_MAX_RX_BUFF + _rx_buffer_head - _rx_buffer_tail) % SS_MAX_RX_BUFF;
}

void SoftSerial::write(uint8_t b)
{
    if (_baudRate == 0)
        return;
    uint8_t mask;
    cli();  // turn off interrupts for a clean txmit

    digitalWrite(_transmitPin, LOW);  // startbit
    whackDelay(_bitDelay*2);

    for (mask = 0x01; mask; mask <<= 1)
    {
        if (b & mask) // choose bit
        {
            digitalWrite(_transmitPin,HIGH); // send 1
        } else
        {
            digitalWrite(_transmitPin,LOW); // send 1
        }
        whackDelay(_bitDelay*2);
    }

    digitalWrite(_transmitPin, HIGH);
    whackDelay(_bitDelay*2);              //wait to finish the 
                                          //transmit before interrupts come back on.
                                          
    sei();  // turn interrupts back on. hooray!
    
}
