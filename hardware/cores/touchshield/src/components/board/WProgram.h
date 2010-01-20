#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <avr/interrupt.h>
#include <avr/signal.h>


#include "wiring.h"

#include "HardwareSerial.h"
#ifndef SUBPGRAPHICS_H
	#include "SubPGraphics.h" //enable SubPGrahpics by default
#endif

#ifdef __cplusplus


// random prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

#endif
