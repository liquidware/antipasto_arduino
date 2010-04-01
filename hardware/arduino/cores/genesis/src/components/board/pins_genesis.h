#ifndef Pins_Genesis_h
#define Pins_Genesis_h

#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C"{
#endif

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

#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define digitalPinToSoftPwmChannel(P) ( pgm_read_byte( digital_pin_to_pwm_channel_PGM + (P) ) )

/* Maximum number of pins, for error checking */
#define NUM_PINS sizeof(pinTable)/sizeof(PIN_DESC_T)

/* A structure to describe the pins */
typedef struct {
    uint8_t  *PortReg;      //The port of the pin ex. PORTA, PORTB,...
    uint8_t   PinNum;       //The pin number in the port, usually PA1, PA2,...
    uint8_t  *PinReg;       //The pin input register
    uint8_t  *DirReg;       //The data direction register
    uint8_t  *PCMSK;        //The PCMSK register number
    uint8_t   PCINT;        //The PCINT bit number
} PIN_DESC_T;

extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];
extern const uint8_t PROGMEM digital_pin_to_pwm_channel_PGM[];
extern PIN_DESC_T const pinTable[];

#ifdef __cplusplus
} // extern "C"
#endif

#endif