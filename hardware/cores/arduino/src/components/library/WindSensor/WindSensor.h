/*
  WindSensor.h - Modern Device Wind Sensor Library
  http://moderndevice.com
  Copyright (c) 2009 Paul Badger, Mike Gionfriddo & Christopher Ladden.  All right reserved.

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

#ifndef WINDSENSOR_H
#define WINDSENSOR_H

#include <inttypes.h>

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

class WindSensor
{
  private:
    uint8_t mySignalPin;
  public:
    WindSensor(uint8_t signalPin);
    int getAirFlow();
    void callibrate(); 
};

#endif
