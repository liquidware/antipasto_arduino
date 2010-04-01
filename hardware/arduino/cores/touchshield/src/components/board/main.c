//*******************************************************************************

#include	"wiring.h"
#include	"startup.h"

//*******************************************************************************
int main()
{

  init();

  sketchEarlyInits();

  sketchLateInits();

  runSketch();

  return 0; /* NOT REACHED */
}
