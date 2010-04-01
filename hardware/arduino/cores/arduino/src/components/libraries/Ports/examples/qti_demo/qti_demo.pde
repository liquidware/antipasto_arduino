// Port interface to the Parallax QTI sensor #27401
// 2009-02-18 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: qti_demo.pde 4727 2009-12-08 21:39:49Z jcw $

#include "Ports.h"

// the QTI sensor is connected to port 1 as follows:
// B = pin 3 (GND), R = pin 4 (AIO), W = pin 5 (VCC)

Port qti (1);

void setup() {
    Serial.begin(57600);
    Serial.print("\n[qti_demo]");
    
    qti.mode2(INPUT);
    qti.digiWrite2(1); // pull-up
}

void loop() {
    delay(1000);
    
    Serial.print("\nQTI ");
    Serial.print(qti.anaRead());
}
