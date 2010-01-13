/******************************************
 * InputShield_Display
 *      A simple example to read the InputShield hardware
 *      state from the Arduino and Display the state on the TouchShield
 *
 *      By Christopher Ladden @ liquidware.com
 *
 *      For use with the following hardware:
 *      Arduino Duemilanove : http://www.liquidware.com/shop/show/ARD/Arduino+Duemilanove
 *      InputShield : http://www.liquidware.com/shop/show/INPT/InputShield
 *      TouchShield Slide : http://www.liquidware.com/shop/show/TSL/TouchShield+Slide
 *      TouchShield Stealth : http://www.liquidware.com/shop/show/TS/TouchShield+Stealth
 *
 ******************************************/
#include <ArduinoReader.h>

ArduinoReader reader = ArduinoReader();
char buffer[10];
char out[30];
int joyLat;
int joyVert;
uint8_t buttons;
int readLen;

void setup() {
    background(0);
    stroke(255);
    fill(0);

    text("Waiting for Arudino...", 10, 60);
    while ( reader.Read(buffer) ) {
        ; //wait
    }
}

void loop() {

    // Try to get serial data from the Arduino
    readLen = reader.Read(buffer);

    // Check the returned length
    if (readLen) {

        // Store the data, see the Arudino InputShield Library for packet details
        joyLat = (buffer[3] << 8) + buffer[2];
        joyVert = (buffer[5] << 8) + buffer[4];
        buttons = buffer[6];

        // Format the output
        sprintf(out, "JL %d, JV %d, B1 %d, B2 %d, B3 %d, s: %d              ",
                joyLat,
                joyVert,
                ((buttons >> 0) & 1),
                ((buttons >> 1) & 1),
                ((buttons >> 2) & 1),
                readLen);

        // Print the output to the screen
        text(out, 10, 60);
    }
}
