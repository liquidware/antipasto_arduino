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




#ifdef _ENABLE_HERSHEY_ASTROLOGY_
	#include	"HersheyAstrology.h"
	//*******************************************************************************
	void	GetFontDef_Astrology(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyAstrologyFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_MATH_
	#include	"HersheyMathlow.h"
	//*******************************************************************************
	void	GetFontDef_Mathlow(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyMathlowFontTable[tableIndex])));
	}
	#include	"HersheyMathupp.h"
	//*******************************************************************************
	void	GetFontDef_Mathupp(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyMathuppFontTable[tableIndex])));
	}
#endif


#ifdef _ENABLE_HERSHEY_MARKERS_
	#include	"HersheyMarkers.h"
	//*******************************************************************************
	void	GetFontDef_Markers(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyMarkersFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_METEOROLOGY_
	#include	"HersheyMeteorology.h"
	//*******************************************************************************
	void	GetFontDef_Meteorology(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyMeteorologyFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_MUSIC_
	#include	"HersheyMusic.h"
	//*******************************************************************************
	void	GetFontDef_Music(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyMusicFontTable[tableIndex])));
	}
#endif


#ifdef _ENABLE_HERSHEY_SYMBOLIC_
	#include	"HersheySymbolic.h"
	//*******************************************************************************
	void	GetFontDef_Symbolic(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershySymbolicFontTable[tableIndex])));
	}
#endif

#endif		//*	_ENABLE_HERSHEY_FONTS_