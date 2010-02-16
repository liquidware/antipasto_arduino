/*
  BarGraph - A Library for the Liquidware TouchShield

  http://www.liquidware.com/shop/show/TSL/TouchShield+Slide

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
#include <stdlib.h>
#include "wiring.h"
#include "SubPGraphics.h"
#include "BarGraph.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/* none */

/******************************************************************************
 * Statics
 ******************************************************************************/

/* none */

/******************************************************************************
 * Constructors
 ******************************************************************************/

BarGraph::BarGraph(char * text,
                   int x,
                   int y) {
    xLocation = x;
    yLocation = y;
    graphWidth = 100;
    graphHeight = 40;
    prevValue = 1.0;
    currValue = 25.0;
    valueClamped = 25.0;

    fgColorR = 0;
    fgColorG = 0;
    fgColorB = 255;

    bgColorR = 0;
    bgColorG = 0;
    bgColorB = 0;

    minValue = 0.0;
    maxValue = 50.0;

    sprintf(graphLabel, text);
    paint();
}

/**
 * Paint the BarGraph
 */
void BarGraph::paint(void) {
    char outStr[25];
    char outValueStr[6];

    //draw bargraph Background and outine
    stroke(fgColorR,
           fgColorG,
           fgColorB);

    fill(bgColorR,
         bgColorG,
         bgColorB);

    rect(xLocation,
         yLocation,
         graphWidth,
         graphHeight);

    //draw the bargraph label and value
    dtostrf(currValue, 5, 1, outValueStr);
    sprintf(outStr,"%s %s", graphLabel, outValueStr);
    text(outStr,
         xLocation,
         yLocation-10);

    //draw the bar
    noStroke();
    fill(fgColorR,
         fgColorG,
         fgColorB);
    rect(xLocation,
         yLocation,
         valueClamped * ((float)graphWidth / (maxValue - minValue)),
         graphHeight);


}

/******************************************************************************
 * User API
 ******************************************************************************/


/**
 * Set the current value of the bar
 * graph and repaint the graph
 *
 * @param value The value to display in the graph
 */
void BarGraph::setValue(float value) {
    currValue = value;

    //Range Clamp
    if (value > maxValue) {
        value = maxValue;
    }
    if (value < minValue) {
        value = minValue;
    }

    valueClamped = value;

    paint();
}


/**
 * Set the Bar Graph min and max range values.
 *
 * The min and max values determine the
 * scaling of the graph
 *
 *
 * @param min The minimum value that setValue() will see
 * @param max The maximum value that setValue() will see
 */
void BarGraph::setRange(float min,
                        float max) {
    minValue = min;
    maxValue = max;
}

/**
 * Set the background color of the graph.
 *
 *
 * @param red
 * @param green
 * @param blue
 */
void BarGraph::setBackground(uint8_t red,
                 uint8_t green,
                 uint8_t blue) {
    bgColorR = red;
    bgColorG = green;
    bgColorB = blue;
}

/**
 * Set the foreground color of the graph. This is the outline
 * and text color.
 *
 *
 * @param red
 * @param green
 * @param blue
 */
void BarGraph::setForeground(uint8_t red,
                             uint8_t green,
                             uint8_t blue) {
    fgColorR = red;
    fgColorG = green;
    fgColorB = blue;
}

/**
 * Set the size of the Graph
 *
 * @param width
 * @param height
 */
void BarGraph::setSize(int width,
                       int height) {

    graphWidth = width;
    graphHeight = height;

}

/**
 * Set the location of the Graph on the screen.
 *
 * @param x
 * @param y
 */
void BarGraph::setLocation(int x,
                           int y) {
    xLocation = x;
    yLocation = y;
}

/**
 * Set the graph's label
 *
 *
 * @param text The text label to display
 */
void BarGraph::setText(char * text) {
    sprintf(graphLabel,"%s",text);
}

