
#include <avr/io.h>
#include "wiring.h"
#include "pins_illuminato.h"


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