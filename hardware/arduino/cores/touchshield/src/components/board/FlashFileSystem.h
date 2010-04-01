//*******************************************************************************
//*	Flsh Memory File System
//*		by Mark Sproul
//*		this source code is part of the liquid ware drivers for the 
//*		TouchShield and TouchSlide
//*		it is public domain under the the GPL as is the rest of the Arduino family
//*
//*	These routines handle the file system for the Flash Memory (16 megabit, 2 megaBYTE)
//*	on board the screens.
//*******************************************************************************
//#include	"FlashFileSystem.h"

#ifndef _FLASH_FILE_SYSTEM_H_
#define _FLASH_FILE_SYSTEM_H_

/* A list of valid executables */
typedef enum {
    FlashTransfer
}ProgramEx;

//*	this is the DIRECTORY entry for the flash file system
#define	kFlashFileNameLen	8
#define	kFlashMagicCookie	0x55AA
typedef struct {
					char	fileName[kFlashFileNameLen];
					char	extension[3];
					long	fileSize;				//*	in bytes
					short	startBlock;
					
					
					
				} FlashDirEntry;


//*	this is a file block stored as a 528 byte PAGE in the Flash memory
typedef struct {
					short	magicCookie;			//*	this should be 0x55AA for a block that has been written by this file system
					short	fileIndex;				//*	which directory index does this block belong to
					short	blockIndex;				//*	which block is this within the file
					short	nextBlock;				//*	the next block that comes after this one
					long	checksum;				//*	32 bit checksum
				} FlashFileBlock;

void DebugRectPrintText(char * msg);
void FlashFileSystemComm(void);
void open(ProgramEx p);

#endif




