/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/

#include "wiring_private.h"
#include "pins_genesis.h"
#include "pins_arduino.h"


/* ===========================================================================
*  FUNCTION: pinMode
*
*  DESIGN DESCRIPTION:
*     Changes the pin direction
*
*  PARAMETER LIST:
*     pin - The pin number to be changed.
*     val - INPUT or OUTPUT value to be written to the pin
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*
*
*===========================================================================*/
void pinMode(uint8_t pin, uint8_t mode)
{
    PIN_DESC_T *p = &pinTable[pin];

    /* Check desired mode */
    if (mode == INPUT)
    {
        /* Look up the DDR reg and set the bit */
        CLRBIT(*p->DirReg,
               p->PinNum);
    }
    else
    {
        /* Look up the DDR reg and set the bit */
        SETBIT(*p->DirReg,
                p->PinNum);
    }
}

// Forcing this inline keeps the callers from having to push their own stuff
// on the stack. It is a good performance win and only takes 1 more byte per
// user than calling. (It will take more bytes on the 168.)
//
// But shouldn't this be moved into pinMode? Seems silly to check and do on
// each digitalread or write.
//
static inline void turnOffPWM(uint8_t timer) __attribute__ ((always_inline));
static inline void turnOffPWM(uint8_t timer)
{
	if (timer == TIMER1A) cbi(TCCR1A, COM1A1);
	if (timer == TIMER1B) cbi(TCCR1A, COM1B1);

#if defined(__AVR_ATmega8__)
	if (timer == TIMER2) cbi(TCCR2, COM21);
#elif defined(__AVR_ATmega645__)
	if (timer == TIMER0A) cbi(TCCR0A, COM0A1);
	if (timer == TIMER1A) cbi(TCCR1A, COM1A1);
	if (timer == TIMER1B) cbi(TCCR1A, COM1B1);
	if (timer == TIMER2A) cbi(TCCR2A, COM2A1);
#else
	if (timer == TIMER0A) cbi(TCCR0A, COM0A1);
	if (timer == TIMER0B) cbi(TCCR0A, COM0B1);
	if (timer == TIMER2A) cbi(TCCR2A, COM2A1);
	if (timer == TIMER2B) cbi(TCCR2A, COM2B1);
#endif

#if defined(__AVR_ATmega1280__)
	if (timer == TIMER3A) cbi(TCCR3A, COM3A1);
	if (timer == TIMER3B) cbi(TCCR3A, COM3B1);
	if (timer == TIMER3C) cbi(TCCR3A, COM3C1);
	if (timer == TIMER4A) cbi(TCCR4A, COM4A1);
	if (timer == TIMER4B) cbi(TCCR4A, COM4B1);
	if (timer == TIMER4C) cbi(TCCR4A, COM4C1);
	if (timer == TIMER5A) cbi(TCCR5A, COM5A1);
	if (timer == TIMER5B) cbi(TCCR5A, COM5B1);
	if (timer == TIMER5C) cbi(TCCR5A, COM5C1);
#endif
}

/* ===========================================================================
*  FUNCTION: digitalWrite
*
*  DESIGN DESCRIPTION:
*     Write a value to a pin
*
*  PARAMETER LIST:
*     pin - The pin number to be changed.
*     val - HIGH or LOW value to write to the pin
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*
*
*===========================================================================*/
void digitalWrite(uint8_t pin, uint8_t val)
{
    PIN_DESC_T *p = &pinTable[pin];

	uint8_t timer = digitalPinToTimer(pin);
	if (timer == SOFT_PWM)
	  DeactivateSoftPwm(digitalPinToSoftPwmChannel(pin));
    else if (timer != NO_TIMER)
	  turnOffPWM(timer);

    /* Check desired state */
    if (val == HIGH)
    {
        /* Look up the port reg and set the bit */
        SETBIT(*p->PortReg,
                p->PinNum);
    }
    else
    {
        /* Look up the port reg and set the bit */
        CLRBIT(*p->PortReg,
                p->PinNum);
    }
}

/* ===========================================================================
*  FUNCTION: digitalRead
*
*  DESIGN DESCRIPTION:
*     Reads a digital pin
*
*  PARAMETER LIST:
*     pin - The pin to be read
*
*  RETURNED:
*     returns 0 or the masked pin register value
*
*  DESIGN NOTES/CONSTRAINTS:
*
*
*===========================================================================*/
int digitalRead(uint8_t pin)
{
    PIN_DESC_T *p = &pinTable[pin];

    /* Return the pin register value at the bit location */
    return CHECKBIT(*p->PinReg,
                     p->PinNum) >> p->PinNum;
}
