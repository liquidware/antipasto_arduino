#include <stdarg.h>
#include <stdio.h>

#include "MessageScroller.h"
#include "Oled.h"
#include "oled_stealth.h"
//#include "LcdConstants.h"
//#include "lcd.h"
#include "Colors.h"

#define BUFFER_SIZE 25
//#define TOP_MARGIN 3
#define SIDE_MARGIN 3
#define LINE_PADDING 2
#define LINE_HEIGHT FONT_HEIGHT+LINE_PADDING


MessageScroller::MessageScroller(Oled* oled, COLOR* textColor, COLOR* bgColor) {
	usedLines = 0;
	this->textColor = textColor;
	this->bgColor = bgColor;
	this->oled = oled;
	offset = 0;
}

void MessageScroller::AddText(char* text) {
	if(LCD_ROWS - usedLines <= LINE_HEIGHT ) {
		ScrollScreen(LINE_HEIGHT - (LCD_ROWS-usedLines));
	}
	oled->DrawString(text, SIDE_MARGIN,
		convertY(usedLines), textColor, bgColor);
	usedLines += LINE_HEIGHT;
}

int MessageScroller::convertY(int y) {
	int temp = y + offset;
	if(temp >= LCD_ROWS)
		return temp - LCD_ROWS;
	else
		return temp;
}

void wait(long length) {
    for(long i = 0; i < length; i++) {
      asm("nop");
}}

void MessageScroller::ScrollScreen(int rows) {
	int temp = offset + rows;

	if(temp >= LCD_ROWS) { // we must be spanning the boundary
		temp -= LCD_ROWS;
		//lcd_clear(0, offset, LCD_COLUMNS - 1, LCD_ROWS - 1, &BLACK);
		//lcd_clear(0, 0, LCD_COLUMNS - 1, temp - 1, &BLACK);
		oled->ClearRegion(0, offset, LCD_COLUMNS - 1, LCD_ROWS - 1);
		wait(500);
		if(temp > 0)
			oled->ClearRegion(0, 0, LCD_COLUMNS - 1, temp - 1);
		wait(500);
	} else {
		oled->ClearRegion(0, offset, LCD_COLUMNS - 1, temp - 1);
		wait(500);
	}
	usedLines -= rows;
	offset = temp;
	oled->SetDisplayStartLine(offset);
	return;
}

void MessageScroller::AddFormattedText(char* format, ...) {
	static char msg_buffer[BUFFER_SIZE];

	va_list argList;
	va_start(argList, format);

	vsprintf((char*)&msg_buffer, format, argList);

	va_end(argList);

	AddText(msg_buffer);
}