// Ping pong echo sketch 2: Press button to ping, flash LED on pong

const int ledPin = BODY_RGB_GREEN_PIN;

void doPingPong(u8 * packet) {
  if (packetScanf(packet,"ping\n") != 5)  // Must be what we want
    return;                               // If not, ignore it
  facePrintln(packetSource(packet),"pong");  // Respond to who pinged us
  ledOn(ledPin);                             // Show we ponged
}

void setup() {
  Body.reflex('p', doPingPong);       // set up reflex
}

void loop() { 
  if (buttonDown()) {           // See if the button is pressed.
    println("ping");            // It is!  Ping all the neighbors
    while (buttonDown()) {      // Wait here for the button to be 
      delay(10);                // released (avoid pinging _too_ fast)
    }
  }

  delay(100);                   // Wait a tenth of a second
  ledOff(ledPin);               // Turn off the light (in case it was on)
}
