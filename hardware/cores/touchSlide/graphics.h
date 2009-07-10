//*******************************************************************************
//*	Detailed Edit history
//*	<MLS>	=	Mark Sproul, msproul@jove.rutgers.edu
//*******************************************************************************
//*	Dec 27,	2008	<MLS> Changed RECT_T to RECT
//*	Dec 28,	2008	<MLS> Changed SCREEN_T to SCREEN
//*	Dec 31,	2008	<CHRIS> released v0012LW
//*	Dec 31,	2008	<MLS> Got 0012LW from Chris, making previous changes to this new version
//*	Dec 31,	2008	<MLS> Copied dispBrightness from 0012
//*	Jan  2,	2009	<MLS> Added negitive checking to w/h of dispRectangle
//*******************************************************************************

#ifndef __STDINT_H_
	#include	<stdint.h>
#endif

//#define	_SUPPORT_CLIPPING_

#ifndef GRAPHICS_H
#define GRAPHICS_H

#ifdef __cplusplus
	extern "C"{
#endif

typedef struct
{
	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;
} RECT;		//* was RECT_T;

typedef struct
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} COLOR;

typedef struct
{
    int width;
    int height;
} SCREEN;	//*	was SCREEN_T

extern	SCREEN	screen;
extern	COLOR	GraphicsColor;

void	dispClearScreen();
void	dispColor(COLOR c);
void	dispCommand(unsigned char command);
void	dispData(unsigned int data);
void	dispLine( int x1,  int y1, int x2,  int y2);
void	dispPixel( int x,  int y);
void	dispRead(COLOR *buffer, uint16_t  x, uint16_t  y);
void	dispRectangle(int  xLoc,  int  yLoc,   int rectWidth,   int  rectHeight);
void	dispSetWindow(int x, int y, int width, int height);

void	dispFillRect();
void	dispPix();



#ifdef __cplusplus
	} // extern "C"
#endif

#endif

