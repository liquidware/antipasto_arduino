/* Synchronized Blinking LED
 * -------------------------
 *
 * Small twist on the original Arduino blinky, that gradually causes
 * connected SFBs to synchronize their blinking.  
 *
 * It's a slow process!  One way to speed it up is by reducing the
 * waiting period (line 36, below).
 *
 * This code has also been corrected for the fact that on the SFB,
 * the LEDs are 'active low' for better efficiency, meaning they
 * go ON when they are set to LOW!
 *
 * Created 31 May 2009
 * copyleft 2009 David Ackley <http://ackleyshack.com>
 *
 * based on an orginal by H. Barragan for the Wiring i/o board
 */

int ledPin = 13;                // LED connected to digital pin 13
int wait;                       // a variable for the delay time

void seeFlash(u8 *) {           // runs when we receive an 'f' 
  if (digitalRead(ledPin)==LOW) // if we're already on when they flash
    ++wait;                     // wait a little longer
}

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  Body.reflex('f', seeFlash);   // create a reflex triggered by 'f'
}

void loop()
{
  wait = 500;                   // assume we'll wait a half second
  digitalWrite(ledPin, LOW);    // sets the LED on
  println("f");                 // send f packet to all neighbors
  delay(wait);                  // waits for a half second 
  digitalWrite(ledPin, HIGH);   // sets the LED off
  delay(wait);                  // waits for a half second 

  while (buttonDown())          // as an extra feature
    delay(1);                   // push the button to add delay
}

