//*******************************************************************************
//*	usart.c
//*******************************************************************************
//*	Edit History
//*******************************************************************************
//*	Dec  ?,	2008	<MATT> Added support for port 1 on the Slide
//*	Jan 28,	2009	<MLS> Working on merging stealth and slide code
//*	Jan 30,	2009	<MLS> Added support for ports 2 and 3 on the Slide
//*	Feb 11,	2009	<MLS> Added usart_isavailable2 & usart_isavailable3
//*	Feb 17,	2009	<MLS> Fixed bug in usart_getByte3
//*******************************************************************************

#include	<avr/io.h>
#include	<avr/interrupt.h>
#include	<string.h>
#include	<stdlib.h>

#include	"HardwareDef.h"

#include	"usart.h"
#include	"bitops.h"
#include	"wiring_private.h"

volatile unsigned int	usart_buff_cnt=0;
unsigned char			usart_buff[USART_BUFF_SIZE];


//*******************************************************************************
void	usart_init(unsigned long br) 
{
unsigned long UB;

	UB	=	F_OSC/(8*br)-1;

	// set baud rate
	UBRR0H	=	(unsigned char)(UB>>8);
	UBRR0L	=	(unsigned char)UB;
	
	//*	Enable transmitter and receiver
	//*	we disabled the interrupts
	UCSR0B	=	(1<<RXEN0) | (1<<TXEN0); //| (1<< RXCIE0);

	UCSR0A	|=	(1<<U2X0);		//double speed

	sei();	//enable interrupts
}



//*******************************************************************************
unsigned char * usart_getBuff_ptr()
{
	return &usart_buff[0];
}


//*******************************************************************************
unsigned int usart_getBuff_size()
{
	return usart_buff_cnt;
}


//*******************************************************************************
void usart_flush()
{
	usart_buff_cnt	=	0; //reset the index
}


//*******************************************************************************
void usart_putc(unsigned char c)
{

	// wait until UDR ready
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0	=	c;		// send character
}


//*******************************************************************************
void	usart_puts(char *s)
{

	//*	loop until *s != NULL
	while (*s)
	{
		usart_putc(*s);
		s++;
	}
}


//*******************************************************************************
void usart_putNum(unsigned int num)
{
char out[10];

	dtostrf(num,5,0,out);
	usart_puts(out);

}


//*******************************************************************************
void usart_read_bytes(unsigned int count)
{

//	usart_flush();
	
	//SETBIT(UCSR0B,RXCIE0);
	usart_buff_cnt	=	0;	//reset the index
	while(usart_buff_cnt < count) 
	{
		
		while ( !(UCSR0A & (1<<RXC0)) )	{;}
		
		usart_buff[usart_buff_cnt]	=	UDR0;
	 	usart_buff_cnt++;	
	
	} //wait for the bytes
//	CLRBIT(UCSR0B,RXCIE0);
}

#pragma mark -
#pragma mark Slide extra serial ports

#ifdef _TOUCH_SLIDE_
//*******************************************************************************
//*	this is port 1 which is mapped to port 0 on the arduino bus
//*******************************************************************************
void usart_init1(unsigned long br) 
{
unsigned long UB;

	UB	=	F_OSC/(8*br)-1;

	// set baud rate
	UBRR1H	=	(unsigned char)(UB>>8);
	UBRR1L	=	(unsigned char)UB;
	
	// Enable transmitter and receiver
	UCSR1B	=	(1<<RXEN1) | (1<<TXEN1);// | (1<< RXCIE1);

 	UCSR1A	|=	(1<<U2X1);  //double speed

	sei(); //enable interrupts

}

//*******************************************************************************
void usart_putc1(unsigned char c)
{

 	// wait until UDR ready
 	while(!(UCSR1A & (1 << UDRE1)));
 	UDR1	=	c;    // send character
}

//*******************************************************************************
void	usart_puts1(char *s)
{
	
	//*	loop until *s != NULL
	while (*s)
	{
		usart_putc1(*s);
		s++;
	}
}

//*******************************************************************************
void usart_putNum1(unsigned int num)
{
char out[10];

	dtostrf(num,5,0,out);
	usart_puts1(out);

}

#pragma mark -
#define    USBS         3
#define    UCSZ1        2
#define    UCSZ0        1
//*******************************************************************************
//*	this is port 2 which is mapped to port 0 on the arduino bus
//*******************************************************************************
void usart_init2(unsigned long br) 
{
unsigned long UB;

	UB	=	F_OSC / (8 * br) - 1;
//	UB	=	F_OSC / (16 * br) - 1;

	// set baud rate
	UBRR2H	=	(unsigned char)(UB >> 8);
	UBRR2L	=	(unsigned char)UB;
	
	// Enable transmitter and receiver
	UCSR2B	=	(1 << RXEN2) | (1 << TXEN2);// | (1<< RXCIE1);

 	UCSR2A	|=	( 1 <<U2X2);  //double speed

	UCSR2C	|=	(1 << USBS)	;					//*	2 stop bits
//				+ (1 < UCSZ1) + (1 < UCSZ0);	//*	8 data bits
				
	sei(); //enable interrupts

}



//*******************************************************************************
unsigned char	usart_isavailable2(void)
{
unsigned char	isAvalable;

	if (UCSR2A & (1<<RXC2))
	{
		isAvalable	=	true;
	}
	else
	{
		isAvalable	=	false;
	}
	return(isAvalable);
}

//*******************************************************************************
unsigned char	usart_getByte2(void)
{
unsigned char	theByte;

	while ( !(UCSR2A & (1<<RXC2)) )
	{
		;	//*	do nothing
	}
		
	theByte	=	UDR2;
	return(theByte)	;
	
}



//#define	_DEBUG_PORT2_VIA_PORT3_
//*******************************************************************************
void usart_putc2(unsigned char c)
{
#ifdef _DEBUG_PORT2_VIA_PORT3_
char	debugString[16];

	sprintf(debugString, "Sending=%02X\r\n", c);
	usart_puts3(debugString);
#endif
 	// wait until UDR ready
 	while(!(UCSR2A & (1 << UDRE2)));
 	UDR2	=	c;    // send character
}

//*******************************************************************************
void	usart_puts2(char *s)
{
	
	//*	loop until *s != NULL
	while (*s)
	{
		usart_putc2(*s);
		s++;
	}
}

//*******************************************************************************
void usart_putNum2(unsigned int num)
{
char out[10];

	dtostrf(num,5,0,out);
	usart_puts2(out);

}
#pragma mark -

//*******************************************************************************
//*	this is port 3 which is mapped to port 0 on the arduino bus
//*******************************************************************************
void usart_init3(unsigned long br) 
{
unsigned long UB;

//	UB	=	F_OSC / (8 * br) - 1;
	UB	=	F_OSC / (16 * br) - 1;

	// set baud rate
	UBRR3H	=	(unsigned char)(UB >> 8);
	UBRR3L	=	(unsigned char)UB;
	
	// Enable transmitter and receiver
	UCSR3B	=	(1 << RXEN3) | (1 << TXEN3);// | (1<< RXCIE1);

// 	UCSR3A	|=	( 1 <<U2X3);  //double speed

//	UCSR3C	=	(1 << USBS)						//*	2 stop bits
//				+ (1 < UCSZ1) + (1 < UCSZ0);	//*	8 data bits
				
	sei(); //enable interrupts

}



//*******************************************************************************
unsigned char	usart_isavailable3(void)
{
unsigned char	isAvalable;

	if (UCSR3A & (1<<RXC3))
	{
		isAvalable	=	true;
	}
	else
	{
		isAvalable	=	false;
	}
	return(isAvalable);
}

//*******************************************************************************
unsigned char	usart_getByte3(void)
{
unsigned char	theByte;

	while ( !(UCSR3A & (1<<RXC3)) )
	{
		;	//*	do nothing
	}
		
	theByte	=	UDR3;
	return(theByte)	;
	
}


//*******************************************************************************
void usart_putc3(unsigned char c)
{

 	// wait until UDR ready
 	while(!(UCSR3A & (1 << UDRE3)));
 	UDR3	=	c;    // send character
}

//*******************************************************************************
void	usart_puts3(char *s)
{
	
	//*	loop until *s != NULL
	while (*s)
	{
		usart_putc3(*s);
		s++;
	}
}

//*******************************************************************************
void usart_putNum3(unsigned int num)
{
char out[10];

	dtostrf(num,5,0,out);
	usart_puts3(out);

}

#endif

