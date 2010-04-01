// Ports demo, interface to Nintendo's "Nunchuk", which simply uses I2C inside
// 2009-02-17 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: nunchuk_demo.pde 4727 2009-12-08 21:39:49Z jcw $

#include "Ports.h"

PortI2C four (4, PortI2C::KHZ100);

DeviceI2C nunchuk (four, 0x52);

void setup() {
    Serial.begin(57600);
    Serial.print("\n[nunchuk_demo]");

    nunchuk.send();
    nunchuk.write(0x40);
    nunchuk.write(0x00);
    nunchuk.stop();
}

void loop() {
    delay(1000);
    
    Serial.print("\nCHUK ");

    nunchuk.send();
    nunchuk.write(0x00);
    nunchuk.stop();
    
    delay(100);

    nunchuk.receive();
    uint8_t joyX = nunchuk.read(0);
    uint8_t joyY = nunchuk.read(0);
    uint16_t accX = nunchuk.read(0) << 2; 
    uint16_t accY = nunchuk.read(0) << 2;
    uint16_t accZ = nunchuk.read(0) << 2;
    uint8_t flags = nunchuk.read(1);
    
    int zBtn = (flags & 0x01) != 0;
    int cBtn = (flags & 0x02) != 0;
    accX |= (flags >> 2) & 0x03;
    accY |= (flags >> 4) & 0x03;
    accZ |= (flags >> 6) & 0x03;

    Serial.print (joyX, DEC);
    Serial.print (' ');
    Serial.print (joyY, DEC);
    Serial.print (' ');
    Serial.print (accX, DEC);
    Serial.print (' ');
    Serial.print (accY, DEC);
    Serial.print (' ');
    Serial.print (accZ, DEC);
    Serial.print (' ');
    Serial.print (zBtn, DEC);
    Serial.print (' ');
    Serial.print (cBtn, DEC);
}
