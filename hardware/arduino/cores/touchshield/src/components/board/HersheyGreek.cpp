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
#ifdef _ENABLE_HERSHEY_GREEK_
	#include	"HersheyGreek.h"
	//*******************************************************************************
	void	GetFontDef_Greek(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyGreekFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_GREEK_SIMPLEX_
	#include	"HersheyGreekSimplex.h"
	//*******************************************************************************
	void	GetFontDef_GreekSimplex(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyGreekSimplexFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_GREEK_COMPLEX_
	#include	"HersheyGreekComplex.h"
	//*******************************************************************************
	void	GetFontDef_GreekComplex(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyGreekComplexFontTable[tableIndex])));
	}
#endif




#endif		//*	_ENABLE_HERSHEY_FONTS_