#ifndef _USART_H_
#define _USART_H_

#define F_OSC	16000000

#define USART_BUFF_SIZE	528

#ifdef __cplusplus
extern "C"{
#endif


void usart_init(unsigned long br);
void usart_putc(unsigned char c);
void usart_puts(char *s);

unsigned char * usart_getBuff_ptr();
unsigned int usart_getBuff_size();
void usart_flush();
void usart_read_bytes(unsigned int count);
//*	the following routines were added Dec 2008 by Matt for use on the Slide
void usart_init1(unsigned long br);
void usart_putc1(unsigned char c);
void usart_puts1(char *s);
void usart_putNum1(unsigned int num);
void usart_putNum(unsigned int num);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

