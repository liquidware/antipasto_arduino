#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>
#include <stddef.h>


#include "Oled.h"
#include "bitops.h"
#include "binary.h"
#include "font.h"

// commands
#define COMM_SET_COLUMN 			0x15
#define COMM_SET_ROW 				0x75
#define COMM_WRITE_RAM 				0x5C
#define COMM_READ_RAM 				0x5D
#define COMM_REMAP_COLOR_DEPTH 		0xA0
#define COMM_SET_DISPLAY_LINE 		0xA1
#define COMM_SET_DISPLAY_OFFSET 	0xA2
#define COMM_DISP_OFF 				0xA4
#define COMM_DISP_ON 				0xA5
#define COMM_DISP_RESET 			0xA6
#define COMM_DISP_INV 				0xA7
#define COMM_MASTER_CONFIG 			0xAD
#define COMM_SLEEP_ON 				0xAE
#define COMM_SLEEP_OFF 				0xAF
#define COMM_SET_POWER_SAVE 		0xB0
#define COMM_SET_PHASE_PER 			0xB1
#define COMM_SET_FREQ 				0xB3
#define COMM_SET_GS_TABLE 			0xB8
#define COMM_GS_RESET 				0xB9
#define COMM_SET_COLOR_PRECHARGE	0xBB
#define COMM_SET_V_COMH 			0xBE
#define COMM_SET_CLR_CONTRAST 		0xC1
#define COMM_SET_MAST_CONTRAST 		0xC7
#define COMM_SET_MUX_RATIO 			0xCA
#define COMM_NOP 					0xE3
#define COMM_SET_COMM_LOCK 			0xFD
#define COMM_DRAW_LINE 				0x83
#define COMM_DRAW_RECT 				0x84
#define COMM_DRAW_CIRC 				0x86
#define COMM_COPY 					0x8A
#define COMM_DIM 					0x8C
#define COMM_CLEAR 					0x8E
#define COMM_SET_FILL 				0x92
#define COMM_SET_HORZ_SCROLL 		0x96
#define COMM_START_SCROLL 			0x9E
#define COMM_STOP_SCROLL 			0x9F

// settings
#define MUX_RATIO 					0x7f
//#define REMAP_COLOR_DEPTH

// batch writes don't work at the moment because of changes
// to the DrawChar method for writing across the VRAM boundary
#define BATCH_WRITES

#define VERTICAL_INCREMENT 		1
#define COL_ADDR_INV			2
#define COL_SEQ_INV				4
#define BUS_ENABLE				8
#define SCAN_REVERSE			16
#define ENABLE_COM_SPLIT		32
#define COLOR_256				0
#define COLOR_65k				64


// be careful with this macro. don't pass vars with ++/--
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)


Oled::Oled() {
	initialized = 0;
	//Initialize();
}

void Oled::WritePixel(COLOR* color)
{

	WriteCommand(COMM_WRITE_RAM); 		//enable write RAM

	SETBIT(LCD_CTRL_PORT,LCD_DC);
	//LCD_CTRL_PORT ^= (1 << LCD_CS) | (1 << LCD_WR);
	CLRBIT(LCD_CTRL_PORT,LCD_CS);
	CLRBIT(LCD_CTRL_PORT,LCD_WR);


	LCD_DATA_HIGH = color->red;
	LCD_DATA_MED = color->green;
	LCD_DATA_LOW = color->blue;

	//LCD_CTRL_PORT ^= (1 << LCD_CS) | (1 << LCD_WR);

	SETBIT(LCD_CTRL_PORT,LCD_WR);
	SETBIT(LCD_CTRL_PORT,LCD_CS);
	SETBIT(LCD_CTRL_PORT,LCD_DC);

}

void Oled::Initialize() {
	if(!initialized) {
		initialized = 1;
		//set output values
		SETBIT(LCD_CTRL_PORT,LCD_DC);
		SETBIT(LCD_CTRL_PORT,LCD_CS);
		SETBIT(LCD_CTRL_PORT,LCD_WR);
		SETBIT(LCD_CTRL_PORT,LCD_RD);
		LCD_DATA_LOW = 0x00;
		LCD_DATA_MED = 0x00;
		LCD_DATA_HIGH = 0x00;

		//setup data direction registers
		SETBIT(LCD_CTRL_DDR,LCD_DC); //output
		SETBIT(LCD_CTRL_DDR,LCD_CS); //output
		SETBIT(LCD_CTRL_DDR,LCD_WR); //output
		SETBIT(LCD_CTRL_DDR,LCD_RD); //output
		SETBIT(LCD_CTRL_DDR,LCD_RESET); //output

		LCD_DATA_LOW_DDR = 0xFF; //output
		LCD_DATA_MED_DDR = 0xFF; //output
		LCD_DATA_HIGH_DDR = 0xFF; //output

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

		WriteCommand(COMM_DISP_RESET);

		//WriteCommand(0xAE); //display off
		WriteCommand(COMM_SLEEP_ON);

		WriteCommand(COMM_SET_MUX_RATIO); // Duty
		WriteData(MUX_RATIO); // 1/128

		//WriteCommand(0xA1); //Set display start line
		//WriteData(0);    //0x00 start

		//WriteCommand(0xA2); //Display offset
		//WriteData(0x80);


		WriteCommand(COMM_REMAP_COLOR_DEPTH); //Color Remap
		WriteData(B10111100);    //65k color, 18-bit
		//WriteData(B01111100);

		WriteCommand(COMM_SET_MAST_CONTRAST); //Master current control
		WriteData(0x0A);    //0x07 Low Brightness
								//0x0A Typical
								//0x0E High Brightness


		WriteCommand(COMM_SET_CLR_CONTRAST); //Contrast levels for R, G, B
		WriteData(0x55);    //Red
		WriteData(0x52);    //Green
		WriteData(0xA1);    //Blue


		WriteCommand(COMM_SET_PHASE_PER); //Phase Adjust
		WriteData(0x22);    //

		WriteCommand(COMM_SET_FREQ); //Frame Rate
		WriteData(0xF0);    //85 Hz?
		//WriteData(0x10);    //85 Hz?

		WriteCommand(COMM_SET_COLOR_PRECHARGE); //Set Pre-carge level for R, G, B
		WriteData(0x00);    //Red
		WriteData(0x00);    //Green
		WriteData(0x00);    //Blue


		WriteCommand(COMM_MASTER_CONFIG); //Master Config
		WriteData(0x8E);    //

		WriteCommand(COMM_SET_POWER_SAVE); 	//Current Saving
		WriteData(0x00);    //

		WriteCommand(COMM_SET_V_COMH); 	//VCOMH Setting
		WriteData(0x1C);    //

		//WriteCommand(0xA6); 	//Inverse Display mode off
		//WriteCommand(0xD1); //Normal Display
		//WriteData(0x02);    //

		WriteCommand(COMM_SLEEP_OFF); //Display On

		WriteCommand(COMM_SET_FILL);
		WriteData(1);


		SETBIT(CHARGE_PUMP_DDR, CHARGE_PUMP_PIN); //turn on the charge pump
		SETBIT(CHARGE_PUMP_PORT,CHARGE_PUMP_PIN);

		Clear();

	}
}

void Oled::Clear() {
	WriteCommand(0x8E);
	WriteData(0x0);
	WriteData(0x0);
	WriteData(0x20);
	WriteData(0x20);
	return;
unsigned int i=512;
  	COLOR c = {0,0,0};
   /* Set XY location   */
   SetColumn(0, 127);
   SetRow(0, 127);

   /* Enable write of gram */
   WriteCommand(COMM_WRITE_RAM);

   /* Clear the screen */
   SETBIT(LCD_CTRL_PORT,LCD_DC);
   CLRBIT(LCD_CTRL_PORT,LCD_CS);
   CLRBIT(LCD_CTRL_PORT,LCD_WR);


   /* Set the color once */
    LCD_DATA_LOW = c.blue;
    LCD_DATA_MED = c.green;
    LCD_DATA_HIGH = c.red;

   /* Start the clocking of the WR pin */

   while(i--)
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

void Oled::WriteCommand(unsigned char command) {
	CLRBIT(LCD_CTRL_PORT,LCD_DC);
	CLRBIT(LCD_CTRL_PORT,LCD_CS);
	SETBIT(LCD_CTRL_PORT,LCD_RD);
	CLRBIT(LCD_CTRL_PORT,LCD_WR);

	LCD_DATA_MED = command;
	LCD_DATA_MED >>= 4; 	//shift right by four
	LCD_DATA_LOW = command;
	LCD_DATA_LOW <<= 2; 	//shift left by two

	SETBIT(LCD_CTRL_PORT,LCD_WR);
	SETBIT(LCD_CTRL_PORT,LCD_CS);
	SETBIT(LCD_CTRL_PORT,LCD_DC);
}

void Oled::WriteData(unsigned char data) {
	SETBIT(LCD_CTRL_PORT,LCD_DC);
	CLRBIT(LCD_CTRL_PORT,LCD_CS);
	SETBIT(LCD_CTRL_PORT,LCD_RD);
	CLRBIT(LCD_CTRL_PORT,LCD_WR);

	LCD_DATA_MED = data;
	LCD_DATA_MED >>= 4; 	//shift right by four
	LCD_DATA_LOW = data;
	LCD_DATA_LOW <<= 2; 	//shift left by two

	SETBIT(LCD_CTRL_PORT,LCD_WR);
	SETBIT(LCD_CTRL_PORT,LCD_CS);
	SETBIT(LCD_CTRL_PORT,LCD_DC);
}

void Oled::DrawChar(unsigned char ch, int x_pos, int y_pos, COLOR* fc, COLOR* bc) {
	unsigned int x,shifted,on_off,letter;
	unsigned int row, byte;
	unsigned int tempY;
	letter=ch-(32);
	letter=(letter*7);
	unsigned char spanned = y_pos + FONT_HEIGHT > LCD_ROWS;

#ifdef BATCH_WRITES
	SetColumn(x_pos, x_pos + FONT_WIDTH - 1);
	if(spanned) {
		SetRow(y_pos, LCD_ROWS - 1);
	} else {
		SetRow(y_pos, y_pos + FONT_HEIGHT - 1);
	}

#endif

	char* pos = (char*)font_5x7+letter;
	for (row=0;row<FONT_HEIGHT;row++,pos++)
	{
		byte = pgm_read_byte(pos);

		shifted = byte;

		for (x=0;x<5;x++)
		{

			tempY = y_pos + row;
#ifdef BATCH_WRITES
			if(spanned && tempY == LCD_ROWS) {
				SetRow(0, FONT_HEIGHT - row);
			}
#else
			SetColumn(x_pos+x, x_pos+x); 	//set column start/ end
			if(spanned && tempY > LCD_ROWS - 1) {
				SetRow(tempY - LCD_ROWS, tempY - LCD_ROWS);
			} else {
				SetRow(tempY ,tempY); 	//set row
			}
#endif

			on_off = shifted & B10000000;
			on_off = on_off>>7;
			shifted = shifted << 1;
			if (on_off > 0)
				{
				WritePixel(fc); //write the pixel
				}
			else
				{
				WritePixel(bc); //write the pixel

				}

		}
	}
}

void Oled::DrawString(char * string, unsigned int x_pos, unsigned int y_pos, COLOR* fc, COLOR* bc) {

	while(*string)
		{
		DrawChar(*string++,x_pos,y_pos,fc,bc);
		x_pos=x_pos+6;
	}
}

void Oled::SetColumn(int start, int end) {
	if (start < 0 ){ start = 0; }
	if (start >= LCD_COLUMNS ){ start = LCD_COLUMNS - 1; }
	if (end < 0){ end  = 0;}
	if (end >= LCD_COLUMNS){ end  = LCD_COLUMNS - 1;}

	//set column (x)
	WriteCommand(COMM_SET_COLUMN);
	WriteData(start);
	WriteData(end);
}


void Oled::SetRow(int start, int end) {
	if (start < 0 ){ start = 0; }
	if (start >= LCD_ROWS ){ start = LCD_ROWS - 1; }
	if (end < 0){ end  = 0;}
	if (end >= LCD_ROWS){ end  = LCD_ROWS - 1;}

	//set row (y)
	WriteCommand(COMM_SET_ROW);
	WriteData(start);
	WriteData(end);
}


void Oled::SetDisplayStartLine(int line) {
	WriteCommand(COMM_SET_DISPLAY_LINE);
	WriteData(line);
}

void Oled::SetDisplayOffset(int offset) {
	WriteCommand(COMM_SET_DISPLAY_OFFSET);
	WriteData(offset);
}

void Oled::ClearRegion(LCD_RECT* region) {
	WriteCommand(COMM_CLEAR);
	WriteData(region->left);
	WriteData(region->top);
	WriteData(region->right);
	WriteData(region->bottom);
}

int FixArg(int arg)
{
	if(arg < 0)
		return 0;
	else if(arg > 128)
		return 128;
}


void Oled::ClearRegion(unsigned char startColumn, unsigned char startRow,
			unsigned char endColumn, unsigned char endRow) {
	//SetColumn(startColumn, endColumn);
	//SetRow(startRow, endRow);

	WriteCommand(COMM_CLEAR);
	WriteData(startColumn);
	WriteData(startRow);
	WriteData(endColumn);
	WriteData(endRow);
	return;

	int sc = FixArg(startColumn);
	int sr = FixArg(startRow);
	int ec = FixArg(endColumn);
	int er = FixArg(endRow);

	WriteCommand(COMM_CLEAR);
	WriteData(0);
	WriteData(MIN(sr, er));
	WriteData(127);
	WriteData(MAX(sr, er));
	return;




}

void Oled::CopyRegion(LCD_RECT* region, unsigned char destColumn, unsigned char destRow) {
	WriteCommand(COMM_COPY);
	WriteData(region->left);
	WriteData(region->top);
	WriteData(region->right);
	WriteData(region->bottom);
	WriteData(destColumn);
	WriteData(destRow);
}

void Oled::DrawLine(int startColumn, int startRow, int endColumn, int endRow, COLOR* color) {
	WriteCommand(COMM_DRAW_LINE);
	WriteData(startColumn);
	WriteData(startRow);
	WriteData(endColumn);
	WriteData(endRow);
	WriteData( (color->red & B11111000) | (color->green >> 5));
	WriteData( ((color->green<<3) & B11100000) | (color->blue>>3) );
}

void Oled::DrawRectangle(unsigned char startColumn, unsigned char startRow,
			unsigned char endColumn, unsigned char endRow, COLOR* lineColor, COLOR* fillColor) {
	lcd_write_C(0x84);
	lcd_write_D(startColumn);
	lcd_write_D(startRow);
	lcd_write_D(endColumn);
	lcd_write_D(endRow);
	lcd_write_D( (lineColor->red & B11111000) | (lineColor->green >> 5));
	lcd_write_D( ((lineColor->green<<3) & B11100000) | (lineColor->blue>>3) );
	lcd_write_D( (fillColor->red & B11111000) | (fillColor->green >> 5));
	lcd_write_D( ((fillColor->green<<3) & B11100000) | (fillColor->blue>>3) );
}

Oled Display = Oled();