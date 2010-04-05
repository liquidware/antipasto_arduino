/*
  TButton - A Button Library for use with the TouchShield from liquidware
            TouchShield Slide : http://www.liquidware.com/shop/show/TSL/TouchShield+Slide
            TouchShield Stealth : http://www.liquidware.com/shop/show/TS/TouchShield+Stealth

  Copyright (c) 2009 Christopher Ladden.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef TBUTTON_H
#define TBUTTON_H

#include <inttypes.h>

#define kButtonTextMaxWidth   25
#define kCharHeight           7
#define kCharWidth            6
#define kCharHeightHalf      (kCharHeight/2)

class TButton
{
    private:
        char buttonText[kButtonTextMaxWidth];
        int16_t left;
        int16_t top;
        int16_t right;
        int16_t bottom;
        int16_t textLeft;
        int16_t textTop;
        int16_t textWidth;
        uint8_t fColorRed;
        uint8_t fColorGreen;
        uint8_t fColorBlue;
        uint8_t bColorRed;
        uint8_t bColorGreen;
        uint8_t bColorBlue;
        uint8_t backColorRed;
        uint8_t backColorGreen;
        uint8_t backColorBlue;
        uint8_t pColorRed;
        uint8_t pColorGreen;
        uint8_t pColorBlue;
        bool touchState;
        bool touchStatePrev;
        void flashButton(void);

    public:
        TButton(char * text, int16_t left, int16_t top);
        bool GetTouch(void);
        void SetText(char * text);
        void SetForeground(uint8_t red, uint8_t green, uint8_t blue);
        void SetBackground(uint8_t red, uint8_t green, uint8_t blue);
        void Paint(void);
};

#endif
