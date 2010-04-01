// Demo of the Gravity Plug, based on the GravityPlug class in the Ports library
// 2010-03-19 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: gravity_demo.pde 4884 2010-03-19 02:34:43Z jcw $

#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

PortI2C myBus (1);
GravityPlug sensor (myBus);

void setup () {
    Serial.begin(57600);
    Serial.println("\n[gravity_demo]");
    sensor.begin();
}

void loop () {
    const int* p = sensor.getAxes();
    Serial.print("GRAV ");
    Serial.print(p[0]);
    Serial.print(' ');
    Serial.print(p[1]);
    Serial.print(' ');
    Serial.println(p[2]);
    
    delay(1000);
}
