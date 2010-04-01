//*******************************************************************************
//*	
//*	Hershey Font handling for
//*	Sub-Processing 
//*	for TouchShield library
//*	by msproul@jove.rutgers.edu
//*	This implementation of Hershey fonts is (C) 2009-2010 by Mark Sproul
//*	The Hershey Font data is covered under its own rules
//*
//*	This library is free software; you can redistribute it and/or
//*	modify it under the terms of the GNU Lesser General Public
//*	License as published by the Free Software Foundation; either
//*	version 2.1 of the License, or (at your option) any later version.
//*
//*		I got the font data from
//*		http://emergent.unpythonic.net/software/hershey
//*******************************************************************************
//*	Jan 21,	2009	<MLS> Started working on large fonts
//*	Jan 21,	2009	<MLS> Formated Hershey Gothic English for Arduino
//*	Jan 21,	2009	<MLS> Formated Hershey Gothic German for Arduino
//*	Jan 22,	2009	<MLS> Got Hershey fonts fully working
//*	Jan 23,	2009	<MLS> Added font name as the first string
//*	Jan 30,	2009	<MLS> Formated Hershey Cyrillic for Arduino
//*	Feb 18,	2009	<MLS> Formated Hershey Gothic Italian for Arduino
//*	Feb 18,	2009	<MLS> Formated Hershey Greek Simplex for Arduino
//*	Feb 18,	2009	<MLS> Formated Hershey Greek Simplex for Arduino
//*	Feb 20,	2009	<MLS> Formated Hershey Script Complex for Arduino
//*	Feb 22,	2009	<MLS> Formated Hershey Script Simplex for Arduino
//*	Mar  2,	2009	<MLS> Added GetHersheyFontNumerFromName
//*	Dec 31,	2009	<MLS> Added HersheyDisplayFontList
//*	Jan  1,	2010	<MLS> Added HersheyDisplayFontSamples
//*	Jan 10,	2010	<MLS> Added HersheyGetCharWidth and HersheyGetStringWidth
//*	Jan 10,	2010	<MLS> Added pensize support to HersheyDrawLetter (size 1,2,3)
//*	JAN 10,	2010	<MLS> Formated Hershey Roman Triplex for Arduion
//*******************************************************************************


#ifdef __MWERKS__
	void sprintf(...);
	short	random(int a, int b);
//	#include	"wiring.h"
#else
	#include	<stdio.h>
#endif

#include	<string.h>
#include	<math.h>
#include	<avr/io.h>
#include	<avr/pgmspace.h>
#include	<ctype.h>

#include	"WProgram.h"
#include	"wiring.h"


#include	"HardwareDef.h"

#if defined(_TOUCH_STEALTH_) || defined(_TOUCH_SLIDE_)
	#include	"graphics.h"
	#include	"font.h"
	#include	"binary.h"

	#ifndef SUBPGRAPHICS_H
		#include	"SubPGraphics.h"
	#endif
	#ifndef _QUICKDRAW_H_
		#include	"QuickDraw.h"
	#endif
#endif

#if defined(_ARDUINO_MEGA_)
	#ifndef _PICASO_H_
		#include	"PicasoVGA.h"
	#endif
#endif

#include	"HersheyFonts.h"

#ifdef _ENABLE_HERSHEY_FONTS_

#define	kMaxVectorStringLength	300


//********************************************************************************
//	http://www.ks.uiuc.edu/Research/vmd/doxygen/Hershey_8C-source.html
//********************************************************************************
//   Hershey.C
//   extracted from the hershey font
//   
//   Charles Schwieters 6/14/99
//
//
//   font info:
//
//Peter Holzmann, Octopus Enterprises
//USPS: 19611 La Mar Court, Cupertino, CA 95014
//UUCP: {hplabs!hpdsd,pyramid}!octopus!pete
//Phone: 408/996-7746
//
//This distribution is made possible through the collective encouragement
//of the Usenet Font Consortium, a mailing list that sprang to life to get
//this accomplished and that will now most likely disappear into the mists
//of time... Thanks are especially due to Jim Hurt, who provided the packed
//font data for the distribution, along with a lot of other help.
//
//This file describes the Hershey Fonts in general, along with a description of
//the other files in this distribution and a simple re-distribution restriction.
//
//USE RESTRICTION:
//        This distribution of the Hershey Fonts may be used by anyone for
//        any purpose, commercial or otherwise, providing that:
//                1. The following acknowledgements must be distributed with
//                        the font data:
//                        - The Hershey Fonts were originally created by Dr.
//                                A. V. Hershey while working at the U. S.
//                                National Bureau of Standards.
//                        - The format of the Font data in this distribution
//                                was originally created by
//                                        James Hurt
//                                        Cognition, Inc.
//                                        900 Technology Park Drive
//                                        Billerica, MA 01821
//                                        (mit-eddie!ci-dandelion!hurt)
//                2. The font data in this distribution may be converted into
//                        any other format *EXCEPT* the format distributed by
//                        the U.S. NTIS (which organization holds the rights
//                        to the distribution and use of the font data in that
//                        particular format). Not that anybody would really
//                        *want* to use their format... each point is described
//                        in eight bytes as "xxx yyy:", where xxx and yyy are
//                        the coordinate values as ASCII numbers.
//
//*PLEASE* be reassured: The legal implications of NTIS' attempt to control
//a particular form of the Hershey Fonts *are* troubling. HOWEVER: We have
//been endlessly and repeatedly assured by NTIS that they do not care what
//we do with our version of the font data, they do not want to know about it,
//they understand that we are distributing this information all over the world,
//etc etc etc... but because it isn't in their *exact* distribution format, they
//just don't care!!! So go ahead and use the data with a clear conscience! (If
//you feel bad about it, take a smaller deduction for something on your taxes
//next week...)
//
//The Hershey Fonts:
//        - are a set of more than 2000 glyph (symbol) descriptions in vector
//                ( &lt;x,y&gt; point-to-point ) format
//        - can be grouped as almost 20 'occidental' (english, greek,
//                cyrillic) fonts, 3 or more 'oriental' (Kanji, Hiragana,
//                and Katakana) fonts, and a few hundred miscellaneous
//                symbols (mathematical, musical, cartographic, etc etc)
//        - are suitable for typographic quality output on a vector device
//                (such as a plotter) when used at an appropriate scale.
//        - were digitized by Dr. A. V. Hershey while working for the U.S.
//                Government National Bureau of Standards (NBS).
//        - are in the public domain, with a few caveats:
//                - They are available from NTIS (National Technical Info.
//                        Service) in a computer-readable from which is *not*
//                        in the public domain. This format is described in
//                        a hardcopy publication "Tables of Coordinates for
//                        Hershey's Repertory of Occidental Type Fonts and
//                        Graphic Symbols" available from NTIS for less than
//                        $20 US (phone number +1 703 487 4763).
//                - NTIS does not care about and doesn't want to know about
//                        what happens to Hershey Font data that is not
//                        distributed in their exact format.
//                - This distribution is not in the NTIS format, and thus is
//                        only subject to the simple restriction described
//                        at the top of this file.
//
//********************************************************************************
//*	Edit History
//********************************************************************************
//*	Oct  7,	2004	<MLS> working on hershey fonts in MacAPRS
//********************************************************************************

#ifndef nil
	#define	nil	0L
#endif

//********************************************************************************
//*	the Hershey romans font in ascii order (first 32 places held by space)
//********************************************************************************

//const unsigned char *gHersheyFontData[]	PROGMEM =	
//prog_char	*gHersheyFontData[]	 =	
//char	*gHersheyFontData[]	 =	
//{
//*	dont waste space with control chars
//prog_char	string_00[]	PROGMEM	=	"JZ";
//prog_char	string_01[]	PROGMEM	=	"JZ";
//prog_char	string_02[]	PROGMEM	=	"JZ";
//prog_char	string_03[]	PROGMEM	=	"JZ";
//prog_char	string_04[]	PROGMEM	=	"JZ";
//prog_char	string_05[]	PROGMEM	=	"JZ";
//prog_char	string_06[]	PROGMEM	=	"JZ";
//prog_char	string_07[]	PROGMEM	=	"JZ";
//prog_char	string_08[]	PROGMEM	=	"JZ";
//prog_char	string_09[]	PROGMEM	=	"JZ";
//prog_char	string_0A[]	PROGMEM	=	"JZ";
//prog_char	string_0B[]	PROGMEM	=	"JZ";
//prog_char	string_0C[]	PROGMEM	=	"JZ";
//prog_char	string_0D[]	PROGMEM	=	"JZ";
//prog_char	string_0E[]	PROGMEM	=	"JZ";
//prog_char	string_0F[]	PROGMEM	=	"JZ";

//prog_char	string_10[]	PROGMEM	=	"JZ";
//prog_char	string_11[]	PROGMEM	=	"JZ";
//prog_char	string_12[]	PROGMEM	=	"JZ";
//prog_char	string_13[]	PROGMEM	=	"JZ";
//prog_char	string_14[]	PROGMEM	=	"JZ";
//prog_char	string_15[]	PROGMEM	=	"JZ";
//prog_char	string_16[]	PROGMEM	=	"JZ";
//prog_char	string_17[]	PROGMEM	=	"JZ";
//prog_char	string_18[]	PROGMEM	=	"JZ";
//prog_char	string_19[]	PROGMEM	=	"JZ";
//prog_char	string_1A[]	PROGMEM	=	"JZ";
//prog_char	string_1B[]	PROGMEM	=	"JZ";
//prog_char	string_1C[]	PROGMEM	=	"JZ";
//prog_char	string_1D[]	PROGMEM	=	"JZ";
//prog_char	string_1E[]	PROGMEM	=	"JZ";
//prog_char	string_1F[]	PROGMEM	=	"JZ";

//*	Jan 10,	2010	<MLS>
//*	this data did not have the pair count, I need it consistant, so I am adding it
prog_char	gHersheyDefault_00[]	PROGMEM	=	"Hershey default";				//*	0X20
prog_char	gHersheyDefault_20[]	PROGMEM	=	" 1JZ";				//*	0X20
prog_char	gHersheyDefault_21[]	PROGMEM	=	"  MWRFRT RRYQZR[SZRY";
prog_char	gHersheyDefault_22[]	PROGMEM	=	"  JZNFNM RVFVM";
prog_char	gHersheyDefault_23[]	PROGMEM	=	"  H]SBLb RYBRb RLOZO RKUYU";
prog_char	gHersheyDefault_24[]	PROGMEM	=	"  H\\PBP_ RTBT_ RYIWGTFPFMGKIKKLMMNOOUQWRXSYUYXWZT[P[MZKX";
prog_char	gHersheyDefault_25[]	PROGMEM	=	"  F^[FI[ RNFPHPJOLMMKMIKIIJGLFNFPGSHVHYG[F RWTUUTWTYV[X[ZZ[X[VYTWT";
prog_char	gHersheyDefault_26[]	PROGMEM	=	"  E_\\O\\N[MZMYNXPVUTXRZP[L[JZIYHWHUISJRQNRMSKSIRGPFNGMIMKNNPQUXWZY[";
prog_char	gHersheyDefault_27[]	PROGMEM	=	"  MWRHQGRFSGSIRKQL";
prog_char	gHersheyDefault_28[]	PROGMEM	=	"  KYVBTDRGPKOPOTPYR]T`Vb";
prog_char	gHersheyDefault_29[]	PROGMEM	=	"  KYNBPDRGTKUPUTTYR]P`Nb";
prog_char	gHersheyDefault_2A[]	PROGMEM	=	"  JZRLRX RMOWU RWOMU";
prog_char	gHersheyDefault_2B[]	PROGMEM	=	"  E_RIR[ RIR[R";
prog_char	gHersheyDefault_2C[]	PROGMEM	=	"  NVSWRXQWRVSWSYQ[";
prog_char	gHersheyDefault_2D[]	PROGMEM	=	"  E_IR[R";
prog_char	gHersheyDefault_2E[]	PROGMEM	=	"  NVRVQWRXSWRV";
prog_char	gHersheyDefault_2F[]	PROGMEM	=	"  G][BIb";
		
prog_char	gHersheyDefault_30[]	PROGMEM	=	"  H\\QFNGLJKOKRLWNZQ[S[VZXWYRYOXJVGSFQF";	/*	0x30	*/	
prog_char	gHersheyDefault_31[]	PROGMEM	=	"  H\\NJPISFS[";
prog_char	gHersheyDefault_32[]	PROGMEM	=	"  H\\LKLJMHNGPFTFVGWHXJXLWNUQK[Y[";
prog_char	gHersheyDefault_33[]	PROGMEM	=	"  H\\MFXFRNUNWOXPYSYUXXVZS[P[MZLYKW";
prog_char	gHersheyDefault_34[]	PROGMEM	=	"  H\\UFKTZT RUFU[";
prog_char	gHersheyDefault_35[]	PROGMEM	=	"  H\\WFMFLOMNPMSMVNXPYSYUXXVZS[P[MZLYKW";
prog_char	gHersheyDefault_36[]	PROGMEM	=	"  H\\XIWGTFRFOGMJLOLTMXOZR[S[VZXXYUYTXQVOSNRNOOMQLT";
prog_char	gHersheyDefault_37[]	PROGMEM	=	"  H\\YFO[ RKFYF";
prog_char	gHersheyDefault_38[]	PROGMEM	=	"  H\\PFMGLILKMMONSOVPXRYTYWXYWZT[P[MZLYKWKTLRNPQOUNWMXKXIWGTFPF";
prog_char	gHersheyDefault_39[]	PROGMEM	=	"  H\\XMWPURRSQSNRLPKMKLLINGQFRFUGWIXMXRWWUZR[P[MZLX";
prog_char	gHersheyDefault_3A[]	PROGMEM	=	"  NVROQPRQSPRO RRVQWRXSWRV";
prog_char	gHersheyDefault_3B[]	PROGMEM	=	"  NVROQPRQSPRO RSWRXQWRVSWSYQ[";
prog_char	gHersheyDefault_3C[]	PROGMEM	=	"  F^ZIJRZ[";
prog_char	gHersheyDefault_3D[]	PROGMEM	=	"  E_IO[O RIU[U";
prog_char	gHersheyDefault_3E[]	PROGMEM	=	"  F^JIZRJ[";
prog_char	gHersheyDefault_3F[]	PROGMEM	=	"  I[LKLJMHNGPFTFVGWHXJXLWNVORQRT RRYQZR[SZRY";
		
		
prog_char	gHersheyDefault_40[]	PROGMEM	=	"  E`WNVLTKQKOLNMMPMSNUPVSVUUVS RQKOMNPNSOUPV RWKVSVUXVZV\\T]Q]O\\L[J";	//*	@
prog_char	gHersheyDefault_41[]	PROGMEM	=	"  I[RFJ[ RRFZ[ RMTWT";													//*	A
prog_char	gHersheyDefault_42[]	PROGMEM	=	"  G\\KFK[ RKFTFWGXHYJYLXNWOTP RKPTPWQXRYTYWXYWZT[K[";					//*	B
prog_char	gHersheyDefault_43[]	PROGMEM	=	"  H]ZKYIWGUFQFOGMILKKNKSLVMXOZQ[U[WZYXZV";								//*	C
prog_char	gHersheyDefault_44[]	PROGMEM	=	"  G\\KFK[ RKFRFUGWIXKYNYSXVWXUZR[K[";									//*	D
prog_char	gHersheyDefault_45[]	PROGMEM	=	"  H[LFL[ RLFYF RLPTP RL[Y[";												//*	E
prog_char	gHersheyDefault_46[]	PROGMEM	=	"  HZLFL[ RLFYF RLPTP";													//*	F
prog_char	gHersheyDefault_47[]	PROGMEM	=	"  H]ZKYIWGUFQFOGMILKKNKSLVMXOZQ[U[WZYXZVZS RUSZS";						//*	G
prog_char	gHersheyDefault_48[]	PROGMEM	=	"  G]KFK[ RYFY[ RKPYP";													//*	H
prog_char	gHersheyDefault_49[]	PROGMEM	=	"  NVRFR[";																//*	I
prog_char	gHersheyDefault_4A[]	PROGMEM	=	"  JZVFVVUYTZR[P[NZMYLVLT";												//*	J
prog_char	gHersheyDefault_4B[]	PROGMEM	=	"  G\\KFK[ RYFKT RPOY[";													//*	K
	//	(char *)	"HYLFL[ RL[X[";															//*	L
prog_char	gHersheyDefault_4C[]	PROGMEM	=	"  HYLFL[L[X[";															//*	L
prog_char	gHersheyDefault_4D[]	PROGMEM	=	"  F^JFJ[ RJFR[ RZFR[ RZFZ[";												//*	M
prog_char	gHersheyDefault_4E[]	PROGMEM	=	"  G]KFK[ RKFY[ RYFY[";													//*	N
prog_char	gHersheyDefault_4F[]	PROGMEM	=	"  G]PFNGLIKKJNJSKVLXNZP[T[VZXXYVZSZNYKXIVGTFPF";							//*	O
prog_char	gHersheyDefault_50[]	PROGMEM	=	"  G\\KFK[ RKFTFWGXHYJYMXOWPTQKQ";										//*	P
prog_char	gHersheyDefault_51[]	PROGMEM	=	"  G]PFNGLIKKJNJSKVLXNZP[T[VZXXYVZSZNYKXIVGTFPF RSWY]";					//*	Q
prog_char	gHersheyDefault_52[]	PROGMEM	=	"  G\\KFK[ RKFTFWGXHYJYLXNWOTPKP RRPY[";									//*	R
prog_char	gHersheyDefault_53[]	PROGMEM	=	"  H\\YIWGTFPFMGKIKKLMMNOOUQWRXSYUYXWZT[P[MZKX";							//*	S
prog_char	gHersheyDefault_54[]	PROGMEM	=	"  JZRFR[ RKFYF";															//*	T
prog_char	gHersheyDefault_55[]	PROGMEM	=	"  G]KFKULXNZQ[S[VZXXYUYF";												//*	U
prog_char	gHersheyDefault_56[]	PROGMEM	=	"  I[JFR[ RZFR[";															//*	V
prog_char	gHersheyDefault_57[]	PROGMEM	=	"  F^HFM[ RRFM[ RRFW[ R\\FW[";											//*	W
prog_char	gHersheyDefault_58[]	PROGMEM	=	"  H\\KFY[ RYFK[";														//*	X
prog_char	gHersheyDefault_59[]	PROGMEM	=	"  I[JFRPR[ RZFRP";														//*	Y
prog_char	gHersheyDefault_5A[]	PROGMEM	=	"  H\\YFK[ RKFYF RK[Y[";													//*	Z
		
		
		
prog_char	gHersheyDefault_5B[]	PROGMEM	=	"  KYOBOb RPBPb ROBVB RObVb";
prog_char	gHersheyDefault_5C[]	PROGMEM	=	"  KYKFY^";
prog_char	gHersheyDefault_5D[]	PROGMEM	=	"  KYTBTb RUBUb RNBUB RNbUb";
prog_char	gHersheyDefault_5E[]	PROGMEM	=	"  JZRDJR RRDZR";
prog_char	gHersheyDefault_5F[]	PROGMEM	=	"  I[Ib[b";

prog_char	gHersheyDefault_60[]	PROGMEM	=	"  NVSKQMQORPSORNQO";
prog_char	gHersheyDefault_61[]	PROGMEM	=	"  I\\XMX[ RXPVNTMQMONMPLSLUMXOZQ[T[VZXX";								//*	a
prog_char	gHersheyDefault_62[]	PROGMEM	=	"  H[LFL[ RLPNNPMSMUNWPXSXUWXUZS[P[NZLX";									//*	b
prog_char	gHersheyDefault_63[]	PROGMEM	=	"  I[XPVNTMQMONMPLSLUMXOZQ[T[VZXX";										//*	c
prog_char	gHersheyDefault_64[]	PROGMEM	=	"  I\\XFX[ RXPVNTMQMONMPLSLUMXOZQ[T[VZXX";								//*	d
prog_char	gHersheyDefault_65[]	PROGMEM	=	"  I[LSXSXQWOVNTMQMONMPLSLUMXOZQ[T[VZXX";									//*	e
prog_char	gHersheyDefault_66[]	PROGMEM	=	"  MYWFUFSGRJR[ ROMVM";													//*	f
prog_char	gHersheyDefault_67[]	PROGMEM	=	"  I\\XMX]W`VaTbQbOa RXPVNTMQMONMPLSLUMXOZQ[T[VZXX";						//*	g
prog_char	gHersheyDefault_68[]	PROGMEM	=	"  I\\MFM[ RMQPNRMUMWNXQX[";												//*	h
prog_char	gHersheyDefault_69[]	PROGMEM	=	"  NVQFRGSFREQF RRMR[";													//*	i
prog_char	gHersheyDefault_6A[]	PROGMEM	=	"  MWRFSGTFSERF RSMS^RaPbNb";												//*	j
prog_char	gHersheyDefault_6B[]	PROGMEM	=	"  IZMFM[ RWMMW RQSX[";													//*	k
prog_char	gHersheyDefault_6C[]	PROGMEM	=	"  NVRFR[";																//*	l
prog_char	gHersheyDefault_6D[]	PROGMEM	=	"  CaGMG[ RGQJNLMOMQNRQR[ RRQUNWMZM\\N]Q][";								//*	m
prog_char	gHersheyDefault_6E[]	PROGMEM	=	"  I\\MMM[ RMQPNRMUMWNXQX[";												//*	n
prog_char	gHersheyDefault_6F[]	PROGMEM	=	"  I\\QMONMPLSLUMXOZQ[T[VZXXYUYSXPVNTMQM";								//*	o

prog_char	gHersheyDefault_70[]	PROGMEM	=	"  H[LMLb RLPNNPMSMUNWPXSXUWXUZS[P[NZLX";									//*	p
prog_char	gHersheyDefault_71[]	PROGMEM	=	"  I\\XMXb RXPVNTMQMONMPLSLUMXOZQ[T[VZXX";								//*	q
prog_char	gHersheyDefault_72[]	PROGMEM	=	"  KXOMO[ ROSPPRNTMWM";													//*	r
prog_char	gHersheyDefault_73[]	PROGMEM	=	"  J[XPWNTMQMNNMPNRPSUTWUXWXXWZT[Q[NZMX";									//*	s
prog_char	gHersheyDefault_74[]	PROGMEM	=	"  MYRFRWSZU[W[ ROMVM";													//*	t
prog_char	gHersheyDefault_75[]	PROGMEM	=	"  I\\MMMWNZP[S[UZXW RXMX[";												//*	u
prog_char	gHersheyDefault_76[]	PROGMEM	=	"  JZLMR[ RXMR[";															//*	v
prog_char	gHersheyDefault_77[]	PROGMEM	=	"  G]JMN[ RRMN[ RRMV[ RZMV[";												//*	w
prog_char	gHersheyDefault_78[]	PROGMEM	=	"  J[MMX[ RXMM[";															//*	x
prog_char	gHersheyDefault_79[]	PROGMEM	=	"  JZLMR[ RXMR[P_NaLbKb";													//*	y
prog_char	gHersheyDefault_7A[]	PROGMEM	=	"  J[XMM[ RMMXM RM[X[";													//*	z
prog_char	gHersheyDefault_7B[]	PROGMEM	=	"  KYTBRCQDPFPHQJRKSMSOQQ RRCQEQGRISJTLTNSPORSTTVTXSZR[Q]Q_Ra RQSSU";		//*	{
prog_char	gHersheyDefault_7C[]	PROGMEM	=	"  NVRBRb";																//*	|
prog_char	gHersheyDefault_7D[]	PROGMEM	=	"  KYPBRCSDTFTHSJRKQMQOSQ RRCSESGRIQJPLPNQPURQTPVPXQZR[S]S_Ra RSSQU";		//*	}
prog_char	gHersheyDefault_7E[]	PROGMEM	=	"  F^IUISJPLONOPPTSVTXTZS[Q RISJQLPNPPQTTVUXUZT[Q[O";						//*	~
prog_char	gHersheyDefault_7F[]	PROGMEM	=	"  JZJFJ[K[KFLFL[M[MFNFN[O[OFPFP[Q[QFRFR[S[SFTFT[U[UFVFV[W[WFXFX[Y[YFZFZ[";//*	
prog_char	gHersheyDefault_80[]	PROGMEM	=	"  ";

//};


PGM_P gHershyFontTable[]	PROGMEM =	   // change "gHersheyDefault_table" name to suit
{
//*	dont waste space with control chars
//	gHersheyDefault_00,
//	gHersheyDefault_01,
//	gHersheyDefault_02,
//	gHersheyDefault_03,
//	gHersheyDefault_04,
//	gHersheyDefault_05,
//	gHersheyDefault_06,
//	gHersheyDefault_07,
//	gHersheyDefault_08,
//	gHersheyDefault_09,
//	gHersheyDefault_0A,
//	gHersheyDefault_0B,
//	gHersheyDefault_0C,
//	gHersheyDefault_0D,
//	gHersheyDefault_0E,
//	gHersheyDefault_0F,

//	gHersheyDefault_10,
//	gHersheyDefault_11,
//	gHersheyDefault_12,
//	gHersheyDefault_13,
//	gHersheyDefault_14,
//	gHersheyDefault_15,
//	gHersheyDefault_16,
//	gHersheyDefault_17,
//	gHersheyDefault_18,
//	gHersheyDefault_19,
//	gHersheyDefault_1A,
//	gHersheyDefault_1B,
//	gHersheyDefault_1C,
//	gHersheyDefault_1D,
//	gHersheyDefault_1E,
//	gHersheyDefault_1F,

	gHersheyDefault_00,
	gHersheyDefault_20,
	gHersheyDefault_21,
	gHersheyDefault_22,
	gHersheyDefault_23,
	gHersheyDefault_24,
	gHersheyDefault_25,
	gHersheyDefault_26,
	gHersheyDefault_27,
	gHersheyDefault_28,
	gHersheyDefault_29,
	gHersheyDefault_2A,
	gHersheyDefault_2B,
	gHersheyDefault_2C,
	gHersheyDefault_2D,
	gHersheyDefault_2E,
	gHersheyDefault_2F,

	gHersheyDefault_30,
	gHersheyDefault_31,
	gHersheyDefault_32,
	gHersheyDefault_33,
	gHersheyDefault_34,
	gHersheyDefault_35,
	gHersheyDefault_36,
	gHersheyDefault_37,
	gHersheyDefault_38,
	gHersheyDefault_39,
	gHersheyDefault_3A,
	gHersheyDefault_3B,
	gHersheyDefault_3C,
	gHersheyDefault_3D,
	gHersheyDefault_3E,
	gHersheyDefault_3F,

	gHersheyDefault_40,
	gHersheyDefault_41,
	gHersheyDefault_42,
	gHersheyDefault_43,
	gHersheyDefault_44,
	gHersheyDefault_45,
	gHersheyDefault_46,
	gHersheyDefault_47,
	gHersheyDefault_48,
	gHersheyDefault_49,
	gHersheyDefault_4A,
	gHersheyDefault_4B,
	gHersheyDefault_4C,
	gHersheyDefault_4D,
	gHersheyDefault_4E,
	gHersheyDefault_4F,

	gHersheyDefault_50,
	gHersheyDefault_51,
	gHersheyDefault_52,
	gHersheyDefault_53,
	gHersheyDefault_54,
	gHersheyDefault_55,
	gHersheyDefault_56,
	gHersheyDefault_57,
	gHersheyDefault_58,
	gHersheyDefault_59,
	gHersheyDefault_5A,
	gHersheyDefault_5B,
	gHersheyDefault_5C,
	gHersheyDefault_5D,
	gHersheyDefault_5E,
	gHersheyDefault_5F,

	gHersheyDefault_60,
	gHersheyDefault_61,
	gHersheyDefault_62,
	gHersheyDefault_63,
	gHersheyDefault_64,
	gHersheyDefault_65,
	gHersheyDefault_66,
	gHersheyDefault_67,
	gHersheyDefault_68,
	gHersheyDefault_69,
	gHersheyDefault_6A,
	gHersheyDefault_6B,
	gHersheyDefault_6C,
	gHersheyDefault_6D,
	gHersheyDefault_6E,
	gHersheyDefault_6F,

	gHersheyDefault_70,
	gHersheyDefault_71,
	gHersheyDefault_72,
	gHersheyDefault_73,
	gHersheyDefault_74,
	gHersheyDefault_75,
	gHersheyDefault_76,
	gHersheyDefault_77,
	gHersheyDefault_78,
	gHersheyDefault_79,
	gHersheyDefault_7A,
	gHersheyDefault_7B,
	gHersheyDefault_7C,
	gHersheyDefault_7D,
	gHersheyDefault_7E,
	gHersheyDefault_7F

};

#pragma mark -
#pragma mark LOCAL functions

//*******************************************************************************
static void	GetFontDef_Default(short tableIndex, char *fontDefString)
{
	strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyFontTable[tableIndex])));
}


//*******************************************************************************
//*	Hershey font definition files
//*		these are put into .h files so they are more managable
//*******************************************************************************


#ifdef _ENABLE_HERSHEY_CURSIVE_
	#include	"HersheyCursive.h"
	//*******************************************************************************
	static void	GetFontDef_Cursive(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyCursiveFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_FUTURAL_
	#include	"HersheyFutural.h"
	//*******************************************************************************
	static void	GetFontDef_Futural(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyFuturalFontTable[tableIndex])));
	}
#endif

#ifdef _ENABLE_HERSHEY_FUTURAM_
	#include	"HersheyFuturam.h"
	//*******************************************************************************
	static void	GetFontDef_Futuram(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyFuturamFontTable[tableIndex])));
	}
#endif


#ifdef _ENABLE_HERSHEY_JAPANESE_
	#include	"HersheyJapanese.h"
	//*******************************************************************************
	static void	GetFontDef_Japanese(short tableIndex, char *fontDefString)
	{
		strcpy_P(fontDefString, (char*)pgm_read_word(&(gHershyJapaneseFontTable[tableIndex])));
	}
#endif





#if 1
//*****************************************************************************
static	short	ScaleFontData(short c, short fontSize)
{
short	newValue;

	newValue	=	(((c-'R') * fontSize) / 20);
//	newValue	=	(((c-'R') * 3) / 6);
//	newValue	=	(((c-'R') * 2) / s);

	return(newValue);

}

#else
	#define	ScaleFontData(c, s)	(((c-'R') * 2) / s)
#endif
//#define	ScaleFontData(c, s)   ((c-'R') / s)
//#define	ScaleFontData(c)   (c-'R')










//********************************************************************************
static void	GetHersheyFontStringFromIndex(char fontNumber, char theCharIndex, char *fontVectorString)
{

	switch(fontNumber)
	{
	#ifdef _ENABLE_HERSHEY_CURSIVE_
		case kHersheyFont_Cursive:
			GetFontDef_Cursive(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_FUTURAL_
		case kHersheyFont_Futural:
			GetFontDef_Futural(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_FUTURAM_
		case kHersheyFont_Futuram:
			GetFontDef_Futuram(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_GREEK_
		case kHersheyFont_Greek:
			GetFontDef_Greek(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_GREEK_SIMPLEX_
		case kHersheyFont_GreekSimplex:
			GetFontDef_GreekSimplex(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_GREEK_COMPLEX_
		case kHersheyFont_GreekComplex:
			GetFontDef_GreekComplex(theCharIndex, fontVectorString);
			break;

	#endif

	//********************************************************************************
	//*	times fonts
	#ifdef _ENABLE_HERSHEY_TIMES_GREEK_
		case kHersheyFont_TimesGreek:
			GetFontDef_TimesGreek(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_TIMES_ITALIC_
		case kHersheyFont_TimesItalic:
			GetFontDef_TimesItalic(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_TIMES_ITALIC_BOLD_
		case kHersheyFont_TimesItalicBold:
			GetFontDef_TimesItalicBold(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_TIMES_ROMAN_
		case kHersheyFont_TimesRoman:
			GetFontDef_TimesRoman(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_TIMES_ROMAN_BOLD_
		case kHersheyFont_TimesRomanBold:
			GetFontDef_TimesRomanBold(theCharIndex, fontVectorString);
			break;
	#endif

	//********************************************************************************
	//*	Roman fonts
	#ifdef _ENABLE_HERSHEY_ROWMAN_SIMPLEX_
		case kHersheyFont_RowmanSimplex:
			GetFontDef_RowmanSimplex(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_ROWMAN_DUPLEX_
		case kHersheyFont_RowmanDuplex:
			GetFontDef_RowmanDuplex(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_ROWMAN_TRIPLEX_
		case kHersheyFont_RowmanTriplex:
			GetFontDef_RowmanTriplex(theCharIndex, fontVectorString);
			break;
	#endif



	//********************************************************************************
	//*	script fonts
	#ifdef _ENABLE_HERSHEY_SCRIPT_SIMPLEX_
		case kHersheyFont_ScriptSimplex:
			GetFontDef_ScriptSimplex(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_SCRIPT_COMPLEX_
		case kHersheyFont_ScriptComplex:
			GetFontDef_ScriptComplex(theCharIndex, fontVectorString);
			break;
	#endif


	//********************************************************************************
	//*	symbol fonts

	#ifdef _ENABLE_HERSHEY_ASTROLOGY_
		case kHersheyFont_Astrology:
			GetFontDef_Astrology(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_MARKERS_
		case kHersheyFont_Markers:
			GetFontDef_Markers(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_MATH_
		case kHersheyFont_Mathlow:
			GetFontDef_Mathlow(theCharIndex, fontVectorString);
			break;

		case kHersheyFont_Mathupp:
			GetFontDef_Mathupp(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_METEOROLOGY_
		case kHersheyFont_Meteorology:
			GetFontDef_Meteorology(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_MUSIC_
		case kHersheyFont_Music:
			GetFontDef_Music(theCharIndex, fontVectorString);
			break;
	#endif


	#ifdef _ENABLE_HERSHEY_SYMBOLIC_
		case kHersheyFont_Symbolic:
			GetFontDef_Symbolic(theCharIndex, fontVectorString);
			break;
	#endif

	//********************************************************************************
	//*	Gothic fonts
	#ifdef _ENABLE_HERSHEY_GOTHIC_ENGLISH_
		case kHersheyFont_GothicEnglish:
			GetFontDef_GothicEnglish(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_GOTHIC_GERMAN_
		case kHersheyFont_GothicGerman:
			GetFontDef_GothicGerman(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_GOTHIC_ITALIAN_
		case kHersheyFont_GothicItalian:
			GetFontDef_GothicItalian(theCharIndex, fontVectorString);
			break;
	#endif


	#ifdef _ENABLE_HERSHEY_GOTHIC_GBT_
		case kHersheyFont_Gothic_GBT:
			GetFontDef_GothicGBT(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_GOTHITT_
		case kHersheyFont_Gothitt:
			GetFontDef_Gothitt(theCharIndex, fontVectorString);
			break;
	#endif



	//********************************************************************************
	#ifdef _ENABLE_HERSHEY_CYRILLIC_
		case kHersheyFont_Cyrillic:
			GetFontDef_Cyrillic(theCharIndex, fontVectorString);
			break;


		case kHersheyFont_Cyrilic1:
			GetFontDef_Cyrilic1(theCharIndex, fontVectorString);
			break;
	#endif

	#ifdef _ENABLE_HERSHEY_JAPANESE_
		case kHersheyFont_Japanese:
			GetFontDef_Japanese(theCharIndex, fontVectorString);
			break;
	#endif


		default:
			GetFontDef_Default(theCharIndex, fontVectorString);
			break;
	}
}

//********************************************************************************
void	GetHersheyFontName(char fontNumber, char *fontName)
{

	GetHersheyFontStringFromIndex(fontNumber, 0, fontName);
	
}

//********************************************************************************
static void	toupperStr(char *string)
{
short	ii;

	ii	=	0;
	while (string[ii] != 0)
	{
		string[ii]	=	toupper(string[ii]);
		ii++;
	}
}

#pragma mark -
#pragma mark GLOBAL functions
//********************************************************************************
char	GetHersheyFontNumerFromName(char *fontName)
{
char	lookUpFontName[32];
char	currentFontName[32];
byte	ii, fontIndex;

	fontIndex	=	0;
	strcpy(lookUpFontName, fontName);
	toupperStr(lookUpFontName);
	
	for (ii=kHershey_FirstFontNumber; ii<kHersheyFont_last; ii++)
	{
		//*	ii is font index
		//*	0 is char index, 0 means the FONT NAME
		GetHersheyFontStringFromIndex(ii, 0, currentFontName);
		toupperStr(currentFontName);
		if (strcmp(lookUpFontName, currentFontName) == 0)
		{
			fontIndex	=	ii;
			break;
		}
	}
	return(fontIndex);
}


//********************************************************************************
long	GetHersheyFontTableSize(char fontNumber)
{
short	ii;
long	byteCount;
char	fontVectorString[kMaxVectorStringLength];

	byteCount	=	0;
	for (ii = 31; ii <= 127; ii++)
	{
		GetHersheyFontStringFromIndex(fontNumber, (ii - 31), fontVectorString);
		byteCount	+=	strlen(fontVectorString);		//*	add the length of the string
		byteCount	+=	1;								//*	add 1 for the 0x00 at the end of the string
		byteCount	+=	2;								//*	add 2 for the address in the table to the string
	}
	
	return(byteCount);
}

//********************************************************************************
//*	the 2nd pair of letters is the left and right side of the char in pixels. They have to be scaled 
short	HersheyGetCharWidth(char fontNumber, char theChar, short fontSize)
{
short	theCharWidth;

#ifdef _HERSHEY_PROP_SPACING_
char	fontVectorString[kMaxVectorStringLength];
short	leftSide, rightSide;

	GetHersheyFontStringFromIndex(fontNumber, (theChar - 31), fontVectorString);

	leftSide		=	ScaleFontData(fontVectorString[2], fontSize);
	rightSide		=	ScaleFontData(fontVectorString[3], fontSize);
	theCharWidth	=	rightSide - leftSide;
#else
	theCharWidth	=	fontSize + (fontSize / 6);
#endif
	return(theCharWidth);
}

//********************************************************************************
short	HersheyGetStringWidth(char fontNumber, char *theString, short fontSize)
{
short	totalWidth;
short	ii;

	totalWidth	=	0;
	ii			=	0;
	while (theString[ii] != 0)
	{
		totalWidth	+=	HersheyGetCharWidth(fontNumber, theString[ii], fontSize);
		ii++;
	}
	return(totalWidth);
}



//********************************************************************************
//  HersheyDrawLetter() interprets the instructions from the array
//  for that letter and renders the letter with line segments.
//********************************************************************************
//*	this version does the scaling of the data EACH time, very inefficient for speed
//*	but much more efficient for memory
//********************************************************************************
//*	returns the width of the char
static short	HersheyDrawLetter(	short	startX,
									short	startY,
									char	fontSize,
									char	penSize,
									short	fontAngle,
									char	startOffset,
									char	*theVectorString)
{
boolean	moveFlag;
short	ii;
short	newX, newY;
short	currentX, currentY;
short	leftMargin, rightMargin;
short	charWidth;
//short	moveCount;
	moveFlag	=	true;

	leftMargin	=	ScaleFontData(theVectorString[2], fontSize);
	rightMargin	=	ScaleFontData(theVectorString[3], fontSize);
	charWidth	=	rightMargin - leftMargin;

	currentX	=	0;
	currentY	=	0;
	ii			=	startOffset;

	while (theVectorString[ii] != 0)
	{
		if ((theVectorString[ii] == ' ') && (theVectorString[ii+1] == 'R' ))
		{
			moveFlag	=	true;
		}
	 	else
		{
		//	float x =  h2float(theVectorString[ii] );
		//	float y = -h2float(theVectorString[ii+1] );


			newX	=	ScaleFontData(theVectorString[ii], fontSize);
			newY	=	ScaleFontData(theVectorString[ii+1], fontSize);

		#ifdef _ENABLE_ROTATED_HERSHEY_FONTS_
			if (fontAngle != 0)
			{
			long	polorR;
			double	polorTheta;
			double	doubleRotateAngle;
			double	doubleXX, doubleYY;

				
				#define	kMultiplyFactor	10.0	//*	this is used to make the sin/cos work better

				doubleXX			=	newX * kMultiplyFactor;
				doubleYY			=	newY * kMultiplyFactor;
				
				//*	first calculate the polor coordinates
				polorR				=	lround(sqrt((doubleXX * doubleXX) + (doubleYY * doubleYY)));
				polorTheta			=	atan(doubleYY / doubleXX);

				//*	we have to adjust for the quadrant
				if (doubleXX < 0)
				{
					polorTheta		+=	PI;
				}
				
				
				//*	add the headding angle
				doubleRotateAngle	=	fontAngle;
				polorTheta			-=	(doubleRotateAngle * DEG_TO_RAD);
				
				
				if (polorTheta > TWO_PI)
				{
					polorTheta	-=	TWO_PI;
				}
				
				//*	convert back to x,y
				newX		=	lround((polorR * cos(polorTheta)) / kMultiplyFactor);
				newY		=	lround((polorR * sin(polorTheta)) / kMultiplyFactor);
				
			}
		#endif
		
			if (moveFlag)
			{
				moveFlag	=	false;
			}
			else
			{
				MoveTo(startX + currentX, startY + currentY);
				LineTo(startX + newX, startY + newY);
				if (penSize > 1)
				{
				short	offsetX, offsetY;
					
					if (newX > newY)
					{
						offsetX	=	0;
						offsetY	=	1;
					}
					else
					{
						offsetX	=	1;
						offsetY	=	0;
					}
					MoveTo(startX + currentX + offsetX, startY + currentY + offsetY);
					LineTo(startX + newX + offsetX, startY + newY + offsetY);
					if (penSize > 1)
					{
						MoveTo(startX + currentX - offsetX, startY + currentY - offsetY);
						LineTo(startX + newX - offsetX, startY + newY - offsetY);
					}
				}
			}
			currentX	=	newX;
			currentY	=	newY;
		}
		ii	+=	2;
	}
	return(charWidth);
}


#pragma mark -
#pragma mark Global functions
//********************************************************************************
//*	arguments
//*		char fontNumber,		this is an 8 bit integer (not a char)
//*									specifying which font to draw. 
//*		short xloc,				16 bit int X location for start of string
//*		short yloc,				16 bit int Y location for start of string
//*		char *theCstr,			the char string to draw
//*		char fontSize,			this is an 8 bit integer in points, 6 is about the smallest
//*									that can be read
//*		short fontAngle,		16 bit integer in degrees (-180 -> +180)
//*									0 degrees is horizontal (normal), positive is UP (counter-clockwise)
//*		char penSize			8 bit integer for line width. Currently ignored
//********************************************************************************
void	HersheyDrawCString(char fontNumber, short xloc, short yloc, char *theCstr, char fontSize, short fontAngle, char penSize)
{
short	ii;
short	myCharIndex;
short	myDeltaX, myDeltaY;
short	myXloc, myYloc;
char	myVectorString[kMaxVectorStringLength];
short	charWidth;

	myXloc		=	xloc;
	myYloc		=	yloc;
	myDeltaX	=	fontSize + (fontSize / 6);
	myDeltaY	=	0;

#ifdef _ENABLE_ROTATED_HERSHEY_FONTS_
	if (fontAngle != 0)
	{
	float	myRadians;
	
		myRadians	=	DEG_TO_RAD * fontAngle;
		myDeltaX	=	myDeltaX * cos(myRadians);
		myDeltaY	=	-(fontSize * sin(myRadians));
	}
#endif
	
	
	if (fontSize < 5)
	{
		fontSize	=	5;
	}
	ii	=	0;
	while (theCstr[ii] > 0)
	{

		myCharIndex	=	theCstr[ii] & 0x7f;
		myCharIndex	-=	32;		//*	we dont have control chars in the table
		myCharIndex	+=	1;		//*	add 1 more because the FIRST entry is the font name
		if (myCharIndex < 0)
		{
			myCharIndex	=	0;
		}

		GetHersheyFontStringFromIndex(fontNumber, myCharIndex, myVectorString);

		//*	we want to start with the 4th char, 1st 2 are # of pairs, 2nd 2 are leftMargin, rightMargin
    	charWidth	=	HersheyDrawLetter(myXloc, myYloc, fontSize, penSize, fontAngle, 4, myVectorString);
	#ifdef _HERSHEY_PROP_SPACING_
		if (fontAngle == 0)
		{
			myXloc	+=	charWidth;
			myXloc	+=	(fontSize / 6);
		}
		else
	#endif
		{
			myXloc	+=	myDeltaX;
			myYloc	+=	myDeltaY;
		}
	
		ii++;
	}
}

//********************************************************************************
void	HersheyDisplayFontList(RECT *displayRect, boolean showSize)
{
int		xLoc, yLoc;
int		ii;
char	theFontName[32];
long	fontDataLen;
short	myDisplayWidth;

#ifdef _USE_WHITE_BACKGROUND_
	fill(255);
#else
	fill(0);
#endif
	myDisplayWidth	=	displayRect->right - displayRect->left;
	
	EraseRect(displayRect);

#ifdef _USE_WHITE_BACKGROUND_
	stroke(0, 0, 0);
#else
	stroke(random(100, 255), random(100, 255), random(100, 255));
#endif

	FrameRect(displayRect);
	yLoc	=	10;
	DrawCString(5, yLoc, "Installed Hershey Fonts");
	xLoc	=	20;
	yLoc	=	30;
	for (ii = kHershey_FirstFontNumber; ii <kHersheyFont_last; ii++)
	{
		GetHersheyFontName(ii, theFontName);
		DrawCString(25, yLoc, theFontName);
		if (showSize)
		{
			fontDataLen	=	GetHersheyFontTableSize(ii);
	
			sprintf(theFontName, "%ld bytes", fontDataLen);
			DrawCString((displayRect->left + (myDisplayWidth / 2)), yLoc, theFontName);
			yLoc	+=	15;
		}
		else
		{
			//*	print some sample text
			HersheyDrawCString(ii, (myDisplayWidth / 2), yLoc + 7, "ABCDEFGHIJKLMNOPQRSTUVWXYZ",   15, 0, 1);
			yLoc	+=	20;
			
		}

		if (yLoc > displayRect->bottom)
		{
			delay(500);
			EraseRect(displayRect);
			FrameRect(displayRect);
			yLoc	=	10;
			DrawCString(5, yLoc, "Installed Hershey Fonts");
			xLoc	=	20;
			yLoc	=	30;

		}
	}
	
	fill(0);

}

//*******************************************************************************
void	HersheyDisplayFontSamples(RECT *displayRect)
{
int		xLoc, yLoc;
int		xLoc2;
int		fontSize;
char	fontName[32];
short	fontNum;

	for (fontNum = kHershey_FirstFontNumber; fontNum <kHersheyFont_last; fontNum++)
	{
		stroke(random(100, 255),random(100, 255),random(100, 255));
		EraseScreen();
		
		FrameRect(displayRect);

		GetHersheyFontName(fontNum, fontName);

		xLoc	=	30;
		yLoc	=	10;
		xLoc2	=	(displayRect->right - displayRect->left) / 2;
		
		DrawCString(xLoc, yLoc, "Hershey fonts");
		yLoc		+=	12;

		//*******************************************************************************
		DrawCString(xLoc, yLoc, fontName);
		DrawCString(xLoc2, yLoc, "Size 6");
		yLoc		+=	20;
		fontSize	=	6;
		
		HersheyDrawCString(fontNum, xLoc, yLoc, "ABCDEFGHIJKLMNOPQRSTUVWXYZ !@#$%^&()[];'><",  fontSize, 0, 1);
		yLoc	+=	15;
		HersheyDrawCString(fontNum, xLoc, yLoc, "abcdefghijklmnopqrstuvwzyz 1234567890 ", fontSize,   0, 1);
		yLoc	+=	15;



		//*******************************************************************************
		DrawCString(xLoc, yLoc, fontName);
		DrawCString(xLoc2, yLoc, "Size 10");
		fontSize	=	10;
		yLoc		+=	25;
		HersheyDrawCString(fontNum, xLoc, yLoc, "ABCDEFGHIJKLMNOPQRSTUVWXYZ",   fontSize, 0, 1);
		yLoc	+=	25;
		HersheyDrawCString(fontNum, xLoc, yLoc, "abcdefghijklmnopqrstuvwzyz 1234567890 ", fontSize,   0, 1);
		yLoc	+=	25;

		//*******************************************************************************
		DrawCString(xLoc, yLoc, fontName);
		DrawCString(xLoc2, yLoc, "Size 40");
		fontSize	=	40;
		yLoc		+=	40;
		HersheyDrawCString(fontNum, xLoc, yLoc, "ABCDEFGHIJKL",  fontSize,  0, 1);
		yLoc	+=	30;

	#ifdef _ARDUINO_MEGA_
		//*******************************************************************************
		DrawCString(xLoc, yLoc, fontName);
		DrawCString(xLoc2, yLoc, "Size 70");
		fontSize	=	70;
		yLoc		+=	70;
		xLoc		+=	15;
		HersheyDrawCString(fontNum, xLoc, yLoc, "ABCDEFG",  fontSize,  0, 1);
		yLoc	+=	30;
	#endif
	
		delay(3000);
	}
}

#endif		//*	_ENABLE_HERSHEY_FONTS_