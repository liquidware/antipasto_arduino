#ifndef __ADC_SAMPLING_H_
#define __ADC_SAMPLING_H_

#ifdef __cplusplus
extern "C"{
#endif

#define ADC_TOTAL_CHANNELS	2
#define ADC_DELAY_MAX		2 //every 3*128us = 376us


#define ADC_CHAN0			0
#define ADC_CHAN1			1

extern unsigned long mSec;

#ifdef __cplusplus
} // extern "C"
#endif

#endif

