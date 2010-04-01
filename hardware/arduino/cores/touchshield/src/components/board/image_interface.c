//*******************************************************************************
//*	Image Interface, 
//*	routines to upload bmp images to flash memory on the stealth and slide
//*******************************************************************************
//*	Jan 10,	2009	<MLS> Started mergering stealth/slide versions of imaage_interface.c
//*	Jan 11,	2009	<MLS> Mostly done with merge
//*	Jan 14,	2009	<MLS> Added status messages via debug rect
//*	Jan 16,	2009	<MLS> Got my own C program on MacOSX to talk to the USB port and to this code
//*******************************************************************************

#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#include "wiring.h"
#include "image_interface.h"


#include	"graphics.h"


//*******************************************************************************
//*		This is where we get the chunks of data 
//*		to program to the dataflash
//*******************************************************************************
void image_interface_begin(void)
{
//unsigned int	page_count	=	0;
unsigned char	command		=	0;
unsigned char	*buff;
unsigned int	page_num	=	0;
boolean			keepGoing;
COLOR			black		=	{0,0,0};
COLOR			white		=	{255,255,255};
	/*
		order of operations:
			1.) purge usart buffers
			2.) recv mode byte
			3.) if mode == STORE,
					recv page_count 
					recv all pages
				else mode == READ
					send page_count
					send pages
	*/
	
	arduinoReset();

	usart_init(115200);
 
	cli();

	buff = usart_getBuff_ptr();

	//enough setup, lets begin
	keepGoing	=	TRUE;
	while(keepGoing)
	{
		usart_read_bytes(1); //Wait for us to read a byte
		command = buff[0]; //store or read command
		
		if (command == IMAGE_INTERFACE_ERASE)
		{
			dispPutS("Erasing",0,0,white,black);
			dataflash_erase();
			usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok
			dispPutS("Erasing Done",0,10,white,black);
		}
		else

		if (command == IMAGE_INTERFACE_STORE)
		{
			uint16_t x;
			unsigned char out_data[DATAFLASH_PAGESIZE],in_data[DATAFLASH_PAGESIZE];	
			char out[10];	
			uint8_t checksum = 0;
			buff = usart_getBuff_ptr();

			
			dispPutS("Store",0,20,white,black);
			usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok

			//give us the page number to write
			usart_read_bytes(2);
			usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok
			page_num = (buff[1] << 8) + buff[0];
			dtostrf(page_num,4,0,out);
			dispPutS("Page Number:",0,30,white,black);
			dispPutS(out,100,30,white,black);
			
			//read the whole page
			usart_read_bytes(DATAFLASH_PAGESIZE);
			usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok
			dispPutS("Page recved",0,40,white,black);
			
			//program the whole page
			for(x=0;x<DATAFLASH_PAGESIZE;x++) { out_data[x] = buff[x]; } //copy the page to the outbound page buffer
			dataflash_program_page(&out_data[0], page_num); 	//program the page
			for(x=0;x<DATAFLASH_PAGESIZE;x++) { in_data[x] = 0x00; } //clear the inbound page buffer

			//wait for checksum command
			usart_read_bytes(1);
			dispPutS("Checksum",0,50,white,black);
			dataflash_cont_read(&in_data[0], page_num, DATAFLASH_PAGESIZE); //read the page out of the chip
			for(x=0;x<DATAFLASH_PAGESIZE;x++) { checksum += in_data[x]; } // build checksum
			checksum = (checksum ^ 0xFF ) + 1; //two's complement
			usart_putc(checksum); //send checksum
			dispPutS("Sent",0,60,white,black);
				
		}
		else if (command == IMAGE_INTERFACE_READ)
		{
			unsigned int x;
			unsigned char out[DATAFLASH_PAGESIZE];


			page_num=0;

			//give us the number of pages you're sending
			usart_read_bytes(2);
			page_num = (buff[0] << 8) + buff[1];	//number of pages
			

			dataflash_cont_read(&out[0], page_num, DATAFLASH_PAGESIZE);


			for (x=0; x<DATAFLASH_PAGESIZE; x++)
				{
				usart_putc(out[x]);
				}

			asm("nop");
		}
		else if (command == IMAGE_INTERFACE_INFO)
		{
			usart_puts("Flasher | ver 001");
			usart_putc(0);
		}
		else if (command == IMAGE_INTERFACE_EXIT)
		{
			usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok
			bmp_init();
			arduinoRun(); 
			sei(); //enable interrupts

			keepGoing	=	FALSE;
			break;
		}
	
	}//end while
	
} //end function
