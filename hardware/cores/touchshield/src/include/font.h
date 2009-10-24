//*******************************************************************************
//*	
//*	                             Font handling for
//*	                             Sub-Processing 
//*                        for TouchShield library
//*	         by inthebitz @ antipastohw.blogspot.com & liquidware.com
//*	
//*******************************************************************************
//*	Jan 21,	2009	<MLS> Started working on large fonts
//*	Jan 22,	2009	<MLS> Got Hershey fonts fully working
//*******************************************************************************
//*	\file Font.h 
//*	\brief	Include all font definitions here.
//*			Fonts are stored in program memory
//*******************************************************************************
//*	if you want to enable or disable various fonts THIS is the place to do it.
//*	disabling fonts can free up LOTS of memory.
//*	to disable a font, just comment out the appropriate ENABLE line
//*******************************************************************************

#ifndef _FONT_H_
#define _FONT_H_

#ifndef _HARDWARE_DEF_H_
	#include	"HardwareDef.h"
#endif

#define	_ENABLE_LARGE_NUMBERS_					//*	1138 bytes


//*******************************************************************************
//*	the rest of these fonts are only practical on the TouchShield-Slide
//*******************************************************************************
#ifdef _TOUCH_SLIDE_
	#define	_ENABLE_XLARGE_NUMBERS_					//*	6974 bytes
#endif

#define	_ENABLE_HERSHEY_FONTS_					//*	base fonts and code = 3984 bytes


#ifndef _AVR_IO_H_
	#include <avr/io.h>
#endif
#ifndef __PGMSPACE_H_
	#include <avr/pgmspace.h>
#endif
#ifndef GRAPHICS_H
	#include "graphics.h"
#endif


#ifdef __cplusplus
	extern "C"{
#endif


	void	dispPutC(unsigned char ch, unsigned int x_pos,unsigned int y_pos,COLOR fc, COLOR bc);
	void	dispPutS(char * string,unsigned int x_pos, unsigned int y_pos, COLOR fc, COLOR bc);
	void	dispPutLargeNum(unsigned char digit, unsigned int x_pos,unsigned int y_pos,COLOR fc, COLOR bc);
#ifdef	_ENABLE_LARGE_NUMBERS_
	void	dispPutLargeNumString(char * string, unsigned int x_pos, unsigned int y_pos, COLOR fc, COLOR bc);
#endif

#ifdef	_ENABLE_XLARGE_NUMBERS_
	void	dispPutXLargeNumString(char * string, unsigned int x_pos, unsigned int y_pos, COLOR fc, COLOR bc);
#endif

#ifdef _ENABLE_HERSHEY_FONTS_
	//*	see source file for argument descriptions (some of the "char" args are 8 bit ints)
	void	HersheyDrawCString(char fontNumber, short xloc, short yloc, char *theCstr, char fontSize, short fontAngle, char penSize);
	void	GetHersheyFontName(char fontNumber, char *fontName);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif




