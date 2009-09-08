#include "wiring_softpwm.h"
#include "pins_illuminato.h"

// List of available SoftPWM channels
SOFTPWM_CHANNEL_T const pwmChannels[CHMAX] = {
	{ &PORTE, 3 },  /* Pin 3 */
	{ &PORTE, 5 },  /* Pin 5 */
	{ &PORTE, 6 },  /* Pin 6 */
	{ &PORTD, 3 },  /* Pin 9 */
	{ &PORTD, 4 },  /* Pin 10 */
	{ &PORTD, 6 },  /* Pin 11 */
	{ &PORTD, 7 },  /* Pin 12 */
};

volatile unsigned char compbuff[CHMAX];
unsigned char compare[CHMAX];
unsigned char activeChannelsMask = 0;

void ActivateSoftPwm(uint8_t channel, int val)
{
	TIMSK2 = (1<<TOIE2);  // enable timer 2 interrupts, for software PWM
	compbuff[channel] = val;
	SETBIT(activeChannelsMask, channel);
}

void DeactivateSoftPwm(uint8_t channel)
{
	CLRBIT(activeChannelsMask, channel);
	if (activeChannelsMask == 0)
		TIMSK2 = (0<<TOIE2);
}

// Capture the Timer 2 overflow interrupt for Soft PWM
SIGNAL(SIG_OVERFLOW2)
{
  static unsigned char pwmReg = PWM_STARTMASK;
  static unsigned char softcount=0xFF;
  
  // update outputs
  if (CHECKBIT(activeChannelsMask, 0)) {
  if (CHECKBIT(pwmReg, 0)) SETBIT( *pwmChannels[0].portReg, pwmChannels[0].pin); 
	else CLRBIT( *pwmChannels[0].portReg, pwmChannels[0].pin); }
	
  if (CHECKBIT(activeChannelsMask, 1)) {
  if (CHECKBIT(pwmReg, 1)) SETBIT( *pwmChannels[1].portReg, pwmChannels[1].pin); 
	else CLRBIT( *pwmChannels[1].portReg, pwmChannels[1].pin); }
	
  if (CHECKBIT(activeChannelsMask, 2)) {
  if (CHECKBIT(pwmReg, 2)) SETBIT( *pwmChannels[2].portReg, pwmChannels[2].pin); 
	else CLRBIT( *pwmChannels[2].portReg, pwmChannels[2].pin); }
	
  if (CHECKBIT(activeChannelsMask, 3)) {
  if (CHECKBIT(pwmReg, 3)) SETBIT( *pwmChannels[3].portReg, pwmChannels[3].pin); 
	else CLRBIT( *pwmChannels[3].portReg, pwmChannels[3].pin); }
	
  if (CHECKBIT(activeChannelsMask, 4)) {
  if (CHECKBIT(pwmReg, 4)) SETBIT( *pwmChannels[4].portReg, pwmChannels[4].pin); 
	else CLRBIT( *pwmChannels[4].portReg, pwmChannels[4].pin); }
	
  if (CHECKBIT(activeChannelsMask, 5)) {
  if (CHECKBIT(pwmReg, 5)) SETBIT( *pwmChannels[5].portReg, pwmChannels[5].pin); 
	else CLRBIT( *pwmChannels[5].portReg, pwmChannels[5].pin); }
	
  if (CHECKBIT(activeChannelsMask, 6)) {
  if (CHECKBIT(pwmReg, 6)) SETBIT( *pwmChannels[6].portReg, pwmChannels[6].pin); 
	else CLRBIT( *pwmChannels[6].portReg, pwmChannels[6].pin); }
	


  if(++softcount == 0){         // increment modulo 256 counter and update
                                // the compare values only when counter = 0.
	  pwmReg = PWM_STARTMASK;
	  compare[0] = compbuff[0];  // Verbose for speed
	  compare[1] = compbuff[1];
	  compare[2] = compbuff[2];
	  compare[3] = compbuff[3];
	  compare[4] = compbuff[4];
	  compare[5] = compbuff[5];
	  compare[6] = compbuff[6];
  }
  
  // clear port pin on compare match (executed on next interrupt)
  if(compare[0] == softcount) CLRBIT(pwmReg, 0);
  if(compare[1] == softcount) CLRBIT(pwmReg, 1);
  if(compare[2] == softcount) CLRBIT(pwmReg, 2);
  if(compare[3] == softcount) CLRBIT(pwmReg, 3);
  if(compare[4] == softcount) CLRBIT(pwmReg, 4);
  if(compare[5] == softcount) CLRBIT(pwmReg, 5);
  if(compare[6] == softcount) CLRBIT(pwmReg, 6);

}
