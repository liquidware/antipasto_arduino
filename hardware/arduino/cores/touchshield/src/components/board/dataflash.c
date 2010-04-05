//*******************************************************************************
//*	DataFlash interface
//*	REAL important info		45DB161D Atmel DataFlash chip
//*	from the PDF doc file
//*		Features
//*		¥ Single 2.5V - 3.6V or 2.7V - 3.6V Supply
//*		¥ RapidSª Serial Interface: 66 MHz Maximum Clock Frequency
//*		Ð SPI Compatible Modes 0 and 3
//*		¥ User Configurable Page Size
//*		Ð 512 Bytes per Page
//*		Ð 528 Bytes per Page
//*		Ð Page Size Can Be Factory Pre-configured for 512 Bytes
//*		¥ Page Program Operation
//*		Ð Intelligent Programming Operation
//*		Ð 4,096 Pages (512/528 Bytes/Page) Main Memory
//*		¥ Flexible Erase Options
//*		Ð Page Erase (512 Bytes)
//*		Ð Block Erase (4 Kbytes)
//*		Ð Sector Erase (128 Kbytes)
//*		Ð Chip Erase (16 Mbits)
//*		¥ Two SRAM Data Buffers (512/528 Bytes)
//*		Ð Allows Receiving of Data while Reprogramming the Flash Array
//*		¥ Continuous Read Capability through Entire Array
//*		Ð Ideal for Code Shadowing Applications
//*		¥ Low-power Dissipation
//*		Ð 7 mA Active Read Current Typical
//*		Ð 25 £gA Standby Current Typical
//*		Ð 9 £gA Deep Power Down Typical
//*		¥ Hardware and Software Data Protection Features
//*		Ð Individual Sector
//*		¥ Sector Lockdown for Secure Code and Data Storage
//*		Ð Individual Sector
//*		¥ Security: 128-byte Security Register
//*		Ð 64-byte User Programmable Space
//*		Ð Unique 64-byte Device Identifier
//*		¥ JEDEC Standard Manufacturer and Device ID Read
//*		¥ 100,000 Program/Erase Cycles Per Page Minimum
//*		¥ Data Retention Ð 20 Years
//*		¥ Industrial Temperature Range
//*		¥ Green (Pb/Halide-free/RoHS Compliant) Packaging Options
//*******************************************************************************
//*	Jan 15,	2009	<MLS> Converstation with Matt, I am going ro re-write the flash file system
//*******************************************************************************
//*


#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>

#include	"HardwareDef.h"

#include "dataflash.h"
#include "bitops.h"
#include "usart.h"
#ifdef _TOUCH_SLIDE_
	#include "touchscreen.h"
#endif
#ifdef _TOUCH_STEALTH_
	#include "touchscreen.h"
#endif

unsigned int dataflash_buff_size = 0;	//!< The size of the on-chip buffer



//*******************************************************************************
void dataflash_init()
{

	CLRBIT(DATAFLASH_DDR,DATAFLASH_MISO);
	SETBIT(DATAFLASH_DDR,DATAFLASH_RESET);
	SETBIT(DATAFLASH_DDR,DATAFLASH_CS);
	SETBIT(DATAFLASH_DDR,DATAFLASH_MOSI);
	SETBIT(DATAFLASH_DDR,DATAFLASH_SCK);
	SETBIT(DATAFLASH_DDR,PB0); //output or else master gets stopped


	CLRBIT(DATAFLASH_PORT,DATAFLASH_RESET); //reset
	asm("nop");
	asm("nop");
	SETBIT(DATAFLASH_PORT,DATAFLASH_RESET); //reset
	SETBIT(DATAFLASH_PORT,PB0); //output or else master gets stopped
	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); 	//dataflash not selected

	/* Enable SPI, Master, set clock rate fck/16 */

//	SPCR = (1<<SPE) | (1<<MSTR) | (1<< SPR0);
	SPCR = 0x5C; //SPI mode 3
	SPSR = (1<<SPI2X);
//	SPCR |= (1<< SPR0);
}










//*******************************************************************************
void dataflash_write_buff(unsigned char * out_data)
{
unsigned int i;

	///Write the buffer
	CLRBIT(DATAFLASH_PORT,DATAFLASH_CS); //select dataflash

	dataflash_out(0x84);
	dataflash_out(0);
	dataflash_out(0);
	dataflash_out(0);


	for (i=0; i<DATAFLASH_PAGESIZE; i++)
	{
		dataflash_out(out_data[i]);
	}

	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); //Deselect flash chip

}








//*******************************************************************************
void dataflash_read_buff(unsigned char * in_data)
{
unsigned int i;
	///Read the buffer
	CLRBIT(DATAFLASH_PORT,DATAFLASH_CS); //select dataflash

	dataflash_out(0xD4);
	dataflash_out(0);
	dataflash_out(0);
	dataflash_out(0);

dataflash_out(0xFF);

for (i=0; i<DATAFLASH_PAGESIZE; i++)
	{
	dataflash_out(0xFF);
	in_data[i] = SPDR;
	}

	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); //Deselect flash chip

}





//*******************************************************************************
void dataflash_clear_buff()
{
unsigned int i;

	///Write the buffer
	CLRBIT(DATAFLASH_PORT,DATAFLASH_CS); //select dataflash

	dataflash_out(0x84);
	dataflash_out(0);
	dataflash_out(0);
	dataflash_out(0);


for (i=0; i<DATAFLASH_PAGESIZE; i++)
	{
	dataflash_out(0xFF);
	}

	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); //Deselect flash chip
}









//*******************************************************************************
void dataflash_program_page(unsigned char * page_buff, unsigned int page_num)
{
unsigned char p=0;

dataflash_clear_buff();
dataflash_write_buff(&page_buff[0]);



//--Program the page
	CLRBIT(DATAFLASH_PORT,DATAFLASH_CS); //select dataflash


//	dataflash_out(0x88);
	dataflash_out(0x83);


	p= page_num>>6;
	dataflash_out(p);
	p= page_num<<2;
	dataflash_out(p);
	dataflash_out(0); //don't c


	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); //Deselect flash chip

//	delay_ms(30); //wait until programmed
	while(dataflash_checkStatus() == DATAFLASH_BUSY) {;} //wait until erased
}




//*******************************************************************************
void dataflash_erase()
{
//unsigned int block_counter=0;


/*
while(block_counter < DATAFLASH_BLOCK_COUNT)
	{
	CLRBIT(DATAFLASH_PORT,DATAFLASH_CS); //select dataflash

	dataflash_out(0x50);
	dataflash_out((unsigned char)(block_counter>>4));
	dataflash_out((unsigned char)(block_counter<<4));
	dataflash_out(0x00);

	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); //deselect dataflash

	block_counter++;
//	delay_ms(20);
	while(dataflash_checkStatus() == DATAFLASH_BUSY) {;} //wait until erased
	}


*/
}


//*******************************************************************************
void dataflash_out(unsigned char cData)
{

	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
#ifndef __MWERKS__
	while(!CHECKBIT(SPSR,SPIF))
		{
		;
		}
#endif

}


//*******************************************************************************
unsigned char dataflash_checkStatus()
{

	unsigned char result=0;
	unsigned char temp = 0;

	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); //Deselect flash chip
	CLRBIT(DATAFLASH_PORT,DATAFLASH_CS); //select flash chip

	dataflash_out(0x57); //read status register

	dataflash_out(0); //to get the data

	result = SPDR; //bits 7-1

	temp = (result & 0x38) >> 3;
	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); //Deselect flash chip

	result = result >> 7;
	return result;

}



//*******************************************************************************
void dataflash_read_block(unsigned char * storage_buff, unsigned long offset, unsigned int size)
{
unsigned long page_num = offset/DATAFLASH_PAGESIZE;
unsigned long byte_offset = offset - (page_num*DATAFLASH_PAGESIZE);
unsigned int i;
unsigned char in,p;


dataflash_clear_buff();


	CLRBIT(DATAFLASH_PORT,DATAFLASH_CS); //select dataflash
	dataflash_out(0x68); //cont read to buffer 1

	p	=	page_num>>6;
	dataflash_out(p);
	p	=	page_num<<2;
	dataflash_out(p | (unsigned char)(byte_offset>>8));
	dataflash_out((unsigned char)byte_offset); //don't c


		dataflash_out(0xFF);
		in	=	SPDR;

		dataflash_out(0xFF);
		in	=	SPDR;

		dataflash_out(0xFF);
		in	=	SPDR;

		dataflash_out(0xFF);
		in	=	SPDR;

	for (i=0; i<size; i++)
	{
		dataflash_out(0xFF);
		in	=	SPDR;
		storage_buff[i]=in;
		asm("nop");
	}

	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); //Deselect flash chip


}





//*******************************************************************************
void dataflash_cont_read(unsigned char * storage_buff, unsigned int page_num, unsigned int size)
{
//unsigned int page_num = addr/DATAFLASH_PAGESIZE;
//unsigned int byte_offset = addr - (page_num*DATAFLASH_PAGESIZE);
unsigned int i;
unsigned char in,p;


dataflash_clear_buff();


	CLRBIT(DATAFLASH_PORT,DATAFLASH_CS); //select dataflash
	dataflash_out(0x68); //cont read to buffer 1

	p= page_num>>6;
	dataflash_out(p);
	p= page_num<<2;
	dataflash_out(p);
	dataflash_out(0); //don't c


		dataflash_out(0xFF);
		in = SPDR;

		dataflash_out(0xFF);
		in = SPDR;

		dataflash_out(0xFF);
		in = SPDR;

		dataflash_out(0xFF);
		in = SPDR;

	for (i=0; i<size; i++)
		{
		dataflash_out(0xFF);
		in = SPDR;
		storage_buff[i]=in;
		asm("nop");
		}

	SETBIT(DATAFLASH_PORT,DATAFLASH_CS); //Deselect flash chip


}







