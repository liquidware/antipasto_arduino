//*******************************************************************************
//*
//*
//*
//*******************************************************************************
//*	Detailed Edit History
//*		PLEASE put comments here every time you work on this code
//*******************************************************************************
//*	Jan  2,	2009	<MLS> Changed name to oled_slealth.c
//*******************************************************************************

#include	"HardwareDef.h"
#ifdef _TOUCH_STEALTH_


#include	<avr/io.h>
#include	<inttypes.h>
#include	<stdlib.h>
#include	<inttypes.h>
#include	<avr/interrupt.h>
#include	<avr/pgmspace.h>
#include	<string.h>
#include	<avr/eeprom.h>


#ifdef _TOUCH_STEALTH_
	#include	"oled_stealth.h"
#endif
#include	"touchscreen.h"
#include	"graphics.h"
#include	"bitops.h"
#include	"usart.h"
#include	"font.h"
#include	"dataflash.h"
#include	"image_interface.h"
#include	"bmp.h"

#include "binary.h"


/* VARIABLES */
int clipWidth	=	127;  //our clipping region width ending pixel
int clipHeight	=	127; //our clipping region height ending pixel
COLOR GraphicsColor;

//*******************************************************************************
void lcd_rect(LCD_RECT r, COLOR outline_color, COLOR fill_color)
{
    lcd_rectangle(r.left, r.top, r.right, r.bottom, outline_color, fill_color);
}

//*******************************************************************************
void dispPix()
{

    lcd_write_C(0x5C);      //enable write RAM

    SETBIT(LCD_CTRL_PORT,LCD_DC);
//LCD_CTRL_PORT ^= (1 << LCD_CS) | (1 << LCD_WR);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);


    LCD_DATA_HIGH	=	GraphicsColor.red;
    LCD_DATA_MED	=	GraphicsColor.green;
    LCD_DATA_LOW	=	GraphicsColor.blue;

//LCD_CTRL_PORT ^= (1 << LCD_CS) | (1 << LCD_WR);

    SETBIT(LCD_CTRL_PORT,LCD_WR);
    SETBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_DC);

}

//*******************************************************************************
void lcd_clear(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2, COLOR * color)
{

    lcd_setColumn(x1, x2);  //set column start/ end
    lcd_setRow(y1,y2);  //set row start /end


    GraphicsColor.red	=	color->red;
    GraphicsColor.green	=	color->green;
    GraphicsColor.blue	=	color->blue;


    unsigned int count	=	((x2-x1)+1)*((y2-y1)+1);
    unsigned int pix;

    lcd_write_C(0x5C);      //enable write RAM

    for (pix=0;pix<count;pix++)
        dispPix();

    lcd_setColumn(0, 127);  //set column start/ end
    lcd_setRow(0,127);  //set row start /end

}


//*******************************************************************************
void lcd_dimWindow(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    lcd_write_C(0x8C);

    lcd_write_D(x1);
    lcd_write_D(y1);
    lcd_write_D(x2);
    lcd_write_D(y2);
}

//*******************************************************************************
void lcd_pixel(int x, int y, COLOR pixel_color)
{
    /* Bounds clipping */
    if ((x<0) || (x>=128) ||
        (y<0) || (y>=128))
    {
        return;
    }

    lcd_setColumn(x,127);   //set column start/ end
    lcd_setRow(y,127);  //set row

    GraphicsColor.red	=	pixel_color.red;
    GraphicsColor.green	=	pixel_color.green;
    GraphicsColor.blue	=	pixel_color.blue;

    dispPix(); //draw the pixel
}



//*******************************************************************************
void lcd_rectangle(int x1, int y1, int x2, int y2, COLOR  outline_color, COLOR  fill_color)
{

    int16_t len	=	((x2-x1)*(y2-y1));

    /* Set XY location   */
    lcd_setColumn(x1, x2);
    lcd_setRow(y1, y2);

    /* Enable write of gram */
    lcd_write_C(0x5C);

    /* Clear the screen */
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);


    /* Set the color once */
    LCD_DATA_LOW	=	fill_color.blue;
    LCD_DATA_MED	=	fill_color.green;
    LCD_DATA_HIGH	=	fill_color.red;

    /* Start the clocking of the WR pin */

    DUFF_DEVICE_8(len,
                  PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
                 PING |= (1 << LCD_CS); PING |= (1 << LCD_CS););

    SETBIT(LCD_CTRL_PORT,LCD_CS);
    /* Set XY location   */
    lcd_setColumn(0, 127);
    lcd_setRow(0, 127);



//color format = RGB
/*
lcd_write_C(0x84);
lcd_write_D(x1);
lcd_write_D(y1);
lcd_write_D(x2);
lcd_write_D(y2);

//line color
lcd_write_D( (outline_color.red & B11111000) | (outline_color.green >> 5));
lcd_write_D( ((outline_color.green<<3) & B11100000) | (outline_color.blue>>3) );

//fill color
lcd_write_D( (fill_color.red & B11111000) | (fill_color.green >> 5));
lcd_write_D( ((fill_color.green<<3) & B11100000) | (fill_color.blue>>3) );

delay_ms(1);
*/


}





//*******************************************************************************
unsigned char lcd_read_status(void)
{
    unsigned char msg=0;

    PORTC	=	0x00;
    PORTD	=	0x00;

    DDRC	=	0x00;
    DDRD	=	0x00;

    CLRBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    CLRBIT(LCD_CTRL_PORT,LCD_RD);   //assert read
    SETBIT(LCD_CTRL_PORT,LCD_WR);

    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    msg	=	(PINC & B00001100) << 4;

    SETBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    SETBIT(LCD_CTRL_PORT,LCD_RD);

    DDRC	=	0xFF;
    DDRD	=	0xFF;

    return msg;
}

//*******************************************************************************
void lcd_write_C(unsigned char command)
{
    CLRBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_RD);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);

    LCD_DATA_MED	=	command;
    LCD_DATA_MED	>>=	4;     //shift right by four
    LCD_DATA_LOW	=	command;
    LCD_DATA_LOW	<<=	2;     //shift left by two

    SETBIT(LCD_CTRL_PORT,LCD_WR);
    SETBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_DC);
}

//*******************************************************************************
void lcd_setBrightness(unsigned char brightnessLevel)
{

    lcd_write_C(0xC7); //Master current control

    if (brightnessLevel < 15)
    {
        if (brightnessLevel==0)
        {
            /* Turn off display */
            CLRBIT(PORTE, PE3);
        }
        else
        {
            /* Turn on display */
            SETBIT(PORTE, PE3);
        }

        //0x0A Typical
        //0x0E High Brightness
        lcd_write_D(brightnessLevel);

    } else
    {
        lcd_write_D(15); //max brightness
    }
}

//*******************************************************************************
void lcd_setContrast(unsigned char red, unsigned char green, unsigned char blue)
{
    lcd_write_C(0xC1); //Contrast levels for R, G, B
    lcd_write_D(0x55);    //Red
    lcd_write_D(0x52);    //Green
    lcd_write_D(0xA1);    //Blue
}


//*******************************************************************************
void lcd_write_D(unsigned char data)
{

    SETBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_RD);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);


    LCD_DATA_MED	=	data;
    LCD_DATA_MED	>>=	4;     //shift right by four
    LCD_DATA_LOW	=	data;
    LCD_DATA_LOW	<<=	2;     //shift left by two


    SETBIT(LCD_CTRL_PORT,LCD_WR);
    SETBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_DC);
}



//*******************************************************************************
void lcd_init()
{
//set output values
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    SETBIT(LCD_CTRL_PORT,LCD_CS);
    SETBIT(LCD_CTRL_PORT,LCD_WR);
    SETBIT(LCD_CTRL_PORT,LCD_RD);
    LCD_DATA_LOW	=	0x00;
    LCD_DATA_MED	=	0x00;
    LCD_DATA_HIGH	=	0x00;

//setup data direction registers
    SETBIT(LCD_CTRL_DDR,LCD_DC); //output
    SETBIT(LCD_CTRL_DDR,LCD_CS); //output
    SETBIT(LCD_CTRL_DDR,LCD_WR); //output
    SETBIT(LCD_CTRL_DDR,LCD_RD); //output
    SETBIT(LCD_CTRL_DDR,LCD_RESET); //output

    LCD_DATA_LOW_DDR	=	0xFF; //output
    LCD_DATA_MED_DDR	=	0xFF; //output
    LCD_DATA_HIGH_DDR	=	0xFF; //output

//init the LCD
    CLRBIT(LCD_CTRL_PORT,LCD_RESET);
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    SETBIT(LCD_CTRL_PORT,LCD_RESET);



    lcd_write_C(0xAE); //display off

    lcd_write_C(0xca); // Duty
    lcd_write_D(0x7f); // 1/128

    lcd_write_C(0xA1); //Set display start line
    lcd_write_D(0);    //0x00 start

    lcd_write_C(0xA2); //Display offset
    lcd_write_D(0x80);    //0x00 start


    lcd_write_C(0xA0); //Color Remap
    lcd_write_D(B10111100);    //65k color, 18-bit

    lcd_write_C(0xC7); //Master current control
    lcd_write_D(0x0A);    //0x07 Low Brightness
    //0x0A Typical
    //0x0E High Brightness


    lcd_write_C(0xC1); //Contrast levels for R, G, B
    lcd_write_D(0x55);    //Red
    lcd_write_D(0x52);    //Green
    lcd_write_D(0xA1);    //Blue


    lcd_write_C(0xB1); //Phase Adjust
    lcd_write_D(0x22);    //

    lcd_write_C(0xB3); //Frame Rate
    lcd_write_D(0xF0);    //85 Hz?
//lcd_write_D(0x10);    //85 Hz?

    lcd_write_C(0xBB); //Set Pre-carge level for R, G, B
    lcd_write_D(0x00);    //Red
    lcd_write_D(0x00);    //Green
    lcd_write_D(0x00);    //Blue


    lcd_write_C(0xAD); //Master Config
    lcd_write_D(0x8E);    //

    lcd_write_C(0xB0);  //Current Saving
    lcd_write_D(0x00);    //

    lcd_write_C(0xBE);  //VCOMH Setting
    lcd_write_D(0x1C);    //

    lcd_write_C(0xA6);  //Inverse Display mode off
    lcd_write_C(0xD1); //Normal Display
    lcd_write_D(0x02);    //

    lcd_write_C(0xAF); //Display On

    lcd_write_C(0x92);
    lcd_write_D(1);


    SETBIT(CHARGE_PUMP_DDR, CHARGE_PUMP_PIN); //turn on the charge pump
    SETBIT(CHARGE_PUMP_PORT,CHARGE_PUMP_PIN);


    COLOR c	=	{0,0,0};
    lcd_clearScreen(c);

}

//*******************************************************************************
void lcd_clearScreen(COLOR c)
{

    unsigned int i=512;

    /* Set XY location   */
    lcd_setColumn(0, 127);
    lcd_setRow(0, 127);

    /* Enable write of gram */
    lcd_write_C(0x5C);

    /* Clear the screen */
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);


    /* Set the color once */
    LCD_DATA_LOW	=	c.blue;
    LCD_DATA_MED	=	c.green;
    LCD_DATA_HIGH	=	c.red;

    /* Start the clocking of the WR pin */

    while (i--)
    {
        /* Set & Clear */
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);

        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);

        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);

        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
        PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
    }

    SETBIT(LCD_CTRL_PORT,LCD_CS);

}

//*******************************************************************************
void lcd_setColumn(int start,int end)
{

/* Bounds clipping */
    if (start < 0 )
    {
        start	=	0;
    }
    if (start >= 128 )
    {
        start	=	127;
    }
    if (end < 0)
    {
        end		=	0;
    }
    if (end >= 128)
    {
        end		=	127;
    }

//set column (x)
    lcd_write_C(0x15);
    lcd_write_D(start);
    lcd_write_D(end);
}

//*******************************************************************************
void lcd_setRow(int start,int end)
{

/* Bounds clipping */
    if (start < 0 )
    {
        start	=	0;
    }
    if (start >= 128 )
    {
        start	=	127;
    }
    if (end < 0)
    {
        end		=	0;
    }
    if (end >= 128)
    {
        end		=	127;
    }

//set row (y)
    lcd_write_C(0x75);
    lcd_write_D(start);
    lcd_write_D(end);
}



//*******************************************************************************
char pointInRect(POINT p, LCD_RECT r)
{
    if (p.x >= r.left)
        if (p.x <= r.right)
            if (p.y >= r.top)
                if (p.y <= r.bottom)
                    return 1;

    return 0;
}

//*******************************************************************************
void dispBrightness(uint8_t brightnessLevel)
{
    lcd_setBrightness(brightnessLevel);
}

//*******************************************************************************
void dispColor(COLOR c)
{
    GraphicsColor.red	=	c.red;
    GraphicsColor.green	=	c.green;
    GraphicsColor.blue	=	c.blue;
}

//*******************************************************************************
void dispClearScreen()
{
    lcd_clearScreen(GraphicsColor);
}

//*******************************************************************************
void dispPixel(int x, int y)
{
    lcd_pixel(x, y, GraphicsColor);
}

//*******************************************************************************
void dispRectangle( int  x,  int  y,  int width,  int  height)
{
    int32_t len	=	width*height;

    /* Set the rectangle window */
    dispSetWindow(x,y,width,height);

    /* Enable write of gram */
    lcd_write_C(0x5C);

    /* Clear the screen */
    SETBIT(LCD_CTRL_PORT,LCD_DC);
    CLRBIT(LCD_CTRL_PORT,LCD_CS);
    CLRBIT(LCD_CTRL_PORT,LCD_WR);

    /* Set the color once */
    LCD_DATA_LOW	=	GraphicsColor.blue;
    LCD_DATA_MED	=	GraphicsColor.green;
    LCD_DATA_HIGH	=	GraphicsColor.red;

    /* Start the clocking of the WR pin */
    DUFF_DEVICE_8(len,
                  PING |= (1 << LCD_CS); PING |= (1 << LCD_CS);
                  PING |= (1 << LCD_CS); PING |= (1 << LCD_CS););

    SETBIT(LCD_CTRL_PORT,LCD_CS);

    /* Set back a wide window   */
    lcd_setColumn(0, 127);
    lcd_setRow(0, 127);
}

//*******************************************************************************
void dispClip(int width, int height)
{
    clipWidth	=	width;
    clipHeight	=	height;
}

//*******************************************************************************
int dispGetClipWidth()
{
    return clipWidth;
}

//*******************************************************************************
int dispGetClipHeight()
{
    return clipHeight;
}

//*******************************************************************************
unsigned char dispSetWindow(int x, int y, int width, int height)
{
    uint16_t temp;

    /* Bounds clipping */
    if (x < 0 ){ width += x-0; x = 0;  }
    if (y < 0 ){ height += y-0; y = 0; }
    if (x > clipWidth ){ x = clipWidth; }
    if (y > clipHeight ){ y = clipHeight; }
    if (x+width  >= clipWidth+1){ width = clipWidth-x; }
    if (y+height >= clipHeight+1){ height = clipHeight-y; }

    /* Set XY location   */
    lcd_setColumn(x, x+width);
    lcd_setRow(y, y+height);
}

#endif
