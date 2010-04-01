
#include <avr/io.h>
#include "wiring.h"
#include "pins_genesis.h"

/* The user pin lookup table.
   This table provides a mapping to the hardware pins. */
PIN_DESC_T const pinTable[] = {

    /* 0 */  { &PORTE, 0, &PINE, &DDRE }, //PCINT0
    /* 1 */  { &PORTE, 1, &PINE, &DDRE },
    /* 2 */  { &PORTE, 2, &PINE, &DDRE },
    /* 3 */  { &PORTE, 3, &PINE, &DDRE },
    /* 4 */  { &PORTE, 4, &PINE, &DDRE },
    /* 5 */  { &PORTE, 5, &PINE, &DDRE },
    /* 6 */  { &PORTE, 6, &PINE, &DDRE },
    /* 7 */  { &PORTE, 7, &PINE, &DDRE },

    /* 8 */  { &PORTD, 2, &PIND, &DDRD },
    /* 9 */  { &PORTD, 3, &PIND, &DDRD },
    /* 10 */ { &PORTD, 4, &PIND, &DDRD },
    /* 11 */ { &PORTD, 6, &PIND, &DDRD },
    /* 12 */ { &PORTD, 7, &PIND, &DDRD },

    /* 13 */ { &PORTG, 0, &PING, &DDRG },

    /* 14 */ { &PORTB, 0, &PINB, &DDRB },
    /* 15 */ { &PORTB, 4, &PINB, &DDRB },
    /* 16 */ { &PORTB, 5, &PINB, &DDRB },
    /* 17 */ { &PORTB, 6, &PINB, &DDRB },

    /* 18 */ { &PORTG, 3, &PING, &DDRG },
    /* 19 */ { &PORTG, 4, &PING, &DDRG },

    /* 20 */ { &PORTD, 0, &PIND, &DDRD },
    /* 21 */ { &PORTD, 1, &PIND, &DDRD },

    /* 22 */ { &PORTG, 1, &PING, &DDRG },

    /* 23 */ { &PORTC, 0, &PINC, &DDRC },
    /* 24 */ { &PORTC, 1, &PINC, &DDRC },
    /* 25 */ { &PORTC, 2, &PINC, &DDRC },
    /* 26 */ { &PORTC, 3, &PINC, &DDRC },
    /* 27 */ { &PORTC, 4, &PINC, &DDRC },
    /* 28 */ { &PORTC, 5, &PINC, &DDRC },
    /* 29 */ { &PORTC, 6, &PINC, &DDRC },
    /* 30 */ { &PORTA, 4, &PINA, &DDRA },
    /* 31 */ { &PORTA, 5, &PINA, &DDRA },
    /* 32 */ { &PORTA, 6, &PINA, &DDRA },
    /* 33 */ { &PORTA, 7, &PINA, &DDRA },
    /* 34 */ { &PORTG, 2, &PING, &DDRG },
    /* 35 */ { &PORTC, 7, &PINC, &DDRC },

    /* 36 */ { &PORTF, 0, &PINF, &DDRF },
    /* 37 */ { &PORTF, 1, &PINF, &DDRF },
    /* 38 */ { &PORTF, 2, &PINF, &DDRF },
    /* 39 */ { &PORTF, 3, &PINF, &DDRF },
    /* 40 */ { &PORTF, 4, &PINF, &DDRF },
    /* 41 */ { &PORTF, 5, &PINF, &DDRF },

    /* 42 */ { &PORTB, 7, &PINB, &DDRB },
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {

    /* 0 */  NO_TIMER,
    /* 1 */  NO_TIMER,
    /* 2 */  NO_TIMER,
    /* 3 */  SOFT_PWM,
    /* 4 */  NO_TIMER,
    /* 5 */  SOFT_PWM,
    /* 6 */  SOFT_PWM,
    /* 7 */  NO_TIMER,

    /* 8 */  NO_TIMER,
    /* 9 */  SOFT_PWM,
    /* 10 */ SOFT_PWM,
    /* 11 */ SOFT_PWM,
    /* 12 */ SOFT_PWM,

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

const uint8_t PROGMEM digital_pin_to_pwm_channel_PGM[] = {

    /* 0 */  NO_SOFTPWM,
    /* 1  */  NO_SOFTPWM,
    /* 2 */  NO_SOFTPWM,
    /* 3 */  SOFTPWM_CHANNEL0,
    /* 4 */  NO_SOFTPWM,
    /* 5 */  SOFTPWM_CHANNEL1,
    /* 6 */  SOFTPWM_CHANNEL2,
    /* 7 */  NO_SOFTPWM,

    /* 8 */  NO_SOFTPWM,
    /* 9 */  SOFTPWM_CHANNEL3,
    /* 10 */ SOFTPWM_CHANNEL4,
    /* 11  */ SOFTPWM_CHANNEL5,
    /* 12 */ SOFTPWM_CHANNEL6,

    /* 13 */ NO_SOFTPWM,

    /* 14 */ NO_SOFTPWM,
    /* 15 */ NO_SOFTPWM,
    /* 16 */ NO_SOFTPWM,
    /* 17 */ NO_SOFTPWM,

    /* 18 */ NO_SOFTPWM,
    /* 19 */ NO_SOFTPWM,

    /* 20 */ NO_SOFTPWM,
    /* 21 */ NO_SOFTPWM,

    /* 22 */ NO_SOFTPWM,
    /* 23 */ NO_SOFTPWM,
    /* 24 */ NO_SOFTPWM,
    /* 25 */ NO_SOFTPWM,
    /* 26 */ NO_SOFTPWM,
    /* 27 */ NO_SOFTPWM,
    /* 28 */ NO_SOFTPWM,
    /* 29 */ NO_SOFTPWM,
    /* 30 */ NO_SOFTPWM,
    /* 31 */ NO_SOFTPWM,
    /* 32 */ NO_SOFTPWM,
    /* 33 */ NO_SOFTPWM,
    /* 34 */ NO_SOFTPWM,
    /* 35 */ NO_SOFTPWM,

    /* 36 */ NO_SOFTPWM,
    /* 37 */ NO_SOFTPWM,
    /* 38 */ NO_SOFTPWM,
    /* 39 */ NO_SOFTPWM,
    /* 40 */ NO_SOFTPWM,
    /* 41 */ NO_SOFTPWM,

    /* 42 */ NO_SOFTPWM,
};