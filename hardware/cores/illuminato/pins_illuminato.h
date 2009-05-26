#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NO_TIMER 0
#define TIMER0A 1
#define TIMER1A 2
#define TIMER1B 3
#define TIMER2A 4
#define SOFT_PWM 5

#define SOFTPWM_CHANNEL0 0
#define SOFTPWM_CHANNEL1 1
#define SOFTPWM_CHANNEL2 2
#define SOFTPWM_CHANNEL3 3
#define SOFTPWM_CHANNEL4 4
#define SOFTPWM_CHANNEL5 5
#define SOFTPWM_CHANNEL6 6
#define NO_SOFTPWM 255

extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];
extern const uint8_t PROGMEM digital_pin_to_pwm_channel_PGM[];

#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define digitalPinToSoftPwmChannel(P) ( pgm_read_byte( digital_pin_to_pwm_channel_PGM + (P) ) )

#endif