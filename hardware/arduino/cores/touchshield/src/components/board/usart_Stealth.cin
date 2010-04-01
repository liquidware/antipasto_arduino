#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart.h"
#include "bitops.h"
#include "wiring_private.h"

volatile unsigned int usart_buff_cnt=0;
unsigned char usart_buff[USART_BUFF_SIZE];


unsigned char * usart_getBuff_ptr()
{
	return &usart_buff[0];
}



unsigned int usart_getBuff_size()
{
	return usart_buff_cnt;
}



void usart_flush()
{
	usart_buff_cnt = 0; //reset the index
}


void usart_read_bytes(unsigned int count)
{
	
	cli();
	
	usart_buff_cnt = 0; //reset the index
	while(usart_buff_cnt < count) 
	{
		
		while ( !(UCSR0A & (1<<RXC0)) )	{;}
		
		usart_buff[usart_buff_cnt] = UDR0;
	 	usart_buff_cnt++;	
	
	} //wait for the bytes
	sei();

}




void usart_init(unsigned long br) 
{

unsigned long UB;

UB = F_OSC/(8*br)-1;

	// set baud rate
	UBRR0H = (unsigned char)(UB>>8);
	UBRR0L = (unsigned char)UB;
	
	// Enable transmitter and receiver
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);

 	UCSR0A |= (1<<U2X0);  //double speed

}




void usart_putc(unsigned char c)
{
 	// wait until UDR ready
 	while(!(UCSR0A & (1 << UDRE0)));
 	UDR0 = c;    // send character
}


void usart_puts(char *s) 
{
	
while (*s) {
	usart_putc(*s);
	s++;
	}
}



