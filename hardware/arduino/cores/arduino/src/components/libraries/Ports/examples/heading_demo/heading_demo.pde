// Demo of the Heading Board based on the HDPM01 (with barometer and compass)
// 2010-03-22 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: heading_demo.pde 4888 2010-03-23 16:28:07Z jcw $

#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

HeadingPlug sensor (4);

void setup () {
    Serial.begin(57600);
    Serial.println("\n[heading_demo]");
    sensor.begin();
}

void loop () {
    int temp, pres, xaxis, yaxis;
    sensor.pressure(temp, pres);
    sensor.heading(xaxis, yaxis);

    Serial.print("HDPM ");
    Serial.print(temp);
    Serial.print(' ');
    Serial.print(pres);
    Serial.print(' ');
    Serial.print(xaxis);
    Serial.print(' ');
    Serial.println(yaxis);
    
    delay(1000);
}
