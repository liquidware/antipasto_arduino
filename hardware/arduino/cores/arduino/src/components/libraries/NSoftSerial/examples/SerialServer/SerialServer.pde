/* NSoftSerial Library Example                        */
/* Description:                                       */
/* Receives serial messages on pins 0, 1, and 3       */
/* Transmits all serial messages received to pin 2    */

#include <NSoftSerial.h>

#define kTrident1RxPin 0
#define kTrident2RxPin 1
#define kTrident3RxPin 3
#define kBroadcastPin 2

/* Setup serial links */
NSoftSerial trident1 = NSoftSerial(kTrident1RxPin, kBroadcastPin);
NSoftSerial trident2 = NSoftSerial(kTrident2RxPin, kBroadcastPin);
NSoftSerial trident3 = NSoftSerial(kTrident3RxPin, kBroadcastPin);

void setup() {  
  trident1.begin(9600);
  trident2.begin(9600);
  trident3.begin(9600);
}

void loop () {

  if (trident1.available()) {
    trident1.print('1');
    trident1.print((char)trident1.read());
    trident1.print('\0');
  }


  if (trident2.available()) {
    trident2.print('2');
    trident2.print((char)trident2.read());
    trident2.print('\0');
  }
  
   if (trident3.available()) {
   trident3.print('3');
   trident3.print((char)trident3.read());
   trident3.print('\0');
   }

}
