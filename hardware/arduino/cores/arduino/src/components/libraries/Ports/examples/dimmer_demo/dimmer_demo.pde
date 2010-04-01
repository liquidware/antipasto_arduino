// Demo for the Dimmer plug
// 2010-03-18 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: dimmer_demo.pde 4881 2010-03-18 21:51:10Z jcw $

#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

PortI2C myBus (1);
DimmerPlug dimmer (myBus, 0x40);
byte brightness;

void setup () {
    Serial.begin(57600);
    Serial.println("\n[dimmer_demo]");
    
    dimmer.setReg(DimmerPlug::MODE1, 0x00);     // normal
    dimmer.setReg(DimmerPlug::MODE2, 0x20);     // blink
    dimmer.setReg(DimmerPlug::GRPPWM, 0x40);    // duty cycle 25%
    dimmer.setReg(DimmerPlug::GRPFREQ, 5);      // blink 4x per sec
    dimmer.setReg(DimmerPlug::LEDOUT0, 0x03);   // LED 0 blinks
}

void loop () {
    dimmer.setReg(DimmerPlug::PWM0, ++brightness);
    delay(250);
}
