#ifndef __BMP_H_
#define __BMP_H_



#define kFileNameSize				12
#define	kSizeOfFlashFileEntry		16
#define kLookupTablePage			4094
#define kLookupTableSize			DATAFLASH_PAGESIZE
#define kLookupTableMaxEntries		kLookupTableSize / kSizeOfFlashFileEntry
#define kLookupTableLocation		kLookupTablePage * DATAFLASH_PAGESIZE

/* A file table entry */
typedef struct {
	char	fileName[kFileNameSize];
	uint32_t flashOffset;
} FLASH_FILE_ENTRY;

typedef uint64_t PImage; 

unsigned char	bmp_lookup_table_checkStatus();
char			bmp_draw(char *bmp_name, int xLoc, int yLoc);
char			bmp_draw_imageN(int imageIndex, int xLoc, int yLoc);
char			bmp_get_entryname(unsigned char entryNumber, char *entryName, short *imgWidth, short *imgHeight);
char			bmp_get_entryInfoFromName(char *bmp_name, short *imgWidth, short *imgHeight);
void            bmp_drawTest(char * bmp_name, unsigned int x, unsigned int y);
void            bmp_store(char* tBuff, char* fileName, uint32_t location);
uint32_t        bmp_find(char * fileName);

extern FLASH_FILE_ENTRY bmpLookupTable[kLookupTableMaxEntries+1];

#ifdef __cplusplus
extern "C"{
#endif

void			bmp_init();

#ifdef __cplusplus
} // extern "C"
#endif

PImage loadImage(char * FileName);
void image(PImage image, int xLoc, int yLoc);

#endif

