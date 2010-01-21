//*******************************************************************************
//*	
//*	                             Sub-Processing 
//*                        for TouchShield library
//*	         by inthebitz @ antipastohw.blogspot.com & liquidware.com
//*	
//*******************************************************************************
//*	History:
//*	
//*	Version 0.2 | Dec 4, 2008  | Chris @ antipastohw.blogspot.com & liquidware.com
//*	Merged into the TouchShield core
//*	
//*	Version 0.1 | Nov 30, 2008 | inthebitz @ antipastohw.blogspot.com & liquidware.com
//*	This is the beginnings of the "Sub-Processing" for TouchShield library
//*	
//*	
//*	DESCRIPTION:
//*	Sub-Processing provides a rich graphics and interaction library
//*	for the TouchShield and TouchSlide
//*
//*******************************************************************************
//*	Detailed Edit history
//*	<inthebitz>	=	Matt <inthebitz @ antipastohw.blogspot.com & liquidware.com>
//*	<CML>		=	Chris Ladden <Chris @ antipastohw.blogspot.com & liquidware.com>
//*	<MLS>		=	Mark Sproul <msproul@jove.rutgers.edu>
//* <TWH>		=	Thom Holtquist, ctsfutures.com
//*******************************************************************************
//*	Nov 30,	2008	<inthebitz> Version 0.1 This is the beginnings of the "Sub-Processing" for TouchShield library
//*	Dec  4,	2008	<CML> Version 0.2 Merged into the TouchShield core
//*	Dec 26,	2008	<MLS> Mark Sproul meet with Chris and Matt in New Haven
//*	Dec 26,	2008	<MLS> taking over the development of Sub-Processing
//*	Dec 27,	2008	<MLS> Fixed arg order error in beginCanvas call to drawchar
//*	Dec 27,	2008	<MLS> Changing many variable names to longer more understandable names
//*	Dec 28,	2008	<MLS> Separated rect into framerect and rect
//*	Dec 28,	2008	<MLS> Wrote on 7-segment display, it looks great
//*	Dec 29,	2008	<CML/Matt> Changed from serialBegin(9600); to beginSerial(9600);
//*	Dec 29,	2008	<MLS> Starting on Macintosh QUICKDRAW interface to graphics library
//*	Dec 30,	2008	<MLS> Changed gettouch to return isTouching
//*	Dec 31,	2008	<MLS> When Chris did v0012LW, the screen structure got lost
//*	Jan  2,	2009	<MLS> Added InsetRect
//*	Jan  2,	2009	<MLS> Changed args in orientation() to LONG, fixed bug in filled triangles
//*	Jan  3,	2009	<MLS> Added OffsetRect
//*	Jan  3,	2009	<MLS> Changed my_point to gMostRecentTouchPt
//*	Jan 18,	2009	<MLS> Meet with Matt, Chris, Mike and Justin, made lots of progress
//*	Jan 18,	2009	<MLS> Version 1.1.0 handed over to Matt, Mike and Chriss
//*	Jan 18,	2009	<MLS> Added bezier curve functions
//*	Jan	19,	2009	<MLS> Changed strokeWeightVal to gStrokeWeightVal and changed it to uint8_t
//*	Jan 19,	2009	<MLS> Changed strokeEnb to gStrokeEnb
//*	Jan 19,	2009	<MLS> Reducing memory footprint by moving some globals that dont need to be globals into routines
//*	Jan 19,	2009	<MLS> Fixed bug in quad, did not restore stroke values completely
//*	Jan 19,	2009	<MLS> Added arc() routine, including fill option
//*	Jan 25,	2009	<MLS> Added QuickDraw MoveTo, LineTo, Move, Line
//*	Jan 26,	2009	<MLS> Added QuickDraw EraseRect
//*	Feb  2,	2009	<MLS> Added QuickDraw DrawCString
//*	Feb 10,	2009	<MLS> PlotIcon now uses program memory
//*	Feb 10,	2009	<MLS> Version 1.1.1 posted to github
//*	Feb 11,	2009	<MLS> Bumping my version to 1.1.2
//*	Feb 11,	2009	<MLS> PlotIcon now uses program memory
//*	Feb 12,	2009	<MLS> 7-seg display table now uses program memory
//*	Feb 25,	2009	<MLS> Fixed bug in lcd_rectangle
//*	Mar  3,	2009	<MLS> Adding processing font routines to interface to the Hershey fonts
//*	Mar  3,	2009	<MLS> rect and ellipse now honor the gStrokeEnb flag
//*	Jan  3,	2010	<MLS> Added EraseScreen
//* Jun 14, 2009	<TWH> Added anti-aliasing support; accessed through smoothingMode
//* Jun 14, 2009	<TWH> Added clipping support; accessed through setClip & resetClip
//* Jun 14, 2009	<TWH> Added HMI: call updatePen() inside of void loop to access
//* Jun 14, 2009	<TWH> Improved HMI: accessed pen, penDown, penTap & penHold
//* Jun 14, 2009	<TWH> Added alphablending: alphaBlend()
//* Jun 14, 2009	<TWH> Added inverting: invertColor()
//* Jun 14, 2009	<TWH> Added getPixel()
//* Jun 14, 2009	<TWH> Updated beginCanvas to work w/ new commands
//* Jun 14, 2009	<TWH> Added new color support to methods
//* Jun 14, 2009	<TWH> Extended support to Hershey
//*	Sep 16,	2009	<CML> Added text functions to support height/width (width ignored)
//*	Jan  5,	2010	<MLS> After several email exchanges with TWH, I am merging is code into mine
//*	Jan  7,	2010	<MLS> Received the version TWH wants me to use, now for the big job of merging
//*	Jan  7,	2010	<MLS> Added _ENABLE_SMOOTHING_ option, this is for TWHs neat code
//*	Jan  8,	2010	<MLS> Changed smoothingMode to gSmoothingMode
//*	Jan  8,	2010	<MLS> Added SetSmoothMode and moved enums to .h file
//*	Jan  9,	2010	<TWH/MLS> Added in TWH clipping code (_ENABLE_CLIPPING_) see graphics.h
//*	Jan  0,	2010	<TWH/MLS> Added in TWH pentap routines
//*	Jan	 9,	2010	<TWH/MLS> Added in TWH Tahoma font support (_ENABLE_TAHOMA_FONTS_) see fonts.h
//*	Jan 10,	2010	<MLS> Added textWidth() to return width of a text string.
//*	Jan 10,	2010	<MLS> Integrated TAHOMA fonts and HERSHEY fonts
//*	Jan 15,	2010	<MLS> strokeWeight() now returns the OLD stroke to alow a routine to reset it back to what it was
//*	Jan 17,	2010	<MLS> Added _ENABLE_TOUCHSHIELD_DEBUG_ which prints out debuging via the Arduino USB port
//*******************************************************************************
/*
Notes for programers

	setbrightness	changed to setBrightness  (capital B)
	for TWH changed clipX to gClipX1, etc
	for TWH changed clipw to gClipX2, etc
	for TWH changed penTap to gPenTap, etc
	for THW changed useFont to gUseFont
	for THW must access gUseFont through setFont, cannot access directly
	
	global width changed to gWidth
	global height changed to gHeight
	
	
*/
//*******************************************************************************
//*	<MLS> Programming style
//*			Never use 1 letter variable names, use LONG names,
//*			{ and } always go on their own line
//*			Never put 2 lines of code on the same line
//*			Use TAB instead of multiple spaces
//*			A comment line of "****"s above the start of each routine
//*			Leave a space before and after each operator "a + b"  not "a+b"
//*		these next few rules have to be broken to keep compatibility
//*			Routine names should start with a CAPITAL letter
//*			Variable names should always start with lower case,
//*			Global variables should start with a lower case "g" followed by an UPPER case char
//*			Constants (#define )  should start with a lower case "k" followed by an UPPER case char
//*			UnderLine "_" is an ok char to use in variable names, routine names, and constants
//*			Changes should include comments with your initials
//*			Macros should have some comments explaining what they do and why
//*******************************************************************************
//*	PREPROCESSOR DIRECTIVES
//*******************************************************************************
#include	<avr/io.h>
#include	<avr/pgmspace.h>
#include	<inttypes.h>
#include	<math.h>
#include	<stdlib.h>
#include	"wiring.h"

#ifdef __MWERKS__
	//*	this is I can test compile with CodeWarrior
	#define abs(x) (x)
#endif


#ifndef _HARDWARE_DEF_H_
	#include	"HardwareDef.h"
#endif

#ifndef _SUBP_OPTIONS_H_
	#include	"SubPOptions.h"
#endif

#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif
#ifndef _FONT_H_
	#include	"font.h"
#endif

#include	"QuickDraw.h"

#ifndef GRAPHICS_H
	#include	"graphics.h"
#endif
#ifdef _TOUCH_SLIDE_
	#include	"oled_slide.h"
#endif
#ifdef _TOUCH_STEALTH_
	#include	"oled_stealth.h"
#endif


#ifdef _ENABLE_HERSHEY_FONTS_
	#ifndef _HESHEYFONT_H_
		#include	"HersheyFonts.h"
	#endif
#endif


//*******************************************************************************
//*	DEFINITIONS
//*******************************************************************************
#define	incx() xx++, dxt += d2xt, t += dxt
#define	incy() yy--, dyt += d2yt, t += dyt

#define MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))



//*******************************************************************************
//*	FONT VARIABLES
//*******************************************************************************
static	PFont	gCurrentFontNumber	=	0;
static	uint8_t	gCurrentFontSize	=	10;
static	uint8_t	gCurrentFontPenSize	=	1;



//*******************************************************************************
//*	GLOBAL CONSTANTS AND VARIABLES
//*******************************************************************************

//* INPUT VARIABLES
COLOR	fcolor				=	{ 255, 255, 255 };
COLOR	bcolor				=	{ 0, 0, 0 };


int		gBrightness			=	BRIGHT_MAX;
int		mouseX				=	screen.width / 2;
int		mouseY				=	screen.height / 2;
int		gWidth				=	kSCREEN_X_size;		//screen.width;
int		gHeight				=	kSCREEN_Y_size;		//screen.height;
POINT	gMostRecentTouchPt;
uint8_t	gStrokeWeightVal	=	1;
uint8_t	gStrokeEnb			=	true;
uint8_t	gFillEnb			=	true;
uint8_t	gServerEnb			=	false;

int		trix1, triy1, trix2, triy2, trix3, triy3;

#ifdef _ENABLE_SMOOTHING_
	uint8_t	gSmoothingMode	=	kSmoothing_Off;
#endif
#ifdef _ENABLE_PEN_TAP_
	POINT			gPen;
	boolean			gPenDown		=	false;
	boolean			gPenHold		=	false;
	boolean			gPenAuto		=	false;
	boolean			gPenTap			=	false;
	unsigned long	_mPenDown		=	0;		// MS (MilliSeconds) at which Pen was last down
	unsigned long	_mPenHold		=	0;		// MS at which Pen started HOLD state
	boolean			_bNoHold		=	false;	// Ignore HOLD for this down?
	POINT			_pPenHold;					// Point pen is HELD at
#if 1
	//*	use defines instead of memory space
	#define			_mWaitDown		10			// MS to wait before calling PenDown
	#define			_mWaitUp		100			// MS to wait before calling PenUp
	#define			_mWaitHold		1500		// MS to wait before calling PenHold
	#define			_mWaitAuto		100			// MS to wait AFTER PenHold for PenAuto
	#define			_iHoldRadius	20			// Distance allowed to move & still trigger HOLD
#else
	int				_mWaitDown		=	10;		// MS to wait before calling PenDown
	int				_mWaitUp		=	100;	// MS to wait before calling PenUp
	int				_mWaitHold		=	1500;	// MS to wait before calling PenHold
	int				_mWaitAuto		=	100;	// MS to wait AFTER PenHold for PenAuto
	int				_iHoldRadius	=	20;		// Distance allowed to move & still trigger HOLD
#endif

#endif


#pragma mark -
#pragma mark Debugging routines
#ifdef _ENABLE_TOUCHSHIELD_DEBUG_
//*******************************************************************************
//*	these enable debugging through the arduino serial port
//*	by putting the Arduino in reset and then opening the serial port
//*******************************************************************************
//*	On the Mac, an alternative way to talk to the Arduino USB port 
//*	is using the "screen" command
//*				tty.usbserial-A8008Ifq is your USB device (look in /dev)
//*				default baud rate is 9600
//*				to exit and close the port properly, its <CTRL-A><K>
//*		>screen /dev/tty.usbserial-A8008Ifq
//*		>screen /dev/tty.usbserial-A8008Ifq 57600
//*******************************************************************************
void	TouchDebug_Enable(unsigned long baudRate)
{
	arduinoReset();
	delay(500);
	usart_init(baudRate);
	delay(100);
	TouchDebug_Print("Debugging through Arduino\r\n");
}

//*******************************************************************************
void	TouchDebug_Print(char *debugString)
{
	usart_puts(debugString);
}
#endif


#pragma mark -
#pragma mark Local Routines
//*******************************************************************************
//* LOCAL FUNCTION PROTOTYPES
//*		Local functions are those that are not called by other routines
//*		therefore they are declared static which makes them NOT able to
//*		be called from other routines (no linker table entries)
//*******************************************************************************
static void	dispFillEllipse(int xCenter, int yCenter, int xRadius, int yRadius);
static void	dispOutlineEllipse(unsigned int xc,unsigned int yc,unsigned int a,unsigned int b);
static void	dispWuLine( int X0, int Y0, int X1, int Y1, COLOR col);
static void	dispTriangle( int x1, int y1, int x2, int y2, int x3, int y3);
static int	point_triangle_intersection(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3);
static int	orientation (long x1, long y1, long x2, long y2, long px, long py);
static void	getstring(uint8_t *buf, int bufsize);
static void	sendStringL( char *buf, int size);
//void	drawNumber(unsigned long n, uint8_t base, int xLoc, int yLoc);

//*******************************************************************************
//*	LOCAL FUNCTIONS
//*******************************************************************************


#pragma mark -
#pragma mark Local Utility routines

//*******************************************************************************
static float	getPointSpacing(int radius)
{
	float ls = 1.06814128;
	if (radius < 0)
	{
		return -ls / radius;
	}
	else if (radius == 0)
	{
		return ls;
	}
	else
	{
		return ls / radius;
	}
}

//*******************************************************************************
//* Jun 14, 2009	<TWH> Added alphablending: alphaBlend()
static int		alphaBlendVal(float fgcolor, float bgcolor, float a)
{
	if (a >= 98) return fgcolor;
	float p1	=	(100 - a) / 100;
	float p2	=	a / 100;
	return (bgcolor * p1) + (fgcolor * p2);
}


#pragma mark -
#pragma mark Local Line routines



//*******************************************************************************
//	There's definitely an easier way to do this, but for now I'll use some help from:
//	http://www.codeproject.com/KB/GDI/antialias.aspx#dwuln
//	Edited by inthebitz
//*	"Wu" refers to Xiaolin Wu's line algorithm http://en.wikipedia.org/wiki/Xiaolin_Wu's_line_algorithm
//*******************************************************************************
//*	TWH added color to the arg list
//*******************************************************************************
static void	dispWuLine( int X0, int Y0, int X1, int Y1, COLOR col)
{
unsigned short	IntensityShift, ErrorAdj, ErrorAcc;
unsigned short	ErrorAccTemp, Weighting, WeightingComplementMask;
short			DeltaX, DeltaY, Temp, XDir;
short			BaseColor		=	0;
short			NumLevels		=	2;
unsigned short	IntensityBits	=	2;

	dispColor(col);

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
	//*	Draw the initial pixel, which is always exactly intersected by
	//*	the line and so needs no weighting
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
	//*	Special-case horizontal, vertical, and diagonal lines, which
	//*	require no weighting because they go right through the center of
	//*	every pixel
	if ((DeltaY = Y1 - Y0) == 0)
	{
		//*	Horizontal line
		while (DeltaX-- != 0)
		{
			X0	+=	XDir;
			dispPixel(X0, Y0);
		}
		return;
	}
	if (DeltaX == 0)
	{
		//*	Vertical line
		do
		{
			Y0++;
			dispPixel(X0, Y0);
		} while (--DeltaY != 0);
		return;
	}
	if (DeltaX == DeltaY)
	{
		//*	Diagonal line
		do
		{
			X0	+=	XDir;
			Y0++;
			dispPixel(X0, Y0);
		} while (--DeltaY != 0);
		return;
	}

	//*	Line is not horizontal, diagonal, or vertical

	ErrorAcc	=	0;	/* initialize the line error accumulator to 0 */
						/* # of bits by which to shift ErrorAcc to get intensity level */

	IntensityShift	=	16 - IntensityBits;
	//*	Mask used to flip all bits in an intensity weighting, producing the
	//*	result (1 - intensity weighting)
	WeightingComplementMask	=	NumLevels - 1;

	//*	Is this an X-major or Y-major line?
	if (DeltaY > DeltaX)
	{
		//*	Y-major line; calculate 16-bit fixed-point fractional part of a
		//*	pixel that X advances each time Y advances 1 pixel, truncating the
		//*	result so that we won't overrun the endpoint along the X axis
		ErrorAdj	=	((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
		//*	Draw all pixels other than the first and last
		while (--DeltaY)
		{
			ErrorAccTemp	=	ErrorAcc;		//*	remember currrent accumulated error
			ErrorAcc		+=	ErrorAdj;		//*	calculate error for next pixel
			if (ErrorAcc <= ErrorAccTemp)
			{
				//*	The error accumulator turned over, so advance the X coord
				X0	+=	XDir;
			}
			Y0++;	//*	Y-major, so always advance Y
			//*	The IntensityBits most significant bits of ErrorAcc give us the
			//*	intensity weighting for this pixel, and the complement of the
			//*	weighting for the paired pixel
			Weighting	=	ErrorAcc >> IntensityShift;
			//////////////////////////////////////////////////////
			dispPixel(X0, Y0);
		}
		//*	Draw the final pixel, which is
		//*	always exactly intersected by the line
		//*	and so needs no weighting
		dispPixel(X1, Y1);
		return;
	}
	//*	It's an X-major line; calculate 16-bit fixed-point fractional part of a
	//*	pixel that Y advances each time X advances 1 pixel, truncating the
	//*	result to avoid overrunning the endpoint along the X axis
	ErrorAdj	=	((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
	//*	Draw all pixels other than the first and last

	while (--DeltaX)
	{
		ErrorAccTemp	=	ErrorAcc;	//*	remember currrent accumulated error
		ErrorAcc += ErrorAdj;			//*	calculate error for next pixel
		if (ErrorAcc <= ErrorAccTemp)
		{
			//*	The error accumulator turned over, so advance the Y coord
			Y0++;
		}
		X0	+=	XDir; //*	X-major, so always advance X
		//*	The IntensityBits most significant bits of ErrorAcc give us the
		//*	intensity weighting for this pixel, and the complement of the
		//*	weighting for the paired pixel
		Weighting	=	ErrorAcc >> IntensityShift;
		///////////////////////////////////////////////////
		dispPixel(X0, Y0);
	}
	//*	Draw the final pixel, which is always exactly intersected by the line
	//*	and so needs no weighting
	dispPixel(X1, Y1);

	//*	set dispColor to fcolor for backwards compatibility
	dispColor(fcolor);
}


#ifdef _ENABLE_SMOOTHING_

//*******************************************************************************
//*	QS - Quick Smooth; the lower quality smoothing mode
static void	dispQSLine(int x1, int y1, int x2, int y2, COLOR fgcol, COLOR bgcol)
{
float	xScope;
float	yScope;
float	dirX;
float	dirY;
float	linearDeviance;
float	counter;
float	aaStrength;
float	endpointIntensity;
float	halfIntensity = 50;

	xScope	=	x2 - x1;
	yScope	=	y2 - y1;

	if (xScope < 0)
	{
		xScope	=	abs(xScope);
		dirX	=	-1;
	}
	else
	{
		dirX	=	1;
	}

	if (yScope < 0)
	{
		yScope	=	abs(yScope);
		dirY	=	-1;
	}
	else
	{
		dirY	=	1;
	}  

	//* No point in drawing a 0 length line
	if (xScope + yScope == 0) return;

	if (xScope > yScope)
	{
		endpointIntensity	=	(33 * yScope) / xScope;
		setForecolor(alphaBlend(fgcol, bgcol, endpointIntensity));
		setPixel(x1 - dirX, y1 - dirY);
		setPixel(x2 + dirX, y2 + dirY);
		setForecolor(alphaBlend(fgcol, bgcol, halfIntensity));
		setPixel(x1 - dirX, y1);
		setPixel(x2 + dirX, y2);

		setForecolor(fgcol);
		linearDeviance	=	xScope / 2;
		for (counter = 0; counter < xScope; counter++)
		{
			setForecolor(fgcol);
			setPixel(x1, y1);
			aaStrength	=	(linearDeviance * 100) / xScope;
			
			setForecolor(alphaBlend(fgcol, bgcol, 100-aaStrength));
			setPixel(x1, y1 - dirY);
			
			setForecolor(alphaBlend(fgcol, bgcol, aaStrength));
			setPixel(x1, y1 + dirY);
			
			linearDeviance	=	(linearDeviance + yScope);
			if (linearDeviance >= xScope)
			{
				linearDeviance	=	linearDeviance - xScope;
				y1	=	y1 + dirY;
			}
			x1	=	x1 + dirX;
		}
	}
	else
	{
		endpointIntensity	=	(33 * xScope) / yScope;
		setForecolor(alphaBlend(fgcol, bgcol, endpointIntensity));
		setPixel(x1 - dirX, y1 - dirY);
		setPixel(x2 + dirX, y2 + dirY);
		setForecolor(alphaBlend(fgcol, bgcol, halfIntensity));
		setPixel(x1, y1 - dirY);
		setPixel(x2, y2 + dirY);

		linearDeviance	=	yScope / 2;
		for (counter = 0; counter < yScope; counter++)
		{
			setForecolor(fgcol);
			setPixel(x1, y1);
			
			aaStrength	=	(linearDeviance * 100) / yScope;

			setForecolor(alphaBlend(fgcol, bgcol, 100 - aaStrength));
			setPixel(x1 - dirX, y1);
			
			setForecolor(alphaBlend(fgcol, bgcol, aaStrength));
			setPixel(x1 - dirX, y1);

			linearDeviance	=	(linearDeviance + xScope);
			if (linearDeviance >= yScope)
			{
				linearDeviance	=	linearDeviance - yScope;
				x1	=	x1 + dirX;
			}
			y1	=	y1 + dirY;

		}
	} 
}

//*******************************************************************************
//*	HS - High Smooth; higher quality
static void	dispHSLine(int x1, int y1, int x2, int y2, COLOR col)
{
long  xScope;
long  yScope;
long  dirX;
long  dirY;
long  linearDeviance;
long  counter;
long  aaStrength;
long  endpointIntensity;
long  halfIntensity	=	50;

	xScope	=	x2 - x1;
	yScope	=	y2 - y1;

	if (xScope < 0)
	{
		xScope	=	abs(xScope);
		dirX	=	-1;
	}
	else
	{
		dirX	=	1;
	}

	if (yScope < 0)
	{
		yScope	=	abs(yScope);
		dirY	=	-1;
	}
	else
	{
		dirY	=	1;
	}  

	//* No point in drawing a 0 length line
	if (xScope + yScope == 0) return;

	if (xScope > yScope)
	{
		endpointIntensity	=	(85 * yScope) / xScope;

		getPixel(&bcolor,x1 - dirX, y1 - dirY);
		setForecolor(alphaBlend(col,bcolor,endpointIntensity));
		setPixel(x1 - dirX, y1 - dirY);

		getPixel(&bcolor,x2 + dirX, y2 + dirY);
		setForecolor(alphaBlend(col,bcolor,endpointIntensity));
		setPixel(x2 + dirX, y2 + dirY);

		getPixel(&bcolor,x1 - dirX, y1);
		setForecolor(alphaBlend(col,bcolor,halfIntensity));
		setPixel(x1 - dirX, y1);

		getPixel(&bcolor,x2 + dirX, y2);
		setForecolor(alphaBlend(col,bcolor,halfIntensity));
		setPixel(x2 + dirX, y2);

		linearDeviance	=	xScope / 2;
		for (counter = 0; counter < xScope; counter++)
		{
	 
			setForecolor(col);
			setPixel(x1,y1);
			aaStrength	=	(linearDeviance * 100) / xScope;
			
			getPixel(&bcolor,x1,y1-dirY);
			setForecolor(alphaBlend(col,bcolor,100-aaStrength));
			setPixel(x1,y1-dirY);
			
			getPixel(&bcolor,x1,y1+dirY);
			setForecolor(alphaBlend(col,bcolor,aaStrength));
			setPixel(x1,y1+dirY);
			
			linearDeviance	=	(linearDeviance + yScope);
			if (linearDeviance >= xScope)
			{
				linearDeviance	=	linearDeviance - xScope;
				y1	=	y1 + dirY;
			}
			x1	=	x1 + dirX;
		}
	}
	else
	{
		endpointIntensity	=	(85 * xScope) / yScope;

		getPixel(&bcolor,y1 - dirY, x1 - dirX);
		setForecolor(alphaBlend(col,bcolor,endpointIntensity));
		setPixel(y1 - dirY, x1 - dirX);

		getPixel(&bcolor,x2 + dirY, y2 + dirX);
		setForecolor(alphaBlend(col,bcolor,endpointIntensity));
		setPixel(x2 + dirY, y2 + dirX);

		getPixel(&bcolor,y1 - dirY, x1);
		setForecolor(alphaBlend(col,bcolor,halfIntensity));
		setPixel(y1 - dirY, x1);

		getPixel(&bcolor,x2 + dirY, y2);
		setForecolor(alphaBlend(col,bcolor,halfIntensity));
		setPixel(x2 + dirY, y2);

		linearDeviance	=	yScope / 2;
		for (counter = 0; counter < yScope; counter++)
		{
			setForecolor(col);
			setPixel(x1,y1);
			
			aaStrength	=	(linearDeviance * 100) / yScope;

			getPixel(&bcolor,x2 + dirY, y2);
			setForecolor(alphaBlend(col,bcolor,100-aaStrength));
			setPixel(x1 - dirX,y1);

			getPixel(&bcolor,x2 + dirY, y2); 
			setForecolor(alphaBlend(col,bcolor,aaStrength));
			setPixel(x1- dirX,y1);

			linearDeviance	=	(linearDeviance + xScope);
			if (linearDeviance >= yScope)
			{
			linearDeviance	=	linearDeviance - yScope;
			x1	=	x1 + dirX;
			}
			y1	=	y1 + dirY;
		}
	}
}


#endif

#pragma mark -
#pragma mark Local Shape routines



//*******************************************************************************
static void	dispOutlineEllipse( int xCenter, int yCenter, int xRadius, int yRadius)
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
			t + a2 * yy <= crit3)	/* e(x+1/2,y) <= 0 */
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

//#define _USE_LINES_FOR_CIRCLE_FILL_

//*******************************************************************************
//*	e(x,y) = b^2*x^2 + a^2*y^2 - a^2*b^2
//*	a	=	xRadius
//*	b	=	yRadius
//*******************************************************************************
static void	dispFillEllipse(int xCenter, int yCenter, int xRadius, int yRadius)
{
int				xx		=	0;
int				yy		=	yRadius;
unsigned int	myWidth	=	1;
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
		if ((t + b2 * xx <= crit1) ||	 /* e(x+1,y-1/2) <= 0 */
			(t + a2 * yy <= crit3))
		{
			/* e(x+1/2,y) <= 0 */
			incx();
			myWidth	+=	2;
		}
		else if (t - a2 * yy > crit2)
		{
		#ifdef _USE_LINES_FOR_CIRCLE_FILL_
			dispWuLine(xCenter - xx, yCenter - yy, (xCenter - xx + myWidth), (yCenter - yy));
		#else
			/* e(x+1/2,y-1) > 0 */
			dispRectangle(xCenter - xx, yCenter - yy, myWidth, 1);
		#endif
			if (yy != 0)
			{
			#ifdef _USE_LINES_FOR_CIRCLE_FILL_
				dispWuLine(xCenter - xx, yCenter + yy, (xCenter - xx + myWidth), (yCenter + yy));
			#else
				dispRectangle(xCenter - xx, yCenter + yy, myWidth, 1);
			#endif
			}
			incy();
		}
		else
		{
		#ifdef _USE_LINES_FOR_CIRCLE_FILL_
			dispWuLine(xCenter - xx, yCenter - yy, (xCenter - xx + myWidth), (yCenter - yy));
		#else
			dispRectangle(xCenter - xx, yCenter - yy, myWidth, 1);
		#endif
			if (yy != 0)
			{
			#ifdef _USE_LINES_FOR_CIRCLE_FILL_
				dispWuLine(xCenter - xx, yCenter + yy, (xCenter - xx + myWidth), (yCenter + yy));
			#else
				dispRectangle(xCenter - xx, yCenter + yy, myWidth, 1);
			#endif
			}
			incx();
			incy();
			myWidth	+=	2;
		}
	}
	if (yRadius == 0)
	{
		//dispRectangle(xCenter - xRadius, yCenter, 2 * xRadius + 1, 1);
		dispRectangle(xCenter - xRadius, yCenter, 2 * xRadius + 1, 1);
	}
}

//*******************************************************************************
static void	dispEllipse(int x, int y, int xRadius, int yRadius, COLOR col)
{
int		xx		=	0;
int		yy		=	yRadius;
long	a2		=	(long)xRadius * xRadius;
long	b2		=	(long)yRadius * yRadius;
long	crit1	=	-(a2 / 4 + xRadius % 2 + b2);
long	crit2	=	-(b2 / 4 + yRadius % 2 + a2);
long	crit3	=	-(b2 / 4 + yRadius % 2);
long	t		=	-a2 * yy; 
long	dxt		=	2 * b2 * xx;
long	dyt		=	-2 * a2 * yy;
long	d2xt	=	2 * b2;
long	d2yt	=	2 * a2;

	dispColor(col);
	while (yy >= 0 && xx <= x)
	{
		dispPixel(x	+ xx, y + yy);
		if (xx != 0 || yy != 0)
		{
			dispPixel(x - xx, y - yy);
		}
		if (xx != 0 && yy != 0)
		{
			dispPixel(x + xx, y - yy);
			dispPixel(x - xx, y + yy);
		}
		if (t + b2 * xx <= crit1 ||	
			t + a2 * yy <= crit3)
		{
			incx();
		}
		else if (t - a2 * yy > crit2)
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

#ifdef _ENABLE_SMOOTHING_
//*******************************************************************************
//*	SM - Smooth Mode; the procedure for drawing an Ellipse with smoothing is
//*	different enough that I gave it its own method and dealt with QS/HS
//*	inside of it.
static void	dispSMEllipse(int x, int y, int xRadius, int yRadius, COLOR col)
{
float    radius;
int		quadrant;
int		c1;
float	alpha;
//float	alphaI;
float	pointSpacing;
float	angle;
float	halfPi = 1.570796;
float	x2;
float	y2;
float	aX;
float	aY;
float	bX;
float	bY;
float	rX1;
float	rX2;
float	rY1;
float	rY2;
float	xp5;
float	L1;
float	L2;
float	L3;
float	L4;
float	savX[4];
float	savY[4];
float	savA[4];
float	x4;
float	y4;

	if (xRadius > yRadius)
	{
		radius	=	xRadius;
	}
	else
	{
		radius	=	yRadius;
	}

	pointSpacing	=	getPointSpacing(radius);

	for (angle = 0; angle < halfPi; angle = angle + pointSpacing)
	{
		x2	=	(xRadius * cos(angle)) + 0.001;
		y2	=	(yRadius * sin(angle)) + 0.001;
		for (quadrant = 0; quadrant < 4; quadrant++)
		{
			switch(quadrant)
			{
				case 0: //* 0-90 degrees
					aX	=	x2 + x - 0.5;
					aY	=	-y2 + y - 0.5;
					break;
				case 1: //* 90-180
					aX	=	x2 + x - 0.5;
					aY	=	y2 + y - 0.5;
					break;
				case 2: // 180-270
					aX	=	-x2 + x - 0.5;
					aY	=	y2 + y - 0.5;
					break;
				case 3: //270-360
					aX	=	-x2 + x - 0.5;
					aY	=	-y2 + y - 0.5;
					break; 
			}

			bX	=	aX + 1;
			bY	=	aY + 1;

			rX1	=	aX;
			rX2	=	rX1 + 1;

			xp5	=	rX1 + 0.5;

			rY1	=	aY;
			rY2	=	bY;

			L1	=	rY1 + 0.5 - aY;
			L2	=	256 * (xp5 - aX) - xp5 + aX;
			L3	=	255 - L2;
			L4	=	bY - rY2 + 0.5;

			savX[0]	=	rX1;
			savY[0]	=	rY1;

			savX[1]	=	rX2;
			savY[1]	=	rY1;

			savX[2]	=	rX1;
			savY[2]	=	rY2;

			savX[3]	=	rX2;
			savY[3]	=	rY2;

			savA[0]	=	L1 * L2;
			savA[1]	=	L1 * L3;
			savA[2]	=	L4 * L2;
			savA[3]	=	L4 * L3;

			for (c1 = 0; c1 < 4; c1++)
			{
				x4	=	savX[c1];
				y4	=	savY[c1];
				if (gSmoothingMode == kSmoothing_High)
				{
					getPixel(&bcolor,x4,y4);
				}
				alpha	=	(savA[c1] / 255) * 100;
				setForecolor(alphaBlend(col,bcolor,savA[c1]));
				setPixel(x4,y4);
			}
		}
	}
}
#endif

//*******************************************************************************
//	http://www.swissdelphicenter.ch/en/showcode.php?id=2400
//	http://tog.acm.org/GraphicsGems/gemsiii/triangleCube.c
//	I'm sure there's a more elegant and cleaner way, but with help from the links above I was able to cludge
//	this together, so that it at least works!
//*******************************************************************************
static void	dispTriangle( int x1, int y1, int x2, int y2, int x3, int y3)
{

	if (gStrokeEnb)
	{
		dispColor(fcolor);
		dispWuLine ( x1, y1, x2, y2, fcolor);
		dispWuLine ( x2, y2, x3, y3, fcolor);
		dispWuLine ( x3, y3, x1, y1, fcolor);
	}

	if (gFillEnb)
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
static int point_triangle_intersection(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3)
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
static int orientation(long x1, long y1, long x2, long y2, long px, long py)
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
static void	floodFillRecursive(int argX, int argY, COLOR backGroundColor)
{
COLOR myPixelColor;

	if ((argX < 0) || (argY < 0) || (argX >= gWidth) || (argY >= gHeight))
	{
		return;
	}

	getPixel(&myPixelColor, argX, argY);
	if (	(myPixelColor.red == backGroundColor.red) && 
			(myPixelColor.blue == backGroundColor.blue) && 
			(myPixelColor.green == backGroundColor.green))
	{
		dispPixel(argX, argY);
		floodFillRecursive(argX - 1,	argY,		backGroundColor);
		floodFillRecursive(argX + 1,	argY,		backGroundColor);
		floodFillRecursive(argX,		argY - 1,	backGroundColor);
		floodFillRecursive(argX,		argY + 1,	backGroundColor);
	}
}



#pragma mark -
#pragma mark GLOBAL FUNCTIONS

//*******************************************************************************
//*	GLOBAL FUNCTIONS
//*******************************************************************************

#ifdef _ENABLE_SMOOTHING_
//*******************************************************************************
void	SetSmoothMode(unsigned char newMode)
{
	gSmoothingMode	=	newMode;
	if (gSmoothingMode >= kSmoothing_last)
	{
		gSmoothingMode	=	kSmoothing_Off;
	}
}
#endif


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
void	clear(COLOR rgbColor)
{
//-uint8_t		saveDisplayMode;

//-	saveDisplayMode	=	dispGetDisplayMode();
//-	dispSetDisplayMode(0);
	bcolor			=	rgbColor;
//	fillRect(-1, -1, kSCREEN_X_size + 1, kSCREEN_Y_size + 1, col);
//-	fillRect(0, 0, kSCREEN_X_size, kSCREEN_Y_size, rgbColor);
	fillRect(0, 0, gWidth, gHeight, rgbColor);
//-	dispSetDisplayMode(saveDisplayMode);
}

//*******************************************************************************
void	clear(void)
{
	clear(bcolor);
}

//*******************************************************************************
void	clear(int argRed, int argGreen, int argBlue)
{

	COLOR myRGBcolor	=	{argRed, argGreen, argBlue};
	clear(myRGBcolor);
}


//*******************************************************************************
COLOR	colorFromLong(long val)
{
COLOR myRGBcolor;

	myRGBcolor.blue		=	val % 0x100;
	val					=	val / 0x100;
	myRGBcolor.green	=	val % 0x100;
	val					=	val / 0x100;
	myRGBcolor.red		=	val % 0x100;
	return(myRGBcolor);
}

//*******************************************************************************
float dist(float x1, float y1, float x2, float y2)
{
int	xx	=	(x2 - x1);
int	yy	=	(y2 - y1);

	return sqrt( (xx * xx)+(yy * yy));
}

//*******************************************************************************
float	dist(POINT p1, POINT p2)
{
	return dist(p1.x, p1.y, p2.x, p2.y);
}


#pragma mark -
#pragma mark Global Pixel routines

//*******************************************************************************
void	setPixel(int xx, int yy, COLOR col)
{
	dispColor(col);
//++	dispPixel2(xx, yy);
	dispPixel(xx, yy);
}

//*******************************************************************************
void	setPixel(int xx, int yy)
{
	setPixel(xx, yy, fcolor);
}

//*******************************************************************************
void	getPixel(COLOR *buffer, int x, int y)
{
	dispRead(buffer, x, y);
}


#pragma mark -
#pragma mark Global Line routines
//*******************************************************************************
void	line(int x1, int y1, int x2, int y2, COLOR col)
{
#ifdef _ENABLE_SMOOTHING_
	switch(gSmoothingMode)
	{
		case kSmoothing_Off:
			dispWuLine(x1, y1, x2, y2, col);
			break;
		
		case kSmoothing_Quick:
			dispQSLine(x1, y1, x2, y2, col, bcolor);
			break;
		
		case kSmoothing_High:
			dispHSLine(x1, y1, x2, y2, col);
			break;
	}
#else
	dispWuLine(x1, y1, x2, y2, col);
#endif
}


//*******************************************************************************
void	line(int x1, int y1, int x2, int y2)
{
	line(x1, y1, x2, y2, fcolor);
}



#pragma mark -
#pragma mark Global Shape routines
//*******************************************************************************
void	drawcircle( int xLoc, int yLoc, int radius)
{
	ellipse(xLoc, yLoc, radius, radius);
}

//*******************************************************************************
void	drawrect(int xLoc, int yLoc, int argWidth, int argHeight)
{
	rect(xLoc, yLoc, argWidth, argHeight);
}

//*******************************************************************************
//*	Dec 28,	2008	<MLS> Separated rect into framerect and rect
void	framerect(int xLeft, int yTop, int width, int height)
{
	if ((width > 0) && (height > 0))
	{
		dispColor(fcolor);
		if (gStrokeWeightVal > 1)
		{
		//stroke
		int		left	=	xLeft - 1;
		int		top		=	yTop - 1;
		int		right	=	xLeft + width;
		int		bottom	=	yTop + height;

			for(int i=0; i<gStrokeWeightVal; i++)
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
		if (gFillEnb)
		{
			dispColor(bcolor);
			dispRectangle(xLeft, yTop, width, height);
		}

		if (gStrokeEnb)
		{
			//*	frame the rect
			framerect(xLeft, yTop, width, height);
		}
	}
}

//*******************************************************************************
void	ellipse( int xLoc, int yLoc, int radx, int rady)
{
int	ii;

	//*	fill
	if (gFillEnb)
	{
		dispColor(bcolor);
		dispFillEllipse(xLoc, yLoc, radx-1, rady-1);
	}

	if (gStrokeEnb)
	{
		//*	stroke
		dispColor(fcolor);
		for(ii = 0; ii < gStrokeWeightVal; ii++)
		{
			dispOutlineEllipse(xLoc, yLoc, radx + ii, rady + ii);
		}
	}
}

//*******************************************************************************
void	drawEllipse(int xx, int yy, int xRadius, int yRadius, COLOR col)
{
#ifdef _ENABLE_SMOOTHING_
	if (gSmoothingMode)
	{
		dispSMEllipse(xx, yy, xRadius, yRadius, col);
	}
	else
#endif
	{
		dispEllipse(xx, yy, xRadius, yRadius, col);
	}
}

//*******************************************************************************
void	drawEllipse(int xx, int yy, int xRadius, int yRadius)
{
	drawEllipse(xx, yy, xRadius, yRadius, fcolor);
}

//*******************************************************************************
void	drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, COLOR col)
{

#ifdef _ENABLE_SMOOTHING_
	switch(gSmoothingMode)
	{
		case kSmoothing_Off:
			dispWuLine ( x1, y1, x2, y2, col);
			dispWuLine ( x2, y2, x3, y3, col);
			dispWuLine ( x3, y3, x4, y4, col);
			dispWuLine ( x4, y4, x1, y1, col);
			break;

		case kSmoothing_Quick:
			dispQSLine ( x1, y1, x2, y2, col, bcolor);
			dispQSLine ( x2, y2, x3, y3, col, bcolor);
			dispQSLine ( x3, y3, x4, y4, col, bcolor);
			dispQSLine ( x4, y4, x1, y1, col, bcolor);
			break;

		case kSmoothing_High:
			dispHSLine ( x1, y1, x2, y2, col);
			dispHSLine ( x2, y2, x3, y3, col);
			dispHSLine ( x3, y3, x4, y4, col);
			dispHSLine ( x4, y4, x1, y1, col);
			break;
	}
#else
	dispWuLine ( x1, y1, x2, y2, col);
	dispWuLine ( x2, y2, x3, y3, col);
	dispWuLine ( x3, y3, x4, y4, col);
	dispWuLine ( x4, y4, x1, y1, col);
#endif
}

//*******************************************************************************
void	drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	drawQuad(x1,y1,x2,y2,x3,y3,x4,y4,fcolor);
}

//*******************************************************************************
void	quad( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
uint8_t		prevStroke;
uint8_t		prevStrokeWeight;


	if (gFillEnb)
	{
		//* Save the stroke state
		prevStroke			=	gStrokeEnb;
		prevStrokeWeight	=	gStrokeWeightVal;

		//*	Disable stroke
		noStroke();
		triangle(x1, y1, x2, y2, x3, y3);
		triangle(x2, y2, x3, y3, x4, y4);
		triangle(x1, y1, x3, y3, x4, y4);

		//* Restore Stroke values
		gStrokeEnb			=	prevStroke;
		gStrokeWeightVal	=	prevStrokeWeight;
	}

	if (gStrokeEnb)
	{
		line(x1, y1, x2, y2);
		line(x2, y2, x3, y3);
		line(x3, y3, x4, y4);
		line(x4, y4, x1, y1);
	}
}


//*******************************************************************************
void	drawRect(int xx, int yy, int width, int height)
{
	drawRect(xx, yy, width, height, fcolor);
}

//*******************************************************************************
void	drawRect(int xx, int yy, int width, int height, COLOR col)
{
	if ((width > 0) && (height > 0))
	{
		dispColor(col);
		if (gStrokeWeightVal > 1)
		{
		//stroke
		int		left	=	xx - 1;
		int		top		=	yy - 1;
		int		right	=	xx + width;
		int		bottom	=	yy + height;

			for(int i=0; i<gStrokeWeightVal; i++)
			{
				dispWuLine(left,	top,	right,	top, col);
				dispWuLine(left,	top,	left,	bottom, col);
				dispWuLine(left,	bottom,	right,	bottom, col);
				dispWuLine(right,	top,	right,	bottom, col);
				left--;
				top--;
				bottom++;
				right++;
			}
		}
		else
		{
			dispWuLine(xx, yy, xx + width, yy, col);				//*	top line
			dispWuLine(xx, yy + height, xx + width, yy + height, col);		//*	bottom line
			dispWuLine(xx, yy, xx, yy + height, col);		//*	left line
			dispWuLine(xx + width, yy, xx + width, yy + height, col);		//*	right line
		}

	}

}

//*******************************************************************************
void	drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, COLOR col)
{
#ifdef _ENABLE_SMOOTHING_
	switch(gSmoothingMode) 
	{
		case kSmoothing_Off:
			dispWuLine (x1, y1, x2, y2, col);
			dispWuLine (x2, y2, x3, y3, col);
			dispWuLine (x3, y3, x1, y1, col);
			break;
			
		case kSmoothing_Quick:
			dispQSLine (x1, y1, x2, y2, col, bcolor);
			dispQSLine (x2, y2, x3, y3, col, bcolor);
			dispQSLine (x3, y3, x1, y1, col, bcolor);
			break;
			
		case kSmoothing_High:
			dispHSLine (x1, y1, x2, y2, col);
			dispHSLine (x2, y2, x3, y3, col);
			dispHSLine (x3, y3, x1, y1, col);
			break;
	}
#else
	dispWuLine (x1, y1, x2, y2, col);
	dispWuLine (x2, y2, x3, y3, col);
	dispWuLine (x3, y3, x1, y1, col);
#endif
}

//*******************************************************************************
void	drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	drawTriangle(x1,y1,x2,y2,x3,y3,fcolor);
}

//*******************************************************************************
void	triangle( int x1, int y1, int x2, int y2, int x3, int y3)
{
	dispTriangle(x1, y1, x2, y2, x3, y3);
}


//*******************************************************************************
void	drawWindow(char* caption, int xx, int yy, int ww, int hh, COLOR bkg)
{
uint8_t	oldFont;	// Backup Font
RECT	myOutsideRect;
RECT	myInsideRect;
int		outsideWidth;
int		outsideHeight;
COLOR	myFrameColor;

	oldFont	=	0;
#ifdef _ENABLE_TAHOMA_FONTS_
	oldFont		=	setFont(2);	//*	get the previous font and set to BOLD
#elif defined(_ENABLE_HERSHEY_FONTS_)
	textFont(kHersheyFont_RowmanTriplex, 9);
#endif

	//*	set up the inside window rect
	myInsideRect.left	=	xx;
	myInsideRect.right	=	xx + ww;
	myInsideRect.top	=	yy;
	myInsideRect.bottom	=	yy + hh;

	//*	set up our outside boarder
	myOutsideRect		=	myInsideRect;
	InsetRect(&myOutsideRect, -4, -4);	//*	make the window bigger
	myOutsideRect.top	-=	14;
	outsideWidth		=	myOutsideRect.right - myOutsideRect.left;
	outsideHeight		=	myOutsideRect.bottom - myOutsideRect.top;

	//*	fill the backgrounds
	//*	this is a bit more code but it looks better on the refresh
	setForecolor(209, 209, 209);
	fillRect(myOutsideRect.left,		myOutsideRect.top,			outsideWidth,	18);			//*	titlebar
	fillRect(myOutsideRect.left,		myOutsideRect.bottom - 4,	outsideWidth,	4);				//*	bottom
	fillRect(myOutsideRect.left,		myOutsideRect.top,			4,				outsideHeight);	//*	left side
	fillRect(myOutsideRect.right - 4,	myOutsideRect.top,			4,				outsideHeight);	//*	right side

	fillRect(myInsideRect.left, myInsideRect.top, (myInsideRect.right - myInsideRect.left), (myInsideRect.bottom - myInsideRect.top), bkg);
	
	//*	frame the backgrounds
	setForecolor(0, 0, 0);
	FrameRect(&myOutsideRect);
	FrameRect(&myInsideRect);
	
	// Draw Text
	setForecolor(0, 0, 0);
	centerText(	myOutsideRect.left,
				myOutsideRect.top,
				myOutsideRect.right,
				myOutsideRect.top + 18,
				caption);		// Title


	setFont(oldFont);		// Restore Font
	gCurrentFontPenSize	=	1;
}


//*******************************************************************************
void	fillEllipse(int argX, int argY, int xRadius, int yRadius, COLOR col)
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

#ifdef _ENABLE_SMOOTHING_
	if (gSmoothingMode > 0)
	{
		drawEllipse(argX, argY, xRadius, yRadius, col);
	}
#endif

	dispColor(col);
	while (yy >= 0 &&  xx <= xRadius)
	{
		if (t + b2 * xx <= crit1 ||	
			t + a2 * yy <= crit3) 
		{
			incx();
			width	+=	2;
		}
		else if (t - a2 * yy > crit2) 
		{
			dispRectangle(argX - xx, argY - yy, width, 1);
			if (yy != 0)
			{
				dispRectangle(argX - xx, argY + yy, width, 1);
			}
			incy();
		}
		else
		{
			dispRectangle(argX - xx, argY - yy, width, 1);
			if (yy != 0)
			{
				dispRectangle(argX - xx, argY + yy, width, 1);
			}
			incx();
			incy();
			width	+=	2;
		}
	}
	if (yRadius == 0)
	{
		dispRectangle(argX - xRadius, argY, 2 * xRadius + 1, 1);
	}
}

//*******************************************************************************
void	fillEllipse(int xx, int yy, int xRadius, int yRadius)
{
	fillEllipse(xx, yy, xRadius, yRadius, fcolor);
}


//*******************************************************************************
void	fillRect(int xx, int yy, int width, int height, COLOR col)
{
	dispColor(col);
	dispRectangle(xx, yy, width, height); 
}

//*******************************************************************************
void	fillRect(int xx, int yy, int width, int height)
{
	fillRect(xx, yy, width, height, fcolor);
}

//*******************************************************************************
void	fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, COLOR col)
{
	int		xx;
	int		yy;
	int		startX;
	int		endX;
	int		startY;
	int		endY;

#ifdef _ENABLE_SMOOTHING_
	if (gSmoothingMode > 0)
	{
		drawTriangle(x1, y1, x2, y2, x3, y3, col);
	}
#endif

	//*	figure out the left most and right most points
	startX	=	MIN3(x1, x2, x3);
	endX	=	MAX3(x1, x2, x3);
	//*	figure out the top most and bottom most points
	startY	=	MIN3(y1, y2, y3);
	endY	=	MAX3(y1, y2, y3);
	dispColor(col);
	for (xx = startX; xx < endX; xx++) 
	{
		for (yy = startY; yy < endY; yy++) 
		{
			if (point_triangle_intersection(xx, yy, x1, y1, x2, y2, x3, y3) )
			{
				dispPixel2(xx , yy);
			}
		}
	}
}


//*******************************************************************************
void	fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	fillTriangle(x1, y1, x2, y2, x3, y3, fcolor);
}

//*******************************************************************************
void	fillQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	fillQuad(x1, y1, x2, y2, x3, y3, x4, y4, fcolor);
}

//*******************************************************************************
void	fillQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, COLOR col)
{
	fillTriangle(x1, y1, x2, y2, x4, y4, col);
	fillTriangle(x2, y2, x4, y4, x3, y3, col);
}

//*******************************************************************************
void	floodFill(int argX, int argY, COLOR col)
{
COLOR bg;

	getPixel(&bg, argX, argY);
	dispColor(col);
	floodFillRecursive(argX, argY, bg);
}



#pragma mark -
#pragma mark Global Color routines
//*******************************************************************************
void	fadein( int time)
{
uint8_t ii;

	for (ii = 0; ii<(gBrightness+1); ii++)
	{
	#if defined(_TOUCH_STEALTH_) || !defined(_VERSION_0012LW_)
		dispBrightness((uint8_t)ii);
	#else
		oled_brightness((uint8_t)ii);
	#endif
		delay(time / gBrightness);
	}
}

//*******************************************************************************
void	fadeout( int time)
{
uint8_t ii;

	for (ii = gBrightness; ii>0; ii--)
	{
	#if defined(_TOUCH_STEALTH_) || !defined(_VERSION_0012LW_)
		dispBrightness((uint8_t)ii);
	#else
		oled_brightness((uint8_t)ii);
	#endif
		delay(time / gBrightness);
	}
}

//*******************************************************************************
int		getBrightness(void)
{
	return	gBrightness;
}


//*******************************************************************************
void	setbcolor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
	bcolor.red		=	redValue;
	bcolor.green	=	greenValue;
	bcolor.blue		=	blueValue;
}

//*******************************************************************************
void	setBrightness( int bright)
{
#if defined(_TOUCH_STEALTH_) || !defined(_VERSION_0012LW_)
	dispBrightness(bright);
#else
	oled_brightness(bright);
#endif
	gBrightness	=	bright;
}

//*******************************************************************************
void	setfcolor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
	fcolor.red		=	redValue;
	fcolor.green	=	greenValue;
	fcolor.blue		=	blueValue;
}

//*******************************************************************************
void	setForecolor(COLOR foreColor)
{
	fcolor	=	foreColor;
}

//*******************************************************************************
void	setForecolor(uint8_t redVal, uint8_t greenVal, uint8_t blueVal)
{
	fcolor.red		=	redVal;
	fcolor.green	=	greenVal;
	fcolor.blue		=	blueVal;
}

//*******************************************************************************
void	setBackcolor(COLOR bkgColor)
{
	bcolor	=	bkgColor;
}

//*******************************************************************************
void	setBackcolor(uint8_t redVal, uint8_t greenVal, uint8_t blueVal)
{
	bcolor.red		=	redVal;
	bcolor.green	=	greenVal;
	bcolor.blue		=	blueVal;
}

//*******************************************************************************
COLOR	invertColor(COLOR col)
{
	COLOR rt;
	rt.red	=	255 - col.red;
	rt.green	=	255 - col.green;
	rt.blue	=	255 - col.blue;
	return rt;
}

//*******************************************************************************
COLOR	alphaBlend(COLOR forecolor, COLOR backcolor, int opacity)
{
COLOR blendColor;

	blendColor.red		=	alphaBlendVal(forecolor.red, backcolor.red, opacity);
	blendColor.green	=	alphaBlendVal(forecolor.green, backcolor.green, opacity);
	blendColor.blue		=	alphaBlendVal(forecolor.blue, backcolor.blue, opacity);

	return(blendColor);
}


#pragma mark -
#pragma mark Fill functions



//*******************************************************************************
void	fill(int fillColor)
{
	gFillEnb	=	true;
	setbcolor(fillColor, fillColor, fillColor);
}

//*******************************************************************************
void	fill(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
	gFillEnb	=	true;
	setbcolor(redValue, greenValue, blueValue);
}

//*******************************************************************************
void	noFill(void)
{
	gFillEnb	=	false;
}

//*******************************************************************************
void	fillback(void)
{
	dispColor(bcolor);
	rect(-1, -1, gWidth+1, gHeight+1);
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
void	noStroke(void)
{
	gStrokeEnb			=	false;
	gStrokeWeightVal	=	0;
}

//*******************************************************************************
void	point( int xLoc, int yLoc)
{
	dispColor(fcolor);
	dispPixel( xLoc, yLoc);
}


//*******************************************************************************
void	size(int w, int h)
{
	gWidth	=	w;
	gHeight	=	h;
	mouseX	=	w / 2; //reset the mouse
	mouseY	=	h / 2; //reset the mouse
}


//*******************************************************************************
void	stroke(int newStrokeColor)
{
	gStrokeEnb			=	true;
	if (!gStrokeWeightVal)
	{
		//*	only reset it if it was zero, this will maintain other settings
		//*	such as a larger value
		gStrokeWeightVal	=	1;
	}
	setfcolor(newStrokeColor, newStrokeColor, newStrokeColor);
}

//*******************************************************************************
//*	returns OLD stroke weight to alow a routine to reset it back to what it was
uint8_t	strokeWeight(int newStrokeWeight)
{
uint8_t	oldStrokeWeight;

	oldStrokeWeight		=	gStrokeWeightVal;
	gStrokeWeightVal	=	newStrokeWeight;
	return(oldStrokeWeight);
}

//*******************************************************************************
void	stroke(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
	gStrokeEnb			=	true;
	if (!gStrokeWeightVal)
	{
		//*	only reset it if it was zero, this will maintain other settings
		//*	such as a larger value
		gStrokeWeightVal	=	1;
	}
	setfcolor(redValue, greenValue, blueValue);
}

#pragma mark -
#pragma mark Global text routines

//*******************************************************************************
void	drawchar( int xLoc, int yLoc, char theChar)
{
	dispPutC(theChar, xLoc, yLoc, fcolor, bcolor);
}


//*******************************************************************************
void	drawstring( int xLoc, int yLoc, char *text)
{
	dispPutS(text, xLoc, yLoc, fcolor, bcolor);
}


//*******************************************************************************
void	text(char *textString, int xLoc, int yLoc)
{
	switch(gCurrentFontNumber)
	{
		case 0:
			dispPutS(textString, xLoc, yLoc, fcolor, bcolor);
			break;
		
	#ifdef _ENABLE_TAHOMA_FONTS_
		case 1:
			dispPutS1(textString, xLoc, yLoc, fcolor, bcolor);
			break;
		
		case 2:
			dispPutS2(textString, xLoc, yLoc, fcolor, bcolor);
			break;
	#endif

		default:
		#ifdef _ENABLE_HERSHEY_FONTS_
			HersheyDrawCString(	gCurrentFontNumber,	//*	which hershey font to draw
								xLoc,				//*	X location
								yLoc,				//*	Y location
								textString,			//*	strint to draw
								gCurrentFontSize,	//*	font size
								0,					//*	font angle
								1);					//*	pen size, currently ignored
		#else
			dispPutS(textString, xLoc, yLoc, fcolor, bcolor);
		#endif
			break;
	}
}

//*******************************************************************************
void	text(char theChar, int xLoc, int yLoc)
{
char	theString[4];

		theString[0]	=	theChar;
		theString[1]	=	0;
		text(theString, xLoc, yLoc);
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
	text((int)data, xLoc, yLoc);
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
//*	added by Chris Sept 2009
//*******************************************************************************
void	text(char *textString, int xLoc, int yLoc, int height)
{
#ifdef _ENABLE_HERSHEY_FONTS_
	HersheyDrawCString(0, xLoc, yLoc, textString, height, 0, 1);
#else
	text(textString, xLoc, yLoc);
#endif
}

//*******************************************************************************
//*	this is here for compatiblity with processing, we ignore width
void	text(char *textString, int xLoc, int yLoc, int width, int height)
{
#ifdef _ENABLE_HERSHEY_FONTS_
	HersheyDrawCString(0, xLoc, yLoc, textString, height, 0, 1);
#else
	text(textString, xLoc, yLoc);
#endif
}


//*******************************************************************************
void	centerText(int x1, int y1, int x2, int y2, char* string)
{
int centerX;
int centerY;
int	stringWidth;
int	leftOffset;

	centerX		=	(x2 - x1) / 2;
	centerY		=	(y2 - y1) / 2;
	stringWidth	=	textWidth(string);
	leftOffset	=	centerX - (stringWidth / 2);

	switch(gCurrentFontNumber)
	{
		case 0:
			text(string, x1 + leftOffset, y1 + (centerY - 4));
			break;

	#ifdef _ENABLE_TAHOMA_FONTS_
		case 1:
			dispPutS1(string, x1 + leftOffset, y1 + (centerY - 4), fcolor, bcolor);
			break;
		
		case 2:
			dispPutS2(string, x1 + leftOffset, y1 + (centerY - 4), fcolor, bcolor);
			break;
	#endif
	
		default:
		#ifdef _ENABLE_HERSHEY_FONTS_
			text(string, x1 + leftOffset, y2 - 6);
		#else
			text(string, x1 + leftOffset, y1 + (centerY - 4));
		#endif
			break;
	}

}

//*******************************************************************************
//*	returns previous font
uint8_t	setFont(uint8_t newFontNumber)
{
uint8_t	previousFont;

	previousFont		=	gCurrentFontNumber;
	gCurrentFontNumber	=	newFontNumber;
	
	return(previousFont);
}


#pragma mark -
#pragma mark Processing font routines

//*********************************************************************
//*	these routines merge the processing functions to the Hershey fonts
//*	experienced programmers are encouraged to use the HesheyFont routines directly
//*********************************************************************

//*********************************************************************
PFont	loadFont(char *fontName)
{
PFont	myPFont;

#ifdef _ENABLE_HERSHEY_FONTS_
	myPFont	=	GetHersheyFontNumerFromName(fontName);
#else
	myPFont	=	-1;
#endif
	return(myPFont);
}

//*********************************************************************
void	textFont(PFont theFontNumber, int fontSize)
{
	gCurrentFontNumber	=	theFontNumber;
	gCurrentFontSize	=	fontSize;
}


//*********************************************************************
void	textFont(char *fontName, int fontSize)
{
PFont	myPFont;

	myPFont	=	loadFont(fontName);
	textFont(myPFont, fontSize);
}


//*******************************************************************************
//*	calculate the width of a string based on the current font
short	textWidth(uint8_t fontNumber, char *textString)
{
short	stringWidthInPixels;

	switch(fontNumber)
	{
		case 0:
			stringWidthInPixels	=	dispMeasureS(textString);
			break;

	#ifdef _ENABLE_TAHOMA_FONTS_
		case 1:
			stringWidthInPixels	=	dispMeasureS1(textString);
			break;
		
		case 2:
			stringWidthInPixels	=	dispMeasureS2(textString);
			break;
	#endif

		default:
		#ifdef _ENABLE_HERSHEY_FONTS_
			stringWidthInPixels	=	HersheyGetStringWidth(fontNumber, textString, gCurrentFontSize);
		#else
			//*	same as case 0
			stringWidthInPixels	=	dispMeasureS(textString);
		#endif
			break;
		
	}
	return(stringWidthInPixels);
}


//*******************************************************************************
//*	calculate the width of a string based on the current font
short	textWidth(char *textString)
{
short	stringWidthInPixels;

	stringWidthInPixels	=	textWidth(gCurrentFontNumber, textString);

	return(stringWidthInPixels);
}


#pragma mark -
#pragma mark Clipping

#ifdef _ENABLE_CLIPPING_
//*******************************************************************************
void	setClip(int x, int y, int width, int height)
{
	dispSetClip(x, y, width, height);
}

//*******************************************************************************
void	resetClip()
{
	dispRemoveClip();
}
#endif


#pragma mark -
#pragma mark Pen Tap routines

#ifdef _ENABLE_PEN_TAP_
//*******************************************************************************
void	updatePen(void)
{
	//* Reset 'PUBLIC' variables
	gPenTap		=	false;
	gPenAuto	=	false;

	unsigned long mil	=	millis();

	if (touch_getCursor(&gPen))
	{
		//* Pen ADC reads DOWN
		if (gPenDown == false)
		{
			//* Check for Pen Down
			if (mil >= (_mPenDown + _mWaitDown))
			{
				//* Pen is down
				_mPenDown	=	mil;
				gPenDown	=	true;
				_pPenHold	=	gPen;		//* We'll use this Point to decide if we HOLD
			}
		}
		else
		{
			if (gPenHold)
			{
				//* Pen already HELD check for auto
				if (mil >= (_mPenHold + _mWaitAuto))
				{
					_mPenHold	=	mil;
					gPenAuto	=	true;
				}
			}
			else
			{
				if (!_bNoHold)
				{
					//* Only care if we're not ignoring HOLDs
					if (dist(gPen, _pPenHold) >= _iHoldRadius)
					{
						//* We've moved too much for it to be a hold
						_bNoHold	=	true;
					}
					else
					{
						//* We're ok on point, how about time?
						if (mil >= (_mPenDown + _mWaitHold))
						{
							gPenHold	=	true;
						}
					}
				}
			}
		}
	}
	else
	{
		//* Pen ADC reads UP    
		if (gPenDown == true && mil >= (_mPenDown + _mWaitUp))
		{
			gPenHold	=	false;
			_bNoHold	=	false;
			gPenDown	=	false;
			gPenTap		=	true;
		}
	}
}
#endif



#pragma mark -
#pragma mark Graphics Display Slave routines


//*******************************************************************************
void	beginCanvas(void)
{
uint8_t	cptr;
uint8_t	buf[32];
char	responseString[16];

	/* Start Serial, if not started already */
	if (!gServerEnb)
	{
	#ifdef _TOUCH_SLIDE_
		//*	Dec 29,	2008	<?> Changed from serialBegin(9600); to beginSerial(9600);
		beginSerial(9600);		//*	version 0012LW
	#else
		serialBegin(9600);		//*	version 0012
	#endif
		gServerEnb	=	true;
	}

	//*	Get our command string
	getstring(buf, sizeof(buf));
	cptr	=	1;

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
			setBrightness(buf[cptr+1]);
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
			drawstring(	(buf[cptr+2] << 8) + buf[cptr+1],
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
			responseString[0]	=	'|';
			responseString[1]	=	kSubP_GET_SCREEN_SIZE;
		//*	Dec 31,	2008	<MLS> When Chris did v0012LW, the screen structure got lost
		#if 1
			responseString[2]	=	(unsigned char)kSCREEN_X_size;
			responseString[3]	=	(unsigned char)(kSCREEN_X_size >> 8);
			responseString[4]	=	(unsigned char)kSCREEN_Y_size;
			responseString[5]	=	(unsigned char)(kSCREEN_Y_size >> 8);
		#else
			responseString[2]	=	(unsigned char)screen.width;
			responseString[3]	=	(unsigned char)(screen.width >> 8);
			responseString[4]	=	(unsigned char)screen.height;
			responseString[5]	=	(unsigned char)(screen.height >> 8);
		#endif
			responseString[6]	=	(unsigned char)0;
			responseString[7]	=	(unsigned char)0;
			responseString[8]	=	0;
			responseString[9]	=	0;
			responseString[10]	=	0;

			sendStringL(responseString, 11);
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

//*********************************************************************
/*
Bezier Curves

Bezier curves (pronounced Òbez-yeahÓ, after their inventor, a French mathematician) are well
suited to graphics applications on the Macintosh for a number of reasons. First, theyÕre
simple to describe. A curve is a function of four points. Second, the curve is efficient to
calculate. From a precomputed table, the segments of the curve can be produced using only
fixed-point multiplication. No trig, no messy quadratics, and no inSANEity. Third, and, to 
some, the most important, the Bezier curve is directly supported by the PostScript curve and
 curveto operators, and is one of the components of PostScriptÕs outlined fonts. 
 The Bezier curve is also one of the principle drawing elements of Adobe Illustratorª. 
 (Recently, theyÕve shown up in a number of other places.)

Bezier curves have some interesting properties. Unlike some other classes of curves, 
they can fold over on themselves. They can also be joined together to form smooth 
(continuous) shapes. Figure 1 shows a few Bezier curves, including two that are joined to 
form a smooth shape.

The Gruesome Details

The description of Bezier curves below is going to get a bit technical. If youÕre not 
comfortable with the math, you can trust that the algorithm works, and skip ahead to the 
implementation. However, if youÕre curious about how the curves work and how to optimize 
their implementation, or just donÕt trust using code that you donÕt understand, read on.

The Bezier curve is a parametric function of four points; two endpoints and two ÒcontrolÓ 
points. The curve connects the endpoints, but doesnÕt necessarily touch the control points. 
The general form Bezier equation, which describes each point on the curve as a function of 
time, is:

where P1 and P4 are the endpoints, P2 and P3 are the control points, and the wnÕs are 
weighting functions, which blend the four points to produce the curve. (The weights are 
applied to the h and v components of each point independently.) The single parameter t 
represents time, and varies from 0 to 1. The full form of the Bezier curve is:

We know that the curve touches each endpoint, so it isnÕt too surprising that at t=0 the 
first weighting function is 1 and all others are 0 (i.e., the initial point on the curve 
is the first endpoint). Likewise, at t=1, the fourth weighting function is 1 and the rest 
are 0. However, itÕs what happens between 0 and 1 thatÕs really interesting. A quick 
side-trip into calculus to take some first derivatives tells us that the second weighting 
function is maximized (has its greatest impact on the curve) at t=1/3, and the third weight 
is maximized at t=2/3. But the clever part--the bit that the graphics books donÕt bother to 
mention--run the curve backwards by solving the equation for 1-t, and you find 
that w1(t)=w4(1-t) and w2(t)=w3(1-t). As weÕll see below, this symmetry halves the
 effort needed to compute values for the weights.

Figure 1. Some Beizer Curves and Shapes

Implementing Bezier Curves

One strategy for implementing Bezier curves is to divide the curve into a fixed number 
of segments and then to pre-compute the values of the weighting functions for each of 
the segments. The greater the number of segments, the smoother the curve. (IÕve found 
that 16 works well for display purposes, but 32 is better for hardcopy.) Computing any 
given curve becomes a simple matter of using the four points and the precomputed weights 
to produce the end-points of the curve segments. Fixed-point math yields reasonable 
accuracy, and is a hands down winner over SANE on the older (pre-Mac II) Macs, so weÕll 
use it.

We can optimize the process a bit. The curve touches each endpoint, so we can assume 
weights of 0 or 1 and neednÕt compute weights for these points. Another optimization 
saves both time and space. By taking advantage of the symmetric nature of the Bezier 
equation, we can compute arrays of values for the first two of the weighting functions, 
and obtain values for the other two weights by indexing backwards into the arrays.

Drawing the curve, given the endpoints of the segments, is the duty of QuickDraw 
(or of PostScript, if youÕre really hacking).

The listing below shows a reasonably efficient implementation of Bezier curves in 
Lightspeed Cª. A few reminders about fixed-point math: an integer times a fixed-point 
number yields a fixed-point number, and a fixed by fixed multiplication uses a trap. 
The storage requirement for the algorithm, assuming 16 segments, (32 fixed-point values), 
is around 32*4*4, or 512 bytes. The algorithm computes all of the segments before drawing 
them so that the drawing can be done at full speed. (Having all of the segments around at 
one time can be useful for other reasons.)

More Fun With Curves

Given an implementation for Bezier curves, there are some neat things that fall out 
for almost free. Drawing a set of joined curves within an OpenPoly/ClosePoly or an 
OpenRgn/CloseRgn envelope yields an object that can be filled with a pattern. 
(Shades of popular illustration packages?) For that matter, lines, arcs, wedges, 
and Bezier curves can be joined to produce complicated shapes, such as outlined fonts. 
Given the direct mapping to PostScriptÕs curve and curveto operators, Bezier curves are 
a natural for taking better advantage of the LaserWriter.

As mentioned above, Bezier curves can be joined smoothly to produce more complicated 
shapes (see figure 1). The catch is that the point at which two curves are joined, and 
the adjacent control points, must be colinear (i.e., the three points must lay on a line). 
If you take a close look at Adobe IllustratorÕs drawing tool, youÕll see what this means.

One nonobvious use of Bezier curves is in animation. The endpoints of the segments can 
be used as anchor-points for redrawing an object, giving it the effect of moving smoothly 
along the curve. One backgammon program that IÕve seen moves the tiles along invisible Bezier 
curves, and the effect is very impressive. For animation, you would probably want to vary 
the number of segments. Fortunately, the algorithm below is easily rewritten to produce the 
nth segment of an m segment curve given the the end and control points.

Further Optimizations

If youÕre really tight on space or pressed for speed, there are a few things that you 
can do to tighten up the algorithm. A bit of code space (and a negligible amount of time) 
can be preserved by eliminating the setup code in favor of statically initializing the 
weight arrays with precomputed constant values.

*/
//*********************************************************************
//*********************************************************************
//*	http://www.mactech.com/articles/mactech/Vol.05/05.01/BezierCurve/
//*********************************************************************
//*
//*	Bezier  --  Support for Bezier curves
//*	Herein reside support routines for drawing Bezier curves.
//*	Copyright (C) 1987, 1988 David W. Smith
//*	Submitted to MacTutor for their source-disk.
//*	MacTutor Magazine
//*	Volume Number:		5
//*	Issue Number:		1
//*	Column Tag:		C Workshop
//*
//*	http://www.mactech.com/articles/mactech/Vol.05/05.01/BezierCurve/
//*********************************************************************
//*	The greater the number of curve segments, the smoother the curve, 
//*	and the longer it takes to generate and draw.  The number below was pulled 
//*	out of a hat, and seems to work o.k.
//*********************************************************************
//*	adapted by Mark Sproul, Jan 2009 for use in the SubPGrapnics lib
//*	changed to use floats instead of Fixed data types
//*	the code origninally had 3 separate routines, one to setup the weights
//*	one to compute the points, and one to draw the code. This used 128 bytes
//*	of static memory table for the weights. In order to save memory foot
//*	print, these tables are computed each time. It takes a bit longer but
//*	considering the amount of memory, its a price we can afford
//*********************************************************************
#define	kSEGMENTS	16
#define w1(s)	weight1[s]
#define w2(s)	weight2[s]
#define w3(s)	weight2[kSEGMENTS - s]
#define w4(s)	weight1[kSEGMENTS - s]

//*********************************************************************
//*
//*	BezierCurve  --  Draw a Bezier Curve
//*	Draw a curve with the given endpoints (p1, p4), and the given 
//*	control points (p2, p3).
//*	Note that we make no assumptions about pen or pen mode.
//*
//*********************************************************************
void	BezierCurve(Point p1, Point p2, Point p3, Point p4)
{
int		ss;
float	tt;
float	weight1[kSEGMENTS + 1];
float	weight2[kSEGMENTS + 1];
Point	segment[kSEGMENTS + 1];


//*	debug code
//	line(p1.h, p1.v, p4.h, p4.v);
//	drawcircle(p2.h, p2.v, 4);
//	drawcircle(p3.h, p3.v, 4);


	//*********************************************************************
	//*	Compute the weights for the Bezier function.
	//*********************************************************************
	weight1[0]	=	1.0;
	weight2[0]	=	0.0;
	for (ss	= 1 ; ss < kSEGMENTS ; ++ss)
	{
		tt			=	(1.0 * ss) / kSEGMENTS;
		weight1[ss]	=	(1.0 - tt) * ((1.0 - tt) * (1.0 - tt));
		weight2[ss]	=	3 * (tt * ((tt - 1.0) * (tt - 1.0)));
	}
	weight1[kSEGMENTS]	=	0.0;
	weight2[kSEGMENTS]	=	0.0;


	//*********************************************************************
	//*
	//*	computeSegments  --  compute segments for the Bezier curve
	//*	Compute the segments along the curve.
	//*	The curve touches the endpoints, so donÕt bother to compute them.
	//*
	//*********************************************************************
	segment[0]	=	p1;
	for (ss = 1 ; ss < kSEGMENTS ; ++ss)
	{
		segment[ss].v	=	round(w1(ss) * p1.v + w2(ss) * p2.v + w3(ss) * p3.v + w4(ss) * p4.v);
		segment[ss].h	=	round(w1(ss) * p1.h + w2(ss) * p2.h + w3(ss) * p3.h + w4(ss) * p4.h);
	}
	segment[kSEGMENTS]	=	p4;

	//*********************************************************************
	//*	now do the drawing itself
	//*********************************************************************
	for (ss = 0 ; ss < kSEGMENTS ; ++ss)
	{
		if ((ss == 0) || (segment[ss].h != segment[ss - 1].h) || (segment[ss].v != segment[ss - 1].v ))
		{
			line(segment[ss].h, segment[ss].v, segment[ss + 1].h, segment[ss + 1].v);
		}
	}
}


//*******************************************************************************
//*	Processing version
//*******************************************************************************
void	bezier(int x1, int y1, int cx1, int cy1, int cx2, int cy2, int x2, int y2)
{
Point	p1, p2, p3, p4;

	p1.h	=	x1;
	p1.v	=	y1;
	p2.h	=	cx1;
	p2.v	=	cy1;
	p3.h	=	cx2;
	p3.v	=	cy2;
	p4.h	=	x2;
	p4.v	=	y2;
	BezierCurve(p1, p2, p3, p4);
}


//*******************************************************************************
void	arc(int xCenter, int yCenter, int xDiameter, int yDiameter, float startAngle, float stopAngle)
{
int		xx, yy;
int		prevxx, prevyy;
float	myAngle;
float	xRadius, yRadius;
float	stepAngle;
float	myStartAngle, myStopAngle;
boolean	drawSegment;


	xRadius		=	xDiameter / 2;
	yRadius		=	yDiameter / 2;
	stepAngle	=	PI / 50;
	if (startAngle < stopAngle)
	{
		myStartAngle	=	startAngle;
		myStopAngle		=	stopAngle;
	}
	else
	{
		//*	reverse the order so we are going in a POSITIVE direction
		myStartAngle	=	stopAngle;
		myStopAngle		=	startAngle;
	}
	myAngle		=	myStartAngle;



	if (gFillEnb)
	{
	uint8_t		prevStroke;
	uint8_t		prevStrokeWeight;

		//* Save the stroke state
		prevStroke			=	gStrokeEnb;
		prevStrokeWeight	=	gStrokeWeightVal;

		//*	Disable stroke
		noStroke();

		//*	go thru and fill triangles

		drawSegment	=	false;
		while (myAngle <= stopAngle)
		{
			xx		=	xCenter + (xRadius * cos(myAngle));
			yy		=	yCenter + (yRadius * sin(myAngle));
			//*	the 1st time thru we dont want to draw a line
			if (drawSegment)
			{
				triangle(xCenter, yCenter, prevxx, prevyy, xx, yy);
			}
			drawSegment	=	true;
			myAngle	+=	stepAngle;

			prevxx	=	xx;
			prevyy	=	yy;
		}

		//* Restore Stroke values
		gStrokeEnb			=	prevStroke;
		gStrokeWeightVal	=	prevStrokeWeight;
	}


	myAngle		=	myStartAngle;
	drawSegment	=	false;
	while (myAngle <= stopAngle)
	{
		xx		=	xCenter + (xRadius * cos(myAngle));
		yy		=	yCenter + (yRadius * sin(myAngle));
		//*	the 1st time thru we dont want to draw a line
		if (drawSegment)
		{
			line(prevxx, prevyy, xx, yy);
		}
		drawSegment	=	true;
		myAngle	+=	stepAngle;

		prevxx	=	xx;
		prevyy	=	yy;
	}
}


#pragma mark -
//*******************************************************************************
//*	these routines are Macintosh QuickDraw style
//*******************************************************************************
#pragma mark Mac Quickdraw Style routines
#ifdef _SUBP_OPTION_QUICKDRAW_

//*	this is the CURRENT pen location
short	gQDcurrentX	=	0;
short	gQDcurrentY	=	0;

//*******************************************************************************
void	MoveTo(short xLoc, short yLoc)
{
	gQDcurrentX	=	xLoc;
	gQDcurrentY	=	yLoc;
}

//*******************************************************************************
void	LineTo(short xLoc, short yLoc)
{
	if ((xLoc != gQDcurrentX) || (yLoc != gQDcurrentY))
	{
		//*	only draw a line if we actually moved anyplace
		line(gQDcurrentX, gQDcurrentY, xLoc, yLoc);
	}
	gQDcurrentX	=	xLoc;
	gQDcurrentY	=	yLoc;
}

//*******************************************************************************
void	Move(short deltaX, short deltaY)
{
	gQDcurrentX	+=	deltaX;
	gQDcurrentY	+=	deltaY;
}

//*******************************************************************************
void	Line(short deltaX, short deltaY)
{
	if ((deltaX != 0) || (deltaY != 0))
	{
		line(gQDcurrentX, gQDcurrentY, gQDcurrentX + deltaX, gQDcurrentY + deltaY);
		gQDcurrentX	+=	deltaX;
		gQDcurrentY	+=	deltaY;
	}
}


//*******************************************************************************
void	SetRGBcolor(COLOR *theColor, unsigned char redValue, unsigned char greenValue, unsigned char blueValue)
{
	theColor->red	=	redValue;
	theColor->green	=	greenValue;
	theColor->blue	=	blueValue;
}

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
void	EraseRect(RECT *theRect)
{
int	myWidth, myHeight;

	myWidth		=	theRect->right - theRect->left;
	myHeight	=	theRect->bottom - theRect->top;

	dispColor(bcolor);
	dispRectangle(theRect->left, theRect->top, myWidth, myHeight);

}

//*******************************************************************************
void	FillRect(RECT *theRect)
{
int	myWidth, myHeight;

	myWidth		=	theRect->right - theRect->left;
	myHeight	=	theRect->bottom - theRect->top;

	dispColor(fcolor);
	dispRectangle(theRect->left, theRect->top, myWidth, myHeight);
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
boolean	pointInRect(int pX, int pY, int rLeft, int rTop, int rRight, int rBottom)
{
	if (pX >= rLeft && pX <= rRight && pY >= rTop && pY <= rBottom)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//*******************************************************************************
void	InsetRect(RECT *theRect, short insetX, short insetY)
{
	theRect->left		+=	insetX;
	theRect->right		-=	insetX;
	theRect->top		+=	insetY;
	theRect->bottom		-=	insetY;
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

//*******************************************************************************
void	DrawCString(short xloc, short yloc, char *theCstr)
{
	drawstring(xloc, yloc, theCstr);
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
//*	Feb 10,	2009	<MLS> PlotIcon now uses program memory
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
	xSize			=	(pgm_read_byte_near(iconDef + cc++) & 0x0ff);
	ySize			=	(pgm_read_byte_near(iconDef + cc++) & 0x0ff);
	totalBytes		=	(xSize * ySize) + 2;
	xx				=	0;
	yy				=	0;
	previousByte	=	0xff;
	while ((cc < totalBytes) && (yy < ySize))
	{
		currentByte	=	(pgm_read_byte_near(iconDef + cc++) & 0x00ff);
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
			currentByte		=	(pgm_read_byte_near(iconDef + cc++) & 0x00ff);
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

//*******************************************************************************
void	EraseScreen(void)
{
	dispColor(bcolor);
	dispClearScreen();
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
prog_uchar		g7SegmentDisplay[]	PROGMEM =	{//	/76543210
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
	EraseRect(&digitRect);

	if (segmentLength > 8)
	{
		endOffset	=	2;
	}
	else
	{
		endOffset	=	1;
	}

	myBinaryValue	=	pgm_read_byte_near(g7SegmentDisplay + (decimalDigit & 0x01f));

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

	EraseRect(&theButton->buttonRect);
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

	bcolor		=	fill_color;
	fcolor		=	outline_color;
	gFillEnb	=	true;
	drawrect(x1, y1, (x2 - x1), (y2 - y1));
}

//*******************************************************************************
void	lcd_pixel(int x, int y, COLOR pixel_color)
{
	dispColor(pixel_color);
	dispPixel(x, y);
}

//*******************************************************************************
void	lcd_line(int x1, int y1, int x2, int y2, COLOR pixel_color)
{
	stroke(	pixel_color.red,
			pixel_color.green,
			pixel_color.blue);
	line(x1,y1,x2,y2);
}

//*******************************************************************************
void	lcd_circle(int x1, int y1, int radius, COLOR outline_color, COLOR fill_color)
{
	bcolor		=	fill_color;
	fcolor		=	outline_color;
	gFillEnb	=	true;
	ellipse(x1,y1,radius,radius);
}

//*******************************************************************************
void	lcd_puts(char * string, int x, int y, COLOR outline_color, COLOR fill_color)
{
	bcolor	=	fill_color;
	fcolor	=	outline_color;
	gFillEnb	=	true;
	text(string,x,y);
}

#if defined(_TOUCH_SLIDE_)
//*******************************************************************************
void	lcd_clearScreen(COLOR fill_color)
{
	background(	fill_color.red,
				fill_color.green,
				fill_color.blue);
}

//*******************************************************************************
void lcd_setBrightness(uint8_t bright) 
{
	setBrightness(bright);
}
#endif


#endif	//	_SUPPORT_OLD_LCD_ROUTINE_NAMES_

