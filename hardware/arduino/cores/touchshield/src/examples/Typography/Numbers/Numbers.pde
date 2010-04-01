/**
 * Numbers
 * Example by Chris Ladden
 *
 * Draw numbers using different techniques on the TouchShield
 */

#include "HersheyFonts.h"   //only needed for dispPutLargeNumString()

COLOR black = {0, 0, 0};
COLOR blue =  {0, 0, 255};
char out[10];

void setup() {
    background(0);
    stroke(0, 0, 255);   //set the font color
}

void loop() {

    dtostrf(123.45, 4, 2, out);     //make a number into a
                                    //character string with a
                                    //width of 4 and precision of 2

    text(out, 30, 10);              //draw text at standard font size

    text(out, 30, 40, 20);          //draw text at a font size of 20

    dispPutLargeNumString(out, 30, 80, blue, black);     //large, thick numbers
}
