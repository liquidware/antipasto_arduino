/**
 *
 * BlingBasic
 * Example by Chris Ladden
 *
 * This example demonstrates how make the Illuminato Genesis
 * light up it's 'bling' LED backlighting. The bling is located on the
 * backside of the board.
 *
 * Download this code to the Illuminato Genesis:
 * http://www.liquidware.com/shop/show/ILL/Illuminato%3A%3AGenesis
 */

void setup() {
 pinMode(13, OUTPUT);   //Setup Pin 13
}

void loop(){

  digitalWrite(13, HIGH);  //Pin 13 LED ON
  bling(0);                //Genesis LED backlighting ON

  delay(250);

  bling(1);                //Genesis LED backlighting OFF
  digitalWrite(13, LOW);   //Pin 13 OFF

  delay(250);
}
