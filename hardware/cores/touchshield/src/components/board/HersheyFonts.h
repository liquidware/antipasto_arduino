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
//*	Jan 23,	2009	<MLS> Added font name as the first string
//*******************************************************************************
//*	if you want to enable or disable various fonts THIS is the place to do it.
//*	disabling fonts can free up LOTS of memory.
//*	to disable a font, just comment out the appropriate ENABLE line
//*******************************************************************************
//#include	"HersheyFonts.h"

#ifndef _HESHEYFONT_H_
#define	_HESHEYFONT_H_

#ifndef Wiring_h
	#include	"wiring.h"
#endif

#ifndef _HARDWARE_DEF_H_
	#include	"HardwareDef.h"
#endif


#if defined(_TOUCH_STEALTH_) || defined(_TOUCH_SLIDE_)
	#ifndef _FONT_H_
		#include	"font.h"
	#endif
#endif

#if defined(_ARDUINO_MEGA_)
	#ifndef _PICASO_H_
		#include	"PicasoVGA.h"
	#endif
#endif

#ifdef __MWERKS__
//	typedef uint8_t boolean;
//	typedef uint8_t byte;
#endif


#ifdef _ENABLE_HERSHEY_FONTS_
	//*	4882 base font and code
	
//	#define	_ENABLE_ROTATED_HERSHEY_FONTS_		//*	1252 bytes (code only, no data)

//	#define	_ENABLE_HERSHEY_CURSIVE_			//*	4656 bytes
//	#define	_ENABLE_HERSHEY_FUTURAL_			//*	3124 bytes
//	#define	_ENABLE_HERSHEY_FUTURAM_			//*	5484 bytes
//	#define	_ENABLE_HERSHEY_GREEK_				//*	3426 bytes
//	#define	_ENABLE_HERSHEY_GREEK_SIMPLEX_		//*	3278 bytes
//	#define	_ENABLE_HERSHEY_GREEK_COMPLEX_		//*	5260 bytes
//	#define	_ENABLE_HERSHEY_TIMES_GREEK_		//*	5456 bytes
//	#define	_ENABLE_HERSHEY_TIMES_ITALIC_		//*	5610 bytes
//	#define	_ENABLE_HERSHEY_TIMES_ITALIC_BOLD_	//*	8444 bytes
//	#define	_ENABLE_HERSHEY_TIMES_ROMAN_		//*	5306 bytes
//	#define	_ENABLE_HERSHEY_TIMES_ROMAN_BOLD_	//*	8734 bytes

//	#define	_ENABLE_HERSHEY_ROWMAN_SIMPLEX_		//*	3103 bytes
//	#define	_ENABLE_HERSHEY_ROWMAN_DUPLEX_		//*	5806 bytes
	#define	_ENABLE_HERSHEY_ROWMAN_TRIPLEX_		//*	8749 bytes
	
//	#define	_ENABLE_HERSHEY_SCRIPT_SIMPLEX_		//*	5161 bytes
//	#define	_ENABLE_HERSHEY_SCRIPT_COMPLEX_		//*	6219 bytes


//	#define	_ENABLE_HERSHEY_ASTROLOGY_			//*	8866 bytes
//	#define	_ENABLE_HERSHEY_MARKERS_			//*	1574 bytes
//	#define	_ENABLE_HERSHEY_MATH_				//*	6606 bytes (3358 for MathLow and 3248 for MathUP)
//	#define	_ENABLE_HERSHEY_METEOROLOGY_		//*	3398 bytes
//	#define	_ENABLE_HERSHEY_MUSIC_				//*	6622 bytes
//	#define	_ENABLE_HERSHEY_SYMBOLIC_			//*	4530 bytes

//	#define	_ENABLE_HERSHEY_GOTHIC_ENGLISH_		//*	9752 bytes
//	#define	_ENABLE_HERSHEY_GOTHIC_GERMAN_		//*	9942 bytes
//	#define	_ENABLE_HERSHEY_GOTHIC_ITALIAN_		//*	8023 bytes
//	#define	_ENABLE_HERSHEY_GOTHIC_GBT_			//*	9731 bytes
//	#define	_ENABLE_HERSHEY_GOTHITT_			//*	8042 bytes
	
//	#define	_ENABLE_HERSHEY_CYRILLIC_			//*	11649 bytes (6015 for Cyrillic and 5634 for Cyrillic 1)

//	#define	_ENABLE_HERSHEY_JAPANESE_			//*	8000 +++


#ifdef _ENABLE_TAHOMA_FONTS_
	#define	kHershey_FirstFontNumber	3
#else
	#define	kHershey_FirstFontNumber	1
#endif

enum	{	kHersheyFont_Default	=	kHershey_FirstFontNumber,
	#ifdef _ENABLE_HERSHEY_CURSIVE_
			kHersheyFont_Cursive,
	#endif
	#ifdef _ENABLE_HERSHEY_FUTURAL_
			kHersheyFont_Futural,
	#endif
	#ifdef _ENABLE_HERSHEY_FUTURAM_
			kHersheyFont_Futuram,
	#endif


	#ifdef _ENABLE_HERSHEY_GREEK_
			kHersheyFont_Greek,
	#endif
	#ifdef _ENABLE_HERSHEY_GREEK_SIMPLEX_
			kHersheyFont_GreekSimplex,
	#endif
	#ifdef _ENABLE_HERSHEY_GREEK_COMPLEX_
			kHersheyFont_GreekComplex,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_GREEK_
			kHersheyFont_TimesGreek,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_ITALIC_
			kHersheyFont_TimesItalic,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_ITALIC_BOLD_
			kHersheyFont_TimesItalicBold,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_ROMAN_
			kHersheyFont_TimesRoman,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_ROMAN_BOLD_
			kHersheyFont_TimesRomanBold,
	#endif
	#ifdef _ENABLE_HERSHEY_ROWMAN_SIMPLEX_
			kHersheyFont_RowmanSimplex,
	#endif
	#ifdef _ENABLE_HERSHEY_ROWMAN_DUPLEX_
			kHersheyFont_RowmanDuplex,
	#endif
	#ifdef _ENABLE_HERSHEY_ROWMAN_TRIPLEX_
			kHersheyFont_RowmanTriplex,
	#endif


	#ifdef _ENABLE_HERSHEY_SCRIPT_SIMPLEX_
			kHersheyFont_ScriptSimplex,
	#endif
	#ifdef _ENABLE_HERSHEY_SCRIPT_COMPLEX_
			kHersheyFont_ScriptComplex,
	#endif
	#ifdef _ENABLE_HERSHEY_ASTROLOGY_
			kHersheyFont_Astrology,
	#endif
	#ifdef _ENABLE_HERSHEY_MARKERS_
			kHersheyFont_Markers,
	#endif
	#ifdef _ENABLE_HERSHEY_MATH_
			kHersheyFont_Mathlow,
			kHersheyFont_Mathupp,
	#endif
	#ifdef _ENABLE_HERSHEY_METEOROLOGY_
			kHersheyFont_Meteorology,
	#endif
	#ifdef _ENABLE_HERSHEY_MUSIC_
			kHersheyFont_Music,
	#endif
	#ifdef _ENABLE_HERSHEY_SYMBOLIC_
			kHersheyFont_Symbolic,
	#endif
	#ifdef _ENABLE_HERSHEY_GOTHIC_ENGLISH_
			kHersheyFont_GothicEnglish,
	#endif
	#ifdef _ENABLE_HERSHEY_GOTHIC_GERMAN_
			kHersheyFont_GothicGerman,
	#endif
	#ifdef _ENABLE_HERSHEY_GOTHIC_ITALIAN_
			kHersheyFont_GothicItalian,
	#endif
	#ifdef _ENABLE_HERSHEY_GOTHIC_GBT_
			kHersheyFont_Gothic_GBT,
	#endif
	#ifdef _ENABLE_HERSHEY_GOTHITT_
			kHersheyFont_Gothitt,
	#endif
	#ifdef _ENABLE_HERSHEY_CYRILLIC_
			kHersheyFont_Cyrillic,
			kHersheyFont_Cyrilic1,
	#endif
	#ifdef _ENABLE_HERSHEY_JAPANESE_
			kHersheyFont_Japanese,
	#endif


			kHersheyFont_last
		};
#ifdef __cplusplus
	extern "C"{
#endif

	long	GetHersheyFontTableSize(char fontNumber);
//	byte	GetHersheyFontNumerFromName(char *fontName);
	char	GetHersheyFontNumerFromName(char *fontName);
	void	HersheyDisplayFontList(RECT *displayRect, boolean showSize);
	void	HersheyDisplayFontSamples(RECT *displayRect);
	short	HersheyGetCharWidth(char fontNumber, char theChar, short fontSize);
	short	HersheyGetStringWidth(char fontNumber, char *theString, short fontSize);

	//*	these routines are for internal use only. They get an signle font char
	//*	definition in hershey vector format. They are split up into multiple files
	//*	because of linker issues with the complier and to far of an address

//*	Rowan fonts
	void	GetFontDef_RowmanSimplex(short tableIndex, char *fontDefString);
	void	GetFontDef_RowmanDuplex(short tableIndex, char *fontDefString);
	void	GetFontDef_RowmanTriplex(short tableIndex, char *fontDefString);
	
//*	Greek fonts
	void	GetFontDef_Greek(short tableIndex, char *fontDefString);
	void	GetFontDef_GreekSimplex(short tableIndex, char *fontDefString);
	void	GetFontDef_GreekComplex(short tableIndex, char *fontDefString);

//*	Script fonts
	void	GetFontDef_ScriptSimplex(short tableIndex, char *fontDefString);
	void	GetFontDef_ScriptComplex(short tableIndex, char *fontDefString);


//*	Gothic fonts
	void	GetFontDef_GothicEnglish(short tableIndex, char *fontDefString);
	void	GetFontDef_GothicGerman(short tableIndex, char *fontDefString);
	void	GetFontDef_GothicItalian(short tableIndex, char *fontDefString);
	void	GetFontDef_GothicGBT(short tableIndex, char *fontDefString);
	void	GetFontDef_Gothitt(short tableIndex, char *fontDefString);

//*	times fonts
	void	GetFontDef_TimesGreek(short tableIndex, char *fontDefString);
	void	GetFontDef_TimesItalic(short tableIndex, char *fontDefString);
	void	GetFontDef_TimesItalicBold(short tableIndex, char *fontDefString);
	void	GetFontDef_TimesRoman(short tableIndex, char *fontDefString);
	void	GetFontDef_TimesRomanBold(short tableIndex, char *fontDefString);
//*	symbols
	void	GetFontDef_Astrology(short tableIndex, char *fontDefString);
	void	GetFontDef_Markers(short tableIndex, char *fontDefString);
	void	GetFontDef_Mathlow(short tableIndex, char *fontDefString);
	void	GetFontDef_Mathupp(short tableIndex, char *fontDefString);
	void	GetFontDef_Music(short tableIndex, char *fontDefString);
	void	GetFontDef_Meteorology(short tableIndex, char *fontDefString);
	void	GetFontDef_Symbolic(short tableIndex, char *fontDefString);

//*	Cyrillic fonts
	void	GetFontDef_Cyrillic(short tableIndex, char *fontDefString);
	void	GetFontDef_Cyrilic1(short tableIndex, char *fontDefString);

	void	GetFontDef_Japansese(short tableIndex, char *fontDefString);



#ifdef __cplusplus
} // extern "C"
#endif


#endif

#endif		//*	_HESHEYFONT_H_