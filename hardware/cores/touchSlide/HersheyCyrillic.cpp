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



#ifdef _ENABLE_HERSHEY_CYRILLIC_
	#include	"HersheyCyrillic.h"
	//*******************************************************************************
	void	GetFontDef_Cyrillic(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyCyrillicFontTable[tableIndex])));
	}


#include	"HersheyCyrilic1.h"
	//*******************************************************************************
	void	GetFontDef_Cyrilic1(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyCyrilic1FontTable[tableIndex])));
	}

#endif




#endif		//*	_ENABLE_HERSHEY_FONTS_