#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

#include	"HardwareDef.h"

#include "usart.h"
#include "bitops.h"

volatile unsigned int usart_buff_cnt=0;
unsigned char usart_buff[USART_BUFF_SIZE];

void usart_read_bytes(unsigned int count)
{
	
//	usart_flush();
	
	//SETBIT(UCSR0B,RXCIE0);
	usart_buff_cnt = 0; //reset the index
	while(usart_buff_cnt < count) 
	{
		
		while ( !(UCSR0A & (1<<RXC0)) )	{;}
		
		usart_buff[usart_buff_cnt] = UDR0;
	 	usart_buff_cnt++;	
	
	} //wait for the bytes

//	CLRBIT(UCSR0B,RXCIE0);
}

void usart_init(unsigned long br) 
{

unsigned long UB;

UB = F_OSC/(8*br)-1;

	// set baud rate
	UBRR0H = (unsigned char)(UB>>8);
	UBRR0L = (unsigned char)UB;
	
	// Enable transmitter and receiver
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); //| (1<< RXCIE0);

 	UCSR0A |= (1<<U2X0);  //double speed


sei(); //enable interrupts

}

void usart_init1(unsigned long br) 
{

unsigned long UB;

UB = F_OSC/(8*br)-1;

	// set baud rate
	UBRR1H = (unsigned char)(UB>>8);
	UBRR1L = (unsigned char)UB;
	
	// Enable transmitter and receiver
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);// | (1<< RXCIE1);

 	UCSR1A |= (1<<U2X1);  //double speed

sei(); //enable interrupts

}

void usart_putc1(unsigned char c)
{

 	// wait until UDR ready
 	while(!(UCSR1A & (1 << UDRE1)));
 	UDR1 = c;    // send character
}

void usart_puts1(char *s) 
{
	
//  loop until *s != NULL
while (*s) {
	usart_putc1(*s);
	s++;
	}
}

void usart_putNum1(unsigned int num)
{
char out[10];

dtostrf(num,5,0,out);
usart_puts1(out);

}

void usart_putNum(unsigned int num)
{
char out[10];

dtostrf(num,5,0,out);
usart_puts(out);

}

void usart_putc(unsigned char c)
{

 	// wait until UDR ready
 	while(!(UCSR0A & (1 << UDRE0)));
 	UDR0 = c;    // send character
}


void usart_puts(char *s) 
{
	
//  loop until *s != NULL
while (*s) {
	usart_putc(*s);
	s++;
	}
}


unsigned char * usart_getBuff_ptr()
{
	return &usart_buff[0];
}



unsigned int usart_getBuff_size()
{
	return usart_buff_cnt;
}

