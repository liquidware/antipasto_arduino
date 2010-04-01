// Ports demo, reads out a digital PIR sensor signal and an analog LDR voltage.
// 2009-02-17 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: pir_demo.pde 4727 2009-12-08 21:39:49Z jcw $

#include "Ports.h"

Port pir (3);   // PIR sensor is connected to DIO (pin 2) of port 3
                // to demo analog I/O, connect an LDR from AIO to GND

uint8_t state;  // tracks previous state of the PIR sensor signal

void setup() {
    Serial.begin(57600);
    Serial.print("\n[pir_demo]");
    
    pir.mode(INPUT);
    pir.mode2(INPUT);
    pir.digiWrite2(1); // pull-up
}

void loop() {
    if (pir.digiRead() != state) {
        state = pir.digiRead();
        uint16_t light = pir.anaRead();
        
        Serial.print("\nPIR ");
        Serial.print(state ? "on " : "off");
        Serial.print(" LDR ");
        Serial.print(light);
    }
}
