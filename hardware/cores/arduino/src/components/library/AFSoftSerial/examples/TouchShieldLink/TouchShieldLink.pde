#include <AFSoftSerial.h>

#define RX_PIN 3
#define TX_PIN 2

AFSoftSerial mySerial = AFSoftSerial(RX_PIN, TX_PIN);

void setup() {
   mySerial.begin(9600);
}


void loop() {
        
   mySerial.print("Hi TouchShield");

   if (mySerial.available()) {
      mySerial.read();
   }
}
