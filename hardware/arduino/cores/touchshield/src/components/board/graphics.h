//*******************************************************************************
//*	Detailed Edit history
//*	<MLS>	=	Mark Sproul, msproul@jove.rutgers.edu
//*******************************************************************************
//*	Dec 27,	2008	<MLS> Changed RECT_T to RECT
//*	Dec 28,	2008	<MLS> Changed SCREEN_T to SCREEN
//*	Dec 31,	2008	<CML> released v0012LW
//*	Dec 31,	2008	<MLS> Got 0012LW from Chris, making previous changes to this new version
//*	Dec 31,	2008	<MLS> Copied dispBrightness from 0012
//*	Jan  2,	2009	<MLS> Added negitive checking to w/h of dispRectangle
//*	Jan 9,	2009	<MLS/TWH> Adding TWHs clipping code pluse _ENABLE_CLIPPING_
//*******************************************************************************

#include	"wiring.h"

#ifndef GRAPHICS_H
#define GRAPHICS_H


#ifndef _HARDWARE_DEF_H_
	#include	"HardwareDef.h"
#endif

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
//-void	dispLine( int x1,  int y1, int x2,  int y2);
void	dispPixel(int xx, int yy);
void	dispRead(COLOR *buffer, int xx, int yy);
void	dispRectangle(int  xLoc,  int  yLoc,   int rectWidth,   int  rectHeight);
unsigned char	dispSetWindow(int xx, int yy, int argWidth, int argHeight);

//-void	dispFillRect();
void	dispPix();

void	dispPowerLCD(uint8_t bOn);
void	dispStandby(uint8_t bStandby);


#ifdef _TOUCH_SLIDE_
	//*	added by Mark Sproul Dec 2009
	int		GetScreenWidth(void);
	int		GetScreenHeight(void);

	//*	THW clipping should only be needed for the touchSlide
//	#define	_ENABLE_CLIPPING_
	//*	THW scroll only works on the touchSlide
//	#define	_ENABLE_HARDWARE_SCROLL_

#elif defined(_TOUCH_STEALTH_)
	#define	GetScreenWidth()	128
	#define	GetScreenHeight()	128
#else
	#error invalid screen type
#endif


#ifdef _ENABLE_SCREEN_ROTATION_
	byte	SetScreenOrientation(char newOrienation);
#endif

#ifdef _ENABLE_CLIPPING_
	void	dispSetClip(int x, int y, int width, int height);
	void	dispRemoveClip();
#endif

#ifdef _ENABLE_HARDWARE_SCROLL_
	void	dispSetScrollArea(int startRow, int endRow);
	void	dispScrollLeft(int steps);
	void	dispScrollRight(int steps);
	void	dispScrollStop();
#endif


//*	routines added by TWH
void	dispPixel2(int xx, int yy);




#ifdef __cplusplus
	} // extern "C"
#endif

#endif

