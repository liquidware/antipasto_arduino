/******************************************
 * ButtonDialog
 *      A simple button dialog example.
 *
 *      By Christopher Ladden @ liquidware.com
 *
 *      For use with the TouchShield from liquidware
 *      TouchShield Slide : http://www.liquidware.com/shop/show/TSL/TouchShield+Slide
 *      TouchShield Stealth : http://www.liquidware.com/shop/show/TS/TouchShield+Stealth
 *
 ******************************************/
#include <TButton.h>

TButton ok = TButton("Ok",20,80);
TButton cancel = TButton("Cancel",80,80);

void setup() {
  ok.Paint();
  cancel.Paint();
}


void loop() {

  if (ok.GetTouch()) {
    stroke(255);
    fill(0);
    text("Pressed 'Ok'    ", 10, 30);
  }

  if (cancel.GetTouch()) {
    stroke(255);
    fill(0);
    text("Pressed 'Cancel'", 10, 30);
  }

}
