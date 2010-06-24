/**
 *
 * PulsingSleepButton
 * Example by Matt Stack
 *
 * This program pulses the backlights of the
 * Illuminato Genesis like the Apple sleep function on
 * Mac laptops. You can program and alter the pulsing
 * speed and intensity, and also control the rate at which
 * the white LED’s fade in or out.
 *
 * Download this code to the Illuminato Genesis:
 * http://www.liquidware.com/shop/show/ILL/Illuminato%3A%3AGenesis
 */


void setup ( void) {

}

void loop(void) {
  fadein(1);
  fadeout(4);
  delay(300);
}

void fadein (int timer) {
  for (int b=20;b>0;b--) {
    for (int i=0; i<timer;i++) {
    bling(ON);
    delay(20-b);
    bling(OFF);
    delay(b);
    }
  }
}

void fadeout (int timer) {
  for (int b=0;b<20;b++) {
    for (int i=0; i<timer;i++) {
    bling(ON);
    delay(20-b);
    bling(OFF);
    delay(b);
    }
  }
}
