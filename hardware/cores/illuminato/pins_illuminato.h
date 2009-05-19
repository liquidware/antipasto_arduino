#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NO_TIMER 0
#define TIMER0A 1
#define TIMER1A 2
#define TIMER1B 3
#define TIMER2A 4

extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )

#endif