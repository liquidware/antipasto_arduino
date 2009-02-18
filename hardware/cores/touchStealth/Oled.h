#ifndef OLED_H
#define OLED_H

//#include "DataTypes.h"
#include "graphics.h"
#include "oled_stealth.h"

#define LCD_COLUMNS 128
#define LCD_ROWS 128
#define LCD_RAM_LENGTH 132

#define HIGH_BRIGHTNESS	0x0E
#define MED_BRIGHTNESS	0x0A
#define LOW_BRIGHTNESS	0x07

#define FONT_HEIGHT 7
#define FONT_WIDTH 5

enum ScrollMode {
	TEST = 0,
	NORMAL = 1,
	SLOW = 2,
	SLOWEST = 3
} ;

class Oled {
	protected:
		void SetColumn(int start, int end);
		void SetRow(int start, int end);
		void WritePixel(COLOR* color);
		void WriteCommand(unsigned char command);
		void WriteData(unsigned char data);
		int initialized;
		//int fillEnabled;
		// enable/disable circle/rectangle fill
		//void SetFillEnabled(int val);

		//int reverseCopyEnabled;
		//void SetReverseCopyEnabled(int val);

		//int xCopyWrapEnabled;
		// enable/disable wrap around in the X direction on copy
		//void SetXCopyWrapEnabled(int val);


	public:
		Oled();
		void Initialize();
		void Clear();
		void SetDisplayStartLine(int line);
		void SetDisplayOffset(int offset);
		void DrawChar(unsigned char ch, int x_pos,
			int y_pos,COLOR* fc, COLOR* bc);
		void DrawString(char * string, unsigned int x_pos,
			unsigned int y_pos, COLOR* fc, COLOR* bc);
		void DrawLine(int startColumn, int startRow,
			int endColumn, int endRow, COLOR* color);
		//void DrawRectangle(LCD_RECT* rectangle,
		//	COLOR* lineColor, COLOR* fillColor);
		void DrawRectangle(unsigned char startColumn, unsigned char startRow,
			unsigned char endColumn, unsigned char endRow, COLOR* lineColor, COLOR* fillColor);
		//void DrawCircle(int centerColumn, int centerRow, int radius,
		//	COLOR* lineColor, COLOR* fillColor);
		void CopyRegion(LCD_RECT* region, unsigned char destColumn, unsigned char destRow);
		//void DimRegion(LCD_RECT* region);
		void ClearRegion(LCD_RECT* region);
		void ClearRegion(unsigned char startColumn, unsigned char startRow,
			unsigned char endColumn, unsigned char endRow);
		//void HorizontalScroll(int hOffset, int startRow, int rows, ScrollMode mode);
		//void StartHorizontalScroll();
		//void StopHorizontalScroll();

};

extern Oled Display;
#endif