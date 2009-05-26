#ifndef Wiring_softpwm_h
#define Wiring_softpwm_h

#include "wiring.h"

#define CHMAX 7
#define CHANNEL_NOT_SET 0
#define BASE_PWM_COMPARE 0x00
#define PWM_STARTMASK 0xFF

void ActivateSoftPwm(uint8_t channel, int value);
void DeactivateSoftPwm(uint8_t channel);

/* A structure to describe the the SoftPWM channel */
typedef struct {
	uint8_t     *portReg;
	uint8_t      pin;       // The pin identified in this Channel
} SOFTPWM_CHANNEL_T;

#endif