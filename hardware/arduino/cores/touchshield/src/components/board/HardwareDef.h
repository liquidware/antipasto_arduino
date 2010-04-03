//*****************************************************************************
//*	This file defines the graphics card
//*	currently there are the TouchShield and TouchSlide
//*
//*	by <MLS> Mark Sproul msproul@jove.rutgers.edu
//*****************************************************************************
//*	Dec 29,	2008	<MLS> Trying to figure out how to use ONE source folder for both
//*	Dec 29,	2008	<MLS> Figured out the __AVR_ARCH__ compile time macro
//*	Mar ??,	2009	<CML> Made some changes
//*	Mar 25,	2009	<MLS> Merging Chris's changes back into my code
//*	Dec 17, 2009	<MLS> Added screen rotation for use with Button Pad
//*****************************************************************************

//#include	"HardwareDef.h"
#ifndef _HARDWARE_DEF_H_
#define	_HARDWARE_DEF_H_

#define	_VERSION_0012LW_
#define	_ENABLE_FLASH_FILE_SYSTEM_

#ifdef __AVR_ARCH__
//	#if (__AVR_ARCH__ == 5)
//		#define	_TOUCH_STEALTH_
//	#endif
//	#if (__AVR_ARCH__ == 6)
//		#define	_TOUCH_SLIDE_
//	#endif
#else
	#error Unable to determine hardware platform (_TOUCH_STEALTH_ / _TOUCH_SLIDE_)
#endif

#ifdef __AVR_ATmega645__
	#define	_TOUCH_STEALTH_
	#warning __AVR_ATmega645__ defined
#endif
#ifdef __AVR_ATmega2560__
	#define	_TOUCH_SLIDE_
	#warning __AVR_ATmega2560__ defined
#endif
#ifdef __AVR_ATmega1280__
	#warning __AVR_ATmega1280__ defined
	#define		_ARDUINO_MEGA_
	#define		_ENABLE_HERSHEY_FONTS_
	#error mega
#endif

#define	nil	0


//*****************************************************************************
#ifdef _TOUCH_STEALTH_
	#define	kDisplayHardwareString	"TouchStealth"
	#define	kDisplayHardwareVersion	"V1.1.0"

	#define	kSCREEN_X_size		128
	#define	kSCREEN_Y_size		128
#endif

//*****************************************************************************
#ifdef _TOUCH_SLIDE_

	#define	kDisplayHardwareString	"TouchSlide"
	#define	kDisplayHardwareVersion	"V1.1.0"

	#define	kSCREEN_X_size		320
	#define	kSCREEN_Y_size		240

//	#define	_ENABLE_SCREEN_ROTATION_
	#ifdef _ENABLE_SCREEN_ROTATION_

	enum	{	kScreenOrientation_Landscape	=	0,
				kScreenOrientation_Portrait,
				kScreenOrientation_Landscape180,
				kScreenOrientation_Portrait180,

				kScreenOrientation_Last
			};
		extern char	gScreenOrientation;
		#define	kDefaultOrienation	kScreenOrientation_Landscape
	#endif

#endif

#endif


#define	_NEW_SERIAL_



#ifdef __MWERKS__
	//*****************************************************************************
	//*	This is for <MLS> to be able to use Codewarrior for editing
	//*	this is so the code compiles under Metrowerks CodeWarrior,
	//*	it doesnt run or link, just compiles
	//*****************************************************************************

	#undef		cli
	#undef		sei
	void		cli();
	void		sei();
	#define		abs(x) (x)
	#define		__MATH_H
	#define		sin(x) x
	#undef		__PGMSPACE_H_
	#define		__PGMSPACE_H_
	#define		prog_uchar unsigned char
	#undef		PROGMEM
	#define		PROGMEM
	#undef		prog_char
	#define		prog_char char
	#undef		pgm_read_byte_near
	#define		pgm_read_byte_near(x)	(char)x
	#undef		pgm_read_word
	#define		pgm_read_word(x)		(int)x
	void		sprintf(char *foo, ...);
	void		ltoa(long displayValue, char *displayString, int x);	//*	convert to ascii string

//	#define		random(x,y)	(y)
	void		dtostrf(int a, int b, int c, char *out);
//	int			srand(int seed);
//	int			rand();

//	#undef		CLRBIT
//	#define		CLRBIT(x,y)	x=y

//	extern	int	PORTL;
//	extern	int	PORTJ;
//	extern	int	DDRJ;
//	extern	int	PINJ;
//	extern	int	DDRL;
#endif


