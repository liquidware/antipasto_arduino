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


#include	<avr/pgmspace.h>

#include	"HersheyFonts.h"


#ifdef _ENABLE_HERSHEY_FONTS_
//*******************************************************************************
//*	Hershey font definition files
//*		these are put into .h files so they are more managable
//*******************************************************************************


#ifdef _ENABLE_HERSHEY_TIMES_GREEK_
	#include	"HersheyTimesGreek.h"
	//*******************************************************************************
	void	GetFontDef_TimesGreek(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyTimesGreekFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_TIMES_ITALIC_
	#include	"HersheyTimesItalic.h"
	//*******************************************************************************
	void	GetFontDef_TimesItalic(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyTimesItalicFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_TIMES_ITALIC_BOLD_
	#include	"HersheyTimesItalicBold.h"
	//*******************************************************************************
	void	GetFontDef_TimesItalicBold(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyTimesItalicBoldFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_TIMES_ROMAN_
	#include	"HersheyTimesRoman.h"
	//*******************************************************************************
	void	GetFontDef_TimesRoman(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyTimesRomanFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_TIMES_ROMAN_BOLD_
	#include	"HersheyTimesRomanBold.h"
	//*******************************************************************************
	void	GetFontDef_TimesRomanBold(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyTimesRomanBoldFontTable[tableIndex])));
	}
#endif


#endif		//*	_ENABLE_HERSHEY_FONTS_