/*
  ButtonPad.h - Liquidware ButtonPad library
  Copyright (c) 2009 Mike Gionfriddo & Christopher Ladden.  All right reserved.

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

#ifndef BUTTONPAD_H
#define BUTTONPAD_H

#include <inttypes.h>

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

class ButtonPad
{
  private:
    uint8_t pinList[6];
    uint8_t ledList[6];

  public:
    ButtonPad();
    int readButtons(); 
    void ledWrite(uint8_t, uint8_t);
};

#endif
