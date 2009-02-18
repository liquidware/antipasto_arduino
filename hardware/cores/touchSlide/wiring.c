//*******************************************************************************
/* 

` wiring.c - Partial implementation of the Wiring API for the ATmega645.
` wiring.c - Partial implementation of the Wiring API for the ATmega2560.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis


  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
	
  **Adapted to the TouchShield and ported to the ATmega645 - Chris Ladden
  **Adapted to the TouchShield and ported to the ATmega2560 - Chris Ladden

*/
//*******************************************************************************
//*	Jan 28,	2009	<MLS> working on merging Shield and Slide version
//*	Jan 29,	2009	<MLS> Finished merging  Shield and Slide versions
//*******************************************************************************


#include	<avr/io.h>
#include	<stdlib.h>
#include	<avr/interrupt.h>


#include	"HardwareDef.h"

#include	"wiring_private.h"

#include	"bitops.h"
#ifndef _TOUCHSCREEN_H_
	#ifdef _TOUCH_STEALTH_
		#include	"touchscreen_stealth.h"
	#endif
	#ifdef _TOUCH_SLIDE_
		#include	"touchscreen.h"
	#endif
#endif
#include	"adc_sampling.h"

#ifdef _TOUCH_STEALTH_
	volatile unsigned char	adcCurrChan		=	0;
	volatile unsigned char	adc_delay		=	0;
#endif
volatile unsigned char	millisecond_cnt;
volatile unsigned long	millis_var			=	0;
volatile unsigned char	millis_var_counter	=	0;

#ifdef _TOUCH_SLIDE_
	typedef enum
	{
		RTI_TASK0,
		RTI_TASK1,
		RTI_TASK2,
		RTI_TASK3,
		RTI_TASK4,
		RTI_TASK5,
		RTI_TASK_MAX =40,
	} RTI_TASK;

	RTI_TASK curr_task = RTI_TASK0;

	volatile unsigned char mSecCnt	=	0;
	unsigned long mSec;
#endif

//*******************************************************************************
//This is our 128uS time base from timer 0
//
//*******************************************************************************
SIGNAL (SIG_OVERFLOW0) //every 128us
{

	/* Millisecond Counter */
	if (millis_var_counter == MILLISECOND_CNT_MAX)
	{
		millis_var++;
		millis_var_counter=0;
	}
	else
	{
		millis_var_counter++;
	}

	if (millisecond_cnt < MILLISECOND_CNT_MAX)
	{
		millisecond_cnt++;
	}

#ifdef _TOUCH_SLIDE_
	/* RTI Tasks */
	if (curr_task == RTI_TASK0)
	{
		touchscreen_setup_x();
		SETBIT(ADCSRA,ADSC); //start ADC conversion!
	}
	else

	if (curr_task == RTI_TASK1)
	{	
		touchscreen_process_x(ADC);
	}
	else

	if (curr_task == RTI_TASK2)
	{	
		touchscreen_setup_y();
		SETBIT(ADCSRA,ADSC); //start ADC conversion!
	}
	else

	if (curr_task == RTI_TASK3)
	{	
		touchscreen_process_y(ADC);
		touchscreen_setup_z1();
		SETBIT(ADCSRA,ADSC); //start ADC conversion!
	}
	else

	if (curr_task == RTI_TASK4)
	{	
		touchscreen_process_z1(ADC);
		touchscreen_setup_z2(); 
		SETBIT(ADCSRA,ADSC); //start ADC conversion!
		
	}
	else

	if (curr_task == RTI_TASK5)
	{	
	 touchscreen_process_z2(ADC);
	 touchscreen_service();	
	}

	/* Next task */
	curr_task++;
	if (curr_task == RTI_TASK_MAX)
	{
		/* Reset the Task Number */
		curr_task = RTI_TASK0;
	}
#endif
#ifdef _TOUCH_STEALTH_
	if (adc_delay == ADC_DELAY_MAX)
	{
	
		adc_delay = 0; //reset the counter
		adcCurrChan++; //next channel
		if (adcCurrChan == ADC_TOTAL_CHANNELS) 
		{
			adcCurrChan = 0; //loop back around to the first channel
		}
		
		switch(adcCurrChan)
		{
			case ADC_CHAN0:
				//Touchscreen: XPLUS
				touchscreen_setup_x();
				SETBIT(ADCSRA,ADSC); //start conversion!
				break;



			case ADC_CHAN1:
				//Touchscreen: YMINUS
				touchscreen_setup_y();
				SETBIT(ADCSRA,ADSC); //start conversion!
				break;

		}//end switch

	}
	else
	{
		adc_delay++; //increment the counter
	}

#endif

} //end interrupt



#ifdef _TOUCH_STEALTH_
//*******************************************************************************
//ADC sample complete!
///*******************************************************************************
SIGNAL (SIG_ADC) 
{

	switch(adcCurrChan)
		{

		case ADC_CHAN0:
		touchscreen_process_x(ADCH); 
		break;

		case ADC_CHAN1:
		touchscreen_process_y(ADCH);
		break;


		}

}
#endif




//*******************************************************************************
unsigned long millis()
{
	return millis_var;
}


//*******************************************************************************
void delay(unsigned long ms)
{

	while(ms--)
	{
		millisecond_cnt	=	0;		//reset the counter
		while(millisecond_cnt != MILLISECOND_CNT_MAX) {;}	//wait for 1 mS
	}

}


#ifdef _TOUCH_STEALTH_
//*******************************************************************************
void bitbang_init()
{
	//disable hardware UART
	CLRBIT(UCSR0B,RXEN0);	
	CLRBIT(UCSR0B,TXEN0);


	SETBIT(RXTX_PORT, TX_PIN);	//idle state
	SETBIT(RXTX_DDR, TX_PIN);	//output
	CLRBIT(RXTX_DDR, RX_PIN);	//input
	sei();

}





//*******************************************************************************
void bitbang_putc(unsigned char c)
{
volatile unsigned int x;
volatile unsigned char bits=0;

	cli();

	CLRBIT(RXTX_PORT, TX_PIN); 

	for (x=0; x<90; x++) {;}

	for (bits = 0; bits<8; bits++)
	{

	
		if ( CHECKBIT(c,bits) )		
		{
			SETBIT(RXTX_PORT,TX_PIN); //bit is a '1'
		}
		else
		{
			CLRBIT(RXTX_PORT,TX_PIN); //bit is a '0'
		}


		for(x=0; x<90; x++) {;}

	}

	SETBIT(RXTX_PORT, TX_PIN);
	for(x=0; x<90; x++) {;}

	sei(); //enable interrupts
}


//*******************************************************************************
unsigned char bitbang_receive()
{
volatile unsigned int x;
volatile unsigned char bits;
volatile unsigned char rx_msg;

	bits	=	0;
	rx_msg	=	0;

	cli();

	while(CHECKBIT(RXTX_PIN, RX_PIN) >> RX_PIN) {;}




	for(x=0; x<45; x++) {;}


//if ( (CHECKBIT(RXTX_PIN, RX_PIN) >> RX_PIN) != 0) //noise on the line?
//	return 0;

	for (bits = 0; bits<8; bits++)
	{

		for(x=0; x<90; x++) {;}

		rx_msg >>= 1; //shift over one bit
		if (CHECKBIT(RXTX_PIN, RX_PIN))		
			rx_msg |= B10000000;	//bit is a '1'
	}

	for(x=0; x<45; x++) {;}

	sei(); //enable interrupts

	return rx_msg;
}
#endif



//*******************************************************************************
void arduinoRun()
{
#ifdef _TOUCH_SLIDE_
	//*	Disable USART transmitter and receiver
	UCSR0B	=	(0<<RXEN0) | (0<<TXEN0) | (0<< RXCIE0);
	SETBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PNUM);		//arduino not in reset
	CLRBIT(ARDUINO_RESET_DDR, ARDUINO_RESET_PNUM);		//remove drive
#endif
#ifdef _TOUCH_STEALTH_
	SETBIT(ARDUINO_TX_ENABLE_DDR,ARDUINO_TX_ENABLE_PIN);	//DDR for TX buffer
	SETBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PIN);			//Run arduino
	SETBIT(ARDUINO_TX_ENABLE_PORT, ARDUINO_TX_ENABLE_PIN);	//Don't Drive TX buffer
#endif
}

//*******************************************************************************
void arduinoReset()
{
#ifdef _TOUCH_SLIDE_
	SETBIT(ARDUINO_RESET_DDR, ARDUINO_RESET_PNUM);
	CLRBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PNUM);	//arduino in reset
#endif
#ifdef _TOUCH_STEALTH_
	CLRBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PIN);			//arduino in reset
	CLRBIT(ARDUINO_TX_ENABLE_PORT, ARDUINO_TX_ENABLE_PIN);	//Drive TX buffer
#endif
}

//*******************************************************************************
void init()
{
#ifdef _TOUCH_SLIDE_
	//Init I/O
	//SETBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PNUM);	 
	
	oled_init();
#endif
#ifdef _TOUCH_STEALTH_
	lcd_init();
#endif

	arduinoRun();

	dataflash_init();
	touch_init();
	bmp_init();
}

