#include <ButtonPad.h>

//Create a new ButtonPad
ButtonPad MyPad = ButtonPad();

void setup()
{
   Serial.begin(9600);
}

void loop()
{
  //Display the pressed buttons to the PC
  Serial.println(MyPad.readButtons());
  
  delay(250);
  
  //Random LED blinkage
  MyPad.ledWrite(random(0,6),HIGH);  
  MyPad.ledWrite(random(0,6),LOW);  
}
