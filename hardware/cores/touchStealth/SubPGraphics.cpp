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
//* <TWH>	=	Thom Holtquist, ctsfutures.com
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

#define	incx() xx++, dxt += d2xt, t += dxt
#define	incy() yy--, dyt += d2yt, t += dyt

#define MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))


//*******************************************************************************
//*	GLOBAL CONSTANTS AND VARIABLES
//*******************************************************************************
const int	SMOOTHING_OFF	=	0;
const int	SMOOTHING_QUICK	=	1;
const int	SMOOTHING_HIGH	=	2;

//* INPUT VARIABLES
COLOR	fcolor		=	{ 255, 255, 255 };
COLOR	bcolor		=	{ 0, 0, 0 };

int		smoothingMode	=	SMOOTHING_OFF;

int		brightness	=	BRIGHT_MAX;
POINT	gMostRecentTouchPt;
int		trix1, triy1, trix2, triy2, trix3, triy3;
	
int		mouseX			=	screen.width / 2;
int		mouseY			=	screen.height / 2;
int		width			=	kSCREEN_X_size;		//screen.width;
int		height			=	kSCREEN_Y_size;		//screen.height;
uint8_t	cptr;
uint8_t	buf[32];
uint8_t	serverEnb		=	false;
char	command[16];
int		strokeWeightVal	=	1;

boolean	penDown			=	false;
boolean	penHold			=	false;
boolean penAuto			=	false;
boolean penTap			=	false;
POINT	pen;

//*******************************************************************************
//* LOCAL FUNCTION PROTOTYPES
//*******************************************************************************
void	dispFillEllipse(int xCenter, int yCenter, int xRadius, int yRadius);
void	dispOutlineEllipse(unsigned int xc,unsigned int yc,unsigned int a,unsigned int b);
void	dispWuLine( int X0, int Y0, int X1, int Y1, COLOR col);
void	dispQSLine(int x1, int y1, int x2, int y2, COLOR col);
void	dispHSLine(int x1, int y1, int x2, int y2, COLOR col);
int		point_triangle_intersection(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3);
int		orientation (long x1, long y1, long x2, long y2, long px, long py);
void	getstring(uint8_t *buf, int bufsize);
void	sendStringL( char *buf, int size);
void	drawNumber(unsigned long n, uint8_t base, int xLoc, int yLoc);
int		alphaBlendVal(float bkg, float fore, float a);
float	getPointSpacing(int radius);
void	dispEllipse(int x, int y, int xRadius, int yRadius, COLOR col);
void	dispSMEllipse(int x, int y, int xRadius, int yRadius, COLOR col);
//*******************************************************************************
//*	LOCAL FUNCTIONS
//*******************************************************************************
void	dispWuLine( int X0, int Y0, int X1, int Y1, COLOR col)
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

	/* set dispColor to fcolor for backwards compatibility */
	dispColor(fcolor);
}
//*******************************************************************************
void dispQSLine(int x1, int y1, int x2, int y2, COLOR fgcol, COLOR bgcol)
{
  float  xScope;
  float  yScope;
  float  dirX;
  float  dirY;
  float  linearDeviance;
  float  counter;
  float  aaStrength;
  float  endpointIntensity;
  float  halfIntensity = 50;
  
  xScope = x2 - x1;
  yScope = y2 - y1;
  
  if (xScope < 0)
  {
    xScope = abs(xScope);
    dirX = -1;
  }
  else
  {
    dirX = 1;
  }
  
  if (yScope < 0)
  {
    yScope = abs(yScope);
    dirY = -1;
  }
  else
  {
    dirY = 1;
  }  
  
  //* No point in drawing a 0 length line
  if (xScope + yScope == 0) return;
  
  if (xScope > yScope)
  {
    endpointIntensity = (33 * yScope) / xScope;
    setForecolor(alphaBlend(fgcol,bgcol,endpointIntensity));
    setPixel(x1 - dirX, y1 - dirY);
    setPixel(x2 + dirX, y2 + dirY);
    setForecolor(alphaBlend(fgcol,bgcol,halfIntensity));
    setPixel(x1 - dirX, y1);
    setPixel(x2 + dirX, y2);
    
    setForecolor(fgcol);
    linearDeviance = xScope / 2;
    for (counter = 0; counter < xScope; counter++)
	{
      
      setForecolor(fgcol);
      setPixel(x1,y1);
      aaStrength = (linearDeviance * 100) / xScope;
      
      setForecolor(alphaBlend(fgcol,bgcol,100-aaStrength));
      setPixel(x1,y1-dirY);
      
      setForecolor(alphaBlend(fgcol,bgcol,aaStrength));
      setPixel(x1,y1+dirY);
      
      linearDeviance = (linearDeviance + yScope);
      if (linearDeviance >= xScope)
	  {
        linearDeviance = linearDeviance - xScope;
        y1 = y1 + dirY;
      }
      x1 = x1 + dirX;
    }
  }
  else
  {
    endpointIntensity = (33 * xScope) / yScope;
    setForecolor(alphaBlend(fgcol,bgcol,endpointIntensity));
    setPixel(x1-dirX,y1-dirY);
    setPixel(x2+dirX,y2+dirY);
    setForecolor(alphaBlend(fgcol,bgcol,halfIntensity));
    setPixel(x1,y1-dirY);
    setPixel(x2,y2+dirY);
    
    linearDeviance = yScope / 2;
    for (counter = 0; counter < yScope; counter++)
    {
      setForecolor(fgcol);
      setPixel(x1,y1);
      
      aaStrength = (linearDeviance * 100) / yScope;

      setForecolor(alphaBlend(fgcol,bgcol,100-aaStrength));
      setPixel(x1 - dirX,y1);
      
      setForecolor(alphaBlend(fgcol,bgcol,aaStrength));
      setPixel(x1- dirX,y1);

      linearDeviance = (linearDeviance + xScope);
      if (linearDeviance >= yScope)
	  {
        linearDeviance = linearDeviance - yScope;
        x1 = x1 + dirX;
      }
      y1 = y1 + dirY;

    }
  } 
}
//*******************************************************************************
void dispHSLine(int x1, int y1, int x2, int y2, COLOR col) {
  long  xScope;
  long  yScope;
  long  dirX;
  long  dirY;
  long  linearDeviance;
  long  counter;
  long  aaStrength;
  long  endpointIntensity;
  long  halfIntensity = 50;
  
  xScope = x2 - x1;
  yScope = y2 - y1;
  
  if (xScope < 0) {
    xScope = abs(xScope);
    dirX = -1;
  } else {
    dirX = 1;
  }
  
  if (yScope < 0) {
    yScope = abs(yScope);
    dirY = -1;
  } else {
    dirY = 1;
  }  
  
  //* No point in drawing a 0 length line
  if (xScope + yScope == 0) return;
  
  if (xScope > yScope) {
    endpointIntensity = (85 * yScope) / xScope;
    
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
    
    linearDeviance = xScope / 2;
    for (counter = 0; counter < xScope; counter++) {
      
      setForecolor(col);
      setPixel(x1,y1);
      aaStrength = (linearDeviance * 100) / xScope;
      
      getPixel(&bcolor,x1,y1-dirY);
      setForecolor(alphaBlend(col,bcolor,100-aaStrength));
      setPixel(x1,y1-dirY);
      
      getPixel(&bcolor,x1,y1+dirY);
      setForecolor(alphaBlend(col,bcolor,aaStrength));
      setPixel(x1,y1+dirY);
      
      linearDeviance = (linearDeviance + yScope);
      if (linearDeviance >= xScope) {
        linearDeviance = linearDeviance - xScope;
        y1 = y1 + dirY;
      }
      x1 = x1 + dirX;
    }
  } else {
    endpointIntensity = (85 * xScope) / yScope;
    
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
    
    linearDeviance = yScope / 2;
    for (counter = 0; counter < yScope; counter++)
    {
      setForecolor(col);
      setPixel(x1,y1);
      
      aaStrength = (linearDeviance * 100) / yScope;

      getPixel(&bcolor,x2 + dirY, y2);
      setForecolor(alphaBlend(col,bcolor,100-aaStrength));
      setPixel(x1 - dirX,y1);

      getPixel(&bcolor,x2 + dirY, y2);     
      setForecolor(alphaBlend(col,bcolor,aaStrength));
      setPixel(x1- dirX,y1);

      linearDeviance = (linearDeviance + xScope);
      if (linearDeviance >= yScope)
	  {
        linearDeviance = linearDeviance - yScope;
        x1 = x1 + dirX;
      }
      y1 = y1 + dirY;
    }
  }
}
//*******************************************************************************
void dispEllipse(int x, int y, int xRadius, int yRadius, COLOR col)
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
void dispSMEllipse(int x, int y, int xRadius, int yRadius, COLOR col)
{
  float    radius;
  int    quadrant;
  int    c1;
  float    alpha;
  float    alphaI;
  float  pointSpacing;
  float  angle;
  float  halfPi = 1.570796;
  float  x2;
  float  y2;
  float  aX;
  float  aY;
  float  bX;
  float  bY;
  float  rX1;
  float  rX2;
  float  rY1;
  float  rY2;
  float  xp5;
  float  L1;
  float  L2;
  float  L3;
  float  L4;
  float  savX[4];
  float  savY[4];
  float  savA[4];
  float  x4;
  float  y4;
  
  if (xRadius > yRadius)
  {
    radius = xRadius;
  }
  else
  {
    radius = yRadius;
  }
  
  pointSpacing = getPointSpacing(radius);
  
  for (angle = 0; angle < halfPi; angle = angle + pointSpacing)
  {
    x2 = (xRadius * cos(angle)) + 0.001;
    y2 = (yRadius * sin(angle)) + 0.001;
    for (quadrant = 0; quadrant < 4; quadrant++)
    {
      switch(quadrant)
      {
        case 0: //* 0-90 degrees
          aX = x2 + x - 0.5;
          aY = -y2 + y - 0.5;
          break;
        case 1: //* 90-180
          aX = x2 + x - 0.5;
          aY = y2 + y - 0.5;
          break;
        case 2: // 180-270
          aX = -x2 + x - 0.5;
          aY = y2 + y - 0.5;
          break;
        case 3: //270-360
          aX = -x2 + x - 0.5;
          aY = -y2 + y - 0.5;
          break; 
      }
      
      bX = aX + 1;
      bY = aY + 1;
      
      rX1 = aX;
      rX2 = rX1 + 1;
      
      xp5 = rX1 + 0.5;
      
      rY1 = aY;
      rY2 = bY;
      
      L1 = rY1 + 0.5 - aY;
      L2 = 256 * (xp5 - aX) - xp5 + aX;
      L3 = 255 - L2;
      L4 = bY - rY2 + 0.5;
      
      savX[0] = rX1;
      savY[0] = rY1;
      
      savX[1] = rX2;
      savY[1] = rY1;
      
      savX[2] = rX1;
      savY[2] = rY2;
      
      savX[3] = rX2;
      savY[3] = rY2;
      
      savA[0] = L1 * L2;
      savA[1] = L1 * L3;
      savA[2] = L4 * L2;
      savA[3] = L4 * L3;
      
      for (c1 = 0; c1 < 4; c1++)
      {
        x4 = savX[c1];
        y4 = savY[c1];
		if (smoothingMode == 2) getPixel(&bcolor,x4,y4);
        alpha = (savA[c1] / 255) * 100;
        setForecolor(alphaBlend(col,bcolor,savA[c1]));
        setPixel(x4,y4);
      }
    }
  }
}
//*******************************************************************************
float getPointSpacing(int radius)
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

unsigned long  _mPenDown    = 0;      // MS at which Pen was last down
unsigned long  _mPenHold    = 0;      // MS at which Pen started HOLD state
int     _mWaitDown   = 10;     // MS to wait before calling PenDown
int     _mWaitUp     = 100;    // MS to wait before calling PenUp
int     _mWaitHold   = 1500;   // MS to wait before calling PenHold
int     _mWaitAuto   = 100;    // MS to wait AFTER PenHold for PenAuto
int     _iHoldRadius = 20;     // Distance allowed to move & still trigger HOLD
boolean _bNoHold     = false;  // Ignore HOLD for this down?
POINT   _pPenHold;             // Point pen is HELD at
//*******************************************************************************
int alphaBlendVal(float fgcolor, float bgcolor, float a) {
	if (a >= 98) return fgcolor;
	float p1 = (100 - a) / 100;
	float p2 = a / 100;
	return (bgcolor * p1) + (fgcolor * p2);
	//return bgcolor - ((bgcolor - fgcolor) * (a / 100));
}
//*******************************************************************************
COLOR alphaBlend(COLOR forecolor, COLOR backcolor, int opacity) {
  COLOR reslt;
  reslt.red = alphaBlendVal(forecolor.red, backcolor.red, opacity);
  reslt.green = alphaBlendVal(forecolor.green, backcolor.green, opacity);
  reslt.blue = alphaBlendVal(forecolor.blue, backcolor.blue, opacity);
  return reslt;
}
//*******************************************************************************
void updatePen() {
  //* Reset 'PUBLIC' variables
  penTap	= false;
  penAuto	= false;

  unsigned long mil = millis();
  
  if (touch_getCursor(&pen)) {
    //* Pen ADC reads DOWN
    if (penDown == false) {
      //* Check for Pen Down
      if (mil >= (_mPenDown + _mWaitDown)) {
        //* Pen is down
        _mPenDown	= mil;
        penDown		= true;
        _pPenHold	= pen; //* We'll use this Point to decide if we HOLD
      }
    } else {
      if (penHold) {
        //* Pen already HELD check for auto
        if (mil >= (_mPenHold + _mWaitAuto)) {
         _mPenHold = mil;
         penAuto	= true; 
        }
      } else {
        if (!_bNoHold) {
          //* Only care if we're not ignoring HOLDs
          if (dist(pen,_pPenHold) >= _iHoldRadius) {
            //* We've moved too much for it to be a hold
            _bNoHold = true;
          } else {
            //* We're ok on point, how about time?
            if (mil >= (_mPenDown + _mWaitHold)) {
              penHold	= true;
            }
          }
        }
      }
    }
  } else {
    //* Pen ADC reads UP    
    if (penDown == true && mil >= (_mPenDown + _mWaitUp)) {
      penHold	= false;
      _bNoHold  = false;
      penDown	= false;
	  penTap	= true;
    }
  }
}

float dist(POINT p1, POINT p2) {
	return dist(p1.x, p1.y, p2.x, p2.y);
}

//*******************************************************************************
void clear(void) {
	clear(bcolor);
}
//*******************************************************************************
void clear(int r, int g, int b) {
	COLOR col = {r,g,b};
	clear(col);
}
//*******************************************************************************
void clear(COLOR col) {
	bcolor = col;
	fillRect(-1, -1, width + 1, height + 1, col);
}
//*******************************************************************************
void drawEllipse(int x, int y, int xRadius, int yRadius)
{
	drawEllipse(x,y,xRadius,yRadius,fcolor);
}
//*******************************************************************************
void drawEllipse(int x, int y, int xRadius, int yRadius, COLOR col)
{
	if (smoothingMode == 0)
	{
		dispEllipse(x,y,xRadius,yRadius,col);
	}
	else
	{
		dispSMEllipse(x,y,xRadius,yRadius,col);
	}
}
//*******************************************************************************
void drawRect(int x, int y, int width, int height)
{
	drawRect(x,y,width,height,fcolor);
}
//*******************************************************************************
void drawRect(int x, int y, int width, int height, COLOR col)
{
	if ((width > 0) && (height > 0))
	{
		dispColor(col);
		if (strokeWeightVal > 1)
		{
		//stroke
		int		left	=	x - 1;
		int		top		=	y - 1;
		int		right	=	x + width;
		int		bottom	=	y + height;

			for(int i=0; i<strokeWeightVal; i++)
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
			dispWuLine(x, y, x + width, y, col);				//*	top line
			dispWuLine(x, y + height, x + width, y + height, col);		//*	bottom line
			dispWuLine(x, y, x, y + height, col);		//*	left line
			dispWuLine(x + width, y, x + width, y + height, col);		//*	right line
		}

	}

}
//*******************************************************************************
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	drawTriangle(x1,y1,x2,y2,x3,y3,fcolor);
}
//*******************************************************************************
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, COLOR col)
{
	switch(smoothingMode) {
	case 0:
		dispWuLine (x1, y1, x2, y2, col);
		dispWuLine (x2, y2, x3, y3, col);
		dispWuLine (x3, y3, x1, y1, col);
		break;
	case 1:
		dispQSLine (x1, y1, x2, y2, col, bcolor);
		dispQSLine (x2, y2, x3, y3, col, bcolor);
		dispQSLine (x3, y3, x1, y1, col, bcolor);
		break;
	case 2:
		dispHSLine (x1, y1, x2, y2, col);
		dispHSLine (x2, y2, x3, y3, col);
		dispHSLine (x3, y3, x1, y1, col);
		break;
	}
}
//*******************************************************************************
void drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	drawQuad(x1,y1,x2,y2,x3,y3,x4,y4,fcolor);
}
//*******************************************************************************
void drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, COLOR col)
{

	switch(smoothingMode) {
	case 0:
		dispWuLine ( x1, y1, x2, y2, col);
		dispWuLine ( x2, y2, x3, y3, col);
		dispWuLine ( x3, y3, x4, y4, col);
		dispWuLine ( x4, y4, x1, y1, col);
		break;
	case 1:
		dispQSLine ( x1, y1, x2, y2, col, bcolor);
		dispQSLine ( x2, y2, x3, y3, col, bcolor);
		dispQSLine ( x3, y3, x4, y4, col, bcolor);
		dispQSLine ( x4, y4, x1, y1, col, bcolor);
		break;
	case 2:
		dispHSLine ( x1, y1, x2, y2, col);
		dispHSLine ( x2, y2, x3, y3, col);
		dispHSLine ( x3, y3, x4, y4, col);
		dispHSLine ( x4, y4, x1, y1, col);
	break;
	}

}
//*******************************************************************************
float dist(float x1, float y1, float x2, float y2)
{
	int	xx	=	(x2 - x1);
	int	yy	=	(y2 - y1);
	return sqrt((xx * xx)+(yy * yy));
}
//*******************************************************************************
void	fadeIn( int time)
{

	uint8_t ii;
	float dlx = time / BRIGHT_MAX;

	for (ii = 0; ii<BRIGHT_MAX - 1; ii++) 
	{
	#if defined(_TOUCH_STEALTH_) || !defined(_VERSION_0012LW_)
		dispBrightness((uint8_t)ii);
	#else
		oled_brightness((uint8_t)ii);
	#endif
		delay(dlx);
	}
}
//*******************************************************************************
void	fadeOut( int time)
{
	if (brightness == 0) return;

	uint8_t ii;
	float dlx = time / brightness;

	for (ii = brightness; ii > 0; ii--)
	{
		#if defined(_TOUCH_STEALTH_) || !defined(_VERSION_0012LW_)
			dispBrightness((uint8_t)ii);
		#else
			oled_brightness((uint8_t)ii);
		#endif
		delay(dlx);
	}

}
//*******************************************************************************
void	setBackcolor(COLOR bkgColor)
{
	bcolor = bkgColor;
}
//*******************************************************************************
void	setBackcolor(uint8_t redVal, uint8_t greenVal, uint8_t blueVal)
{
	bcolor.red		=	redVal;
	bcolor.green	=	greenVal;
	bcolor.blue		=	blueVal;
}
//*******************************************************************************
void	setForecolor(COLOR foreColor)
{
	fcolor = foreColor;
}
//*******************************************************************************
void	setForecolor(uint8_t redVal, uint8_t greenVal, uint8_t blueVal)
{
	fcolor.red		=	redVal;
	fcolor.green	=	greenVal;
	fcolor.blue		=	blueVal;
}
//*******************************************************************************
void fillEllipse(int x, int y, int xRadius, int yRadius)
{
	fillEllipse(x,y,xRadius,yRadius,fcolor);
}
//*******************************************************************************
void fillEllipse(int x, int y, int xRadius, int yRadius, COLOR col)
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

	if (smoothingMode > 0) drawEllipse(x,y,xRadius,yRadius,col);

	dispColor(col);
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
			dispRectangle(x - xx, y - yy, width, 1);
			if (yy != 0)
			{
				//dispRectangle(xCenter- xx, yCenter + y, width, 1);
				dispRectangle(x - xx, y + yy, width, 1);
			}
			incy();
		}
		else
		{
			//dispRectangle(xCenter - xx, yCenter - y, width, 1);
			dispRectangle(x - xx, y - yy, width, 1);
			if (yy != 0)
			{
				//dispRectangle(xCenter - xx, yCenter + y, width, 1);
				dispRectangle(x - xx, y + yy, width, 1);
			}
			incx();
			incy();
			width	+=	2;
		}
	}
	if (yRadius == 0)
	{
		//dispRectangle(xCenter - xRadius, yCenter, 2 * xRadius + 1, 1);
		dispRectangle(x - xRadius, y, 2 * xRadius + 1, 1);
	}
}
//*******************************************************************************
void	fillRect(int x, int y, int width, int height)
{
	fillRect(x,y,width,height,fcolor);
}
//*******************************************************************************
void	fillRect(int x, int y, int width, int height, COLOR col)
{
	dispColor(col);
	dispRectangle(x, y, width, height); 
}
//*******************************************************************************
void	fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	fillTriangle(x1,y1,x2,y2,x3,y3,fcolor);
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

	if (smoothingMode > 0) {
		drawTriangle(x1,y1,x2,y2,x3,y3,col);
	}

	//*	figure out the left most and right most points
	startX	=	MIN3(x1, x2, x3);
	endX	=	MAX3(x1, x2, x3);
	//*	figure out the top most and bottom most points
	startY	=	MIN3(y1,y2,y3);
	endY	=	MAX3(y1,y2,y3);
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
void fillQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	fillQuad(x1,y1,x2,y2,x3,y3,x4,y4,fcolor);
}
//*******************************************************************************
void fillQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, COLOR col)
{
	fillTriangle(x1,y1,x2,y2,x4,y4,col);
	fillTriangle(x2,y2,x4,y4,x3,y3,col);
}
//*******************************************************************************
COLOR	invertColor(COLOR col)
{
	COLOR rt;
	rt.red = 255 - col.red;
	rt.green = 255 - col.green;
	rt.blue = 255 - col.blue;
	return rt;
}
//*******************************************************************************
void	line(int x1, int y1, int x2, int y2)
{
	line(x1,y1,x2,y2,fcolor);
}
//*******************************************************************************
void	line(int x1, int y1, int x2, int y2, COLOR col)
{
	switch(smoothingMode) {
	case 0:
		dispWuLine(x1, y1, x2, y2, fcolor);
		break;
	case 1:
		dispQSLine(x1, y1, x2, y2, fcolor, bcolor);
		break;
	case 2:
		dispHSLine(x1, y1, x2, y2, fcolor);
		break;
	}
}
//*******************************************************************************
boolean	pointInRect(POINT pt, RECT rt)
{
	return pointInRect(pt.x,pt.y,rt.left,rt.top,rt.right,rt.bottom);
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
void	resetClip()
{
	dispSetClip(0,0,width,height);
}
//*******************************************************************************
int		getBrightness()
{
	return	brightness;
}
//*******************************************************************************
void	setBrightness( int bright)
{
#if defined(_TOUCH_STEALTH_) || !defined(_VERSION_0012LW_)
	dispBrightness(bright);
#else
	oled_brightness(bright);
#endif
	brightness = bright;
}
//*******************************************************************************
void	setClip(int x, int y, int width, int height)
{
	dispSetClip(x,y,width,height);
}
//*******************************************************************************
void	getPixel(COLOR *buffer, int x, int y)
{
	dispRead(buffer, x, y);
}
//*******************************************************************************
void setPixel(int x, int y)
{
	setPixel(x,y,fcolor);
}
//*******************************************************************************
void setPixel(int x, int y, COLOR col)
{
	dispColor(col);
	dispPixel2(x,y);
}
//*******************************************************************************
void	strokeWeight(int s)
{
	strokeWeightVal=s;
}
//*******************************************************************************
void	text(char *textString, int xLoc, int yLoc)
{
	dispPutS(textString, xLoc, yLoc, fcolor, bcolor);
}
//*******************************************************************************
void	text(char *textString, int xLoc, int yLoc, COLOR col)
{
	dispPutS(textString, xLoc, yLoc, col, bcolor);
}
//*******************************************************************************
void	text(char theChar, int xLoc, int yLoc)
{
	dispPutC(theChar, xLoc, yLoc, fcolor, bcolor);
}
//*******************************************************************************
void	text(char theChar, int xLoc, int yLoc, COLOR col)
{
	dispPutC(theChar, xLoc, yLoc, col, bcolor);
}
//*******************************************************************************
void	text(int data, int xLoc, int yLoc)
{
	char out[10];
	dtostrf(data,4,0,out);
	text(out, xLoc, yLoc);
}
//*******************************************************************************
void	text(int data, int xLoc, int yLoc, COLOR col)
{
	char out[10];
	dtostrf(data,4,0,out);
	text(out, xLoc, yLoc, col);
}
//*******************************************************************************
void	text(unsigned int data, int xLoc, int yLoc)
{
	text((int)data,  xLoc,  yLoc);
}
//*******************************************************************************
void	text(unsigned int data, int xLoc, int yLoc, COLOR col)
{
	text((int)data,  xLoc,  yLoc, col);
}
//*******************************************************************************
void	text(long data, int xLoc, int yLoc)
{
	char out[10];
	dtostrf(data,4,0,out);
	text(out, xLoc, yLoc);
}
//*******************************************************************************
void	text(long data, int xLoc, int yLoc, COLOR col)
{
	char out[10];
	dtostrf(data,4,0,out);
	text(out, xLoc, yLoc, col);
}
//*******************************************************************************
void	text(double data, int xLoc, int yLoc)
{
	char out[12];
	dtostrf(data,4,3,out);
	text(out, xLoc, yLoc);
}
//*******************************************************************************
void	text(double data, int xLoc, int yLoc, COLOR col)
{
	char out[12];
	dtostrf(data,4,3,out);
	text(out, xLoc, yLoc, col);
}
//*******************************************************************************
void	text(char *textString, int xLoc, int yLoc, int size)
{
    HersheyDrawCString(0, xLoc, yLoc, textString, size, 0, strokeWeightVal); 
}

//*******************************************************************************
void	text(char *textString, int xLoc, int yLoc, int size, int angle)
{
    HersheyDrawCString(0, xLoc, yLoc, textString, size, angle, strokeWeightVal); 
}
//*******************************************************************************
void	text(char *textString, int xLoc, int yLoc, int size, COLOR col)
{
	fcolor = col;
    HersheyDrawCString(0, xLoc, yLoc, textString, size, 0, strokeWeightVal); 
}

//*******************************************************************************
void	text(char *textString, int xLoc, int yLoc, int size, int angle, COLOR col)
{
	fcolor = col;
    HersheyDrawCString(0, xLoc, yLoc, textString, size, angle, strokeWeightVal); 
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
//*******************************************************************************
void	drawchar( int xLoc, int yLoc, char theChar)
{
	dispPutC(theChar, xLoc, yLoc, fcolor, bcolor);
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
			setForecolor( buf[cptr+1], buf[cptr+2], buf[cptr+3]);
			break;

		case kSubP_SETBCOLOR:
			setBackcolor( buf[cptr+1], buf[cptr+2], buf[cptr+3]); 
			break;

		case kSubP_FILLBACK:
			clear(bcolor);
			//fillback();		
			break;

		case kSubP_SET_BRIGHTNESS:
			setBrightness(buf[cptr+1]);
			break;

		case kSubP_FADEOUT:
			fadeOut((buf[cptr+2] << 8) + buf[cptr+1]);
			break;

		case kSubP_FADEIN:
			fadeIn((buf[cptr+2] << 8) + buf[cptr+1]);
			break;

		case kSubP_DRAWPOINT:
			setPixel((buf[cptr+2] << 8) + buf[cptr+1],
				(buf[cptr+4] << 8) + buf[cptr+3]);
			break;

		case kSubP_CIRCLE:
			int r;
			r	=	(buf[cptr+6] << 8) + buf[cptr+5];
			drawEllipse((buf[cptr+2] << 8) + buf[cptr+1], 
					(buf[cptr+4] << 8) + buf[cptr+3],
					r,
					r);
			break;

		case kSubP_RECT:
			drawRect((buf[cptr+2] << 8) + buf[cptr+1], 
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
			text((char*)&buf[cptr+8],
					 (buf[cptr+2] << 8) + buf[cptr+1], 
					 (buf[cptr+4] << 8) + buf[cptr+3]
					 );
			break;

		case kSubP_GET_TOUCH:
			//gettouch();
			//delay(2);
			//POINT pt = pen;
			serialWrite('|');
			delay(2);
			serialWrite((unsigned char)pen.x);
			delay(2);
			serialWrite((unsigned char)(pen.x>>8));
			delay(2);
			serialWrite((unsigned char)pen.y);
			delay(2);
			serialWrite((unsigned char)(pen.y>>8));
			delay(2);
			serialWrite((unsigned char)0);

			//gettouch();
			//delay(2);
			//serialWrite('|');
			//delay(2);
			//serialWrite((unsigned char)mouseX);
			//delay(2);
			//serialWrite((unsigned char)(mouseX>>8));
			//delay(2);
			//serialWrite((unsigned char)mouseY);
			//delay(2);
			//serialWrite((unsigned char)(mouseY>>8));
			//delay(2);
			//serialWrite((unsigned char)0);
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
			drawEllipse((buf[cptr+2] << 8) + buf[cptr+1], 
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
			drawTriangle(trix1, triy1, trix2, triy2, trix3, triy3);
			break;
			
		default:
			break; 
		}
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
	setForecolor(theColor->red, theColor->green, theColor->blue);
}

//*******************************************************************************
void	RGBBackColor(COLOR *theColor)
{
	setBackcolor(theColor->red, theColor->green, theColor->blue);
}

//*******************************************************************************
void	FrameRect(RECT *theRect)
{
	drawRect(theRect->left, theRect->top, (theRect->right - theRect->left), (theRect->bottom - theRect->top));
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
	fillRect(x1, y1, (x2 - x1), (y2, y1));
}

//*******************************************************************************
void	lcd_pixel(int x, int y, COLOR pixel_color)
{
	dispColor(pixel_color);
	dispPixel(x, y);
}



#endif	//	_SUPPORT_OLD_LCD_ROUTINE_NAMES_
