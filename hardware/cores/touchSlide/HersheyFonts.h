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
	#define	_ENABLE_ROTATED_HERSHEY_FONTS_		//*	1252 bytes (code only, no data)

	#define	_ENABLE_HERSHEY_CURSIVE_			//*	4656 bytes
//	#define	_ENABLE_HERSHEY_FUTURAL_
	#define	_ENABLE_HERSHEY_GREEK_				//*	3426
//	#define	_ENABLE_HERSHEY_TIMES_GREEK_		//*	5456 bytes
//	#define	_ENABLE_HERSHEY_TIMES_ITALIC_		//*	5610
//	#define	_ENABLE_HERSHEY_TIMES_ITALIC_BOLD_	//*	8444
	#define	_ENABLE_HERSHEY_TIMES_ROMAN_		//*	5306
//	#define	_ENABLE_HERSHEY_TIMES_ROMAN_BOLD_	//*	8734

//	#define	_ENABLE_HERSHEY_ROWMAND_			//*	5806


//	#define	_ENABLE_HERSHEY_ASTROLOGY_			//*	
//	#define	_ENABLE_HERSHEY_MARKERS_			//*	1574
//	#define	_ENABLE_HERSHEY_MATH_
//	#define	_ENABLE_HERSHEY_METEOROLOGY_		//*	3398
//	#define	_ENABLE_HERSHEY_MUSIC_				//*	6622
//	#define	_ENABLE_HERSHEY_SYMBOLIC_			//*	4530 bytes

	#define	_ENABLE_HERSHEY_GOTHIC_ENGLISH_		//*	9752 bytes
//	#define	_ENABLE_HERSHEY_GOTHIC_GERMAN_		//*	10000 bytes

	#define	_ENABLE_HERSHEY_CYRILLIC_

//	#define	_ENABLE_HERSHEY_JAPANESE_			//*	8000 +++
	

enum	{	kHerheyFont_Default	=	0,
			kHerheyFont_Cursive,
//			kHerheyFont_Futural,
			kHerheyFont_Greek,

//			kHerheyFont_TimesGreek,
//			kHerheyFont_TimesItalic,
//			kHerheyFont_TimesItalicBold,
			kHerheyFont_TimesRoman,
//			kHerheyFont_TimesRomanBold,

//			kHerheyFont_Rowmand,
			
//			kHerheyFont_Astrology,
//			kHerheyFont_Markers,
//			kHerheyFont_Mathlow,
//			kHerheyFont_Mathupp,
//			kHerheyFont_Meteorology,
//			kHerheyFont_Music,
//			kHerheyFont_Symbolic,


			kHerheyFont_GothicEnglish,
//			kHerheyFont_GothicGerman,

			kHerheyFont_Cyrillic,
			kHerheyFont_Cyrilic1,

			
//			kHerheyFont_Japanese,


			kHerheyFont_last
		};
#ifdef __cplusplus
	extern "C"{
#endif

	long	GetHersheyFontTableSize(char fontNumber);

	//*	these routines are for internal use only. They get an signle font char
	//*	definition in hershey vector format. They are split up into multiple files
	//*	because of linker issues with the complier and to far of an address

//*	Rowand fonts
	void	GetFontDef_Rowmand(short tableIndex, char *fontDefString);
	
	void	GetFontDef_Greek(short tableIndex, char *fontDefString);
//*	gothic fonts
	void	GetFontDef_GothicEnglish(short tableIndex, char *fontDefString);
	void	GetFontDef_GothicGerman(short tableIndex, char *fontDefString);
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
