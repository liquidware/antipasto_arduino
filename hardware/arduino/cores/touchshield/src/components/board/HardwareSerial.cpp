//*******************************************************************************
/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

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
//*******************************************************************************
//*	Edit History
//*******************************************************************************
//*	Nov 23,	2006	Modified by David A. Mellis
//*	Apr  4,	2008	Modified by Chris Ladden
//*	Jan 28,	2009	Modified by Mark Sproul <MLS>
//*	Jan 28,	2009	<MLS> Added available
//*	Jan 29,	2009	<MLS> Finished merging steatlh and slide versions
//*******************************************************************************

#include	<stdio.h>
#include	<string.h>
#include	<inttypes.h>

#include	"HardwareDef.h"
#include	"wiring.h"


#include	"HardwareSerial.h"

// Constructors ////////////////////////////////////////////////////////////////

//*******************************************************************************
HardwareSerial::HardwareSerial(uint8_t uart)
{
	;
}

// Public Methods //////////////////////////////////////////////////////////////

//*******************************************************************************
void HardwareSerial::begin(long speed)
{
#if defined(_TOUCH_SLIDE_) || defined(_NEW_SERIAL_)
//	serialBegin(speed);

	beginSerial(speed);	//*	<MLS> Jan 28, 2009
#endif
#ifdef _TOUCH_STEALTH_
	bitbang_init(); //always 9600
#endif
}

//*******************************************************************************
//*	Jan 28,	2009	<MLS> Added available
int HardwareSerial::available(void)
{
uint8_t	availableFlag;

#if defined(_TOUCH_SLIDE_) || defined(_NEW_SERIAL_)
	availableFlag	=	serialAvailable();
#endif
#ifdef _TOUCH_STEALTH_
	availableFlag	=	true;
#endif
	return(availableFlag);
}


//*******************************************************************************
int HardwareSerial::read(void)
{

#if defined(_TOUCH_SLIDE_) || defined(_NEW_SERIAL_)
	return serialRead();
#endif
#ifdef _TOUCH_STEALTH_
	return (unsigned char)bitbang_receive();
#endif
}

//*******************************************************************************
void HardwareSerial::flush()
{
	;
}

//*******************************************************************************
void HardwareSerial::print(char c)
{
#if defined(_TOUCH_SLIDE_) || defined(_NEW_SERIAL_)
	serialWrite(c);
#endif
#ifdef _TOUCH_STEALTH_
	bitbang_putc(c);
#endif
}

//*******************************************************************************
void HardwareSerial::print(const char c[])
{
	while (*c)
		print(*c++);
}


//*******************************************************************************
void HardwareSerial::print(uint8_t b)
{
#if defined(_TOUCH_SLIDE_) || defined(_NEW_SERIAL_)
	serialWrite(b);
#endif
#ifdef _TOUCH_STEALTH_
	bitbang_putc(b);
#endif
}

//*******************************************************************************
void HardwareSerial::print(int n)
{
	print((long) n);
}

#ifndef __MWERKS__
//*******************************************************************************
void HardwareSerial::print(unsigned int n)
{
	print((unsigned long) n);
}
#endif

//*******************************************************************************
void HardwareSerial::print(long n)
{
	if (n < 0)
	{
		print('-');
		n = -n;
	}
	printNumber(n, 10);
}

//*******************************************************************************
void HardwareSerial::print(unsigned long n)
{
	printNumber(n, 10);
}

//*******************************************************************************
void HardwareSerial::print(long n, int base)
{
	if (base == 0)
		print((char) n);
	else if (base == 10)
		print(n);
	else
		printNumber(n, base);
}

//*******************************************************************************
void HardwareSerial::println(void)
{
	print('\r');
	print('\n');
}

//*******************************************************************************
void HardwareSerial::println(char c)
{
	print(c);
	println();
}

//*******************************************************************************
void HardwareSerial::println(const char c[])
{
	print(c);
	println();
}

//*******************************************************************************
void HardwareSerial::println(uint8_t b)
{
	print(b);
	println();
}

//*******************************************************************************
void HardwareSerial::println(int n)
{
	print(n);
	println();
}

//*******************************************************************************
void HardwareSerial::println(long n)
{
	print(n);
	println();
}

//*******************************************************************************
void HardwareSerial::println(unsigned long n)
{
	print(n);
	println();
}

//*******************************************************************************
void HardwareSerial::println(long n, int base)
{
	print(n, base);
	println();
}

// Private Methods /////////////////////////////////////////////////////////////

//*******************************************************************************
void HardwareSerial::printNumber(unsigned long n, uint8_t base)
{
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	unsigned long i = 0;

	if (base==0)
		base=10;

	if (n == 0)
	{
		print('0');
		return;
	} 

	while (n > 0)
	{
		buf[i++] = n % base;
		n /= base;
	}

	for (; i > 0; i--)
	{
		unsigned char out;
		if (buf[i - 1] < 10)
			out = '0' + buf[i - 1];
		else
			out = 'A' + buf[i - 1] - 10;

		print(out);
	}
}


// Preinstantiate Objects //////////////////////////////////////////////////////
HardwareSerial Serial = HardwareSerial(0);

