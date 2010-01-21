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
//*******************************************************************************
//*	Nov 30,	2008	<inthebitz> Version 0.1 This is the beginnings of the "Sub-Processing" for TouchShield library
//*	Dec  4,	2008	<CML> Version 0.2 Merged into the TouchShield core
//*	Dec 26,	2008	<MLS> Mark Sproul meet with Chris and Matt in New Haven
//*	Dec 26,	2008	<MLS> taking over the development of Sub-Processing
//*	Dec 26,	2008	<MLS> Changing names of command ENUMS to kSubP_xxx
//*	Jan  3,	2009	<MLS> Changed my_point to gMostRecentTouchPt
//*	Jan 11,	2009	<MLS> started on support for lcd family of calls
//*	Jan 18,	2009	Version 1.1.0 handed over to Matt, Mike and Chriss
//*	Jan 18,	2009	<MLS> Added bezier curve functions
//*	Feb 10,	2009	<MLS> Version 1.1.1 posted to github
//*	Feb 11,	2009	<MLS> Bumping my version to 1.1.2
//*	Dec 24,	2009	<MLS> Bumping my version to 1.2.0
//*	Jan 16,	2010	<MLS> Bumping my version to 1.3.0
//*******************************************************************************
//#include	"SubPGraphics.h"
//*******************************************************************************
//*	PREPROCESSOR DIRECTIVES
//*******************************************************************************
#ifndef SUBPGRAPHICS_H /* Idempotence guard */
#define SUBPGRAPHICS_H

#ifndef _SUBP_OPTIONS_H_
	#include	"SubPOptions.h"
#endif



#ifndef Wiring_h
	#include	"wiring.h"
#endif
#ifndef _HARDWARE_DEF_H_
	#include	"HardwareDef.h"
#endif
#ifndef GRAPHICS_H
	#include	"graphics.h"
#endif



#define	kSubP_VersionString			"SubP V1.3.0"
#define	kSubP_ReleaseDateString		"Jan 18, 2010"

//#define _ENABLE_SMOOTHING_
//#define _ENABLE_PEN_TAP_
//#define _ENABLE_TOUCHSHIELD_DEBUG_


#ifdef _ENABLE_SMOOTHING_
	typedef enum
	{
		kSmoothing_Off	=	0,
		kSmoothing_Quick,
		kSmoothing_High,
		
		kSmoothing_last
	} systemSmoothing;

void	SetSmoothMode(unsigned char newMode);
#endif



//*******************************************************************************
//*	TYPES
//*******************************************************************************
typedef enum
{
	kSubP_SETFCOLOR			=	1,
	kSubP_SETBCOLOR			=	2,
	kSubP_FILLBACK			=	3,
	kSubP_SET_BRIGHTNESS	=	4,
	kSubP_FADEOUT			=	5,
	kSubP_FADEIN			=	6,
	kSubP_DRAWPOINT			=	7,
	kSubP_CIRCLE			=	8,
	kSubP_RECT				=	9,
	kSubP_LINE				=	10,
	kSubP_CHAR				=	11,
	kSubP_STRING			=	12,
	kSubP_GET_TOUCH			=	13,
	kSubP_ELLIPSE			=	14,
	kSubP_TRIANGLE1			=	15,
	kSubP_TRIANGLE2			=	16,
	kSubP_GET_SCREEN_SIZE	=	17,
	kSubP_NOFILL			=	18,
	kSubP_NOSTROKE			=	19,
} SubPCommand;



//*******************************************************************************
//*	GLOBAL CONSTANTS AND VARIABLES
//*******************************************************************************
extern	COLOR	fcolor;
extern	COLOR	bcolor;
extern	COLOR	green;
extern	COLOR	blue;
extern	COLOR	yellow;
extern	COLOR	grey;
extern	COLOR	red;
extern	COLOR	black;
extern	COLOR	white;
extern	COLOR	mycolor;

extern	POINT	gMostRecentTouchPt;
extern	int		mouseX;
extern	int		mouseY;
extern	int		gWidth;	//*	<MLS> Dec 18, 2009 changed from "wdith" to gWitdh because "width" was used in to many routines
extern	int		gHeight;
//extern int max_distance;
 
 #ifdef __cplusplus

//*******************************************************************************
//*	GLOBAL FUNCTION PROTOTYPES
//*******************************************************************************

void	background(uint8_t backGroundColor);
void	background(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
void	beginCanvas();
void	clearscreen( void);
float	dist(float x1, float y1, float x2, float y2);
void	drawchar( int xLoc, int yLoc, char text);
void	drawcircle( int xLoc, int yLoc, int radius); 
void	drawrect( int xLoc, int yLoc, int width, int height);
void	drawstring( int xLoc, int yLoc, char *text);
void	ellipse( int xLoc, int yLoc, int radx, int rady);
void	fadein( int time);
void	fadeout( int time);
void	fill(int fillColor);
void	fill(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
void	fillback( void);
boolean	gettouch(void);
void	line( int x1, int y1, int x2, int y2);
void	noStroke(void);
void	noFill(void);
void	point( int xLoc, int yLoc);
void	quad( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void	rect(int xLeft, int yTop, int width, int height);
void	setbcolor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
void	setfcolor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
void	setBrightness( int bright);
void	size(int width, int height);
void	stroke(int newStrokeColor);
void	stroke(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
uint8_t	strokeWeight(int newStrokeWeight);
void	text(char theChar, int xLoc, int yLoc);
void	text(char *textString, int xLoc, int yLoc);
void	text(int data,int xLoc, int yLoc);
void	text(unsigned int data, int xLoc, int yLoc);
void	text(long data, int xLoc, int yLoc);
void	text(double data, int xLoc, int yLoc);
void	text(char *textString, int xLoc, int yLoc, int height);
void	text(char *textString, int xLoc, int yLoc, int width, int height);
void	triangle( int x1, int y1, int x2, int y2, int x3, int y3);
void	bezier(int x1, int y1, int cx1, int cy1, int cx2, int cy2, int x2, int y2);
void	arc(int xCenter, int yCenter, int xDiameter, int yDiameter, float startAngle, float stopAngle);

//*******************************************************************************
//*	Added by <TWH>
COLOR	alphaBlend(COLOR forecolor, COLOR backcolor, int Opacity);
void	centerText(int x1, int y1, int x2, int y2, char* string);
void	clear(void);
void	clear(COLOR col);
void	clear(int argRed, int argGreen, int argBlue);
COLOR	colorFromLong(long val);
float	dist(POINT p1, POINT p2);
void	drawEllipse(int xx, int yy, int xRadius, int yRadius);
void	drawEllipse(int xx, int yy, int xRadius, int yRadius, COLOR col);
void	drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void	drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, COLOR col);
void	drawRect(int xx, int yy, int width, int height, COLOR col);
void	drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
void	drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, COLOR col);
void	drawWindow(char* caption, int xx, int yy, int ww, int hh, COLOR bkg);
void	fillEllipse(int xx, int yy, int xRadius, int yRadius);
void	fillEllipse(int xx, int yy, int xRadius, int yRadius, COLOR col);
void	fillQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void	fillQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, COLOR col);
void	fillRect( int xLoc, int yLoc, int width, int height);
void	fillRect(int x, int y, int width, int height, COLOR col);
void	fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
void	fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, COLOR col);
void	floodFill(int argX, int argY, COLOR col);
int		getBrightness(void);
void	getPixel(COLOR *buffer, int x, int y);
COLOR	invertColor(COLOR col);
void	line( int x1, int y1, int x2, int y2, COLOR col);
boolean	pointInRect(int pX, int pY, int rLeft, int rTop, int rRight, int rBottom);

#ifdef _ENABLE_CLIPPING_
	void	setClip(int x, int y, int width, int height);
	void	resetClip();
#endif
#ifdef _ENABLE_PEN_TAP_
	void	updatePen(void);
#endif

void	setBackcolor(COLOR bkgColor);
void	setBackcolor(uint8_t redVal, uint8_t greenVal, uint8_t blueVal);
uint8_t	setFont(uint8_t iFont);
void	setForecolor(COLOR foreColor);
void	setForecolor(uint8_t redVal, uint8_t greenVal, uint8_t blueVal);
void	setPixel(int xx, int yy);
void	setPixel(int xx, int yy, COLOR col);
//int		text(char theChar, int xLoc, int yLoc);
//int		text(char *textString, int xLoc, int yLoc);
//int		text(int data,int xLoc, int yLoc);
//int		text(unsigned int data, int xLoc, int yLoc);
//int		text(long data, int xLoc, int yLoc);
//int		text(double data, int xLoc, int yLoc);
int		text(char theChar, int xLoc, int yLoc, COLOR col);
int		text(char *textString, int xLoc, int yLoc, COLOR col);
int		text(int *textString, int xLoc, int yLoc);
int		text(int *textString, int xLoc, int yLoc, COLOR col);
int		text(int data,int xLoc, int yLoc, COLOR col);
int		text(unsigned int data, int xLoc, int yLoc, COLOR col);
int		text(long data, int xLoc, int yLoc, COLOR col);
int		text(double data, int xLoc, int yLoc, COLOR col);





//*******************************************************************************
//*	processing font routines
//*	Added by <MLS>
typedef unsigned char PFont;
PFont	loadFont(char *fontName);
void	textFont(char *fontName, int fontSize);
void	textFont(PFont theFontNumber, int fontSize);
short	textWidth(uint8_t fontNumber, char *textString);
short	textWidth(char *textString);


#ifdef _SUBP_OPTION_7_SEGMENT_
	void	Display7SegmentDigit(int xLeft, int yTop, char decimalDigit, char segmentLength);
	void	Display7SegmentChar(int xLeft, int yTop, char theChar, char segmentLength);
	void	Display7SegmentString(int xLeft, int yTop, char *hexString, char segmentLength);

#endif	//*	_SUBP_OPTION_7_SEGMENT_


#ifdef _SUBP_OPTION_KEYBOARD_

char	GetQWERTYkeyboardInput(void);

enum {	kButtonMode_dontDraw	=	0,
		kButtonMode_drawHex,
		kButtonMode_drawName

};
	typedef struct  {
						RECT			buttonRect;
						boolean			validButton;
						char			bottonMode;
						short			buttonSize;
						char			imageFileIndex;		//*	file index number to speed up drawing
						char			buttonChar;
						char			buttonName[14];
						char			bmpImageOK;
						char			bmpWidth;
						char			bmpHeight;
					//	COLOR	buttonColor;
					} Button;

void	DrawKeyPadButton(Button *theButton);

#endif	//*	_SUBP_OPTION_KEYBOARD_



#ifdef _ENABLE_TOUCHSHIELD_DEBUG_
//*******************************************************************************
//*	these enable debugging through the arduino serial port
//*	by putting the Arduino in reset and then opening the serial port
//*******************************************************************************
void	TouchDebug_Enable(unsigned long baudRate);
void	TouchDebug_Print(char *debugString);
#endif



#endif	//*	__cplusplus


//*******************************************************************************
#ifdef _SUPPORT_OLD_LCD_ROUTINE_NAMES_

#ifdef __cplusplus
	extern "C"{
#endif

#ifndef __LCD_H_
	#define	LCD_RECT	RECT
#endif

	//*	from lcd.h
	#define		LCD_DC			PG4
	#define		LCD_CS			PG0
	#define		LCD_RD			PG2
	#define		LCD_WR			PG1
	#define		LCD_RESET		PG3
	#define		LCD_CTRL_PORT	PORTG
	#define		LCD_CTRL_DDR	DDRG


	//*	routines with new names
	#define		touch_get_cursor	touch_getCursor


	#define		LCD_DATA_LOW	PORTD
	#define		LCD_DATA_MED	PORTC
	#define		LCD_DATA_HIGH	PORTA

	void			lcd_init();
	void			lcd_setRow( int start, int end);
	void			lcd_setColumn( int start, int end);
	void			lcd_write_C(unsigned char command);
	void			lcd_write_D(unsigned char data);
	void			lcd_pix();
	unsigned char	lcd_read_status(void);

	void			lcd_clear(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2, COLOR * color);
	void			lcd_clearScreen(COLOR c);
	void			lcd_setBrightness(unsigned char brightnessLevel);
	void			lcd_setContrast(unsigned char red, unsigned char green, unsigned char blue);

	void			lcd_pixel(int x, int y, COLOR pixel_color);
	void			lcd_circle(int x, int y, int radius, COLOR outline_color, COLOR fill_color);
	void			lcd_rectangle(int x1, int y1, int x2, int y2, COLOR outline_color, COLOR fill_color);
	void			lcd_line(int x1, int y1, int x2, int y2, COLOR line_color);

	void			lcd_putc(unsigned char ch, int x_pos, int y_pos,COLOR fc, COLOR bc);
	void			lcd_puts(char * string, int x_pos, int y_pos, COLOR fc, COLOR bc);
	void			lcd_dimWindow(unsigned char left, unsigned char top, unsigned char right, unsigned char bottom);  

	void			lcd_rect(LCD_RECT r, COLOR outline_color, COLOR fill_color);

	char			pointInRect(POINT p, LCD_RECT r);

	void			dispColor(COLOR c);
	void			dispClearScreen();
	void			dispPixel(int x, int y);
	void			dispLine( int x1,  int y1, int x2,  int y2);
	void			dispRectangle( int  x,  int  y,  int width,  int  height);
	void			dispPutC(unsigned char ch, unsigned int x_pos,unsigned int y_pos,COLOR fc, COLOR bc);
	void			dispPutS(char * string,unsigned int x_pos, unsigned int y_pos, COLOR fc, COLOR bc);
#ifdef __cplusplus
	} // extern "C"
#endif
#endif


#endif /* Idempotence guard */

