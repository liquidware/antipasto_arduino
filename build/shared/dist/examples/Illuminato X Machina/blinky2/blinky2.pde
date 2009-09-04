/* Blinking the SFB Blue LED
 * -------------------------
 *
 */

int ledPin = BODY_RGB_BLUE_PIN;

void setup()
{
  // Nothing needed: The RGB LEDs are automatically set as output pins
}

void loop()
{
  ledOn(ledPin);                // sets the LED on
  delay(1000);                  // waits for a second
  ledOff(ledPin);               // sets the LED off
  delay(1000);                  // waits for a second
}
