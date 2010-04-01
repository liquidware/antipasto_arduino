//*******************************************************************************
//#include	"startup.h"

#define	_STARTUP_H_
#ifdef _STARTUP_H_

//*	if you want more memory space or want to get rid of the startup logo, comment out this line
#define	_STARTUPSCREEN_LIQUIDWARE_


#ifdef __cplusplus
extern "C"{
#endif

	extern void sketchEarlyInits();
	extern void sketchLateInits();
	extern void runSketch();

#ifdef __cplusplus
} // extern "C"
#endif


#endif		//_STARTUP_H_