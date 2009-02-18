//*******************************************************************************
//*		
//*	
//*	
//*******************************************************************************
//*	Detailed Edit History
//*		PLEASE put comments here every time you work on this code
//*******************************************************************************
//*	Dec 27,	2008	<MLS> Copied some bounds check from version 0.9 for dispRectangle
//*	Dec 27,	2008	<MLS> the new version of dispRectangle does not work, using old versioon
//*	Dec 27,	2008	<MLS> Changed RECT_T to RECT
//*	Dec 28,	2008	<MLS> Changed SCREEN_T to SCREEN
//*	Dec 29,	2008	<CHRIS> released v0012LW
//*	Dec 31,	2008	<MLS> Copied dispBrightness from 0012
//*	Jan  1,	2009	<MLS> dispRectangle -  Fixed overflow problem on large rectangles
//*	Jan  2,	2009	<MLS> Added negitive checking to w/h of dispRectangle
//*	Jan  3,	2009	<MLS> dispRectangle, changed xLoc, yLoc back to signed ints
//*	Jan 14,	2009	<MLS> Changed all 320/240/319/239 number constants to define constants
//*	Jan 19,	2009	<MLS> Got permission from Chris to start working on cleanin up slide code
//*	Jan 19,	2009	<MLS> Added more bounds checking to dispRectangle, fixed offscreen center problem
//*******************************************************************************


//*******************************************************************************
//*	Includeds
//*******************************************************************************
#include	<avr/io.h>
#include	<avr/pgmspace.h>
#include	<avr/interrupt.h>

#include	"HardwareDef.h"
#include	"graphics.h"
#ifdef _TOUCH_STEALTH_
	#include	"oled_stealth.h"
#endif
#ifdef _TOUCH_SLIDE_
	#include	"oled_slide.h"
#endif
#include	"bitops.h"
#include	"font.h"

#define incx() x++, dxt += d2xt, t += dxt
#define incy() y--, dyt += d2yt, t += dyt

#define DUFF_DEVICE_8(aCount, aAction) \
do { \
	int32_t count_ = (aCount); \
	int32_t times_ = (count_ + 15) >> 4; \
	switch (count_ & 7)		\
	{ \
		case 0: do { aAction; \
		case 15: aAction; \
		case 14: aAction; \
		case 13: aAction; \
		case 12: aAction; \
		case 11: aAction; \
		case 10: aAction; \
		case 9: aAction; \
		case 8: aAction; \
		case 7: aAction; \
		case 6: aAction; \
		case 5: aAction; \
		case 4: aAction; \
		case 3: aAction; \
		case 2: aAction; \
		case 1: aAction; \
	} while (--times_ > 0); \
	} \
} while (0)

COLOR	GraphicsColor;
SCREEN	screen	=	{ 
					kSCREEN_X_size,			//*	Width
					kSCREEN_Y_size};		//*	Height

#pragma mark -
#pragma mark _TOUCH_SLIDE_

#ifdef _TOUCH_SLIDE_
//*******************************************************************************
//*	Slide specific stuff
//*******************************************************************************

//*******************************************************************************
void	dispCommand(unsigned char command)
{
volatile unsigned char c = command;

	CLRBIT(OLED_CTRL_PORT,OLED_DC);
	CLRBIT(OLED_CTRL_PORT,OLED_CS);
	SETBIT(OLED_CTRL_PORT,OLED_RD);
	CLRBIT(OLED_CTRL_PORT,OLED_WR);

	OLED_DATA_HIGH	=	0;
	OLED_DATA_MED	=	(c >> 5) << 2;
	OLED_DATA_LOW	=	c << 3;

	SETBIT(OLED_CTRL_PORT,OLED_WR);
	SETBIT(OLED_CTRL_PORT,OLED_CS);
	SETBIT(OLED_CTRL_PORT,OLED_DC);

}

//*******************************************************************************
void	dispData(unsigned int data)
{

	volatile unsigned char lB	=	(unsigned char)data;
	volatile unsigned char hB	=	(unsigned char)(data>>8);
	
	SETBIT(OLED_CTRL_PORT,OLED_DC);
	CLRBIT(OLED_CTRL_PORT,OLED_CS);
	// SETBIT(OLED_CTRL_PORT,OLED_RD);
	CLRBIT(OLED_CTRL_PORT,OLED_WR);
	
	OLED_DATA_LOW	=	(lB << 3);
	OLED_DATA_MED	=	(lB >> 3);
	OLED_DATA_MED	=	OLED_DATA_MED | (hB << 6);
	OLED_DATA_HIGH	=	hB;
	
	
	SETBIT(OLED_CTRL_PORT,OLED_WR);
	SETBIT(OLED_CTRL_PORT,OLED_CS);
	// SETBIT(OLED_CTRL_PORT,OLED_DC);
}


//*******************************************************************************
void	dispColor(COLOR c)
{
	GraphicsColor.red	=	c.red;
	GraphicsColor.green	=	c.green;
	GraphicsColor.blue	=	c.blue;
}


//*******************************************************************************
void	dispPix()
{
	CLRBIT(OLED_CTRL_PORT,OLED_CS);
	SETBIT(OLED_CTRL_PORT,OLED_DC);
	SETBIT(OLED_CTRL_PORT,OLED_RD);
	CLRBIT(OLED_CTRL_PORT,OLED_WR);

	OLED_DATA_LOW	=	GraphicsColor.blue;
	OLED_DATA_MED	=	GraphicsColor.green;
	OLED_DATA_HIGH	=	GraphicsColor.red;

	SETBIT(OLED_CTRL_PORT,OLED_WR);
	SETBIT(OLED_CTRL_PORT,OLED_CS);
}

//*******************************************************************************
void	dispSetWindow(int x, int y, int width, int height)
{
	uint16_t temp;

	//* Error checking
	if ((width <= 0) || (height <= 0) )
	{
		return;
	}

	//* Bounds clipping
	if (x < 0 )
	{
		width += x-0;
		x	=	0;
	}
	if (y < 0 )
	{
		height += y-0;
		y	=	0;
	}
	if (x >= kSCREEN_X_size )
	{
		x	=	(kSCREEN_X_size - 1);
	}
	if (y >= kSCREEN_Y_size )
	{
		y	=	(kSCREEN_Y_size - 1);
	}
	if (x+width  >= kSCREEN_X_size)
	{
		width	=	(kSCREEN_X_size - 1) - x;
	}
	if (y+height >= kSCREEN_Y_size)
	{
		height	=	(kSCREEN_Y_size - 1) - y;
	}


	/* Specify the Vertical start positions */
	dispCommand(kOLEDcmd_VerticalRamAddrHi);
	dispData(x);

	/* Specify the Vertical end positions */
	temp	=	(x+width);
	dispCommand(kOLEDcmd_VerticalRamAddrLow);
	dispData(temp);
	
    /* Specify the horizontal start/end positions */
	dispCommand(kOLEDcmd_HorizontalRamAddr);
	dispData( ((y)<<8)|(y+height) );	
	
	/* Specify the x address in RAM */			
	dispCommand(kOLEDcmd_GRAMaddressSetX);
	dispData(y);
	
	/* Specify the y address in RAM */
	dispCommand(kOLEDcmd_GRAMaddressSetY);
	dispData(x);

	/* RAM write */
	dispCommand(kOLEDcmd_GRAMread_write);

}

//*******************************************************************************
void	dispPixel(int x, int y)
{
	
/* Bounds clipping */	
	if ( (x >= kSCREEN_X_size) || (y >= kSCREEN_Y_size))
	{
		return;
	}
	if ( (x < 0) || (y < 0))
	{
		return;
	}

/* Set XY location   */
	dispCommand(kOLEDcmd_GRAMaddressSetX);	//Specify the x address in RAM
	dispData(0x00FF & y);

	dispCommand(kOLEDcmd_GRAMaddressSetY);	//Specify the y address in RAM
	dispData(0x01FF & x);

	dispCommand(kOLEDcmd_GRAMread_write);	//RAM write	

/* Draw pixel */
	CLRBIT(OLED_CTRL_PORT,OLED_CS);
	CLRBIT(OLED_CTRL_PORT,OLED_WR);

	OLED_DATA_LOW	=	GraphicsColor.blue;
	OLED_DATA_MED	=	GraphicsColor.green;
	OLED_DATA_HIGH	=	GraphicsColor.red;

	SETBIT(OLED_CTRL_PORT,OLED_WR);
	SETBIT(OLED_CTRL_PORT,OLED_CS);

}

//*******************************************************************************
//*	Jan 29, 2009	<MLS> Changed args to int (were uint16_t)
void	dispRead(COLOR *buffer, int  x, int  y)
{

	/* Set XY location   */
	dispCommand(kOLEDcmd_GRAMaddressSetX);	//Specify the x address in RAM
	dispData(0x00FF & y);

	dispCommand(kOLEDcmd_GRAMaddressSetY);	//Specify the y address in RAM
	dispData(0x01FF & x);

	/* RAM write */
	dispCommand(kOLEDcmd_GRAMread_write);

	/* Input Direction */
	OLED_DATA_LOW_DDR	=	0x00;
	OLED_DATA_MED_DDR	=	0x00;
	OLED_DATA_HIGH_DDR	=	0x00;

	/* Read pixel */
	SETBIT(OLED_CTRL_PORT,OLED_WR);
	CLRBIT(OLED_CTRL_PORT,OLED_CS);
	CLRBIT(OLED_CTRL_PORT,OLED_RD);

	CLRBIT(OLED_CTRL_PORT,OLED_RD);
	CLRBIT(OLED_CTRL_PORT,OLED_RD);

	buffer[0].blue	=	OLED_DATA_LOW_PIN;
	buffer[0].green	=	OLED_DATA_MED_PIN;
	buffer[0].red	=	OLED_DATA_HIGH_PIN;
	
	SETBIT(OLED_CTRL_PORT,OLED_RD);
	SETBIT(OLED_CTRL_PORT,OLED_CS);

	/* Output direction */
	OLED_DATA_LOW_DDR	=	0xFF;
	OLED_DATA_MED_DDR	=	0xFF;
	OLED_DATA_HIGH_DDR	=	0xFF;
}

//*******************************************************************************
//*	Jan  1,	2009	<MLS> dispRectangle -  Fixed overflow problem on large rectangles
//*	Jan  3,	2009	<MLS> dispRectangle, changed xLoc, yLoc back to signed ints
//*******************************************************************************
void	dispRectangle(int  xLoc,  int  yLoc,   int rectWidth,   int  rectHeight) 
{
//int32_t len	=	(width*height);	//*	if width or height get modified, this is messed up
long	len;			
long	myWidth;
long	myHeight;

	myWidth		=	rectWidth;
	myHeight	=	rectHeight;
	//*	Jan  2,	2009	<MLS> Added negitive checking to w/h of dispRectangle
	if ((xLoc >= kSCREEN_X_size)  || (yLoc >= kSCREEN_Y_size) || (rectWidth <= 0) || (rectHeight <= 0))
	{
		return;
	}
	//*	Jan 19,	2009	<MLS> Added more bounds checking to dispRectangle, fixed offscreen center problem
	if (((xLoc + myWidth) < 0) || ((yLoc + myHeight) < 0))
	{
		return;
	}

	if ((xLoc + myWidth) >= kSCREEN_X_size)
	{
		myWidth	=	(kSCREEN_X_size - 1) - xLoc;
	}
	if ((yLoc + myHeight) >= kSCREEN_Y_size) 
	{
		myHeight	=	(kSCREEN_Y_size - 1) - yLoc;
	}

	
	//*	Set XY location
	dispSetWindow(xLoc, yLoc, myWidth, myHeight);

	//*	Clear the screen
	CLRBIT(OLED_CTRL_PORT,OLED_CS);

	//*	Set the color once
	OLED_DATA_LOW	=	GraphicsColor.blue;
	OLED_DATA_MED	=	GraphicsColor.green;
	OLED_DATA_HIGH	=	GraphicsColor.red;

	len	=	(myWidth * myHeight);

#ifndef __MWERKS__
	DUFF_DEVICE_8(len,
			  asm("ldi r24,0x20		""\n\t"
				  "sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
				  "sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
				  "sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
				  "sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
				  "sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
				  "sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
				  "sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
				  "sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
				  :
				  :
				  : "r24" ););

	//*	SET the screen CS
	SETBIT(OLED_CTRL_PORT,OLED_CS);
#endif
}

//*******************************************************************************
void	dispClearScreen()
{
	unsigned int i=2400;
  
	/* Set XY location   */
	dispCommand(kOLEDcmd_GRAMaddressSetX);	//Specify the x address in RAM
	dispData(0);

	dispCommand(kOLEDcmd_GRAMaddressSetY);	//Specify the y address in RAM
	dispData(0);

	dispCommand(kOLEDcmd_GRAMread_write);	//RAM write


	/* Clear the screen */
	CLRBIT(OLED_CTRL_PORT,OLED_CS);

	/* Set the color once */
	OLED_DATA_LOW	=	GraphicsColor.blue;
	OLED_DATA_MED	=	GraphicsColor.green;
	OLED_DATA_HIGH	=	GraphicsColor.red;
  
	/* Start the clocking of the WR pin */

#ifndef __MWERKS__
	while(i--)
	{
		/* Set & Clear */
		asm("ldi r24,0x20	   ""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"

			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"


			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"

			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			"sts 0x0109,r24	""\n\t" "sts 0x0109,r24	""\n\t"
			:
			:
			: "r24"
		);
	}
#endif
}
#endif	//	_TOUCH_SLIDE_


#if 0
//*******************************************************************************
void	dispScrollVertical(short startRow, short endRow, short numRows)
{
//*	this worked but veritcal is actually horzontal in the way this code works
//*	also, once scrolled, the cordinates are messed up and it stays scrolled,
//*	in other words, USELESS!!!!
#ifdef _TOUCH_SLIDE_

	dispCommand(kOLEDcmd_VerticalScrollCtrlHi);
	dispData(0);

	dispCommand(kOLEDcmd_VerticalScrollCtrlLo);
	dispData(300);

	dispCommand(kOLEDcmd_VerticalScrollCtrl2);
	dispData(10);
#endif

}
#endif