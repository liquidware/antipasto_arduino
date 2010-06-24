/**
 *
 * AnalogPrint
 * Example by Chris Ladden
 *
 * This example demonstrates how make the Illuminato Genesis
 * print out multiple analog valuee to the PC.
 *
 * Download this code to the Illuminato Genesis:
 * http://www.liquidware.com/shop/show/ILL/Illuminato%3A%3AGenesis
 */

void setup() {
 pinMode(13, OUTPUT);   //Setup Pin 13
 Serial.begin(9600);    //Start PC serial communications
}

char out[30];

void loop(){

  digitalWrite(13, HIGH);  //Pin 13 LED ON
  bling(0);                //Genesis LED backlighting ON

  delay(250);

  bling(1);                //Genesis LED backlighting OFF
  digitalWrite(13, LOW);   //Pin 13 OFF

  delay(250);

  //build a string of analog signals
  sprintf(out, "a0:%d a1:%d a2:%d a3:%d a4:%d a5:%d\n", analogRead(0),
                                                        analogRead(1),
                                                        analogRead(2),
                                                        analogRead(3),
                                                        analogRead(4),
                                                        analogRead(5));

  //print to the PC
  Serial.print(out);
}
