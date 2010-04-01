//*******************************************************************************
//	Routines to read bmp files from data flash
//*******************************************************************************
//*	<CML>		=	Chris Ladden <Chris @ antipastohw.blogspot.com & liquidware.com>
//*	<MLS>		=	Mark Sproul <msproul@jove.rutgers.edu>
//*******************************************************************************
//*	Jan  4,	2009	<MLS> Started cleaning up and documenting this code
//*	Jan 12,	2009	<MLS> Added bmp_get_entryname
//*	Jan 13,	2009	<MLS> There is no way the lookup table can be kept in ram, re-writing
//*	Jan 13,	2009	<MLS> Added bmp_draw_imageN
//*	Jan 14,	2009	<MLS> bmp_draw modified to return true if succesfull
//*	Sep 16,	2009	<CML> Added PImage routines
//*	Jan 12,	2010	<MLS> Merged Chris's updates with mine
//*	Jan 14,	2010	<MLS> Added _USE_7LETTER_NAMES_ to be able to use the old system
//*******************************************************************************

#include	"HardwareDef.h"

#include	<avr/io.h>
#include	<inttypes.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<inttypes.h>
#include	<string.h>
#include	<avr/interrupt.h>

#include	"dataflash.h"
#include	"graphics.h"
#include	"bmp.h"
#include	"bitops.h"
#include	"usart.h"
#include	"wiring.h"
#include	"FlashFileSystem.h"

#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif

#define BMP_LOOKUP_TABLE_SIZE DATAFLASH_PAGESIZE //!< Two dataflash pages

FLASH_FILE_ENTRY	gBMPlookupTable[kLookupTableMaxEntries+1];
uint16_t			gBmpLookupTableEntries	=	0;


//*	for some VERY strange reason, it gets into a reset loop if this isnt here
//unsigned char	bmp_lookup_table[BMP_LOOKUP_TABLE_SIZE];
//unsigned char	bmp_lookup_table_status	=	FALSE;



//*******************************************************************************
void	bmp_init()
{
	//*	read the look up table from the data flash
//	dataflash_cont_read(&bmp_lookup_table[0],0,BMP_LOOKUP_TABLE_SIZE);
//	bmp_lookup_table_status	=	TRUE;

}

//*******************************************************************************
unsigned char	bmp_lookup_table_checkStatus()
{
//	return bmp_lookup_table_status;
	return TRUE;
}


//*******************************************************************************
void	GetFlashFileEntry(short fileIndex, FLASH_FILE_ENTRY *fileEntry)
{
long		myByteIndex;

	myByteIndex	=	fileIndex * kSizeOfFlashFileEntry;

	dataflash_read_block((unsigned char *)fileEntry, myByteIndex, kSizeOfFlashFileEntry);
}

#if 1
//*******************************************************************************
char	bmp_get_entryname(unsigned char entryNumber, char *entryName, short *imgWidth, short *imgHeight)
{
//long				myIndex;
char				cc;
long				longByte1, longByte2;
#ifdef _USE_7LETTER_NAMES_
	long			longByte3, longByte4;
#endif
long				bmp_loc;
unsigned char		bmp_buff[16];
int					bmpWidth;
int					bmpHeight;
FLASH_FILE_ENTRY	myFileEntry;
char				validImage;


	GetFlashFileEntry(entryNumber, &myFileEntry);
	
	
	strncpy(entryName, myFileEntry.fileName, 7);
	if (entryName[0] >= 0x20)
	{
		//*	we have a valid entry
		validImage		=	TRUE;
		cc				=	7;
		entryName[cc++]	=	0x20;
		entryName[cc++]	=	0x20;
		entryName[cc++]	=	0x20;
		entryName[cc++]	=	0;

#ifdef _USE_7LETTER_NAMES_
		sprintf((char *)bmp_buff, "%02X%02X %02X%02X",
									(myFileEntry.flashOffsetByte1 & 0x0ff),
									(myFileEntry.flashOffsetByte2 & 0x0ff),
									(myFileEntry.flashOffsetByte3 & 0x0ff),
									(myFileEntry.flashOffsetByte4 & 0x0ff));
		strcat(entryName, (char *)bmp_buff);

		longByte1		=	(myFileEntry.flashOffsetByte1 & 0x0ff);
		longByte2		=	(myFileEntry.flashOffsetByte2 & 0x0ff);
		longByte3		=	(myFileEntry.flashOffsetByte3 & 0x0ff);
		longByte4		=	(myFileEntry.flashOffsetByte4 & 0x0ff);

		bmp_loc		=	longByte1 << 24;
		bmp_loc		|=	longByte2 << 16;
		bmp_loc		|=	longByte3 << 8;
		bmp_loc		|=	longByte4;
#else
		bmp_loc	=	myFileEntry.flashOffset;
		sprintf((char *)bmp_buff, "%04X", bmp_loc);
		strcat(entryName, (char *)bmp_buff);
#endif

		//* Get the width and height
		dataflash_read_block(bmp_buff, bmp_loc, 2);
		
		longByte1		=	bmp_buff[0];
		longByte2		=	bmp_buff[1];
	//	longByte3		=	bmp_buff[2];
	//	longByte4		=	bmp_buff[3];

	//	bmpWidth	=	(longByte1 << 8) + longByte2;
	//	bmpHeight	=	(longByte3 << 8) + longByte4;
		bmpWidth	=	longByte1 & 0x0ff;
		bmpHeight	=	longByte2 & 0x0ff;

		sprintf((char *)bmp_buff, " %3d x %3d", bmpWidth, bmpHeight);
		strcat(entryName, (char *)bmp_buff);
		
		*imgWidth	=	bmpWidth;
		*imgHeight	=	bmpHeight;
		
	}
	else
	{
		validImage		=	FALSE;
		entryName[0]	=	0;
	}
	return(validImage);
}
#endif


//*******************************************************************************
//*	returns true if found
char	bmp_get_entryInfoFromName(char *bmp_name, short *imgWidth, short *imgHeight)
{
unsigned int		ii;
FLASH_FILE_ENTRY	myFileEntry;
char				imageOK;
short				bmpWidth;
short				bmpHeight;
char				fileNameInfo[32];

	imageOK	=	FALSE;
	cli(); //disable interrupts
	for (ii = 0; ii < BMP_MAXBMP_COUNT; ii++)
	{

		GetFlashFileEntry(ii, &myFileEntry);
		if (strncmp(bmp_name, myFileEntry.fileName, 7) == 0)
		{
			bmp_get_entryname(ii, fileNameInfo, &bmpWidth, &bmpHeight);
			imageOK		=	TRUE;
			*imgWidth	=	bmpWidth;
			*imgHeight	=	bmpHeight;
			
			break;
		}
	}

	sei(); //enable interrupts
	return(imageOK);
}


//*******************************************************************************
//*	returns TRUE if image was drawn
char	bmp_draw_imageN(int imageIndex, int xLoc, int yLoc)
{
unsigned long		bmp_loc;
unsigned char		bmp_buff[DATAFLASH_PAGESIZE];
int					bmpWidth;
int					bmpHeight;
long				length;
int					byteCnt;
int					pixelX, pixelY;
long				longByte1, longByte2, longByte3, longByte4;
FLASH_FILE_ENTRY	myFileEntry;
char				imageOK;

	imageOK	=	FALSE;
	cli(); //disable interrupts


	GetFlashFileEntry(imageIndex, &myFileEntry);
	if (myFileEntry.fileName[0] >= 0x20)
	{
		//*	OK, it looks like a valid file
		imageOK		=	TRUE;

	#ifdef _USE_7LETTER_NAMES_
		longByte1	=	(myFileEntry.flashOffsetByte1 & 0x0ff);
		longByte2	=	(myFileEntry.flashOffsetByte2 & 0x0ff);
		longByte3	=	(myFileEntry.flashOffsetByte3 & 0x0ff);
		longByte4	=	(myFileEntry.flashOffsetByte4 & 0x0ff);
		bmp_loc		=	longByte1 << 24;
		bmp_loc		|=	longByte2 << 16;
		bmp_loc		|=	longByte3 << 8;
		bmp_loc		|=	longByte4;
	#else
		bmp_loc		=	myFileEntry.flashOffset;
	#endif
		/* Get the width and height */
		dataflash_read_block(bmp_buff, bmp_loc, 2);
		
		longByte1		=	bmp_buff[0];
		longByte2		=	bmp_buff[1];
	//	longByte3		=	bmp_buff[2];
	//	longByte4		=	bmp_buff[3];

		bmpWidth	=	longByte1 & 0x0ff;
		bmpHeight	=	longByte2 & 0x0ff;
		length		=	(uint32_t)bmpWidth * (uint32_t)bmpHeight;


		//* Incremement the bmp pointer passed the width/height
		bmp_loc		+=	2;
		dataflash_read_block(bmp_buff, bmp_loc, DATAFLASH_PAGESIZE);
		byteCnt		=	0;

		if ((xLoc == -1) && (yLoc == -1))
		{
			//*	if xLoc and yLoc are -1, then CENTER the image
			xLoc	=	(gWidth - bmpWidth) / 2;
			yLoc	=	(gHeight - bmpHeight) / 2;
			if (xLoc < 0)
			{
				xLoc	=	0;
			}
			if (yLoc < 0)
			{
				yLoc	=	0;
			}
			GraphicsColor.red	=	0;
			GraphicsColor.blue	=	0;
			GraphicsColor.green	=	0;
			dispRectangle(0, 0, gWidth-1, gHeight-1);
		}
		pixelX	=	0;
		pixelY	=	0;

		while ((pixelY < bmpHeight) && (length > 0))
		{
			GraphicsColor.red	=	bmp_buff[byteCnt++];
			GraphicsColor.blue	=	bmp_buff[byteCnt++];
			GraphicsColor.green	=	bmp_buff[byteCnt++];
			
			dispPixel(xLoc + pixelX, yLoc + pixelY);
			pixelX++;
			if (pixelX >= bmpWidth)
			{
				pixelX	=	0;
				pixelY++;
			}

			if (byteCnt >= DATAFLASH_PAGESIZE)
			{
				bmp_loc	+=	DATAFLASH_PAGESIZE;
				dataflash_read_block(bmp_buff, bmp_loc, DATAFLASH_PAGESIZE);
				byteCnt	=	0;
			}
		
			length--;
		}
	}

	sei(); //enable interrupts
	
	return(imageOK);
}


//*******************************************************************************
char	bmp_draw(char *bmp_name, int xLoc, int yLoc)
{
unsigned int		ii;
FLASH_FILE_ENTRY	myFileEntry;
char				imageOK;

	cli(); //disable interrupts
	imageOK	=	FALSE;
	for (ii = 0; ii < BMP_MAXBMP_COUNT; ii++)
	{

		GetFlashFileEntry(ii, &myFileEntry);
		if (strncmp(bmp_name, myFileEntry.fileName, 7) == 0)
		{
			imageOK	=	bmp_draw_imageN(ii, xLoc, yLoc);
			
			break;
		}
	}

	sei(); //enable interrupts
	return(imageOK);
}

//*********************************************************************************
void bmp_drawTest(char * bmp_name, unsigned int x, unsigned int y)
{
unsigned int	i		=	0;
//unsigned char * lookup_table = &bmp_lookup_table[0];
unsigned long	bmp_loc	=	0;
//uint8_t bmpNameLen = strlen(bmp_name);
COLOR			black	=	{0,0,0};
COLOR			white	=	{255,255,255};

	cli(); //disable interrupts

	bmp_loc	=	bmp_find(bmp_name);

	/* Bitmap not found
		error handling here */
#if 0
	if (bmp_loc == 0)
	{
		/* Display the not found placeholder */
		dispPutS(bmp_name,x,y,white,black);
		return;
	}
#endif

	unsigned char bmp_buff[DATAFLASH_PAGESIZE];

/* Get the width and height */
	dataflash_read_block(bmp_buff,bmp_loc,4);

	uint16_t width	=	(uint16_t)(bmp_buff[0]<<8) + bmp_buff[1];
	uint16_t height	=	(uint16_t)(bmp_buff[2]<<8) + bmp_buff[3];
	uint32_t length	=	(uint32_t)width * (uint32_t)height;
	uint16_t byteCnt=0;

	dispSetWindow(x,y,width-1,height);

/* Incremement the bmp pointer passed the width/height */
	bmp_loc+=4;
	dataflash_read_block(bmp_buff, bmp_loc, DATAFLASH_PAGESIZE);
	while (length--)
	{
		GraphicsColor.blue	=	bmp_buff[byteCnt];
		GraphicsColor.green =	bmp_buff[byteCnt+1];
		GraphicsColor.red	=	bmp_buff[byteCnt+2];
		dispPix();

		byteCnt+=3;

		if (byteCnt>=528)
		{
			bmp_loc+=528;
			dataflash_read_block(bmp_buff, bmp_loc, DATAFLASH_PAGESIZE);
			byteCnt=0;
		}
	}

	sei(); //enable interrupts
}


//*********************************************************************************
//* Stores an image file entry into the lookup table
//* Concat's the file entry to the flash lookup table. 
//* 
//* tBuff should be 528 bytes or more.
//*
//* Be careful with lots of flash writes.
void bmp_store(char* tBuff, char* fileName, uint32_t location)
{

	FLASH_FILE_ENTRY *fileEntry;
	uint16_t x;

	fileEntry	=	&gBMPlookupTable[gBmpLookupTableEntries];

	/* Store the file name */
	for (x=0;x<kFileNameSize; x++)
	{
		fileEntry->fileName[x]	=	fileName[x];
	}

	/* Store the location */
#ifdef _USE_7LETTER_NAMES_

#else
	fileEntry->flashOffset	=	location;
#endif

	dataflash_program_page((unsigned char*)gBMPlookupTable, kLookupTablePage); 	//program the page

	gBmpLookupTableEntries++;
}

//**********************************************************************************
// Returns the bitmap location in flash
// Uses the lookup table stored in the bottom of flash.
uint32_t bmp_find(char *fileName)
{
	
	FLASH_FILE_ENTRY fileEntry;
	uint16_t ii;
	uint32_t lookupLoc;
	
	lookupLoc	=	(uint32_t)kLookupTablePage * (uint32_t)DATAFLASH_PAGESIZE;

	for (ii=0;ii < kLookupTableMaxEntries; ii++)
	{
		
		/* Read the file entry out of the flash lookup table */
		dataflash_read_block((unsigned char*)&fileEntry,
							 lookupLoc,
							 sizeof(FLASH_FILE_ENTRY));

		/* Check for a file name match */
		if (!strncmp(fileName,(char*)&fileEntry.fileName[0],kFileNameSize))
		{
			
		#ifdef _USE_7LETTER_NAMES_
		#else
			return fileEntry.flashOffset;
		#endif
		}
		
		/* Next entry */
		lookupLoc+= sizeof(FLASH_FILE_ENTRY);
	
	}

	return 0;
}

//********************************************************
// A processing style image loading function.
// Returns image pointer to an external flash location 
PImage loadImage(char * fileName)
{

	/* Get the image page location */
	return (PImage)bmp_find(fileName);
}

//********************************************************
// A processing style image display function.
void image(PImage image, int xLoc, int yLoc)
{
unsigned char bmp_buff[DATAFLASH_PAGESIZE];

	cli(); //disable interrupts

	/* Get the width and height */
	dataflash_read_block(bmp_buff,image,4);

	uint16_t width		=	(uint16_t)(bmp_buff[0]<<8) + bmp_buff[1];
	uint16_t height		=	(uint16_t)(bmp_buff[2]<<8) + bmp_buff[3];
	uint32_t length		=	(uint32_t)width * (uint32_t)height;
	uint16_t byteCnt	=	0;

	dispSetWindow(xLoc,yLoc,width-1,height);

	/* Incremement the bmp pointer passed the width/height */
	image+=4;
	dataflash_read_block(bmp_buff, image, DATAFLASH_PAGESIZE);
	while (length--)
	{
		GraphicsColor.blue	=	bmp_buff[byteCnt];
		GraphicsColor.green =	bmp_buff[byteCnt+1];
		GraphicsColor.red	=	bmp_buff[byteCnt+2];
		dispPix();

		byteCnt+=3;

		if (byteCnt>=528)
		{
			image	+=	528;
			dataflash_read_block(bmp_buff, image, DATAFLASH_PAGESIZE);
			byteCnt	=	0;
		}
	}

	sei(); //enable interrupts

}


