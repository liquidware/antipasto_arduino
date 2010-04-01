//*******************************************************************************

/*                                              -*- mode:C; fill-column:100 -*-
  startup.c - The Arduino C-level entry point
  Now: Copyright (C) 2008 Christopher Ladden
  Previous: Copyright (C) 2008 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
  USA
*/
//*******************************************************************************
//*	Edit history
//*******************************************************************************
//*	Jan 20,	2010	<MLS> Adding LiquidWare startup logo
//*******************************************************************************


#ifndef	_STRING_H_
	#include <string.h>
#endif

#include	"HardwareDef.h"

#include	"wiring_private.h"
#include	"graphics.h"
#include	"SubPGraphics.h"
#include	"SubPOptions.h"
#include	"font.h"

#include	"startup.h"


#ifdef _STARTUPSCREEN_LIQUIDWARE_
	#include	"LiquidWareLogo.h"
	static void	DisplayColorRLEfromPGMmem(unsigned char *rleBuff, COLOR *colorFade, short startX, short startY);
#endif

#define	kLinrSpacing	11

//*******************************************************************************
void sketchEarlyInits()
{
COLOR	myBackColor;
COLOR	myFontColor;
char	startupMsg[128];
int		yTextLoc;

	gWidth	=	GetScreenWidth();
	gHeight	=	GetScreenHeight();


#ifdef _ENABLE_CLIPPING_
	dispRemoveClip();
#endif

	myBackColor.red		=	0;
	myBackColor.green	=	0;
	myBackColor.blue	=	0;

	myFontColor.red		=	0;
	myFontColor.green	=	255;
	myFontColor.blue	=	0;

	yTextLoc	=	10;
	strcpy(startupMsg, kDisplayHardwareString);
	strcat(startupMsg, " ");
	strcat(startupMsg, kDisplayHardwareVersion);
	dispPutS(startupMsg, 5, yTextLoc, myFontColor, myBackColor);
	yTextLoc	+=	kLinrSpacing;
	
	
	//*	display the SubProcessing library version
	strcpy(startupMsg, "Arduino Procssing Library ");
	strcat(startupMsg, kSubP_VersionString);
	dispPutS(startupMsg, 5, yTextLoc, myFontColor, myBackColor);
	yTextLoc	+=	kLinrSpacing;

	strcpy(startupMsg, "Options: ");
#ifdef _SUBP_OPTION_7_SEGMENT_
	strcat(startupMsg, "+7");
#endif
#ifdef _SUBP_OPTION_GAMES_
	strcat(startupMsg, "+G");
#endif
#ifdef _ENABLE_HERSHEY_FONTS_
	strcat(startupMsg, "+H");
#endif
#ifdef _SUBP_OPTION_KEYBOARD_
	strcat(startupMsg, "+K");
#endif
#ifdef _ENABLE_SCREEN_ROTATION_
	strcat(startupMsg, "+SR");
#endif
#ifdef _ENABLE_TAHOMA_FONTS_
	strcat(startupMsg, "+T");
#endif
#ifdef _ENABLE_XLARGE_NUMBERS_
	strcat(startupMsg, "+X");
#endif

	dispPutS(startupMsg, 5, yTextLoc, myFontColor, myBackColor);
	yTextLoc	+=	kLinrSpacing;


#ifdef _STARTUPSCREEN_LIQUIDWARE_
   	for (int ii=0; ii<230; ii+=10)
	{
	COLOR	fadeColor;

		//*	this is the FADE color, how much to subtract from the actual colors
		//*	0 means none.
		fadeColor.red	=	ii;
		fadeColor.green	=	ii;
		fadeColor.blue	=	ii;
		DisplayColorRLEfromPGMmem(gLWstartupLogo, &fadeColor, -1, -1);
	}
#endif

}


//*******************************************************************************
void sketchLateInits()
{
   /* Nothing */
}


//*******************************************************************************
void runSketch()
{

	setup();			/* Finally!  Run user's setup routine! */

	while (1)
	{					/* Then forever, */
		loop();			/* then do some of the user's business */
	}

}



#ifdef _STARTUPSCREEN_LIQUIDWARE_
//*********************************************************
//*	Dec 26,	2008	Run Length Encoding for RGB
static void	DisplayColorRLEfromPGMmem(unsigned char *rleBuff, COLOR *colorFade, short startX, short startY)
{
COLOR			rleColor;
short			ii, jj;
short			rlePixCount;
int				pixelX, pixelY;
int				rleColorValue;
unsigned char	theByte1;
unsigned char	theByte2;
unsigned char	theByte3;
unsigned char	theByte4;
int				xSize, ySize;
int				adjusted_Red;
int				adjusted_Green;
int				adjusted_Blue;


	ii			=	0;
	theByte1	=	pgm_read_byte_near(rleBuff + ii);
	theByte2	=	pgm_read_byte_near(rleBuff + ii + 1);
	xSize		=	(theByte1 << 8) + theByte2;
	ii			+=	2;
	
	theByte1	=	pgm_read_byte_near(rleBuff + ii);
	theByte2	=	pgm_read_byte_near(rleBuff + ii + 1);
	ySize		=	(theByte1 << 8) + theByte2;
	ii			+=	2;
	
	//*	< 0 means CENTER
	if (startX < 0)
	{
		startX	=	(gWidth - xSize) / 2;
	}
	if (startY < 0)
	{
		startY	=	(gHeight - ySize) / 2;
	}


	
	pixelX	=	0;
	pixelY	=	0;
	while ((pixelY < ySize) && (ii < 10000))	//*	(ii < 10000) safety
	{

		theByte1	=	pgm_read_byte_near(rleBuff + ii) & 0x00ff;
		theByte2	=	pgm_read_byte_near(rleBuff + ii + 1) & 0x00ff;
		theByte3	=	pgm_read_byte_near(rleBuff + ii + 2) & 0x00ff;
		theByte4	=	pgm_read_byte_near(rleBuff + ii + 3) & 0x00ff;
		ii			+=	4;

		//*	look for end of line
		if ((theByte1 == 0) && (theByte2 == 0) && (theByte3 == 0) && (theByte4 == 0))
		{
			pixelX	=	0;
			pixelY++;
		}
		else
		{
			adjusted_Red	=	theByte1 - colorFade->red;
			adjusted_Green	=	theByte2 - colorFade->green;
			adjusted_Blue	=	theByte3 - colorFade->blue;

			if (adjusted_Red < 0)	adjusted_Red	=	0;
			if (adjusted_Green < 0)	adjusted_Green	=	0;
			if (adjusted_Blue < 0)	adjusted_Blue	=	0;

			rleColor.red	=	adjusted_Red;
			rleColor.green	=	adjusted_Green;
			rleColor.blue	=	adjusted_Blue;

			rlePixCount		=	theByte4;
			
   			dispColor(rleColor);
		
			for (jj=0; jj<rlePixCount; jj++)
			{
				dispPixel(startX + pixelX, startY + pixelY);
				pixelX++;
			}
		}
	}
}


#endif

