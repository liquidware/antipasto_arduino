/*
  BarGraph.h - A TouchShield Library
  Copyright (c) 2009 Chris Ladden.  All right reserved.

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

BarGraph::BarGraph(int xPos, 
                   int yPos, 
                   int width, 
                   int height, 
                   float min, 
                   float max) {


}

/******************************************************************************
 * User API
 ******************************************************************************/


/**
 *
 * @param red   index for the red color 
 * @param green index for the green color
 * @param blue  index for the blue color
 */
void BarGraph::setBackground(uint8_t red,
                             uint8_t green,
                             uint8_t blue) {


}

void BarGraph::setRange(float min, float max) {

}

void BarGraph::setValue(float value) {

}

void BarGraph::repaint() {
   
}

float BarGraph::getValue() {

   return 0.0;
}
