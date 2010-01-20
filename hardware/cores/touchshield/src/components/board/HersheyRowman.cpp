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


#ifdef _ENABLE_HERSHEY_ROWMAN_SIMPLEX_
	#include	"HersheyRowmanSimplex.h"
	//*******************************************************************************
	void	GetFontDef_RowmanSimplex(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyRowmanSimplexFontTable[tableIndex])));
	}
#endif


#ifdef _ENABLE_HERSHEY_ROWMAN_DUPLEX_
	#include	"HersheyRowmanDuplex.h"
	//*******************************************************************************
	void	GetFontDef_RowmanDuplex(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyRowmanDuplexFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_ROWMAN_TRIPLEX_
	#include	"HersheyRowmanTriplex.h"
	//*******************************************************************************
	void	GetFontDef_RowmanTriplex(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyRowmanTriplexFontTable[tableIndex])));
	}
#endif


#endif		//*	_ENABLE_HERSHEY_FONTS_