// Demo of the Lux Plug, based on the LuxPlug class in the Ports library
// 2010-03-18 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: lux_demo.pde 4884 2010-03-19 02:34:43Z jcw $

#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(
    
PortI2C myBus (1);
LuxPlug sensor (myBus, 0x39);

void setup () {
    Serial.begin(57600);
    Serial.println("\n[lux_demo]");
    sensor.begin();
}

void loop () {
    const word* p = sensor.getData();
    Serial.print("LUX ");
    Serial.print(p[0]);
    Serial.print(' ');
    Serial.print(p[1]);
    Serial.print(' ');
    Serial.println(sensor.calcLux());
    
    delay(1000);
}
