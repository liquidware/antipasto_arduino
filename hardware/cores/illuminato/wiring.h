#ifndef WIRING_H
#define WIRING_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "binary.h"

#include "wiring_serial.h"

#ifdef __cplusplus
extern "C"{
#endif

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1

#define true 0x1
#define false 0x0

#define PI 3.14159265
#define HALF_PI 1.57079
#define TWO_PI 6.283185
#define DEG_TO_RAD 0.01745329
#define RAD_TO_DEG 57.2957786

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define INTERNAL 3
#define DEFAULT 1
#define EXTERNAL 0

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT)) //!<Set a bit within a byte
#define CLRBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT)) //!< Clear a bit within a byte
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT)) //!< Flip a bit within a byte
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT)) //!< Check a bit within a byte

#define lowByte(w) ((w) & 0xff)
#define highByte(w) ((w) >> 8)

#define TRUE	1	//!< A Global true
#define FALSE	0	//!< A global false
#define ON		1	//!< A boolean
#define OFF		0	//!< A boolean
#define SET		1	//!< A boolean
#define CLEAR	0	//!< A boolean
#define INPUT   0   
#define OUTPUT  1
#define HIGH    1
#define LOW     0

#define bit(b) (1 << (b))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

typedef unsigned int word;
typedef uint8_t boolean;
typedef uint8_t byte;

/* A structure to describe the pins */
typedef struct {
    uint8_t  *PortReg;      //The port of the pin ex. PORTA, PORTB,...
    uint8_t   PinNum;       //The pin number in the port, usually PA1, PA2,...
    uint8_t  *PinReg;       //The pin input register
    uint8_t  *DirReg;       //The data direction register
    uint8_t  *PCMSK;        //The PCMSK register number
    uint8_t   PCINT;        //The PCINT bit number
} PIN_DESC_T;



void init(void);
void setup(void);
void loop(void);

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
uint8_t digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogWrite(uint8_t, int);
unsigned long millis();
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
void bling(uint8_t percent);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
