
#include <avr/io.h>
#include "wiring.h"
#include "pins_illuminato.h"


const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {

    /* 0 */  NO_TIMER,
    /* 1 */  NO_TIMER,
    /* 2 */  NO_TIMER,
    /* 3 */  NO_TIMER,
    /* 4 */  NO_TIMER,
    /* 5 */  NO_TIMER,
    /* 6 */  NO_TIMER,
    /* 7 */  NO_TIMER,

    /* 8 */  NO_TIMER,
    /* 9 */  NO_TIMER,
    /* 10 */ NO_TIMER,
    /* 11 */ NO_TIMER,
    /* 12 */ NO_TIMER,

    /* 13 */ NO_TIMER,

    /* 14 */ NO_TIMER,
    /* 15 */ TIMER0A,
    /* 16 */ TIMER1A,
    /* 17 */ TIMER1B,

    /* 18 */ NO_TIMER,
    /* 19 */ NO_TIMER,

    /* 20 */ NO_TIMER,
    /* 21 */ NO_TIMER,

    /* 22 */ NO_TIMER,
    /* 23 */ NO_TIMER,
    /* 24 */ NO_TIMER,
    /* 25 */ NO_TIMER,
    /* 26 */ NO_TIMER,
    /* 27 */ NO_TIMER,
    /* 28 */ NO_TIMER,
    /* 29 */ NO_TIMER,
    /* 30 */ NO_TIMER,
    /* 31 */ NO_TIMER,
    /* 32 */ NO_TIMER,
    /* 33 */ NO_TIMER,
    /* 34 */ NO_TIMER,
    /* 35 */ NO_TIMER,

    /* 36 */ NO_TIMER,
    /* 37 */ NO_TIMER,
    /* 38 */ NO_TIMER,
    /* 39 */ NO_TIMER,
    /* 40 */ NO_TIMER,
    /* 41 */ NO_TIMER,

    /* 42 */ TIMER2A,
};