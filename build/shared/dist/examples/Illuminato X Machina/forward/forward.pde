// Forward 'd' packets between east and west only

void doForward(u8 * packet) {

  // Send a copy of the packet out the opposite face of its source; so
  // if source is EAST, print to WEST, and vv.  (NORTH and SOUTH can't
  // arise as sources here, because there are no reflexes for that)

  facePrintln(OPPOSITE_FACE(packetSource(packet)),packet,packetLength(packet)); 
}

void setup() {
  EastFace.reflex('d', doForward);       // doForward handles 'd' from EAST 
  WestFace.reflex('d', doForward);       // and doForward handles 'd' from WEST 
}

void loop() { /* Nothing to do! */ }
