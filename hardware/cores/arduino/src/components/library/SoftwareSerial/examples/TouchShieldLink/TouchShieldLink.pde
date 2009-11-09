#include <SoftwareSerial.h>

#define RX_PIN 3
#define TX_PIN 2

SoftwareSerial mySerial = SoftwareSerial(RX_PIN, TX_PIN);

void setup() {
   mySerial.begin(9600);
}


void loop() {

   mySerial.print("Hi TouchShield");
   mySerial.read();
}
