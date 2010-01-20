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



#ifdef _ENABLE_HERSHEY_GOTHIC_ENGLISH_
	#include	"HersheyGothicEnglish.h"
	//*******************************************************************************
	void	GetFontDef_GothicEnglish(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyGothicEngFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_GOTHIC_GERMAN_
	#include	"HersheyGothicGerman.h"
	//*******************************************************************************
	void	GetFontDef_GothicGerman(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyGothicGermanFontTable[tableIndex])));
	}
#endif


#ifdef _ENABLE_HERSHEY_GOTHIC_ITALIAN_
	#include	"HersheyGothicItalian.h"
	//*******************************************************************************
	void	GetFontDef_GothicItalian(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyGothicItalianFontTable[tableIndex])));
	}
#endif


#ifdef _ENABLE_HERSHEY_GOTHIC_GBT_
	#include	"HersheyGothgbt.h"
	//*******************************************************************************
	void	GetFontDef_GothicGBT(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyGothgbtFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_GOTHITT_
	#include	"HersheyGothitt.h"
	//*******************************************************************************
	void	GetFontDef_Gothitt(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyGothittFontTable[tableIndex])));
	}
#endif

#endif		//*	_ENABLE_HERSHEY_FONTS_