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

#include	"HardwareDef.h"

#include "wiring.h"
#include "image_interface.h"
#include "usart.h"
#include "dataflash.h"
#include "touchscreen.h"
//#include "wiring_private.h"
#include "bitops.h"
#ifdef _TOUCH_SLIDE_
	#include	"oled_slide.h"
#endif
#include "bmp.h"

#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif

//#define	_USE_DBBUG_RECT_STATUS_

//*******************************************************************************
//*		This is where we get the chunks of data 
//*		to program to the dataflash
//*******************************************************************************
void image_interface_begin(void)
{
unsigned int	page_count	=	0;
unsigned char	command		=	0;
unsigned char	*buff;
unsigned int	page_num	=	0;
boolean			keepGoing;
#ifdef _TOUCH_STEALTH_
COLOR			black 		=	{0,0,0};
COLOR			white		=	{255,255,255};
#endif

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

#ifdef _USE_DBBUG_RECT_STATUS_
	DebugRectPrintText("image_interface_begin()");
#endif


	arduinoReset();
	usart_init(115200);
//	usart_init(57600);

#ifdef _USE_DBBUG_RECT_STATUS_
	DebugRectPrintText("usart_init 115200");
#endif


#ifdef _USE_DBBUG_RECT_STATUS_
	DebugRectPrintText("arduino in reset");
#endif

	cli();

	buff	=	usart_getBuff_ptr();

	//enough setup, lets begin
	keepGoing	=	TRUE;
	while(keepGoing)
	{
		usart_read_bytes(1); //	Wait for us to read a byte
		command	=	buff[0]; //	store or read command

#ifdef _TOUCH_STEALTH_
		if (command == IMAGE_INTERFACE_ERASE)
		{
			dispPutS("Erasing",0,0,white,black);
			dataflash_erase();
			usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok
			dispPutS("Erasing Done",0,10,white,black);
		}
		else
#endif
		if (command == IMAGE_INTERFACE_STORE)
		{
		unsigned int	x;
		unsigned int	ii,wait,wait2;
		unsigned char	out_data[DATAFLASH_PAGESIZE],in_data[DATAFLASH_PAGESIZE];
		

		#ifdef _USE_DBBUG_RECT_STATUS_
			DebugRectPrintText("IMAGE_INTERFACE_STORE");
		#endif

			buff	=	usart_getBuff_ptr();
			
			dataflash_erase();
			usart_putc(IMAGE_INTERFACE_PAGE_DONE); //done erasing 

		#ifdef _USE_DBBUG_RECT_STATUS_
			DebugRectPrintText("done erasing ");
		#endif
			//give us the number of pages you're sending
			usart_read_bytes(2);
			page_count	=	(buff[0] << 8) + buff[1];  //number of pages			

			for (ii=0; ii< page_count; ii++)
			{
			unsigned int buffer_count	=	0;

			#ifdef _USE_DBBUG_RECT_STATUS_
			//	DebugRectPrintText("Page");
			#endif

				usart_read_bytes(DATAFLASH_PAGESIZE);	//read the whole page

				buffer_count	=	usart_getBuff_size();



				//program the whole page
				for(x=0;x<DATAFLASH_PAGESIZE;x++)
				{
					out_data[x]	=	buff[x];
				}
				dataflash_program_page(&out_data[0], ii); 	//program the page
            
            				
				for(x=0;x<DATAFLASH_PAGESIZE;x++)
					in_data[x]	=	0x00;
	         
 
				dataflash_cont_read(&in_data[0], ii, DATAFLASH_PAGESIZE);
   

				for (x=0;x<DATAFLASH_PAGESIZE;x++)
				{
					if (x==256)
					{
						for (wait = 0; wait < 254; wait++)
						{
							for (wait2 = 0; wait2 < 254; wait2++)
							{
								asm("nop");
							}
						}
					}
					usart_putc(in_data[x]);
					asm("nop");
				}
			}
		}
		else if (command == IMAGE_INTERFACE_READ)
		{
		unsigned int x;
		unsigned char out[DATAFLASH_PAGESIZE];


		#ifdef _USE_DBBUG_RECT_STATUS_
			DebugRectPrintText("IMAGE_INTERFACE_READ");
		#endif

			page_num=0;

			//give us the number of pages you're sending
			usart_read_bytes(2);
			page_num	=	(buff[0] << 8) + buff[1];  //number of pages
			

			dataflash_cont_read(&out[0], page_num, DATAFLASH_PAGESIZE);


			for (x=0; x<DATAFLASH_PAGESIZE; x++)
			{
				usart_putc(out[x]);
			}
			asm("nop");
		}
		else if (command == IMAGE_INTERFACE_INFO)
		{
		short	ii;
		char	validImage;
		short	bmpWidth;
		short	bmpHeight;
		char	displayLine[64];
		
			usart_puts("\nTouch Shield | ver 002\n");
			usart_putc(0);
			ii			=	0;
			validImage	=	TRUE;

			while (validImage && (ii < BMP_MAXBMP_COUNT))
			{
				bmp_get_entryname(ii, displayLine, &bmpWidth, &bmpHeight);
				usart_puts(displayLine);
				usart_puts("\n");
				ii++;
			}
			
		}
		else if (command == IMAGE_INTERFACE_EXIT)
		{
			usart_puts("Exit Command\n");

			sei(); //enable interrupts
			bmp_init();
			usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok
			arduinoRun();

			keepGoing	=	FALSE;
		//	break;
		}
	}//end while
	
} //end function
