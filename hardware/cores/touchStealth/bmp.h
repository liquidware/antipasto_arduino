#ifndef __BMP_H_
#define __BMP_H_

#ifdef __cplusplus
extern "C"{
#endif

#define BMP_LOOKUP_TABLE_SIZE DATAFLASH_PAGESIZE*2 //!< Two dataflash pages
#define BMP_MAXBMP_COUNT	44						//!< Max BMP count



void			bmp_init();
unsigned char	bmp_lookup_table_checkStatus();
char			bmp_draw(char *bmp_name, int xLoc, int yLoc);
char			bmp_draw_imageN(int imageIndex, int xLoc, int yLoc);
char			bmp_get_entryname(unsigned char entryNumber, char *entryName, short *imgWidth, short *imgHeight);
char			bmp_get_entryInfoFromName(char *bmp_name, short *imgWidth, short *imgHeight);

#ifdef __cplusplus
} // extern "C"
#endif

#endif


