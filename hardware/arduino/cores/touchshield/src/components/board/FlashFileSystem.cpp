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
//*	Jan 17,	2009	<MLS> Started seperate file for new file system
//*******************************************************************************

#include	<avr/io.h>
#include	<inttypes.h>
#include	<string.h>
#include	<inttypes.h>
#include	<avr/interrupt.h>
#include	<stdlib.h>
#include	<stdio.h>

#include	"HardwareDef.h"

#ifdef _ENABLE_FLASH_FILE_SYSTEM_

//#include	"image_interface.h"
#include	"FlashFileSystem.h"
#include	"usart.h"
#include	"dataflash.h"
#include	"bitops.h"
#include	"bmp.h"
#include	"wiring.h"

#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif

#define	_USE_DBBUG_RECT_STATUS_

#warning this needs to be removed for memory convervation
COLOR black = { 0,0,0};
COLOR textColor = {255,255,255};
COLOR white = {255,255,255};
uint16_t gPageIndex = 0;


//************************************************************************
//* A debug tool.
//* AutoIncrements the line.
static void DebugRectPrintTextLocal(char * msg)
{
	static int line;

	dispColor(black);
	dispRectangle(0,line,319,line+8);
	dispPutS(msg,10,line,textColor, black);

	textColor.red +=20;
	textColor.green -=50;
	textColor.blue += 50;

	line+=10;
	if (line > 220)
	{
		line = 0;
	}

}

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
//long	ii;

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
	DebugRectPrintTextLocal("sending ACK +++");
	usart_putc(kAscii_ACK);
	usart_puts("ACK");

}

//*******************************************************************************
static void	SendNAK(void)
{
	DebugRectPrintTextLocal("sending NAK ---");

	usart_putc(kAscii_NAK);
	usart_puts("NAK");

}

//*******************************************************************************
static void	FlashFileDownload(void)
{
unsigned int	xx;
unsigned int	ii;
//unsigned int	wait,wait2;
unsigned char	dataBuff[DATAFLASH_PAGESIZE];
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
	DebugRectPrintTextLocal("STORE");
#endif

	buff	=	usart_getBuff_ptr();

	/* Clear the buffer */
	for (ii=0; ii<17; ii++)
	{
		buff[ii]	=	0;
	}

	usart_putc(IMAGE_INTERFACE_PAGE_DONE); //respond

	while (buff[0] != 0x01)	//*	wait for SOH
	{
		usart_read_bytes(1);
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

		page_count	=	newFileSize / DATAFLASH_PAGESIZE;		//*	number of pages
		if ((newFileSize % DATAFLASH_PAGESIZE) > 0)
		{
			page_count++;
		}

		strncpy((char*)newFileName, (const char*)buff + 5, 12);
		newFileName[12]	=	0;

		sprintf(msgBuff, "File=%s, size=%ld pages=%d", newFileName, newFileSize, page_count);
		DebugRectPrintTextLocal(msgBuff);

		bmp_store((char*)dataBuff,(char*)newFileName,(uint32_t)gPageIndex*DATAFLASH_PAGESIZE); //save the file name and offset

		usart_putc(IMAGE_INTERFACE_PAGE_DONE); //respond

		for (ii=gPageIndex; ii< gPageIndex+page_count; ii++)
		{
		unsigned int buffer_count	=	0;
		unsigned char outChecksum = 0;
		unsigned char inChecksum = 0;

			usart_read_bytes(DATAFLASH_PAGESIZE);	//read the whole page
			buff	=	usart_getBuff_ptr();

			//program the whole page
			for(xx=0; xx<DATAFLASH_PAGESIZE; xx++)
			{
				dataBuff[xx]	=	buff[xx];
				inChecksum += buff[xx];					//increment the checksum
			}

			inChecksum = (inChecksum ^ 0xFF) + 1;		// two's compliment
			dataflash_program_page(&dataBuff[0], ii); 	//program the page

			/* Clear the buffer */
			for(xx=0; xx<DATAFLASH_PAGESIZE; xx++)
			{
				dataBuff[xx]	=	0x00;
			}

			dataflash_cont_read(&dataBuff[0], ii, DATAFLASH_PAGESIZE);	//read the page

			for (xx=0; xx<DATAFLASH_PAGESIZE; xx++)						//calculate the checksum
			{
				outChecksum += dataBuff[xx];
			}

			outChecksum = (outChecksum ^ 0xFF) + 1;						// two's compliment

			/* Make sure the checksum matches */
			if (inChecksum == outChecksum)
			{

				sprintf(msgBuff, "File=%s %d / %d",newFileName, (ii-gPageIndex)+1, page_count);
				DebugRectPrintTextLocal(msgBuff);
				usart_putc(IMAGE_INTERFACE_PAGE_DONE); 			//respond

			}
			else
			{
				DebugRectPrintTextLocal("checksum fail");
				usart_putc(0); 									//error
			}

		} //end for

		/* Incremement the page index*/
		gPageIndex+= page_count;
	}
	else
	{
		usart_putc(kAscii_NAK);
	}

#ifdef _USE_DBBUG_RECT_STATUS_
	DebugRectPrintTextLocal("--DONE");
#endif
}

//*******************************************************************************
static void	FlashFileInfo(void)
{
//short	ii;
//char	validImage;
//short	bmpWidth;
//short	bmpHeight;
//char	displayLine[64];


//	usart_puts("\nTouchShield/Slide Flash File System Ver 0.1\n");
	usart_puts("Flasher v0.2");
	usart_putc(0);

#if 0
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
#endif
}


//*******************************************************************************
static void	FlashFileRead(void)
{
unsigned int	x;
unsigned char	out[DATAFLASH_PAGESIZE];
unsigned int	page_num;
unsigned char	*buff;


#ifdef _USE_DBBUG_RECT_STATUS_
	DebugRectPrintTextLocal("IMAGE_INTERFACE_READ");
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


	DebugRectPrintTextLocal("FlashFileSystemComm()");
	DebugRectPrintTextLocal("Waiting for images");

	arduinoReset();

	//usart_init(57600);
	usart_init(115200);

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
		DebugRectPrintTextLocal(commandString);

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
			//	DebugRectPrintTextLocal("IMAGE_INTERFACE_INFO");
				FlashFileInfo();
				break;


			case IMAGE_INTERFACE_EXIT:

				DebugRectPrintTextLocal("File Send complete.");

				sei(); //enable interrupts
				bmp_init();
				usart_putc(IMAGE_INTERFACE_PAGE_DONE); //ok
				arduinoRun();

				keepGoing	=	FALSE;
				break;

			default:
				DebugRectPrintTextLocal("Unknown Command");
				break;

		}
	}//end while

} //end function

//*************************************
// Launches an executable program
void FlashFile_open(ProgramEx p)
{
	/* Determine the program */
	switch (p)
	{
		case FlashTransfer:
			FlashFileSystemComm();
			break;

		default:
			break;
	}
}

//*************************************
// A wrapper to launch an executable program
void open(ProgramEx p) {
	FlashFile_open(p);
}

#endif	//	_ENABLE_FLASH_FILE_SYSTEM_

