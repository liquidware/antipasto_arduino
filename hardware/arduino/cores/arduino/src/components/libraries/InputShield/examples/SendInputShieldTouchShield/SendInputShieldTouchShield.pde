#include <AFSoftSerial.h>
#include <InputShield.h>

#define RX_PIN  3    /* TouchShield's Tx Pin */
#define TX_PIN  2    /* TouchShield's Rx Pin */

/* Create a serial connection to the TouchShield */
AFSoftSerial mySerial =  AFSoftSerial(RX_PIN, TX_PIN);

/* Create InputShield on modes A and pass it software serial */
InputShield inputShield = InputShield(0, mySerial);

void setup() {
}

void loop() {
  inputShield.SendHardwareState(mySerial);
  
  delay(100);
}
