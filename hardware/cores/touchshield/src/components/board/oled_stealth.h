//*******************************************************************************
//*
//*
//*
//*******************************************************************************
//*	Detailed Edit History
//*		PLEASE put comments here every time you work on this code
//*	<MLS>	=	Mark Sproul, msproul@jove.rutgers.edu
//*******************************************************************************
//*	Jan  2,	2009	<MLS> Started working on converging touchStealth with touchSlide cores
//*	Jan  3,	2009	<MLS> changed name to oled_stealth.h
//*	Jan  3,	2009	<MLS> added BRIGHT_MAX (value 15) to be compatible with oled_slide.h
//*******************************************************************************


#ifndef GRAPHICS_H
	#include	"graphics.h"
#endif

#ifndef __LCD_H_
#define __LCD_H_

#define BRIGHT_MAX 15

#include "touchscreen.h"
#define LCD_DC		PG4
#define LCD_CS		PG0
#define LCD_RD		PG2
#define LCD_WR		PG1
#define LCD_RESET	PG3

#define LCD_CTRL_PORT	PORTG
#define LCD_CTRL_DDR	DDRG


#define LCD_DATA_LOW 	PORTD
#define LCD_DATA_MED	PORTC
#define LCD_DATA_HIGH	PORTA

#define LCD_DATA_LOW_DDR	DDRD
#define LCD_DATA_MED_DDR	DDRC
#define LCD_DATA_HIGH_DDR	DDRA

#define CHARGE_PUMP_DDR		DDRE
#define CHARGE_PUMP_PORT	PORTE
#define CHARGE_PUMP_PIN		PE3

#define DUFF_DEVICE_8(aCount, aAction) \
do { \
    int32_t count_ = (aCount); \
    int32_t times_ = (count_ + 15) >> 4; \
    switch (count_ & 7){ \
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


typedef struct Lcd_rect
{
	unsigned char left; 	/*!< The left side of the rectangle */
	unsigned char top; 		/*!< The top position of the rectangle */
	unsigned char right; 	/*!< The right side of the rectangle */
	unsigned char bottom; 	/*!< The bottom position of the rectangle */
}LCD_RECT;


typedef struct Bmp
	{
	unsigned char 	width;
	unsigned char 	height;
	unsigned int 	length;
	COLOR			pixels[];
	} BMP;


#ifdef __cplusplus
extern "C"{
#endif

extern COLOR GraphicsColor;
extern SCREEN screen;

void lcd_init();
void lcd_setRow( int start, int end);
void lcd_setColumn( int start, int end);
void lcd_write_C(unsigned char command);
void lcd_write_D(unsigned char data);
void dispPix();
unsigned char lcd_read_status(void);

void lcd_clear(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2, COLOR * color);
void lcd_clearScreen(COLOR c);
void lcd_setBrightness(unsigned char brightnessLevel);
void lcd_setContrast(unsigned char red, unsigned char green, unsigned char blue);

void lcd_pixel(int x, int y, COLOR pixel_color);
//void lcd_circle(int x, int y, int radius, COLOR outline_color, COLOR fill_color);
void lcd_rectangle(int x1, int y1, int x2, int y2, COLOR outline_color, COLOR fill_color);
//void lcd_line(int x1, int y1, int x2, int y2, COLOR line_color);

void lcd_putc(unsigned char ch, int x_pos, int y_pos,COLOR fc, COLOR bc);
//void lcd_puts(char * string, int x_pos, int y_pos, COLOR fc, COLOR bc);
void lcd_dimWindow(unsigned char left, unsigned char top, unsigned char right, unsigned char bottom);

void lcd_rect(LCD_RECT r, COLOR outline_color, COLOR fill_color);

char pointInRect(POINT p, LCD_RECT r);

void dispBrightness(uint8_t brightnessLevel);
void dispClip(int width, int height);
int dispGetClipWidth();
int dispGetClipHeight();
unsigned char dispSetWindow(int x, int y, int width, int height);

#ifdef __cplusplus
} // extern "C"
#endif



#endif

