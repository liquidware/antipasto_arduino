//*******************************************************************************
//*	
//*	                             Sub-Processing 
//*                        for TouchShield library
//*	         by inthebitz @ antipastohw.blogspot.com & liquidware.com
//*	
//*******************************************************************************
//*	 History:
//*	
//*	 Version 0.2 | Dec 4, 2008  | Chris @ antipastohw.blogspot.com & liquidware.com
//*	 Merged into the TouchShield core
//*	
//*	 Version 0.1 | Nov 30, 2008 | inthebitz @ antipastohw.blogspot.com & liquidware.com
//*	 This is the beginnings of the "Sub-Processing" for TouchShield library
//*	 
//*	
//*	 DESCRIPTION:
//*	 Sub-Processing provides a rich graphics and interaction library 
//*	 for the TouchShield.
//*	 
//*******************************************************************************
//*	Detailed Edit history
//*	<MLS>	=	Mark Sproul, msproul@jove.rutgers.edu
//*******************************************************************************
//*	Nov 30,	2008	<inthebitz> Version 0.1 This is the beginnings of the "Sub-Processing" for TouchShield library
//*	Dec  4,	2008	<Chris> Version 0.2 Merged into the TouchShield core
//*	Dec 26,	2008	<MLS> Mark Sproul meet with Chris and Matt in New Haven
//*	Dec 26,	2008	<MLS> taking over the development of Sub-Processing
//*	Dec 27,	2008	<MLS> Fixed arg order error in beginCanvas call to drawchar
//*	Dec 27,	2008	<MLS> Changing many variable names to longer more understandable names
//*	Dec 28,	2008	<MLS> Separated rect into framerect and rect
//*	Dec 28,	2008	<MLS> Wrote on 7-segment display, it looks great
//*	Dec 29,	2008	<?> Changed from serialBegin(9600); to beginSerial(9600);
//*	Dec 29,	2008	<MLS> Starting on Macintosh QUICKDRAW interface to graphics library
//*	Dec 30,	2008	<MLS> Changed gettouch to return isTouching
//*	Dec 31,	2008	<MLS> When Chris did v0012LW, the screen structure got lost
//*	Jan  2,	2009	<MLS> Added InsetRect
//*	Jan  2,	2009	<MLS> Changed args in orientation() to LONG, fixed bug in filled triangles
//*	Jan  3,	2009	<MLS> Added OffsetRect
//*	Jan  3,	2009	<MLS> Changed my_point to gMostRecentTouchPt
//*******************************************************************************
//*	<MLS> Programming style
//*			never use 1 letter variable names, use LONG names, 
//*			{ and } always go on their own line
//*			never put 2 lines of code on the same line
//*			use TAB instead of multiple spaces
//*			a comment line of "****"s above the start of each routine
//*			leave a space before and after each operator "a + b"  not "a+b"
//*		these next few rules have to be broken to keep compatibility
//*			routine names should start with a CAPITAL letter
//*			variable names should always start with lower case,
//*			global variables should start with a lower case "g" followed by an UPPER case char
//*			constants (#define )  should start with a lower case "k" followed by an UPPER case char
//*			underLine "_" is an ok char to use in variable names, routine names, and constants
//*			changes should include comments with your initials
//*******************************************************************************
//*	PREPROCESSOR DIRECTIVES
//*******************************************************************************
#include	<avr/io.h>
#include	<inttypes.h>
#include	<math.h>
#include	<stdlib.h>
#include	"wiring.h"


#ifndef _HARDWARE_DEF_H_
	#include	"HardwareDef.h"
#endif

#ifndef _SUBP_OPTIONS_H_
	#include	"SubPOptions.h"
#endif
#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif
#include	"QuickDraw.h"
#ifndef GRAPHICS_H
	#include	"graphics.h"
#endif

//#define	incx() x++, dxt += d2xt, t += dxt
#define	incx() xx++, dxt += d2xt, t += dxt
//#define	incy() y--, dyt += d2yt, t += dyt
#define	incy() yy--, dyt += d2yt, t += dyt

#define MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))


//*******************************************************************************
//*	GLOBAL CONSTANTS AND VARIABLES
//*******************************************************************************

//* INPUT VARIABLES
COLOR	fcolor		=	{ 255, 255, 255 };
COLOR	bcolor		=	{ 0, 0, 0 };
//COLOR	green		=	{ 0, 255, 0 };
//COLOR	blue		=	{0,0,255};
//COLOR	yellow		=	{255,255,0};
//COLOR	grey		=	{0x77,0x77,0x77};
//COLOR	red			=	{255,0,0};
//COLOR	black		=	{0,0,0};
//COLOR	white		=	{255,255,255};
//COLOR	mycolor		=	{255, 255, 255};

int		brightness	=	BRIGHT_MAX;
POINT	gMostRecentTouchPt;
int		trix1, triy1, trix2, triy2, trix3, triy3;
	
int		mouseX			=	screen.width / 2;
int		mouseY			=	screen.height / 2;
int		width			=	kSCREEN_X_size;		//screen.width;
int		height			=	kSCREEN_Y_size;		//screen.height;
int		strokeWeightVal	=	1;
uint8_t	strokeEnb		=	true;
uint8_t	fillEnb			=	true;
uint8_t	cptr;
uint8_t	buf[32];
uint8_t	serverEnb		=	false;
char	command[16];


//*******************************************************************************
//* LOCAL FUNCTION PROTOTYPES
//*******************************************************************************
void	dispFillEllipse(int xCenter, int yCenter, int xRadius, int yRadius);
void	dispOutlineEllipse(unsigned int xc,unsigned int yc,unsigned int a,unsigned int b);
void	dispWuLine( int X0, int Y0, int X1, int Y1); 
void	dispTriangle( int x1, int y1, int x2, int y2, int x3, int y3);
int		point_triangle_intersection(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3);
int		orientation (long x1, long y1, long x2, long y2, long px, long py);
void	getstring(uint8_t *buf, int bufsize);
void	sendStringL( char *buf, int size);
void	drawNumber(unsigned long n, uint8_t base, int xLoc, int yLoc);

//*******************************************************************************
//*	LOCAL FUNCTIONS
//*******************************************************************************

//*******************************************************************************
void	dispOutlineEllipse( int xCenter, int yCenter, int xRadius, int yRadius)
{
int		xx		=	0;
int		yy		=	yRadius;
long	a2		=	(long)xRadius * xRadius;
long	b2		=	(long)yRadius * yRadius;
long	crit1	=	-(a2 / 4 + xRadius % 2 + b2);
long	crit2	=	-(b2 / 4 + yRadius % 2 + a2);
long	crit3	=	-(b2 / 4 + yRadius % 2);
long	t		=	-a2 * yy; /* e(x+1/2,y-1/2) - (a^2+b^2)/4 */
long	dxt		=	2 * b2 * xx;
long	dyt		=	-2 * a2 * yy;
long	d2xt	=	2 * b2;
long	d2yt	=	2 * a2;

	while (yy >= 0 && xx <= xRadius)
	{
		dispPixel(xCenter	+ xx, yCenter + yy);
		if (xx != 0 || yy != 0)
		{
			dispPixel(xCenter - xx, yCenter - yy);
		}
		if (xx != 0 && yy != 0)
		{
			dispPixel(xCenter + xx, yCenter - yy);
			dispPixel(xCenter - xx, yCenter + yy);
		}
		if (t + b2 * xx <= crit1 ||	/* e(x+1,y-1/2) <= 0 */
			t + a2 * yy <= crit3)	 /* e(x+1/2,y) <= 0 */
		{
			incx();
		}
		else if (t - a2 * yy > crit2) /* e(x+1/2,y-1) > 0 */
		{
			incy();
		}
		else
		{
			incx();
			incy();
		}
	}
}

//*******************************************************************************
//*	e(x,y) = b^2*x^2 + a^2*y^2 - a^2*b^2
//*	a	=	xRadius
//*	b	=	yRadius
//*******************************************************************************
void	dispFillEllipse(int xCenter, int yCenter, int xRadius, int yRadius)
{
int				xx		=	0;
int				yy		=	yRadius;
unsigned int	width	=	1;
long			a2		=	(long)xRadius * xRadius;
long			b2		=	(long)yRadius * yRadius;
long			crit1	=	-(a2 / 4 + xRadius % 2 + b2);
long			crit2	=	-(b2 / 4 + yRadius % 2 + a2);
long			crit3	=	-(b2/4 + yRadius % 2);
long			t		=	-a2 * yy; /* e(x+1/2,y-1/2) - (a^2+b^2)/4 */
long			dxt		=	2 * b2 * xx;
long			dyt		=	-2 * a2 * yy;
long			d2xt	=	2 * b2;
long			d2yt	=	2 * a2;

	while (yy >= 0 &&  xx <= xRadius)
	{
		if (t + b2 * xx <= crit1 ||	 /* e(x+1,y-1/2) <= 0 */
			t + a2 * yy <= crit3) 
		{
			/* e(x+1/2,y) <= 0 */
			incx();
			width	+=	2;
		}
		else if (t - a2 * yy > crit2) 
		{
			/* e(x+1/2,y-1) > 0 */
			//dispRectangle(xCenter - x, yCenter - y, width, 1);
			dispRectangle(xCenter - xx, yCenter - yy, width, 1);
			if (yy != 0)
			{
				//dispRectangle(xCenter- xx, yCenter + y, width, 1);
				dispRectangle(xCenter - xx, yCenter + yy, width, 1);
			}
			incy();
		}
		else
		{
			//dispRectangle(xCenter - xx, yCenter - y, width, 1);
			dispRectangle(xCenter - xx, yCenter - yy, width, 1);
			if (yy != 0)
			{
				//dispRectangle(xCenter - xx, yCenter + y, width, 1);
				dispRectangle(xCenter - xx, yCenter + yy, width, 1);
			}
			incx();
			incy();
			width	+=	2;
		}
	}
	if (yRadius == 0)
	{
		//dispRectangle(xCenter - xRadius, yCenter, 2 * xRadius + 1, 1);
		dispRectangle(xCenter - xRadius, yCenter, 2 * xRadius + 1, 1);
	}
} 
 
//*******************************************************************************
//There's definitely an easier way to do this, but for now I'll use some help from:
//http://www.codeproject.com/KB/GDI/antialias.aspx#dwuln
//Edited by inthebitz
//*******************************************************************************
void	dispWuLine( int X0, int Y0, int X1, int Y1)
{
unsigned short	IntensityShift, ErrorAdj, ErrorAcc;
unsigned short	ErrorAccTemp, Weighting, WeightingComplementMask;
short			DeltaX, DeltaY, Temp, XDir;
short			BaseColor		=	0;
short			NumLevels		=	2;
unsigned short	IntensityBits	=	2;

	/* Make sure the line runs top to bottom */
	if (Y0 > Y1)
	{
		Temp	=	Y0;
		Y0		=	Y1;
		Y1		=	Temp;
		Temp	=	X0;
		X0		=	X1;
		X1		=	Temp;
	}
	/* Draw the initial pixel, which is always exactly intersected by
		the line and so needs no weighting */
	dispPixel(X0, Y0);

	if ((DeltaX = X1 - X0) >= 0)
	{
		XDir	=	1;
	}
	else
	{
		XDir	=	-1;
		DeltaX	=	-DeltaX; /* make DeltaX positive */
	}
	/* Special-case horizontal, vertical, and diagonal lines, which
		require no weighting because they go right through the center of
		every pixel */
	if ((DeltaY = Y1 - Y0) == 0)
	{
		/* Horizontal line */
		while (DeltaX-- != 0)
		{
			X0	+=	XDir;
			dispPixel(X0, Y0);
		}
		return;
	}
	if (DeltaX == 0)
	{
		/* Vertical line */
		do
		{
			Y0++;
			dispPixel(X0, Y0);
		} while (--DeltaY != 0);
		return;
	}
	if (DeltaX == DeltaY)
	{
		/* Diagonal line */
		do
		{
			X0	+=	XDir;
			Y0++;
			dispPixel(X0, Y0);
		} while (--DeltaY != 0);
		return;
	}

	/* Line is not horizontal, diagonal, or vertical */

	ErrorAcc	=	0;	/* initialize the line error accumulator to 0 */
						/* # of bits by which to shift ErrorAcc to get intensity level */

	IntensityShift	=	16 - IntensityBits;
	/* Mask used to flip all bits in an intensity weighting, producing the
		result (1 - intensity weighting) */
	WeightingComplementMask	=	NumLevels - 1;

	/* Is this an X-major or Y-major line? */
	if (DeltaY > DeltaX)
	{
		/* Y-major line; calculate 16-bit fixed-point fractional part of a
		 pixel that X advances each time Y advances 1 pixel, truncating the
		 result so that we won't overrun the endpoint along the X axis */
		ErrorAdj	=	((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
		/* Draw all pixels other than the first and last */
		while (--DeltaY)
		{
			ErrorAccTemp	=	ErrorAcc;		/* remember currrent accumulated error */
			ErrorAcc		+=	ErrorAdj;		/* calculate error for next pixel */
			if (ErrorAcc <= ErrorAccTemp)
			{
				/* The error accumulator turned over, so advance the X coord */
				X0	+=	XDir;
			}
			Y0++;	/* Y-major, so always advance Y */
			/* The IntensityBits most significant bits of ErrorAcc give us the
			intensity weighting for this pixel, and the complement of the
			weighting for the paired pixel */
			Weighting	=	ErrorAcc >> IntensityShift;
			//////////////////////////////////////////////////////
			dispPixel(X0, Y0);
			//dispPixel(X0 + XDir, Y0, mycolora);
			//dispPixel(X0, Y0, BaseColor + Weighting);
			//dispPixel(X0 + XDir, Y0,
			//		BaseColor + (Weighting ^ WeightingComplementMask));
		}
		/* Draw the final pixel, which is
			always exactly intersected by the line
			and so needs no weighting */
		dispPixel(X1, Y1);
		return;
	}
	/* It's an X-major line; calculate 16-bit fixed-point fractional part of a
		pixel that Y advances each time X advances 1 pixel, truncating the
		result to avoid overrunning the endpoint along the X axis */
	ErrorAdj	=	((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
	/* Draw all pixels other than the first and last */

	while (--DeltaX)
	{
		ErrorAccTemp	=	ErrorAcc;	/* remember currrent accumulated error */
		ErrorAcc += ErrorAdj;		/* calculate error for next pixel */
		if (ErrorAcc <= ErrorAccTemp)
		{
			/* The error accumulator turned over, so advance the Y coord */
			Y0++;
		}
		X0	+=	XDir; /* X-major, so always advance X */
		/* The IntensityBits most significant bits of ErrorAcc give us the
		 intensity weighting for this pixel, and the complement of the
		 weighting for the paired pixel */
		Weighting	=	ErrorAcc >> IntensityShift;
		///////////////////////////////////////////////////
		dispPixel(X0, Y0);
		//dispPixel(X0, Y0 + 1, linecolor);
		//dispPixel(X0, Y0, BaseColor + Weighting);
		//dispPixel(X0, Y0 + 1,
		//		BaseColor + (Weighting ^ WeightingComplementMask));
	}
	/* Draw the final pixel, which is always exactly intersected by the line
		and so needs no weighting */
	dispPixel(X1, Y1);
}

//*******************************************************************************
//	http://www.swissdelphicenter.ch/en/showcode.php?id=2400
//	http://tog.acm.org/GraphicsGems/gemsiii/triangleCube.c
//	I'm sure there's a more elegant and cleaner way, but with help from the links above I was able to cludge
//	this together, so that it at least works! 
//*******************************************************************************
void	dispTriangle( int x1, int y1, int x2, int y2, int x3, int y3) 
{

	if (strokeEnb) 
	{
		dispColor(fcolor);
		dispWuLine ( x1, y1, x2, y2);
		dispWuLine ( x2, y2, x3, y3);
		dispWuLine ( x3, y3, x1, y1);
	}

	if (fillEnb) 
	{
	int		xx;
	int		yy;
	int		startX;
	int		endX;
	int		startY;
	int		endY;
	
		//*	figure out the left most and right most points
		startX	=	MIN3(x1, x2, x3);
		endX	=	MAX3(x1, x2, x3);
		//*	figure out the top most and bottom most points
		startY	=	MIN3(y1,y2,y3);
		endY	=	MAX3(y1,y2,y3);
		dispColor(bcolor);
		for (xx = startX; xx < endX; xx++) 
		{
			for (yy = startY; yy < endY; yy++) 
			{
				if (point_triangle_intersection(xx, yy, x1, y1, x2, y2, x3, y3) ) 
				{
					dispPixel(xx , yy);
				}
			}
		}
	}
}

//*******************************************************************************
int point_triangle_intersection(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3)
{
int or1, or2, or3;
 
	or1	=	orientation(x1, y1, x2, y2, px, py);
	or2	=	orientation(x2, y2, x3, y3, px, py);
	or3	=	orientation(x3, y3, x1, y1, px, py);
 
	if ((or1 == or2) && (or2 == or3) )
	{
		return 1;
	}
	else if (or1 == 0 )
	{
		return ((or2 == 0) || (or3 == 0));
	}
	else if (or2 == 0)
	{
		return ((or1 == 0) || (or3 == 0));
	}
	else if ( or3 == 0)
	{
		return ((or2 == 0) || (or1 == 0));
	}
	else
	{
		return 0;
	}
}

//*******************************************************************************
//*	Jan  2,	2009	<MLS> Changed args in orientation() to LONG, fixed bug in filled triangles
int orientation (long x1, long y1, long x2, long y2, long px, long py)
{
long	orin;

	orin	=	(x2 - x1) * (py - y1) - (px - x1) * (y2 - y1);

	if (orin > 0)
	{
		return 1;
	}
	else if (orin < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
} 

//*******************************************************************************
//*	GLOBAL FUNCTIONS
//*******************************************************************************

//*******************************************************************************
void	background(uint8_t backGroundColor)
{
	setbcolor(backGroundColor, backGroundColor, backGroundColor);
	fillback();
}

//*******************************************************************************
void	background(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
	setbcolor(redValue, greenValue, blueValue);
	fillback();
}

//*******************************************************************************
void	clearscreen( void)
{
	setbcolor( 0, 0, 0);
	fillback();
}

//*******************************************************************************
float dist(float x1, float y1, float x2, float y2)
{
int	xx	=	(x2 - x1);
int	yy	=	(y2 - y1);

	return sqrt( (xx * xx)+(yy * yy));
}

//*******************************************************************************
void	drawchar( int xLoc, int yLoc, char theChar)
{
	dispPutC(theChar, xLoc, yLoc, fcolor, bcolor);
}

//*******************************************************************************
void	drawcircle( int xLoc, int yLoc, int radius)
{
	ellipse(xLoc, yLoc, radius, radius);
}

//*******************************************************************************
void	drawrect( int xLoc, int yLoc, int width, int height)
{
	rect(xLoc, yLoc, width, height);
}

//*******************************************************************************
void	drawstring( int xLoc, int yLoc, char *text)
{
	dispPutS(text, xLoc, yLoc, fcolor, bcolor);
}

//*******************************************************************************
void	ellipse( int xLoc, int yLoc, int radx, int rady)
{

	//fill
	if (fillEnb) 
	{
		dispColor(bcolor);
		dispFillEllipse(xLoc, yLoc, radx-1, rady-1);
	}
	
	//stroke
	dispColor(fcolor);
	for(int i=0; i<strokeWeightVal; i++)
	{
		dispOutlineEllipse(xLoc, yLoc, radx+i, rady+i);
	}
}


//*******************************************************************************
void	fadein( int time)
{
uint8_t ii;

	for (ii = 0; ii<(brightness+1); ii++) 
	{
	#if defined(_TOUCH_STEALTH_) || !defined(_VERSION_0012LW_)
		dispBrightness((uint8_t)ii);
	#else
		oled_brightness((uint8_t)ii);
	#endif
		delay(time / brightness);
	}
}

//*******************************************************************************
void	fadeout( int time)
{
uint8_t ii;

	for (ii = brightness; ii>0; ii--) 
	{
	#if defined(_TOUCH_STEALTH_) || !defined(_VERSION_0012LW_)
		dispBrightness((uint8_t)ii);
	#else
		oled_brightness((uint8_t)ii);
	#endif
		delay(time / brightness);
	}
}

//*******************************************************************************
void	fill(int fillColor)
{
	fillEnb	=	true;
	setbcolor(fillColor, fillColor, fillColor);
}

//*******************************************************************************
void	fill(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
	fillEnb	=	true;
	setbcolor(redValue, greenValue, blueValue);
}


//*******************************************************************************
void	fillback(void)
{
	dispColor(bcolor);
	rect(-1, -1, width+1, height+1);
}

//*******************************************************************************
boolean	gettouch(void)
{
boolean	isTouching;

	if (touch_getCursor(&gMostRecentTouchPt))
	{
		mouseX	=	gMostRecentTouchPt.x;
		mouseY	=	gMostRecentTouchPt.y;
		isTouching	=	true;
	}
	else
	{
		isTouching	=	false;
	}
	return(isTouching);
}

//*******************************************************************************
void	line(int x1, int y1, int x2, int y2)
{
int	temp;

//	if (y2 < y1)
//	{
//		temp	=	y1;
//		y1		=	y2;
//		y2		=	temp;
//	}
//	if (x2 < x1)
//	{
//		temp	=	x1;
//		x1		=	x2;
//		x2		=	temp;
//	}
	dispColor(fcolor);

	dispWuLine (x1, y1, x2, y2);
}

//*******************************************************************************
void	noFill(void)
{
	fillEnb	=	false;
}

//*******************************************************************************
void	noStroke(void)
{
	strokeEnb		=	false;
	strokeWeightVal	=	0;
}

//*******************************************************************************
void	point( int xLoc, int yLoc)
{
	dispColor(fcolor);
	dispPixel( xLoc, yLoc);
}

//*******************************************************************************
void	quad( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	/* Save the stroke state */
	uint8_t prevStroke	=	strokeEnb;

	if (fillEnb) 
	{
		/* Disable stroke */
		noStroke();
		triangle(x1, y1, x2, y2, x3, y3);
		triangle(x2, y2, x3, y3, x4, y4);
		triangle(x1, y1, x3, y3, x4, y4);

		/* Restore Stroke */
		if (prevStroke)
		{
			strokeEnb	=	true;
		}
	}

	if (strokeEnb) 
	{
		line(x1,y1,x2,y2);
		line(x2,y2,x3,y3);
		line(x3,y3,x4,y4);
		line(x4,y4,x1,y1);
	}
}


//*******************************************************************************
//*	Dec 28,	2008	<MLS> Separated rect into framerect and rect
void	framerect(int xLeft, int yTop, int width, int height)
{
	if ((width > 0) && (height > 0))
	{
		dispColor(fcolor);
		if (strokeWeightVal > 1)
		{
		//stroke
		int		left	=	xLeft - 1;
		int		top		=	yTop - 1;
		int		right	=	xLeft + width;
		int		bottom	=	yTop + height;

			for(int i=0; i<strokeWeightVal; i++)
			{
				line(left,	top,	right,	top);
				line(left,	top,	left,	bottom);
				line(left,	bottom,	right,	bottom);
				line(right,	top,	right,	bottom);
				left--;
				top--;
				bottom++;
				right++;
			}
		}
		else
		{
			line(xLeft,			yTop,			xLeft + width,	yTop);				//*	top line
			line(xLeft,			yTop + height,	xLeft + width,	yTop + height);		//*	bottom line
			
			line(xLeft,			yTop,			xLeft,			yTop + height);		//*	left line
			line(xLeft + width,	yTop,			xLeft + width,	yTop + height);		//*	right line
		}

	}
}
//*******************************************************************************
//*	Dec 27,	2008	<MLS> Rewriting rect because it doesnt work when compiled on Mac
void	rect(int xLeft, int yTop, int width, int height)
{

	if ((width > 0) && (height > 0))
	{
		//	fill
		if (fillEnb)
		{
			dispColor(bcolor);
			dispRectangle(xLeft, yTop, width, height); 
		}

		//*	frame the rect
		framerect(xLeft, yTop, width, height);
	}
}

//*******************************************************************************
void	setbcolor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
	bcolor.red		=	redValue;
	bcolor.green	=	greenValue;
	bcolor.blue		=	blueValue;
}

//*******************************************************************************
void	setbrightness( int bright)
{
#if defined(_TOUCH_STEALTH_) || !defined(_VERSION_0012LW_)
	dispBrightness(bright);
#else
	oled_brightness(bright);
#endif
	brightness = bright;
}

//*******************************************************************************
void	setfcolor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
	fcolor.red		=	redValue;
	fcolor.green	=	greenValue;
	fcolor.blue		=	blueValue;
}

//*******************************************************************************
void	size(int w, int h)
{
#ifdef _SUPPORT_CLIPPING_
	dispClip(w,h); //set the clipping region
#endif
	width	=	w; 
	height	=	h;
	mouseX	=	w / 2; //reset the mouse
	mouseY	=	h / 2; //reset the mouse
}


//*******************************************************************************
void	stroke( int s)
{
	setfcolor(s,s,s);
}

//*******************************************************************************
void	strokeWeight(int s)
{
	strokeWeightVal=s;
}

//*******************************************************************************
void	stroke(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
	setfcolor(redValue, greenValue, blueValue);
}

//*******************************************************************************
void	text(char *textString, int xLoc, int yLoc)
{
	dispPutS(textString, xLoc, yLoc, fcolor, bcolor);
}

//*******************************************************************************
void	text(char theChar, int xLoc, int yLoc)
{
	dispPutC(theChar, xLoc, yLoc, fcolor, bcolor);
}

//*******************************************************************************
void	text(int data, int xLoc, int yLoc)
{
	char out[10];
	dtostrf(data,4,0,out);
	text(out, xLoc, yLoc);
}

//*******************************************************************************
void	text(unsigned int data, int xLoc, int yLoc)
{
	text((int)data,  xLoc,  yLoc);
}


//*******************************************************************************
void	text(long data, int xLoc, int yLoc)
{
	char out[10];
	dtostrf(data,4,0,out);
	text(out, xLoc, yLoc);
}

//*******************************************************************************
void	text(double data, int xLoc, int yLoc)
{
	char out[12];
	dtostrf(data,4,3,out);
	text(out, xLoc, yLoc);
}
 
//*******************************************************************************
void	triangle( int x1, int y1, int x2, int y2, int x3, int y3) 
{ 
	dispTriangle(x1, y1, x2, y2, x3, y3);
} 

//*******************************************************************************
void	beginCanvas(void)
{

	/* Start Serial, if not started already */
	if (!serverEnb)
	{
	#ifdef _TOUCH_SLIDE_
		//*	Dec 29,	2008	<?> Changed from serialBegin(9600); to beginSerial(9600);
		beginSerial(9600);		//*	version 0012LW
	#else
		serialBegin(9600);		//*	version 0012
	#endif
		serverEnb	=	true;
	}

	/* Get our command string */
	getstring(buf,sizeof(buf));
	cptr=1;

	/* Is it a valid command? */
	if (buf[0] == '|') 
	{
		/* Determine the command */
		switch(buf[cptr]) 
		{
		case kSubP_SETFCOLOR:
			setfcolor( buf[cptr+1], buf[cptr+2], buf[cptr+3]);
			break;

		case kSubP_SETBCOLOR:
			setbcolor( buf[cptr+1], buf[cptr+2], buf[cptr+3]); 
			break;

		case kSubP_FILLBACK:
			fillback();		
			break;

		case kSubP_SET_BRIGHTNESS:
			setbrightness(buf[cptr+1]);
			break;

		case kSubP_FADEOUT:
			fadeout((buf[cptr+2] << 8) + buf[cptr+1]);
			break;

		case kSubP_FADEIN:
			fadein((buf[cptr+2] << 8) + buf[cptr+1]);
			break;

		case kSubP_DRAWPOINT:
			point((buf[cptr+2] << 8) + buf[cptr+1],
				(buf[cptr+4] << 8) + buf[cptr+3]);
			break;

		case kSubP_CIRCLE:
			int r;
			r	=	(buf[cptr+6] << 8) + buf[cptr+5];
			ellipse((buf[cptr+2] << 8) + buf[cptr+1], 
					(buf[cptr+4] << 8) + buf[cptr+3],
					r,
					r);
			break;

		case kSubP_RECT:
			rect((buf[cptr+2] << 8) + buf[cptr+1], 
				(buf[cptr+4] << 8) + buf[cptr+3], 
				(buf[cptr+6] << 8) + buf[cptr+5], 
				(buf[cptr+8] << 8) + buf[cptr+7]);
			break;

		case kSubP_LINE:
			line((buf[cptr+2] << 8) + buf[cptr+1], 
				(buf[cptr+4] << 8) + buf[cptr+3], 
				(buf[cptr+6] << 8) + buf[cptr+5],
				(buf[cptr+8] << 8) + buf[cptr+7]);
			break;

		case kSubP_CHAR:
			//*	Dec 27,	2008	<MLS> Fixed arg order error in beginCanvas call to drawchar
			drawchar(	(buf[cptr+2] << 8) + buf[cptr+1],
						(buf[cptr+4] << 8) + buf[cptr+3],
						buf[cptr+5]);
			break;

		case kSubP_STRING:
			drawstring((buf[cptr+2] << 8) + buf[cptr+1], 
					 (buf[cptr+4] << 8) + buf[cptr+3],
					 (char*)&buf[cptr+8]);
			break;

		case kSubP_GET_TOUCH:
			gettouch();
			delay(2);
			serialWrite('|');
			delay(2);
			serialWrite((unsigned char)mouseX);
			delay(2);
			serialWrite((unsigned char)(mouseX>>8));
			delay(2);
			serialWrite((unsigned char)mouseY);
			delay(2);
			serialWrite((unsigned char)(mouseY>>8));
			delay(2);
			serialWrite((unsigned char)0);
			break;

		case kSubP_GET_SCREEN_SIZE:
			command[0]	=	'|';
			command[1]	=	kSubP_GET_SCREEN_SIZE;
		//*	Dec 31,	2008	<MLS> When Chris did v0012LW, the screen structure got lost
		#if 1
			command[2]	=	(unsigned char)kSCREEN_X_size;
			command[3]	=	(unsigned char)(kSCREEN_X_size >> 8);
			command[4]	=	(unsigned char)kSCREEN_Y_size;
			command[5]	=	(unsigned char)(kSCREEN_Y_size >> 8);
		#else
			command[2]	=	(unsigned char)screen.width;
			command[3]	=	(unsigned char)(screen.width >> 8);
			command[4]	=	(unsigned char)screen.height;
			command[5]	=	(unsigned char)(screen.height >> 8);
		#endif
			command[6]	=	(unsigned char)0;
			command[7]	=	(unsigned char)0;
			command[8]	=	0;
			command[9]	=	0;
			command[10]=0;

			sendStringL(command,11);
			break;
			
		case kSubP_ELLIPSE:
			ellipse((buf[cptr+2] << 8) + buf[cptr+1], 
					(buf[cptr+4] << 8) + buf[cptr+3],
					(buf[cptr+6] << 8) + buf[cptr+5],
					(buf[cptr+8] << 8) + buf[cptr+7]);
			break;

		case kSubP_TRIANGLE1:
			trix1	=	(buf[cptr+2] << 8) + buf[cptr+1];
			triy1	=	(buf[cptr+4] << 8) + buf[cptr+3];
			trix2	=	(buf[cptr+6] << 8) + buf[cptr+5];
			triy2	=	(buf[cptr+8] << 8) + buf[cptr+7];
			break;

		case kSubP_TRIANGLE2:
			trix3	=	(buf[cptr+2] << 8) + buf[cptr+1];
			triy3	=	(buf[cptr+4] << 8) + buf[cptr+3];
			triangle(trix1, triy1, trix2, triy2, trix3, triy3);
			break;
			
		default:
			break; 
		}
	}
}

//*******************************************************************************
void	getstring(uint8_t *buf, int bufsize)
{
int	ii;
	
	//*	Send the termination command, we're ready
	serialWrite(0);

	/* Read data */
	for (ii=0; ii<bufsize - 1; ++ii)
	{
		while(!serialAvailable())
		{
			;	//*	do nothing
		}
		buf[ii]	=	serialRead();
		
		if (buf[ii] == 0)	// is it the terminator byte?
		{
			if (ii > 9) break;
		}
	}
	
	buf[ii]	=	0; // 0 string terminator just in case
}
 
//*******************************************************************************
void	sendStringL( char *buf, int size)
{
int	ii;

	for (ii = 0;ii < size; ii++)
	{
		delay(2);
		serialWrite(buf[ii]);
	}
} 

#pragma mark -
//*******************************************************************************
//*	these routines are Macintosh QuickDraw style
//*******************************************************************************
#pragma mark Mac Quickdraw Style routines
#ifdef _SUBP_OPTION_QUICKDRAW_

//*******************************************************************************
void	RGBForeColor(COLOR *theColor)
{
	setfcolor(theColor->red, theColor->green, theColor->blue);
}

//*******************************************************************************
void	RGBBackColor(COLOR *theColor)
{
	setbcolor(theColor->red, theColor->green, theColor->blue);
}

//*******************************************************************************
void	FrameRect(RECT *theRect)
{
	framerect(theRect->left, theRect->top, (theRect->right - theRect->left), (theRect->bottom - theRect->top));
}

//*******************************************************************************
void	FillRect(RECT *theRect)
{
#if 1
int	myWidth, myHeight;

	myWidth		=	theRect->right - theRect->left;
	myHeight	=	theRect->bottom - theRect->top;
	
	dispColor(bcolor);
	dispRectangle(theRect->left, theRect->top, myWidth, myHeight); 
#else
	rect(theRect->left, theRect->top, (theRect->right - theRect->left), (theRect->bottom - theRect->top));
#endif
}

//*******************************************************************************
Boolean	PtInRect(Point thePoint, const RECT *theRect)
{
Boolean	ptIsInRect;

	ptIsInRect	=	true;
	if (thePoint.h < theRect->left)
	{
		ptIsInRect	=	false;
	}
	else if (thePoint.h > theRect->right)
	{
		ptIsInRect	=	false;
	}
	else if (thePoint.v < theRect->top)
	{
		ptIsInRect	=	false;
	}
	else if (thePoint.v > theRect->bottom)
	{
		ptIsInRect	=	false;
	}
	return(ptIsInRect);
}

//*******************************************************************************
boolean	POINTinRect(POINT thePoint, const RECT *theRect)
{
Point myNewPoint;

	myNewPoint.h	=	thePoint.x;
	myNewPoint.v	=	thePoint.y;
	return(PtInRect(myNewPoint, theRect));
}

//*******************************************************************************
void	InsetRect(RECT *theRect, short insetX, short insetY)
{
	theRect->left		+=	insetX;
	theRect->right		-=	insetX;
	theRect->top		+=	insetX;
	theRect->bottom		-=	insetX;
	
	
}

//*******************************************************************************
void	OffsetRect(RECT *theRect, short deltaX, short deltaY)
{
	theRect->left		+=	deltaX;
	theRect->right		+=	deltaX;
	theRect->top		+=	deltaY;
	theRect->bottom		+=	deltaY;
	
	
}




//*******************************************************************************
void	SetRect(RECT *theRect, short left, short top, short right, short bottom)
{
	theRect->left	=	left;
	theRect->top	=	top;
	theRect->right	=	right;
	theRect->bottom	=	bottom;
}


//#define	_DEBUG_ICON_
//*******************************************************************************
//*	1st 2 bytes are size, x,y
//*	follwed by that number of bytes (i.e. 16 x 16 = 256
//*	bytes consist of 4 2 bit fields, 
//*		b7,b6	=	future use
//*		b5,b4	=	red
//*		b3,b2	=	green
//*		b1,b0	=	blue
//*
//*		0xff (all 1s) means its transparent
//*		b10xxxxxx means that number of the following char
//*******************************************************************************
void	PlotIcon(short xLoc, short yLoc, unsigned char *iconDef)
{
short	xSize, ySize;
short	xx, yy;
short	cc;
short	currentByte;
short	previousByte;
int		totalBytes;
int		rleCount;
COLOR	myColor;


	cc				=	0;
	xSize			=	(iconDef[cc++] & 0x0ff);
	ySize			=	(iconDef[cc++] & 0x0ff);
	totalBytes		=	(xSize * ySize) + 2;
	xx				=	0;
	yy				=	0;
	previousByte	=	0xff;
	while ((cc < totalBytes) && (yy < ySize))
	{
		currentByte	=	(iconDef[cc++] & 0x00ff);
		if (currentByte == 0x00ff)		//*	0xff means BACKGROUND
		{
			//*	just skip the location
			xx++;
		}
		else if ((currentByte & 0xC0) == kIconRLEcode)
		{
			//*	we have a RLE encoding
			rleCount		=	currentByte & 0x3f;
			//*	get the next byte
			currentByte		=	(iconDef[cc++] & 0x00ff);
			myColor.red		=	((currentByte & B00110000) << 2) + 0x3F;
			myColor.green	=	((currentByte & B00001100) << 4) + 0x3F;
			myColor.blue	=	((currentByte & B00000011) << 6) + 0x3F;
			dispColor(myColor);

			while (rleCount > 0)
			{
				if (currentByte != 0x00ff)	//*	again, this can be a transparent byte
				{
				#ifdef _DEBUG_ICON_
					dispPixel(xLoc + (xx * 2),		yLoc + (yy * 2));
					dispPixel(xLoc + (xx * 2) + 1,	yLoc + (yy * 2));

					dispPixel(xLoc + (xx * 2),		yLoc + (yy * 2) + 1);
					dispPixel(xLoc + (xx * 2) + 1,	yLoc + (yy * 2) + 1);
				
				#else
					dispPixel(xLoc + xx, yLoc + yy);
				#endif
				}
				xx++;
				rleCount--;
			}

			previousByte	=	0xff;
		}
		else
		{
			if (currentByte != previousByte)
			{
				myColor.red		=	((currentByte & B00110000) << 2) + 0x3F;
				myColor.green	=	((currentByte & B00001100) << 4) + 0x3F;
				myColor.blue	=	((currentByte & B00000011) << 6) + 0x3F;
				dispColor(myColor);
			 	previousByte	=	currentByte;
			}
		#ifdef _DEBUG_ICON_
			dispPixel(xLoc + (xx * 2),		yLoc + (yy * 2));
			dispPixel(xLoc + (xx * 2) + 1,	yLoc + (yy * 2));

			dispPixel(xLoc + (xx * 2),		yLoc + (yy * 2) + 1);
			dispPixel(xLoc + (xx * 2) + 1,	yLoc + (yy * 2) + 1);
		
		#else
			dispPixel(xLoc + xx, yLoc + yy);
		#endif
			xx++;
		}
		if (xx >= xSize)
		{
			yy++;
			xx	=	0;
		}
	}
}



#endif	//*	_SUBP_OPTION_QUICKDRAW_




//*******************************************************************************
//*	these routines draw 7-segment LED numbers
//*******************************************************************************
#pragma mark -
#pragma mark 7-segment LED numbers

#ifdef _SUBP_OPTION_7_SEGMENT_
#ifndef Binary_h
	#include	"binary.h"
#endif
int		g7SegmentDisplay[]	=	{//	/76543210
									B01110111,			//*	0
									B00010010,			//*	1
									B01011101,			//*	2
									B01011011,			//*	3
									B00111010,			//*	4
									B01101011,			//*	5
									B01101111,			//*	6
									B01010010,			//*	7
									B01111111,			//*	8
									B01111011,			//*	9
									B01111110,			//*	a
									B00101111,			//*	b
									B01100101,			//*	c
									B00011111,			//*	d
									B01101101,			//*	e
									B01101100,			//*	f
									B00001000,			//*	- minus sign
								};

enum	{	kLEDsegExtra_Minus	=	0x10,

			kLEDsegExtra_last
		};

//*******************************************************************************
//*	the "character" is a decimal value from 0 -> 15
//*******************************************************************************
void	Display7SegmentDigit(int xLeft, int yTop, char decimalDigit, char segmentLength)
{
//*	Jan  9,	2009	<MLS> now does a fill rect before each digit to erase old digit
//*	Dec 31,	2008	<MLS> Changed the bit defnition from 8 bit to 16 bit to allow more chars
//*	this is the definition of a 7 segment display, we use 8 bits to define each char
//*	the most significant bit will be used for a decimal point
//		---------	bit 6			
//		|		|
//		|		|	bit 5   bit 4	
//		|		|
//		---------	bit 3
//		|		|
//		|		|	bit 2	bit 1
//		|		|
//		---------	bit 0

int		myBinaryValue;
int		endOffset;		//*	this is to make the ends look pointed
RECT	digitRect;

	digitRect.left		=	xLeft - 1;
	digitRect.right		=	xLeft + segmentLength + 1;
	digitRect.top		=	yTop - 1;
	digitRect.bottom	=	yTop + (2 * segmentLength) + 1;
	FillRect(&digitRect);

	if (segmentLength > 8)
	{
		endOffset	=	2;
	}
	else
	{
		endOffset	=	1;
	}

	myBinaryValue	=	g7SegmentDisplay[decimalDigit & 0x01f];
	
	if (myBinaryValue & B01000000)
	{
		//*	draw segment 6 horizontal
		line(xLeft,				yTop,				(xLeft + segmentLength),		yTop);
		line(xLeft + endOffset,	yTop - 1,	(xLeft + segmentLength - endOffset),	(yTop - 1));
		line(xLeft + endOffset,	yTop + 1,	(xLeft + segmentLength - endOffset),	(yTop + 1));
	}
	if (myBinaryValue & B00100000)
	{
		//*	draw segment 5 vertical
		line(xLeft,						yTop,				xLeft,			(yTop + segmentLength));
		line(xLeft - 1,					yTop + endOffset,	xLeft - 1,		(yTop + segmentLength - endOffset));
		line(xLeft + 1,					yTop + endOffset,	xLeft + 1,		(yTop + segmentLength - endOffset));
	}
	if (myBinaryValue & B00010000)
	{
		//*	draw segment 4 vertical
		line((xLeft + segmentLength),		yTop,				(xLeft + segmentLength),		(yTop + segmentLength));
		line((xLeft + segmentLength - 1 ),	yTop + endOffset,	(xLeft + segmentLength - 1),	(yTop + segmentLength - endOffset));
		line((xLeft + segmentLength + 1),	yTop + endOffset,	(xLeft + segmentLength + 1),	(yTop + segmentLength - endOffset));

	}
	if (myBinaryValue & B00001000)
	{
		//*	draw segment 3 horizontal
		line(xLeft,				(yTop + segmentLength),		(xLeft + segmentLength),				(yTop + segmentLength));
		line(xLeft + endOffset,	(yTop + segmentLength - 1),	(xLeft + segmentLength - endOffset),	(yTop + segmentLength - 1));
		line(xLeft + endOffset,	(yTop + segmentLength + 1),	(xLeft + segmentLength - endOffset),	(yTop + segmentLength + 1));
	}
	if (myBinaryValue & B00000100)
	{
		//*	draw segment 2 vertical
		line(xLeft,		(yTop + segmentLength),				xLeft,		(yTop + segmentLength + segmentLength));
		line(xLeft - 1,	(yTop + segmentLength + endOffset),	xLeft - 1,	(yTop + segmentLength + segmentLength - endOffset));
		line(xLeft + 1,	(yTop + segmentLength + endOffset),	xLeft + 1,	(yTop + segmentLength + segmentLength - endOffset));
	}
	if (myBinaryValue & B00000010)
	{
		//*	draw segment 1 vertical
		line((xLeft + segmentLength),		(yTop + segmentLength),				(xLeft + segmentLength),	(yTop + segmentLength + segmentLength));
		line((xLeft + segmentLength - 1),	(yTop + segmentLength + endOffset),	(xLeft + segmentLength - 1),	(yTop + segmentLength + segmentLength - endOffset));
		line((xLeft + segmentLength + 1),	(yTop + segmentLength + endOffset),	(xLeft + segmentLength + 1),	(yTop + segmentLength + segmentLength - endOffset));
	}

	if (myBinaryValue & B00000001)
	{
		//*	draw segment 0 horizontal
		line(xLeft,				(yTop + segmentLength + segmentLength),		(xLeft + segmentLength),				(yTop + segmentLength + segmentLength));
		line(xLeft + endOffset,	(yTop + segmentLength + segmentLength - 1),	(xLeft + segmentLength - endOffset),	(yTop + segmentLength + segmentLength - 1));
		line(xLeft + endOffset,	(yTop + segmentLength + segmentLength + 1),	(xLeft + segmentLength - endOffset),	(yTop + segmentLength + segmentLength + 1));
	}
}


//*******************************************************************************
void	Display7SegmentChar(int xLeft, int yTop, char theChar, char segmentLength)
{
	if ((theChar <= 0x0f) || ((theChar >= 0x30) && (theChar <= 0x39)))
	{
		Display7SegmentDigit(xLeft, yTop, (theChar & 0x0f), segmentLength);
	}
	else if ((theChar >= 0x41) && (theChar <= 0x4F))
	{
		//*	display A->F
		Display7SegmentDigit(xLeft, yTop, ((theChar - 7) & 0x0f), segmentLength);
	}
	else if (theChar == '-')
	{
		Display7SegmentDigit(xLeft, yTop, kLEDsegExtra_Minus, segmentLength);
	}
	else 
	{
		drawchar(xLeft, yTop, theChar);
	}
}

//*******************************************************************************
//*	the arg says hexString, but it doesnt matter if its hex or decimal, it works for either
//*******************************************************************************
void	Display7SegmentString(int xLeft, int yTop, char *hexString, char segmentLength)
{
short	myXvalue;
short	ii;

	myXvalue	=	xLeft;
	ii			=	0;
	while ((hexString[ii] > 0) && (myXvalue < kSCREEN_X_size))
	{

		Display7SegmentChar(myXvalue, yTop, hexString[ii], segmentLength);
		myXvalue	+=	segmentLength;
		myXvalue	+=	segmentLength / 2;
		ii++;
	}
}

#endif	//*	_SUBP_OPTION_7_SEGMENT_

#ifdef _SUBP_OPTION_KEYBOARD_
//*******************************************************************************
void	DrawKeyPadButton(Button *theButton)
{
short	buttonWidth;
short	segmentSize;

	FillRect(&theButton->buttonRect);
	FrameRect(&theButton->buttonRect);

	buttonWidth	=	(theButton->buttonRect.right - theButton->buttonRect.left);
	segmentSize	=	buttonWidth /3;
	segmentSize	-=	1;
	
	Display7SegmentChar(	theButton->buttonRect.left + (buttonWidth >> 2) + (buttonWidth >> 3),
							theButton->buttonRect.top + (buttonWidth / 6),
							theButton->buttonChar,
							segmentSize);

}




#endif


#pragma mark -
#pragma mark Old LCD routines
#if defined(_SUPPORT_OLD_LCD_ROUTINE_NAMES_) && !defined(__LCD_H_)
//*******************************************************************************
void	lcd_rectangle(int x1, int y1, int x2, int y2, COLOR outline_color, COLOR fill_color)
{

	bcolor	=	fill_color;
	fcolor	=	outline_color;
	fillEnb	=	true;
	drawrect(x1, y1, (x2 - x1), (y2, y1));
}

//*******************************************************************************
void	lcd_pixel(int x, int y, COLOR pixel_color)
{
	dispColor(pixel_color);
	dispPixel(x, y);
}



#endif	//	_SUPPORT_OLD_LCD_ROUTINE_NAMES_
