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

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>
#include <string.h>
#include "wiring.h"
#include "TButton.h"

/******************************************************************************
 * Statics
 ******************************************************************************/

/* none */

/******************************************************************************
 * Constructors
 ******************************************************************************/

TButton::TButton(char * text, int16_t left, int16_t top) {
    this->left = left;
    this->top = top;
    touchState = false;
    touchStatePrev = false;

    SetText(text);

    SetForeground(255, 255, 255);
    SetBackground(0, 255, 0);
}

/******************************************************************************
 * User API
 ******************************************************************************/

bool TButton::GetTouch(void) {
    bool pointInRect;
    bool isTouching;

    isTouching = gettouch();

    pointInRect = ( (mouseX >= left) &&
                    (mouseY >= top) &&
                    (mouseX <= (right)) &&
                    (mouseY <= (bottom)));

    if (pointInRect) {
        touchState = isTouching;
    } else {
        touchState = false;
    }

    /* Check for a touch state change */
    if (touchState != touchStatePrev) {
        if (touchState) {
            /* Set the background color to the depressed color */
            bColorRed = pColorRed;
            bColorGreen = pColorGreen;
            bColorBlue = pColorBlue;
        } else {
            /* Reset the backgorund color */
            bColorRed = backColorRed;
            bColorGreen = backColorGreen;
            bColorBlue = backColorBlue;
        }

        /* Save the previous */
        touchStatePrev = touchState;
        Paint();
    }

    return  touchState;
}

void TButton::SetText(char * text) {

    /* Erase the boundary */
    fill(0,0,0);
    stroke(0,0,0);

    rect(left,
         top,
         right - left,
         bottom - top);

    /* Store the text */
    strncpy(buttonText,
            text,
            kButtonTextMaxWidth);

    textWidth = (kCharWidth * strlen(buttonText));  //calculate text width
    right = left + textWidth + 14;                  //calculate right dimension based on text width
    bottom = top + (kCharHeight * 5);               //calculate bottom dimension based on character height

    textLeft = left +
               ((right - left)/2) -
               (textWidth / 2);
    textTop  = top +
               ((bottom - top) / 2) -
               kCharHeightHalf;

    Paint();
}

void TButton::SetForeground(uint8_t red, uint8_t green, uint8_t blue) {
    fColorRed = red;
    fColorGreen = green;
    fColorBlue = blue;
    Paint();
}

void TButton::SetBackground(uint8_t red, uint8_t green, uint8_t blue) {

    /* Set the background color */
    backColorRed = red;
    backColorGreen = green;
    backColorBlue = blue;

    bColorRed = red;
    bColorGreen = green;
    bColorBlue = blue;

    /* Set the depressed color */
    pColorRed = red ^ 0xFF;
    pColorGreen = green ^ 0xFF;
    pColorBlue = blue ^ 0xFF;

    Paint();
}

void TButton::Paint(void) {

    stroke(0,0,0);

    fill(bColorRed,
         bColorGreen,
         bColorBlue);

    rect(left,
         top,
         right - left,
         bottom - top);

    stroke(fColorRed,
           fColorGreen,
           fColorBlue);

    /* Draw the outline */
    line(left+3,     //top line
         top,
         right-3,
         top);

    line(left+3,     //bottom line
         bottom,
         right-3,
         bottom);

    line(left,       //left line
         top+3,
         left,
         bottom-3);

    line(right,      //right line
         top+3,
         right,
         bottom-3);

    /* Draw the radius corners */
    stroke(fColorRed,
           fColorGreen,
           fColorBlue);
    point(left + 2, top + 1);
    point(left + 1, top + 2);

    point(right - 2, top + 1);
    point(right - 1, top + 2);

    point(left + 2, bottom - 1);
    point(left + 1, bottom - 2);

    point(right - 2, bottom - 1);
    point(right - 1, bottom - 2);

    stroke(0,
           0,
           0);

    point(left + 1, top + 1);
    point(right - 1, top + 1);
    point(left + 1, bottom - 1);
    point(right - 1, bottom - 1);

    stroke(fColorRed,
           fColorGreen,
           fColorBlue);

    /* Draw the text */
    text(buttonText, textLeft, textTop);
}

