//*****************************************************************************
//*	This file defines the graphics card
//*	currently there are the TouchShield and TouchSlide
//*
//*	by <MLS> Mark Sproul msproul@jove.rutgers.edu
//*****************************************************************************
//*	Dec 29,	2008	<MLS> Trying to figure out how to use ONE source folder for both
//*	Dec 29,	2008	<MLS> Figured out the __AVR_ARCH__ compile time macro
//*****************************************************************************

//#include	"HardwareDef.h"
#ifndef _HARDWARE_DEF_H_
#define	_HARDWARE_DEF_H_

#define	_VERSION_0012LW_

#ifdef __AVR_ARCH__
	#if (__AVR_ARCH__ == 5)
		#define	_TOUCH_STEALTH_
	#endif
	#if (__AVR_ARCH__ == 6)
		#define	_TOUCH_SLIDE_
	#endif
#else
	#error Unable to determine hardware platform (_TOUCH_STEALTH_ / _TOUCH_SLIDE_)
#endif

#ifdef __AVR_ATmega128__
	#warning __AVR_ATmega128__ defined
#endif
#ifdef __AVR_ATmega2560__
	#warning __AVR_ATmega2560__ defined
#endif

#define	nil	0

//*****************************************************************************
#ifdef _TOUCH_STEALTH_
	#define	kDisplayHardwareString	"TouchStealth"

	#define	kSCREEN_X_size		128
	#define	kSCREEN_Y_size		128
#endif

//*****************************************************************************
#ifdef _TOUCH_SLIDE_
	#define	_ENABLE_FLASH_FILE_SYSTEM_

	#define	kDisplayHardwareString	"TouchSlide"

	#define	kSCREEN_X_size		320
	#define	kSCREEN_Y_size		240
#endif

#define	_NEW_SERIAL_



#ifdef __MWERKS__
	//*****************************************************************************
	//*	this is so the code compiles under Metrowerks CodeWarrior,
	//*	it doesnt run or link, just compiles
	//*****************************************************************************
	#undef		cli
	#undef		sei
	#define		__MATH_H
	#define		sin(x) x
	#undef		__PGMSPACE_H_
	#define		__PGMSPACE_H_
	#define		prog_uchar unsigned char
	#undef		PROGMEM
	#define		PROGMEM
	#undef		prog_char
	#define		prog_char char
//	#undef		pgm_read_byte_near
	#undef		pgm_read_byte_near
	#define		pgm_read_byte_near(x)	(char)x
	#undef		pgm_read_word
	#define		pgm_read_word(x)		(int)x
//	#define		random(x,y)	(y)
	void		dtostrf(int a, int b, int c, char *out);
	
//	#undef		CLRBIT
//	#define		CLRBIT(x,y)	x=y

//	extern	int	PORTL;
//	extern	int	PORTJ;
//	extern	int	DDRJ;
//	extern	int	PINJ;
//	extern	int	DDRL;
#endif


#endif
