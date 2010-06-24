//****************************************************************************
//*	BMP support for flash memory
//****************************************************************************
//*	Edit History
//****************************************************************************
//*	??? ??,	2009	<CML> worked on bmp stuff
//****************************************************************************


#ifndef __BMP_H_
#define __BMP_H_

//*	this was the old system
//#define	_USE_7LETTER_NAMES_

#define	kLookupTablePage			4094
#define	kLookupTableSize			DATAFLASH_PAGESIZE
#define	kLookupTableMaxEntries		(kLookupTableSize / kSizeOfFlashFileEntry)
#define	kLookupTableLocation		(kLookupTablePage * DATAFLASH_PAGESIZE)
#define	BMP_MAXBMP_COUNT			44 //!< Max BMP count

#ifdef _USE_7LETTER_NAMES_

	#define	kFileNameSize				7

	typedef struct {
					char	fileName[7];
					char	flashOffsetByte1;
					char	flashOffsetByte2;
					char	flashOffsetByte3;
					char	flashOffsetByte4;
					} FLASH_FILE_ENTRY;

	#define	kSizeOfFlashFileEntry	11

#else
	#define	kFileNameSize				12
	//* A file table entry
	typedef struct
	{
		char		fileName[kFileNameSize];
		uint32_t	flashOffset;
	} FLASH_FILE_ENTRY;

	#define	kSizeOfFlashFileEntry		16

#endif

typedef uint64_t PImage;

#ifdef __cplusplus
extern "C"{
#endif


unsigned char	bmp_lookup_table_checkStatus();
char			bmp_draw(char *bmp_name, int xLoc, int yLoc);
char			bmp_draw_imageN(int imageIndex, int xLoc, int yLoc);
char			bmp_get_entryname(unsigned char entryNumber, char *entryName, short *imgWidth, short *imgHeight);
char			bmp_get_entryInfoFromName(char *bmp_name, short *imgWidth, short *imgHeight);
void            bmp_drawTest(char * bmp_name, unsigned int x, unsigned int y);
void            bmp_store(char* tBuff, char* fileName, uint32_t location);
uint32_t        bmp_find(char * fileName);

extern FLASH_FILE_ENTRY gBMPlookupTable[kLookupTableMaxEntries+1];


void			bmp_init();

#ifdef __cplusplus
} // extern "C"
#endif

PImage loadImage(char * FileName);
void image(PImage image, int xLoc, int yLoc);

#endif

