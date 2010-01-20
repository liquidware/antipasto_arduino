//*******************************************************************************
//*	futural.jhf		Futural
//*	formated for use with Arduino Processing 
//*	by Mark Sproul
//*******************************************************************************
//*	Jan 25,	2009	<MLS>	Formated Hershey Futural for Arduino
//*******************************************************************************
//#include	"HersheyCursive.h"



//*	the 1st 2 bytes are decimal # of pairs
//*	the rest are offset coordinate pairs, each letter gets 'R' subtracted from it to come up with a number
//*	the firt pair is left position and right position

prog_char	gFutural_00[]	PROGMEM	=	"Futural";

prog_char	gFutural_20[]	PROGMEM	=	" 1JZ";
prog_char	gFutural_21[]	PROGMEM	=	" 9MWRFRT RRYQZR[SZRY";
prog_char	gFutural_22[]	PROGMEM	=	" 6JZNFNM RVFVM";
prog_char	gFutural_23[]	PROGMEM	=	"12H]SBLb RYBRb RLOZO RKUYU";
prog_char	gFutural_24[]	PROGMEM	=	"27H\\PBP_ RTBT_ RYIWGTFPFMGKIKKLMMNOOUQWRXSYUYXWZT[P[MZKX";
prog_char	gFutural_25[]	PROGMEM	=	"32F^[FI[ RNFPHPJOLMMKMIKIIJGLFNFPGSHVHYG[F RWTUUTWTYV[X[ZZ[X[VYTWT";
prog_char	gFutural_26[]	PROGMEM	=	"35E_\\O\\N[MZMYNXPVUTXRZP[L[JZIYHWHUISJRQNRMSKSIRGPFNGMIMKNNPQUXWZY[[[\\Z\\Y";
prog_char	gFutural_27[]	PROGMEM	=	" 8MWRHQGRFSGSIRKQL";
prog_char	gFutural_28[]	PROGMEM	=	"11KYVBTDRGPKOPOTPYR]T`Vb";
prog_char	gFutural_29[]	PROGMEM	=	"11KYNBPDRGTKUPUTTYR]P`Nb";
prog_char	gFutural_2A[]	PROGMEM	=	" 9JZRLRX RMOWU RWOMU";
prog_char	gFutural_2B[]	PROGMEM	=	" 6E_RIR[ RIR[R";
prog_char	gFutural_2C[]	PROGMEM	=	" 8NVSWRXQWRVSWSYQ[";
prog_char	gFutural_2D[]	PROGMEM	=	" 3E_IR[R";
prog_char	gFutural_2E[]	PROGMEM	=	" 6NVRVQWRXSWRV";
prog_char	gFutural_2F[]	PROGMEM	=	" 3G][BIb";

prog_char	gFutural_30[]	PROGMEM	=	"18H\\QFNGLJKOKRLWNZQ[S[VZXWYRYOXJVGSFQF";
prog_char	gFutural_31[]	PROGMEM	=	" 5H\\NJPISFS[";
prog_char	gFutural_32[]	PROGMEM	=	"15H\\LKLJMHNGPFTFVGWHXJXLWNUQK[Y[";
prog_char	gFutural_33[]	PROGMEM	=	"16H\\MFXFRNUNWOXPYSYUXXVZS[P[MZLYKW";
prog_char	gFutural_34[]	PROGMEM	=	" 7H\\UFKTZT RUFU[";
prog_char	gFutural_35[]	PROGMEM	=	"18H\\WFMFLOMNPMSMVNXPYSYUXXVZS[P[MZLYKW";
prog_char	gFutural_36[]	PROGMEM	=	"24H\\XIWGTFRFOGMJLOLTMXOZR[S[VZXXYUYTXQVOSNRNOOMQLT";
prog_char	gFutural_37[]	PROGMEM	=	" 6H\\YFO[ RKFYF";
prog_char	gFutural_38[]	PROGMEM	=	"30H\\PFMGLILKMMONSOVPXRYTYWXYWZT[P[MZLYKWKTLRNPQOUNWMXKXIWGTFPF";
prog_char	gFutural_39[]	PROGMEM	=	"24H\\XMWPURRSQSNRLPKMKLLINGQFRFUGWIXMXRWWUZR[P[MZLX";
prog_char	gFutural_3A[]	PROGMEM	=	"12NVROQPRQSPRO RRVQWRXSWRV";
prog_char	gFutural_3B[]	PROGMEM	=	"14NVROQPRQSPRO RSWRXQWRVSWSYQ[";
prog_char	gFutural_3C[]	PROGMEM	=	" 4F^ZIJRZ[";
prog_char	gFutural_3D[]	PROGMEM	=	" 6E_IO[O RIU[U";
prog_char	gFutural_3E[]	PROGMEM	=	" 4F^JIZRJ[";
prog_char	gFutural_3F[]	PROGMEM	=	"21I[LKLJMHNGPFTFVGWHXJXLWNVORQRT RRYQZR[SZRY";

prog_char	gFutural_40[]	PROGMEM	=	"56E`WNVLTKQKOLNMMPMSNUPVSVUUVS RQKOMNPNSOUPV RWKVSVUXVZV\\T]Q]O\\L[JYHWGTFQFNGLHJJILHOHRIUJWLYNZQ[T[WZYYZX RXKWSWUXV";
prog_char	gFutural_41[]	PROGMEM	=	" 9I[RFJ[ RRFZ[ RMTWT";
prog_char	gFutural_42[]	PROGMEM	=	"24G\\KFK[ RKFTFWGXHYJYLXNWOTP RKPTPWQXRYTYWXYWZT[K[";
prog_char	gFutural_43[]	PROGMEM	=	"19H]ZKYIWGUFQFOGMILKKNKSLVMXOZQ[U[WZYXZV";
prog_char	gFutural_44[]	PROGMEM	=	"16G\\KFK[ RKFRFUGWIXKYNYSXVWXUZR[K[";
prog_char	gFutural_45[]	PROGMEM	=	"12H[LFL[ RLFYF RLPTP RL[Y[";
prog_char	gFutural_46[]	PROGMEM	=	" 9HZLFL[ RLFYF RLPTP";
prog_char	gFutural_47[]	PROGMEM	=	"23H]ZKYIWGUFQFOGMILKKNKSLVMXOZQ[U[WZYXZVZS RUSZS";
prog_char	gFutural_48[]	PROGMEM	=	" 9G]KFK[ RYFY[ RKPYP";
prog_char	gFutural_49[]	PROGMEM	=	" 3NVRFR[";
prog_char	gFutural_4A[]	PROGMEM	=	"11JZVFVVUYTZR[P[NZMYLVLT";
prog_char	gFutural_4B[]	PROGMEM	=	" 9G\\KFK[ RYFKT RPOY[";
prog_char	gFutural_4C[]	PROGMEM	=	" 6HYLFL[ RL[X[";
prog_char	gFutural_4D[]	PROGMEM	=	"12F^JFJ[ RJFR[ RZFR[ RZFZ[";
prog_char	gFutural_4E[]	PROGMEM	=	" 9G]KFK[ RKFY[ RYFY[";
prog_char	gFutural_4F[]	PROGMEM	=	"22G]PFNGLIKKJNJSKVLXNZP[T[VZXXYVZSZNYKXIVGTFPF";

prog_char	gFutural_50[]	PROGMEM	=	"14G\\KFK[ RKFTFWGXHYJYMXOWPTQKQ";
prog_char	gFutural_51[]	PROGMEM	=	"25G]PFNGLIKKJNJSKVLXNZP[T[VZXXYVZSZNYKXIVGTFPF RSWY]";
prog_char	gFutural_52[]	PROGMEM	=	"17G\\KFK[ RKFTFWGXHYJYLXNWOTPKP RRPY[";
prog_char	gFutural_53[]	PROGMEM	=	"21H\\YIWGTFPFMGKIKKLMMNOOUQWRXSYUYXWZT[P[MZKX";
prog_char	gFutural_54[]	PROGMEM	=	" 6JZRFR[ RKFYF";
prog_char	gFutural_55[]	PROGMEM	=	"11G]KFKULXNZQ[S[VZXXYUYF";
prog_char	gFutural_56[]	PROGMEM	=	" 6I[JFR[ RZFR[";
prog_char	gFutural_57[]	PROGMEM	=	"12F^HFM[ RRFM[ RRFW[ R\\FW[";
prog_char	gFutural_58[]	PROGMEM	=	" 6H\\KFY[ RYFK[";
prog_char	gFutural_59[]	PROGMEM	=	" 7I[JFRPR[ RZFRP";
prog_char	gFutural_5A[]	PROGMEM	=	" 9H\\YFK[ RKFYF RK[Y[";
prog_char	gFutural_5B[]	PROGMEM	=	"12KYOBOb RPBPb ROBVB RObVb";
prog_char	gFutural_5C[]	PROGMEM	=	" 3KYKFY^";
prog_char	gFutural_5D[]	PROGMEM	=	"12KYTBTb RUBUb RNBUB RNbUb";
prog_char	gFutural_5E[]	PROGMEM	=	" 6JZRDJR RRDZR";
prog_char	gFutural_5F[]	PROGMEM	=	" 3I[Ib[b";

prog_char	gFutural_60[]	PROGMEM	=	" 8NVSKQMQORPSORNQO";
prog_char	gFutural_61[]	PROGMEM	=	"18I\\XMX[ RXPVNTMQMONMPLSLUMXOZQ[T[VZXX";
prog_char	gFutural_62[]	PROGMEM	=	"18H[LFL[ RLPNNPMSMUNWPXSXUWXUZS[P[NZLX";
prog_char	gFutural_63[]	PROGMEM	=	"15I[XPVNTMQMONMPLSLUMXOZQ[T[VZXX";
prog_char	gFutural_64[]	PROGMEM	=	"18I\\XFX[ RXPVNTMQMONMPLSLUMXOZQ[T[VZXX";
prog_char	gFutural_65[]	PROGMEM	=	"18I[LSXSXQWOVNTMQMONMPLSLUMXOZQ[T[VZXX";
prog_char	gFutural_66[]	PROGMEM	=	" 9MYWFUFSGRJR[ ROMVM";
prog_char	gFutural_67[]	PROGMEM	=	"23I\\XMX]W`VaTbQbOa RXPVNTMQMONMPLSLUMXOZQ[T[VZXX";
prog_char	gFutural_68[]	PROGMEM	=	"11I\\MFM[ RMQPNRMUMWNXQX[";
prog_char	gFutural_69[]	PROGMEM	=	" 9NVQFRGSFREQF RRMR[";
prog_char	gFutural_6A[]	PROGMEM	=	"12MWRFSGTFSERF RSMS^RaPbNb";
prog_char	gFutural_6B[]	PROGMEM	=	" 9IZMFM[ RWMMW RQSX[";
prog_char	gFutural_6C[]	PROGMEM	=	" 3NVRFR[";
prog_char	gFutural_6D[]	PROGMEM	=	"19CaGMG[ RGQJNLMOMQNRQR[ RRQUNWMZM\\N]Q][";
prog_char	gFutural_6E[]	PROGMEM	=	"11I\\MMM[ RMQPNRMUMWNXQX[";
prog_char	gFutural_6F[]	PROGMEM	=	"18I\\QMONMPLSLUMXOZQ[T[VZXXYUYSXPVNTMQM";

prog_char	gFutural_70[]	PROGMEM	=	"18H[LMLb RLPNNPMSMUNWPXSXUWXUZS[P[NZLX";
prog_char	gFutural_71[]	PROGMEM	=	"18I\\XMXb RXPVNTMQMONMPLSLUMXOZQ[T[VZXX";
prog_char	gFutural_72[]	PROGMEM	=	" 9KXOMO[ ROSPPRNTMWM";
prog_char	gFutural_73[]	PROGMEM	=	"18J[XPWNTMQMNNMPNRPSUTWUXWXXWZT[Q[NZMX";
prog_char	gFutural_74[]	PROGMEM	=	" 9MYRFRWSZU[W[ ROMVM";
prog_char	gFutural_75[]	PROGMEM	=	"11I\\MMMWNZP[S[UZXW RXMX[";
prog_char	gFutural_76[]	PROGMEM	=	" 6JZLMR[ RXMR[";
prog_char	gFutural_77[]	PROGMEM	=	"12G]JMN[ RRMN[ RRMV[ RZMV[";
prog_char	gFutural_78[]	PROGMEM	=	" 6J[MMX[ RXMM[";
prog_char	gFutural_79[]	PROGMEM	=	"10JZLMR[ RXMR[P_NaLbKb";
prog_char	gFutural_7A[]	PROGMEM	=	" 9J[XMM[ RMMXM RM[X[";
prog_char	gFutural_7B[]	PROGMEM	=	"40KYTBRCQDPFPHQJRKSMSOQQ RRCQEQGRISJTLTNSPORSTTVTXSZR[Q]Q_Ra RQSSUSWRYQZP\\P^Q`RaTb";
prog_char	gFutural_7C[]	PROGMEM	=	" 3NVRBRb";
prog_char	gFutural_7D[]	PROGMEM	=	"40KYPBRCSDTFTHSJRKQMQOSQ RRCSESGRIQJPLPNQPURQTPVPXQZR[S]S_Ra RSSQUQWRYSZT\\T^S`RaPb";
prog_char	gFutural_7E[]	PROGMEM	=	"24F^IUISJPLONOPPTSVTXTZS[Q RISJQLPNPPQTTVUXUZT[Q[O";
prog_char	gFutural_7F[]	PROGMEM	=	"35JZJFJ[K[KFLFL[M[MFNFN[O[OFPFP[Q[QFRFR[S[SFTFT[U[UFVFV[W[WFXFX[Y[YFZFZ[";


PGM_P gHershyFuturalFontTable[]	PROGMEM =
{

	gFutural_00,
	gFutural_20,
	gFutural_21,
	gFutural_22,
	gFutural_23,
	gFutural_24,
	gFutural_25,
	gFutural_26,
	gFutural_27,
	gFutural_28,
	gFutural_29,
	gFutural_2A,
	gFutural_2B,
	gFutural_2C,
	gFutural_2D,
	gFutural_2E,
	gFutural_2F,

	gFutural_30,
	gFutural_31,
	gFutural_32,
	gFutural_33,
	gFutural_34,
	gFutural_35,
	gFutural_36,
	gFutural_37,
	gFutural_38,
	gFutural_39,
	gFutural_3A,
	gFutural_3B,
	gFutural_3C,
	gFutural_3D,
	gFutural_3E,
	gFutural_3F,

	gFutural_40,
	gFutural_41,
	gFutural_42,
	gFutural_43,
	gFutural_44,
	gFutural_45,
	gFutural_46,
	gFutural_47,
	gFutural_48,
	gFutural_49,
	gFutural_4A,
	gFutural_4B,
	gFutural_4C,
	gFutural_4D,
	gFutural_4E,
	gFutural_4F,

	gFutural_50,
	gFutural_51,
	gFutural_52,
	gFutural_53,
	gFutural_54,
	gFutural_55,
	gFutural_56,
	gFutural_57,
	gFutural_58,
	gFutural_59,
	gFutural_5A,
	gFutural_5B,
	gFutural_5C,
	gFutural_5D,
	gFutural_5E,
	gFutural_5F,

	gFutural_60,
	gFutural_61,
	gFutural_62,
	gFutural_63,
	gFutural_64,
	gFutural_65,
	gFutural_66,
	gFutural_67,
	gFutural_68,
	gFutural_69,
	gFutural_6A,
	gFutural_6B,
	gFutural_6C,
	gFutural_6D,
	gFutural_6E,
	gFutural_6F,

	gFutural_70,
	gFutural_71,
	gFutural_72,
	gFutural_73,
	gFutural_74,
	gFutural_75,
	gFutural_76,
	gFutural_77,
	gFutural_78,
	gFutural_79,
	gFutural_7A,
	gFutural_7B,
	gFutural_7C,
	gFutural_7D,
	gFutural_7E,
	gFutural_7F

};
