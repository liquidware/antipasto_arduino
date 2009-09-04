// Ping pong echo sketch: Get ping, send pong

void doPingPong(u8 * packet) {
  if (packetScanf(packet,"ping\n") != 5)  // Must be what we want
    return;                               // If not, ignore it
  facePrintln(packetSource(packet),"pong");  // Respond to who pinged us
}

void setup() {
  Body.reflex('p', doPingPong);       // set up reflex
}

void loop() { /* Nothing to do! */ }
