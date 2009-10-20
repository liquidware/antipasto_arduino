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

#ifndef BarGraph_h
#define BarGraph_h

#include <inttypes.h>

class BarGraph
{
   private:
      float currentValue; 
      int   width;
      int   height;
      float min;
      float max;

   public:
      BarGraph(int xPos, 
               int yPos, 
               int width, 
               int height, 
               float min, 
               float max);

      void setBackground(uint8_t red,
                         uint8_t green,
                         uint8_t blue);

      void setRange(float min, float max);   
      void setValue(float value);

      void repaint();
      float getValue();
};

#endif

