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

unsigned char	*usart_getBuff_ptr();
unsigned int	usart_getBuff_size();
void			usart_flush();
void			usart_read_bytes(unsigned int count);
void			usart_putNum(unsigned int num);
//*	Dec  ?,	2008	<MATT> Added support for port 1 on the Slide
void			usart_init1(unsigned long br);
void			usart_putc1(unsigned char c);
void			usart_puts1(char *s);
void			usart_putNum1(unsigned int num);
//*	Jan 30,	2009	<MLS> Added support for port 2 on the Slide
void			usart_init2(unsigned long br);
unsigned char	usart_isavailable2(void);
unsigned char	usart_getByte2(void);
void			usart_putc2(unsigned char c);
void			usart_puts2(char *s);
void			usart_putNum2(unsigned int num);
//*	Jan 30,	2009	<MLS> Added support for port 3 on the Slide
void			usart_init3(unsigned long br);
unsigned char	usart_isavailable3(void);
unsigned char	usart_getByte3(void);
void			usart_putc3(unsigned char c);
void			usart_puts3(char *s);
void			usart_putNum3(unsigned int num);


#ifdef __cplusplus
} // extern "C"
#endif

#endif

