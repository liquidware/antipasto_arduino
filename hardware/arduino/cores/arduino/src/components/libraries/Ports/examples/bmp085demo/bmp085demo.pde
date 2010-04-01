// Ports demo, reads out a BMP085 sensor connected via I2C
// 2009-02-17 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: bmp085demo.pde 4727 2009-12-08 21:39:49Z jcw $

#include "Ports.h"
#include "PortsBMP085.h"
#include <RF12.h> // needed to avoid a linker error :(

PortI2C two (2);

BMP085 psensor (two);

void setup() {
    Serial.begin(57600);
    Serial.print("\n[bmp085demo]");
    
    // may omit following call if calculate() won't be called later on
    psensor.getCalibData();
}

void loop() {
    delay(1000);

    Serial.print("\nBMP ");
    
    uint16_t traw = psensor.measure(BMP085::TEMP);
    Serial.print(traw);
    
    uint16_t praw = psensor.measure(BMP085::PRES);
    Serial.print(' ');
    Serial.print(praw);
    
    // omit following code to avoid linking in some complex calculation code
    int16_t temp;
    int32_t pres;
    psensor.calculate(temp, pres);
    Serial.print(' ');
    Serial.print(temp);
    Serial.print(' ');
    Serial.print(pres);
}
