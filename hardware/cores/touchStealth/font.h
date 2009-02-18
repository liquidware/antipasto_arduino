/*! \file Font.h
	\brief	Include all font definitions here.
			Fonts are stored in program memory
*/
#ifndef _FONT_H_
#define _FONT_H_
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C"{
#endif

#include "graphics.h"

void dispPutC(unsigned char ch, unsigned int x_pos,unsigned int y_pos,COLOR fc, COLOR bc);
void dispPutS(char * string,unsigned int x_pos, unsigned int y_pos, COLOR fc, COLOR bc);
extern unsigned char *font_5x7;
#ifdef __cplusplus
} // extern "C"
#endif

#endif




