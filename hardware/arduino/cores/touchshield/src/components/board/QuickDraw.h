//*******************************************************************************
//*	
//*	QuickDraw style graphics routines
//*	for TouchShield and TouchSlide library
//*		by msproul@jove.rutgers.edu
//*	
//*******************************************************************************
//*	Detailed Edit history
//*******************************************************************************
//*	Dec 26,	2008	<MLS> Mark Sproul meet with Chris and Matt in New Haven
//*	Dec 26,	2008	<MLS> taking over the development of Sub-Processing
//*	Dec 29,	2008	<MLS> Starting on Macintosh QUICKDRAW interface to graphics library
//*	Jan 25,	2009	<MLS> Added QuickDraw MoveTo, LineTo, Move, Line
//*	Jan 26,	2009	<MLS> Added QuickDraw EraseRect
//*	Feb  2,	2009	<MLS> Added QuickDraw DrawCString
//*******************************************************************************
//#include	"QuickDraw.h"

#ifndef _QUICKDRAW_H_ 
#define _QUICKDRAW_H_

#ifndef _SUBP_OPTIONS_H_
	#include	"SubPOptions.h"
#endif
#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif


#ifdef __cplusplus
	extern "C" {
#endif

typedef boolean Boolean;
typedef COLOR RGBColor;

//*	Macintosh QuickDraw data types
typedef struct  {
	short	v;
	short	h;
} Point;

//*	this is slightly different then RECT
//typedef struct  {
//	short	top;
//	short	left;
//	short	bottom;
//	short	right;
//} Rect;


//*******************************************************************************
//*	Macintosh QuickDraw functions
	void	MoveTo(short xLoc, short yLoc);
	void	LineTo(short xLoc, short yLoc);
	void	Move(short deltaX, short deltaY);
	void	Line(short deltaX, short deltaY);

	void	FrameRect(RECT *theRect);
	void	FillRect(RECT *theRect);		//*	uses foreground color
	void	EraseRect(RECT *theRect);		//*	uses background color
	boolean	PtInRect(Point thePoint, const RECT *theRect);
	boolean	POINTinRect(POINT thePoint, const RECT *theRect);
	void	SetRect(RECT *theRect, short left, short top, short right, short bottom);
	void	InsetRect(RECT *theRect, short insetX, short insetY);
	void	OffsetRect(RECT *theRect, short deltaX, short deltaY);
	void	SetRGBcolor(COLOR *theColor, unsigned char redValue,  unsigned char greenValue,  unsigned char blueValue);
	void	RGBForeColor(COLOR *theColor);
	void	RGBBackColor(COLOR *theColor);
	void	PlotIcon(short xLoc, short yLoc, unsigned char *iconDef);

	void	DrawCString(short xloc, short yloc, char *theCstr);
	void	EraseScreen(void);

	void	BezierCurve(Point p1, Point p2, Point p3, Point p4);

#define	kIconRLEcode	0x80





#ifdef __cplusplus
} // extern "C"
#endif


#endif	//* _QUICKDRAW_H_
