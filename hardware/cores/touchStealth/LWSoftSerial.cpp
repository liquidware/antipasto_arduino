/*
  SoftwareSerial.cpp - Software serial library
  Copyright (c) 2006 David A. Mellis.  All right reserved. - hacked by ladyada 

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
#include "LWSoftSerial.h"
#include "wiring_private.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

#define LWSS_MAX_RX_BUFF 64

/******************************************************************************
 * Statics
 ******************************************************************************/
static uint8_t _receivePin;
static uint8_t _transmitPin;
static int _bitDelay;

static char _receive_buffer[LWSS_MAX_RX_BUFF]; 
static uint8_t _receive_buffer_index;

#if (F_CPU == 16000000)
void whackDelay(uint16_t delay) { 
  //volatile uint16_t t = delay; 
  //temp uint8_t tmp=0;
   
  while(delay--)
  {
   asm volatile(" NOP ");
  }
  /*
  asm volatile("sbiw    %0, 0x01 \n\t"
	       "ldi %1, 0xFF \n\t"
	       "cpi %A0, 0xFF \n\t"
	       "cpc %B0, %1 \n\t"
	       "brne .-10 \n\t"
	       : "+r" (delay), "+a" (tmp)
	       : "0" (delay)
	       );
	       */
}
#endif

/******************************************************************************
 * Interrupts
 ******************************************************************************/

SIGNAL(SIG_PIN_CHANGE0) {
  //if ((_receivePin >=8) && (_receivePin <= 13)) {
    while(1) { asm("NOP");}
     recv();
  //}
}
//SIGNAL(SIG_PIN_CHANGE2)
//{
//  if (_receivePin <8) {
//    recv();
//  }
//}


void recv(void) { 
  char i, d = 0; 
  //if (digitalRead(_receivePin)) 
  if (CHECKBIT(RXTX_PIN, RX_PIN)) 
    return;       // not ready! 
  whackDelay(_bitDelay + 2); 
  for (i=0; i<8; i++) { 
    //PORTB |= _BV(5); 
    whackDelay(_bitDelay*2 +3);  // digitalread takes some time
    //PORTB &= ~_BV(5); 
    //if (digitalRead(_receivePin))
    if (CHECKBIT(RXTX_PIN, RX_PIN))  
      d |= (1 << i); 
   } 
  whackDelay(_bitDelay*2 + 3);
  if (_receive_buffer_index >=  LWSS_MAX_RX_BUFF)
    return;
  _receive_buffer[_receive_buffer_index] = d; // save data 
  _receive_buffer_index++;  // got a byte 
} 
  


/******************************************************************************
 * Constructors
 ******************************************************************************/

LWSoftSerial::LWSoftSerial(uint8_t receivePin, uint8_t transmitPin)
{
  _receivePin = receivePin;
  _transmitPin = transmitPin;
  _baudRate = 0;
}

void LWSoftSerial::setTX(uint8_t tx) {
  _transmitPin = tx;
}
void LWSoftSerial::setRX(uint8_t rx) {
  _receivePin = rx;
}

/******************************************************************************
 * User API
 ******************************************************************************/

void LWSoftSerial::begin(long speed)
{
  pinMode(_transmitPin, OUTPUT);
//  digitalWrite(_transmitPin, HIGH);
   SETBIT(DDRE,PE6); //setup the TX pin
   SETBIT(PORTE,PE6);
   
//   pinMode(_receivePin, INPUT); 
//   digitalWrite(_receivePin, HIGH);  // pullup!
   CLRBIT(DDRE,PE5); //setup the RX pin
   SETBIT(PORTE,PE5);   

  _baudRate = speed;
  switch (_baudRate) {
  case 115200: // For xmit -only-!
    _bitDelay = 7; break;
  case 57600:
    _bitDelay = 14; break;
  case 38400:
    _bitDelay = 24; break;
  case 31250:
    _bitDelay = 31; break;
  case 19200:
    _bitDelay = 54; break;
  case 9600:
    _bitDelay = 113; break;
  case 4800:
    _bitDelay = 232; break;
  case 2400:
    _bitDelay = 470; break;
  default:
    _bitDelay = 0;
  }    

   if (_receivePin < 8) {
     SETBIT(EIMSK, PCIE0);         //PCINT0..7 bank
     SETBIT(PCMSK0, PE5);           //PINE5
  } else if (_receivePin <= 13) {
    // a PINB pin, PCINT0-5
    //PCICR |= _BV(0);    
    //PCMSK0 |= _BV(_receivePin-8);
  } 

  whackDelay(_bitDelay*2); // if we were low this establishes the end
}

int LWSoftSerial::read(void)
{
  uint8_t d,i;

  if (! _receive_buffer_index)
    return -1;

  d = _receive_buffer[0]; // grab first byte
  // if we were awesome we would do some nifty queue action
  // sadly, i dont care
  for (i=0; i<_receive_buffer_index; i++) {
    _receive_buffer[i] = _receive_buffer[i+1];
  }
  _receive_buffer_index--;
  return d;
}

uint8_t LWSoftSerial::available(void)
{
  return _receive_buffer_index;
}

void LWSoftSerial::print(uint8_t b)
{
  if (_baudRate == 0)
    return;
  byte mask;

  cli();  // turn off interrupts for a clean txmit

  //digitalWrite(_transmitPin, LOW);  // startbit
  CLRBIT(RXTX_PORT, TX_PIN);
  whackDelay(_bitDelay*2);

  for (mask = 0x01; mask; mask <<= 1) {
    if (b & mask){ // choose bit
      //digitalWrite(_transmitPin,HIGH); // send 1
      SETBIT(RXTX_PORT, TX_PIN);
    }
    else{
      //digitalWrite(_transmitPin,LOW); // send 1
      CLRBIT(RXTX_PORT, TX_PIN);
    }
    whackDelay(_bitDelay*2);
  }
  
  //digitalWrite(_transmitPin, HIGH);
  SETBIT(RXTX_PORT, TX_PIN);
  sei();  // turn interrupts back on. hooray!
  whackDelay(_bitDelay*2);
}

void LWSoftSerial::print(const char *s)
{
  while (*s)
    print(*s++);
}

void LWSoftSerial::print(char c)
{
  print((uint8_t) c);
}

void LWSoftSerial::print(int n)
{
  print((long) n);
}

void LWSoftSerial::print(unsigned int n)
{
  print((unsigned long) n);
}

void LWSoftSerial::print(long n)
{
  if (n < 0) {
    print('-');
    n = -n;
  }
  printNumber(n, 10);
}

void LWSoftSerial::print(unsigned long n)
{
  printNumber(n, 10);
}

void LWSoftSerial::print(long n, int base)
{
  if (base == 0)
    print((char) n);
  else if (base == 10)
    print(n);
  else
    printNumber(n, base);
}

void LWSoftSerial::println(void)
{
  print('\r');
  print('\n');  
}

void LWSoftSerial::println(char c)
{
  print(c);
  println();  
}

void LWSoftSerial::println(const char c[])
{
  print(c);
  println();
}

void LWSoftSerial::println(uint8_t b)
{
  print(b);
  println();
}

void LWSoftSerial::println(int n)
{
  print(n);
  println();
}

void LWSoftSerial::println(long n)
{
  print(n);
  println();  
}

void LWSoftSerial::println(unsigned long n)
{
  print(n);
  println();  
}

void LWSoftSerial::println(long n, int base)
{
  print(n, base);
  println();
}

// Private Methods /////////////////////////////////////////////////////////////

void LWSoftSerial::printNumber(unsigned long n, uint8_t base)
{
  unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
  unsigned long i = 0;

  if (n == 0) {
    print('0');
    return;
  } 

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    print((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}
