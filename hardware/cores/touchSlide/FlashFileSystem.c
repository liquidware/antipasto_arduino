//*******************************************************************************
//*	Flsh Memory File System
//*		by Mark Sproul
//*		this source code is part of the liquid ware drivers for the 
//*		TouchShield and TouchSlide
//*		it is public domain under the the GPL as is the rest of the Arduino family
//*
//*	These routines handle the file system for the Flash Memory (16 megabit, 2 megaBYTE)
//*	on board the screens.
//*******************************************************************************
//*	Jan 10,	2009	<MLS> Started mergering stealth/slide versions of imaage_interface.c
//*	Jan 11,	2009	<MLS> Mostly done with merge
//*	Jan 14,	2009	<MLS> Added status messages via debug rect
//*	Jan 15,	2009	<MLS> Discussed File System with Matt, decided to do entirely new system
//*	Jan 16,	2009	<MLS> Got my own C program on MacOSX to talk to the USB port and to this code
//*	Jan 17,	2009	<MLS> Strated seperate file for new file system
//*******************************************************************************

#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#include	"HardwareDef.h"

#ifdef _ENABLE_FLASH_FILE_SYSTEM_

//#include	"image_interface.h"
#include	"FlashFileSystem.h"
#include	"usart.h"
#include	"dataflash.h"
//#include	"touchscreen.h"
#include	"bitops.h"
#include	"bmp.h"
#include	"wiring.h"

#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif

#define	_USE_DBBUG_RECT_STATUS_

//*******************************************************************************
//*	this can also be considered ERASE
static void	FlashFileFormat(void)
{
	dispPutS("Erasing",0,0,white,black);
	dataflash_erase();
	usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok
	dispPutS("Erasing Done",0,10,white,black);

}

//*******************************************************************************
long	DelayBigTime(void)
{
long	answer;
long	ii;

	answer	=	0;
//	for (ii=0; ii<10000000; ii++)
//	{
//		answer	+=	ii;
//	}
	return(answer % 6);
}

#define	kAscii_ACK 0x06
#define	kAscii_NAK 0x15

//*******************************************************************************
static void	SendAck(void)
{
	DebugRectPrintText("sending ACK +++");
	usart_putc(kAscii_ACK);
	usart_puts("ACK");
	
}

//*******************************************************************************
static void	SendNAK(void)
{
	DebugRectPrintText("sending NAK ---");

	usart_putc(kAscii_NAK);
	usart_puts("NAK");
	
}

//*******************************************************************************
static void	FlashFileDownload(void)
{
unsigned int	xx;
unsigned int	ii,wait,wait2;
//unsigned char	out_data[DATAFLASH_PAGESIZE],in_data[DATAFLASH_PAGESIZE];
char			msgBuff[48];
unsigned char	*buff;
unsigned int	page_count;
long			newFileSize;
char			newFileName[16];
long			longByte1;
long			longByte2;
long			longByte3;
long			longByte4;

#ifdef _USE_DBBUG_RECT_STATUS_
	DebugRectPrintText("IMAGE_INTERFACE_STORE");
#endif

	buff	=	usart_getBuff_ptr();
	

	buff[0]	=	0;
	while (buff[0] != 0x01)	//*	wait for SOH
	{
		usart_read_bytes(1);
	}


	for (ii=0; ii<17; ii++)
	{
		buff[ii]	=	0;
	}

	//*	read the file header
	usart_read_bytes(17);
	if (buff[0] == 0x01)	//*	look for SOH
	{
		longByte1	=	buff[1] & 0x0ff;
		longByte2	=	buff[2] & 0x0ff;
		longByte3	=	buff[3] & 0x0ff;
		longByte4	=	buff[4] & 0x0ff;
		
		newFileSize	=	(longByte1 << 24) +
						(longByte2 << 16) +
						(longByte3 << 8) +
						longByte4;
						
		page_count	=	newFileSize / 512;		//*	number of pages			
		if ((newFileSize % page_count) > 0)
		{
			page_count++;
		}

		strncpy(newFileName, buff + 5, 12);
		newFileName[12]	=	0;
		
		sprintf(msgBuff, "File=%s, size=%ld page=%d", newFileName, newFileSize, page_count);
		DebugRectPrintText(msgBuff);
		
		DelayBigTime();
		SendAck();

		for (ii=0; ii< page_count; ii++)
		{
		unsigned int buffer_count	=	0;

			usart_read_bytes(519);	//read the whole page
			buff	=	usart_getBuff_ptr();
			DelayBigTime();
			if (buff[0] == 0x02)	//*	look for STX
			{
				SendAck();
			}
			else
			{
				SendNAK();
			}
		#if 0

			buffer_count	=	usart_getBuff_size();



			//program the whole page
			for(xx=0; xx<DATAFLASH_PAGESIZE; xx++)
			{
				out_data[xx]	=	buff[xx];
			}
			dataflash_program_page(&out_data[0], ii); 	//program the page
	    
	    				
			for(xx=0; xx<DATAFLASH_PAGESIZE; xx++)
			{
				in_data[xx]	=	0x00;
	     	}

			dataflash_cont_read(&in_data[0], ii, DATAFLASH_PAGESIZE);


			for (xx=0; xx<DATAFLASH_PAGESIZE; xx++)
			{
				if (xx==256)
				{
					for (wait = 0; wait < 254; wait++)
					{
						for (wait2 = 0; wait2 < 254; wait2++)
						{
							asm("nop");
						}
					}
				}
				usart_putc(in_data[xx]);
				asm("nop");
			}
		#endif
		}
	}
	else
	{
		usart_putc(kAscii_NAK);
	}
	
#ifdef _USE_DBBUG_RECT_STATUS_
	DebugRectPrintText("--DONE");
#endif
}

//*******************************************************************************
static void	FlashFileInfo(void)
{
short	ii;
char	validImage;
short	bmpWidth;
short	bmpHeight;
char	displayLine[64];


	usart_puts("\nTouchShield/Slide Flash File System Ver 0.1\n");
//	usart_putc(0);

	ii			=	0;
	validImage	=	TRUE;

//	while (validImage && (ii < BMP_MAXBMP_COUNT))
	while (ii < BMP_MAXBMP_COUNT)
	{
		bmp_get_entryname(ii, displayLine, &bmpWidth, &bmpHeight);
		usart_puts(displayLine);
		usart_puts("\n");
		ii++;
	}
}


//*******************************************************************************
static void	FlashFileRead(void)
{
unsigned int	x;
unsigned char	out[DATAFLASH_PAGESIZE];
unsigned int	page_num;
unsigned char	*buff;


#ifdef _USE_DBBUG_RECT_STATUS_
	DebugRectPrintText("IMAGE_INTERFACE_READ");
#endif

	buff		=	usart_getBuff_ptr();
	page_num	=	0;

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


//*******************************************************************************
//*		This is the download protocol rotuine, it interacts with the USB port
//*		and exchanges data with the host
//*******************************************************************************
void FlashFileSystemComm(void)
{
unsigned char	commandFromHost;
unsigned char	*buff;
boolean			keepGoing;
char			commandString[8];


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

	DebugRectPrintText("FlashFileDownload()");


	arduinoReset();
//	DebugRectPrintText("arduino in reset");

//	usart_init(115200);
	usart_init(57600);
//	DebugRectPrintText("usart_init 115200");

	cli();	//*	disable interupts

	buff	=	usart_getBuff_ptr();

	//*	send out newlines to get the programs attention
//	usart_puts("\n\n\n\n");


	//*******************************************************************************
	keepGoing	=	TRUE;
	while(keepGoing)
	{
		usart_puts("\n");

		usart_read_bytes(1);			//	Wait for us to read a byte
		commandFromHost	=	buff[0];	//	get the command byte

		strcpy(commandString, "cmd=");
		commandString[4]	=	commandFromHost;
		commandString[5]	=	0;
		DebugRectPrintText(commandString);

		switch(commandFromHost)
		{
			case IMAGE_INTERFACE_FORMAT:
				FlashFileFormat();
				break;

			case IMAGE_INTERFACE_STORE:
				FlashFileDownload();
				break;

			case IMAGE_INTERFACE_READ:
				FlashFileRead();
				break;

			case IMAGE_INTERFACE_INFO:
			//	DebugRectPrintText("IMAGE_INTERFACE_INFO");
				FlashFileInfo();
				break;


			case IMAGE_INTERFACE_EXIT:
				usart_puts("Exit Command\n");

				sei(); //enable interrupts
				bmp_init();
				usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok
				arduinoRun();

				keepGoing	=	FALSE;
				break;
				
			default:
				usart_puts("Unknown Command\n");
				DebugRectPrintText("Unknown Command");
				break;

		}
	}//end while
	
} //end function


#endif	//	_ENABLE_FLASH_FILE_SYSTEM_

