/*
  HarwareSerial.cpp - Hardware serial library for Wiring
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
  
  Modified 23 November 2006 by David A. Mellis
  Modified 4 April 2008 by Chris Ladden
*/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "wiring.h"


#include "HardwareSerial.h"

// Constructors ////////////////////////////////////////////////////////////////

HardwareSerial::HardwareSerial(uint8_t uart)
{
	;
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(long speed)
{
  serialBegin(speed);
}

int HardwareSerial::read(void)
{
  return serialRead();
}

void HardwareSerial::flush()
{
 ;
}

void HardwareSerial::print(char c)
{
  serialWrite(c);
}

void HardwareSerial::print(const char c[])
{
  	while (*c)
		print(*c++);
}


void HardwareSerial::print(uint8_t b)
{
  serialWrite(b);
}

void HardwareSerial::print(int n)
{
  print((long) n);
}

void HardwareSerial::print(unsigned int n)
{
  print((unsigned long) n);
}

void HardwareSerial::print(long n)
{
  if (n < 0) {
    print('-');
    n = -n;
  }
  printNumber(n, 10);
}

void HardwareSerial::print(unsigned long n)
{
  printNumber(n, 10);
}

void HardwareSerial::print(long n, int base)
{
  if (base == 0)
    print((char) n);
  else if (base == 10)
    print(n);
  else
    printNumber(n, base);
}

void HardwareSerial::println(void)
{
  print('\r');
  print('\n');  
}

void HardwareSerial::println(char c)
{
  print(c);
  println();  
}

void HardwareSerial::println(const char c[])
{
  print(c);
  println();
}

void HardwareSerial::println(uint8_t b)
{
  print(b);
  println();
}

void HardwareSerial::println(int n)
{
  print(n);
  println();
}

void HardwareSerial::println(long n)
{
  print(n);
  println();  
}

void HardwareSerial::println(unsigned long n)
{
  print(n);
  println();  
}

void HardwareSerial::println(long n, int base)
{
  print(n, base);
  println();
}

// Private Methods /////////////////////////////////////////////////////////////

void HardwareSerial::printNumber(unsigned long n, uint8_t base)
{
  	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	unsigned long i = 0;

	if (base==0)
		base=10;

	if (n == 0) {
		print('0');
		return;
	} 

	while (n > 0) {
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

