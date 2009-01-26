/*==============================================================================
 *
 * Title: wiring.c
 * Description: These are the processor specific functions  
 *      
 *
  =============================================================================
 *
 * 
 *============================================================================*/ 

/*==============================================================================
 * PREPROCESSOR DIRECTIVES
 *============================================================================*/
#include "wiring.h"

/* Maximum number of pins, for error checking */
#define NUM_PINS sizeof(pinTable)/sizeof(PIN_DESC_T) 
#define MILLISECOND_CNT_MAX	7 	//8*128uS = 1.024 mS

/*==============================================================================
 * CONSTANTS
 *============================================================================*/

/* The user pin lookup table.
   This table provides a mapping to the hardware pins. */
PIN_DESC_T const pinTable[] = {

    /* 0 */  { &PORTE, 0, &PINE, &DDRE }, //PCINT0
    /* 1 */  { &PORTE, 1, &PINE, &DDRE },
    /* 2 */  { &PORTE, 2, &PINE, &DDRE },
    /* 3 */  { &PORTE, 3, &PINE, &DDRE },
    /* 4 */  { &PORTE, 4, &PINE, &DDRE },
    /* 5 */  { &PORTE, 5, &PINE, &DDRE },
    /* 6 */  { &PORTE, 6, &PINE, &DDRE },
    /* 7 */  { &PORTE, 7, &PINE, &DDRE },

    /* 8 */  { &PORTD, 2, &PIND, &DDRD },
    /* 9 */  { &PORTD, 3, &PIND, &DDRD },
    /* 10 */ { &PORTD, 4, &PIND, &DDRD },
    /* 11 */ { &PORTD, 6, &PIND, &DDRD },
    /* 12 */ { &PORTD, 7, &PIND, &DDRD },

    /* 13 */ { &PORTG, 0, &PING, &DDRG },

    /* 14 */ { &PORTB, 0, &PINB, &DDRB },
    /* 15 */ { &PORTB, 4, &PINB, &DDRB },
    /* 16 */ { &PORTB, 5, &PINB, &DDRB },
    /* 17 */ { &PORTB, 6, &PINB, &DDRB },

    /* 18 */ { &PORTG, 3, &PING, &DDRG },
    /* 19 */ { &PORTG, 4, &PING, &DDRG },

    /* 20 */ { &PORTD, 0, &PIND, &DDRD },
    /* 21 */ { &PORTD, 1, &PIND, &DDRD },

    /* 22 */ { &PORTG, 1, &PING, &DDRG },

    /* 23 */ { &PORTC, 0, &PINC, &DDRC },
    /* 24 */ { &PORTC, 1, &PINC, &DDRC },
    /* 25 */ { &PORTC, 2, &PINC, &DDRC },
    /* 26 */ { &PORTC, 3, &PINC, &DDRC },
    /* 27 */ { &PORTC, 4, &PINC, &DDRC },
    /* 28 */ { &PORTC, 5, &PINC, &DDRC },
    /* 29 */ { &PORTC, 6, &PINC, &DDRC },
    /* 30 */ { &PORTC, 7, &PINC, &DDRC },
    /* 31 */ { &PORTG, 2, &PING, &DDRG },
    /* 32 */ { &PORTA, 7, &PINA, &DDRA },
    /* 33 */ { &PORTA, 6, &PINA, &DDRA },
    /* 34 */ { &PORTA, 5, &PINA, &DDRA },
    /* 35 */ { &PORTA, 4, &PINA, &DDRA },

    /* 36 */ { &PORTF, 5, &PINF, &DDRF },
    /* 37 */ { &PORTF, 4, &PINF, &DDRF },
    /* 38 */ { &PORTF, 3, &PINF, &DDRF },
    /* 39 */ { &PORTF, 2, &PINF, &DDRF },
    /* 40 */ { &PORTF, 1, &PINF, &DDRF },
    /* 41 */ { &PORTF, 0, &PINF, &DDRF },

    /* 42 */ { &PORTB, 7, &PINB, &DDRB },
};

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

volatile unsigned long millis_var=0;
volatile unsigned char millis_var_counter=0;

/*==============================================================================
 * FUNCTIONS
 *============================================================================*/

/* ===========================================================================
*  FUNCTION: pinMode
*
*  DESIGN DESCRIPTION:
*     Changes the pin direction
*
*  PARAMETER LIST:
*     pin - The pin number to be changed.
*     val - INPUT or OUTPUT value to be written to the pin
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*
*===========================================================================*/
void pinMode(uint8_t pin, uint8_t mode)
{
    PIN_DESC_T *p = &pinTable[pin];

    /* Check desired mode */
    if (mode == INPUT)
    {
        /* Look up the DDR reg and set the bit */ 
        CLRBIT(*p->DirReg, 
               p->PinNum);
    }
    else
    {
        /* Look up the DDR reg and set the bit */
        SETBIT(*p->DirReg, 
                p->PinNum);         
    }
}

/* ===========================================================================
*  FUNCTION: digitalWrite
*
*  DESIGN DESCRIPTION:
*     Write a value to a pin
*
*  PARAMETER LIST:
*     pin - The pin number to be changed.
*     val - HIGH or LOW value to write to the pin
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*
*===========================================================================*/
void digitalWrite(uint8_t pin, uint8_t val)
{
    PIN_DESC_T *p = &pinTable[pin];

    /* Check desired state */
    if (val == HIGH)
    {
        /* Look up the port reg and set the bit */
        SETBIT(*p->PortReg, 
                p->PinNum); 
    }
    else
    {
        /* Look up the port reg and set the bit */ 
        CLRBIT(*p->PortReg, 
                p->PinNum);        
    }
}

/* ===========================================================================
*  FUNCTION: digitalRead
*
*  DESIGN DESCRIPTION:
*     Reads a digital pin
*
*  PARAMETER LIST:
*     pin - The pin to be read
*
*  RETURNED:
*     returns 0 or the masked pin register value
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*
*===========================================================================*/
uint8_t digitalRead(uint8_t pin)
{
    PIN_DESC_T *p = &pinTable[pin];
    
    /* Return the pin register value at the bit location */
    return CHECKBIT(*p->PinReg,
                     p->PinNum);
}

/* ===========================================================================
*  FUNCTION: analogRead
*
*  DESIGN DESCRIPTION:
*     Reads an analog pin
*
*  PARAMETER LIST:
*     pin - The pin to be read
*
*  RETURNED:
*     returns the 10-bit analog value from the pin  
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*
*===========================================================================*/
int analogRead(uint8_t pin)
{
	uint8_t low, high;

	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits)
	ADMUX = (1 << REFS0) | ((5-pin) & 0x0f);
    
	// start the conversion
	SETBIT(ADCSRA, ADSC);

	// ADSC is cleared when the conversion finishes
	while (CHECKBIT(ADCSRA, ADSC)) { ; }

	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	low = ADCL;
	high = ADCH;

	// combine the two bytes
	return (high << 8) | low;
}

/* ===========================================================================
*  FUNCTION: SIG_OVERFLOW0
*
*  DESIGN DESCRIPTION:
*     128usec timer 0 interrupt
*
*  PARAMETER LIST:
*     none
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*
*===========================================================================*/
SIGNAL(SIG_OVERFLOW0)
{
    /* The millisecond counter */
    if (millis_var_counter == MILLISECOND_CNT_MAX)
    {
        millis_var++;           // incremement the free running counter 
        millis_var_counter=0;   // reset the 128usec counter
    } else
    {
        millis_var_counter++;   // incremement the 128usec counter
    }

}

/* ===========================================================================
*  FUNCTION: millis
*
*  DESIGN DESCRIPTION:
*     Returns the free-running millisecond counter 
*
*  PARAMETER LIST:
*     none
*
*  RETURNED:
*     unsigned long - the milliseconds since startup
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*
*===========================================================================*/
unsigned long millis()
{
	return millis_var;
}

/* ===========================================================================
*  FUNCTION: delay
*
*  DESIGN DESCRIPTION:
*     A delay function. 
*
*  PARAMETER LIST:
*     ms - the number of milliseconds to delay
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*     
*
*===========================================================================*/
void delay(unsigned long ms)
{
    long pTime = millis(); //save the current time
    while( (millis() - pTime) < ms ) //check the ellapsed time
    {
        ; //burn cycles
    }
}


void init()
{
    //Init TIMER 0 
	TCCR0A = (0<<CS02) | (1<<CS01) | (0<<CS00); //timer 0 setup to overflow every 128us
	TIMSK0 = (1<<TOIE0); //enable timer 0 overflow interrupts

    //Init ADC      
    ADCSRA = (1 << ADEN) |                                 // Turn on the ADC converter
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);   // Clock = F_CPU / 128

    sei();          //enable global interupts
    //delay(2500);    //wait for the DTR to stop messing with us
}

void bling(uint8_t percent)
{
    pinMode(42, OUTPUT);

    if (percent)
    {   
        digitalWrite(42, LOW);
    }
    else
    {
        digitalWrite(42, HIGH);
    }
}
