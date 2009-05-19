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

#ifndef _FONT_H_
	#include	"font.h"
#endif




#ifdef _ENABLE_HERSHEY_FONTS_					//*	4882 base font and code
//	#define	_ENABLE_ROTATED_HERSHEY_FONTS_		//*	1252 bytes (code only, no data)

//	#define	_ENABLE_HERSHEY_CURSIVE_			//*	4656 bytes
//	#define	_ENABLE_HERSHEY_FUTURAL_
//	#define	_ENABLE_HERSHEY_GREEK_				//*	3426
//	#define	_ENABLE_HERSHEY_GREEK_SIMPLEX_		//*	
//	#define	_ENABLE_HERSHEY_GREEK_COMPLEX_		//*	5260
//	#define	_ENABLE_HERSHEY_TIMES_GREEK_		//*	5456
//	#define	_ENABLE_HERSHEY_TIMES_ITALIC_		//*	5610
//	#define	_ENABLE_HERSHEY_TIMES_ITALIC_BOLD_	//*	8444
//	#define	_ENABLE_HERSHEY_TIMES_ROMAN_		//*	5306
//	#define	_ENABLE_HERSHEY_TIMES_ROMAN_BOLD_	//*	8734

//	#define	_ENABLE_HERSHEY_ROWMAND_			//*	5806

//	#define	_ENABLE_HERSHEY_SCRIPT_SIMPLEX_
//	#define	_ENABLE_HERSHEY_SCRIPT_COMPLEX_


//	#define	_ENABLE_HERSHEY_ASTROLOGY_			//*	
//	#define	_ENABLE_HERSHEY_MARKERS_			//*	1574
//	#define	_ENABLE_HERSHEY_MATH_
//	#define	_ENABLE_HERSHEY_METEOROLOGY_		//*	3398
//	#define	_ENABLE_HERSHEY_MUSIC_				//*	6622
//	#define	_ENABLE_HERSHEY_SYMBOLIC_			//*	4530 bytes

//	#define	_ENABLE_HERSHEY_GOTHIC_ENGLISH_		//*	9752 bytes
//	#define	_ENABLE_HERSHEY_GOTHIC_GERMAN_		//*	10000 bytes
//	#define	_ENABLE_HERSHEY_GOTHIC_ITALIAN_
//	#define	_ENABLE_HERSHEY_GOTHIC_GBT_
	
//	#define	_ENABLE_HERSHEY_CYRILLIC_

//	#define	_ENABLE_HERSHEY_JAPANESE_			//*	8000 +++
	

enum	{	kHerheyFont_Default	=	0,
	#ifdef _ENABLE_HERSHEY_CURSIVE_
			kHerheyFont_Cursive,
	#endif
	#ifdef _ENABLE_HERSHEY_FUTURAL_
			kHerheyFont_Futural,
	#endif
	#ifdef _ENABLE_HERSHEY_GREEK_
			kHerheyFont_Greek,
	#endif
	#ifdef _ENABLE_HERSHEY_GREEK_SIMPLEX_
			kHerheyFont_GreekSimplex,
	#endif
	#ifdef _ENABLE_HERSHEY_GREEK_COMPLEX_
			kHerheyFont_GreekComplex,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_GREEK_
			kHerheyFont_TimesGreek,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_ITALIC_
			kHerheyFont_TimesItalic,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_ITALIC_BOLD_
			kHerheyFont_TimesItalicBold,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_ROMAN_
			kHerheyFont_TimesRoman,
	#endif
	#ifdef _ENABLE_HERSHEY_TIMES_ROMAN_BOLD_
			kHerheyFont_TimesRomanBold,
	#endif
	#ifdef _ENABLE_HERSHEY_ROWMAND_
			kHerheyFont_Rowmand,
	#endif
	#ifdef _ENABLE_HERSHEY_SCRIPT_SIMPLEX_
			kHerheyFont_ScriptSimplex,
	#endif
	#ifdef _ENABLE_HERSHEY_SCRIPT_COMPLEX_
			kHerheyFont_ScriptComplex,
	#endif
	#ifdef _ENABLE_HERSHEY_ASTROLOGY_
			kHerheyFont_Astrology,
	#endif
	#ifdef _ENABLE_HERSHEY_MARKERS_
			kHerheyFont_Markers,
	#endif
	#ifdef _ENABLE_HERSHEY_MATH_
			kHerheyFont_Mathlow,
			kHerheyFont_Mathupp,
	#endif
	#ifdef _ENABLE_HERSHEY_METEOROLOGY_
			kHerheyFont_Meteorology,
	#endif
	#ifdef _ENABLE_HERSHEY_MUSIC_
			kHerheyFont_Music,
	#endif
	#ifdef _ENABLE_HERSHEY_SYMBOLIC_
			kHerheyFont_Symbolic,
	#endif
	#ifdef _ENABLE_HERSHEY_GOTHIC_ENGLISH_
			kHerheyFont_GothicEnglish,
	#endif
	#ifdef _ENABLE_HERSHEY_GOTHIC_GERMAN_
			kHerheyFont_GothicGerman,
	#endif
	#ifdef _ENABLE_HERSHEY_GOTHIC_ITALIAN_
			kHerheyFont_GothicItalian,
	#endif
	#ifdef _ENABLE_HERSHEY_GOTHIC_GBT_
			kHerheyFont_Gothic_GBT,
	#endif
	#ifdef _ENABLE_HERSHEY_CYRILLIC_
			kHerheyFont_Cyrillic,
			kHerheyFont_Cyrilic1,
	#endif
	#ifdef _ENABLE_HERSHEY_JAPANESE_
			kHerheyFont_Japanese,
	#endif


			kHerheyFont_last
		};
#ifdef __cplusplus
	extern "C"{
#endif

	long	GetHersheyFontTableSize(char fontNumber);

	//*	these routines are for internal use only. They get an signle font char
	//*	definition in hershey vector format. They are split up into multiple files
	//*	because of linker issues with the complier and to far of an address

//*	Rowan fonts
	void	GetFontDef_Rowmand(short tableIndex, char *fontDefString);
	
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
