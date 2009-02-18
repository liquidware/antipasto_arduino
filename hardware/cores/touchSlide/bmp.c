//*******************************************************************************
//	Routines to read bmp files from data flash
//*******************************************************************************
//*	Jan  4,	2009	<MLS> Started cleaning up and documenting this code
//*	Jan 12,	2009	<MLS> Added bmp_get_entryname
//*	Jan 13,	2009	<MLS> There is no way the lookup table can be kept in ram, re-writing
//*	Jan 13,	2009	<MLS> Added bmp_draw_imageN
//*	Jan 14,	2009	<MLS> bmp_draw modified to return true if succesfull
//*******************************************************************************

#include	"HardwareDef.h"

#include	<avr/io.h>
#include	<inttypes.h>
#include	<stdlib.h>
#include	<inttypes.h>
#include	<string.h>
#include	<avr/interrupt.h>

#include	"dataflash.h"
#include	"graphics.h"
#include	"bmp.h"
#include	"bitops.h"
#include	"usart.h"
#include	"wiring.h"

#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif


typedef struct {
				char	fileName[7];
				char	flashOffsetByte1;
				char	flashOffsetByte2;
				char	flashOffsetByte3;
				char	flashOffsetByte4;
				} FLASH_FILE_ENTRY;

#define	kSizeOfFlashFileEntry	11


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


//*******************************************************************************
char	bmp_get_entryname(unsigned char entryNumber, char *entryName, short *imgWidth, short *imgHeight)
{
long				myIndex;
char				cc;
long				longByte1, longByte2, longByte3, longByte4;
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


		sprintf(bmp_buff, "%02X%02X %02X%02X",	(myFileEntry.flashOffsetByte1 & 0x0ff),
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

		sprintf(bmp_buff, " %3d x %3d", bmpWidth, bmpHeight);
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
		longByte1	=	(myFileEntry.flashOffsetByte1 & 0x0ff);
		longByte2	=	(myFileEntry.flashOffsetByte2 & 0x0ff);
		longByte3	=	(myFileEntry.flashOffsetByte3 & 0x0ff);
		longByte4	=	(myFileEntry.flashOffsetByte4 & 0x0ff);

		bmp_loc		=	longByte1 << 24;
		bmp_loc		|=	longByte2 << 16;
		bmp_loc		|=	longByte3 << 8;
		bmp_loc		|=	longByte4;
		
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
			xLoc	=	(kSCREEN_X_size - bmpWidth) / 2;
			yLoc	=	(kSCREEN_Y_size - bmpHeight) / 2;
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
			dispRectangle(0, 0, kSCREEN_X_size-1, kSCREEN_Y_size-1);
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
