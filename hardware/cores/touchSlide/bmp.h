#ifndef __BMP_H_
#define __BMP_H_

#ifdef __cplusplus
extern "C"{
#endif

void			bmp_init();
unsigned char	bmp_lookup_table_checkStatus();
char			bmp_draw(char *bmp_name, int xLoc, int yLoc);
char			bmp_draw_imageN(int imageIndex, int xLoc, int yLoc);
char			bmp_get_entryname(unsigned char entryNumber, char *entryName, short *imgWidth, short *imgHeight);
char			bmp_get_entryInfoFromName(char *bmp_name, short *imgWidth, short *imgHeight);
void            bmp_drawTest(char * bmp_name, unsigned int x, unsigned int y);
void            bmp_store(char* tBuff, char* fileName, uint32_t location);
uint32_t        bmp_find(char fileName);

#ifdef __cplusplus
} // extern "C"
#endif

#endif


