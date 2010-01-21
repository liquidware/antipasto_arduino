//*******************************************************************************
//*	
//*	Sub-Processing 
//*	for TouchShield library
//*		by Mark Sproul
//*		This file is the ONLY file that end users should have to touch
//*		the various "#defines" can be commented out to save space
//*		the normal shipping version should have all of the options turned on
//*		this will allow for development in the Arduino without having
//*		to also write code for the Slide/Stealth graphics cards
//*
//*		It is my intent to provide enough horse power in the graphics card
//*		to allow for full funtion apps in the Arduino without have to write 2 programs
//*
//*		Mark Sproul
//*******************************************************************************
//*	Dec 27,	2008	<MLS> Added SubPOptions.h
//*******************************************************************************
//#include	"SubPOptions.h"





#ifndef _SUBP_OPTIONS_H_

#define _SUBP_OPTIONS_H_

//#define	_SUBP_OPTION_GAMES_		//	nothing implmented yet
#define	_SUBP_OPTION_KEYBOARD_
#define	_SUBP_OPTION_QUICKDRAW_		//	enable QUICKDRAW funtions
#define	_SUBP_OPTION_7_SEGMENT_


#define	_SUPPORT_OLD_LCD_ROUTINE_NAMES_

#ifdef __cplusplus
extern "C"{
#endif



#ifdef __cplusplus
} // extern "C"
#endif


#endif
