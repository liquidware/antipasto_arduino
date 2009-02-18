#ifndef MESSAGE_SCROLLER_H
#define MESSAGE_SCROLLER_H

//#include "DataTypes.h"
#include "graphics.h"
#include "Oled.h"
#include "oled_stealth.h"

class MessageScroller {
	private:
		int usedLines;
		int offset;
		COLOR* textColor;
		COLOR* bgColor;
		Oled* oled;
		int convertY(int y) ;
	public:
		MessageScroller(Oled* oled, COLOR* textColor, COLOR* bgColor);
		void AddFormattedText(char* format, ...);
		void AddText(char* text);
		void ScrollScreen(int rows);
};


#endif