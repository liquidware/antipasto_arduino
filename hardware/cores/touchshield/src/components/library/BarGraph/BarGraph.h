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

#ifndef BarGraph_h
#define BarGraph_h

#include <inttypes.h>

class BarGraph
{
   private:
      float prevValue;
      float currValue;
      float valueClamped;
      int   xLocation;
      int   yLocation;
      int   graphWidth;
      int   graphHeight;
      float minValue;
      float maxValue;
      char  graphLabel[25];
      uint8_t fgColorR, fgColorG, fgColorB;
      uint8_t bgColorR, bgColorG, bgColorB;

      void paint(void);

   public:
      BarGraph(char * text,
               int x,
               int y);

      void setLocation(int x,
                       int y);

      void setSize(int width,
                   int height);

      void setRange(float min,
                    float max);

      void setBackground(uint8_t red,
                         uint8_t green,
                         uint8_t blue);

      void setForeground(uint8_t red,
                         uint8_t green,
                         uint8_t blue);

      void setText(char * text);
      void setValue(float value);
};

#endif

