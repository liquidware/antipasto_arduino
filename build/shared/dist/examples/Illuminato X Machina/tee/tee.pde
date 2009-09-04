// Forward otherwise unhandled packets between east and west only,
// logging north a copy of all forwarded packets

void doForwardTee(u8 * packet) {

  // Send a copy of the packet out the opposite face of its source; so
  // if source is EAST, print to WEST, and vv.  (NORTH and SOUTH can't
  // arise as sources here, because there are no reflexes for that)

  facePrintln(OPPOSITE_FACE(packetSource(packet)),packet,packetLength(packet)); 

  // In addition, log forwarded packets to the north.  We'll send
  // packet type 'L's, which is conventionally used for logging by the
  // core software

  facePrint(NORTH,"L from ");
  facePrint(NORTH,FACE_CODE(packetSource(packet))); // prints 'E' or 'W' (in this case)
  facePrint(NORTH,": ");
  facePrintln(NORTH,packet,packetLength(packet)); 
}

void setup() {
  EastFace.otherwise(doForwardTee);    // Make doForwardTee the catch-all for EAST 
  WestFace.otherwise(doForwardTee);    // Make doForwardTee the catch-all for WEST 
}
void loop() { /* Nothing to do! */ }
