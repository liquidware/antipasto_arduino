#ifndef __DATAFLASH_H_
#define __DATAFLASH_H_

#include "pindefs.h"

#define DATAFLASH_DDR	DDRB
#define DATAFLASH_PORT	PORTB
#define DATAFLASH_CS	PB4
#define DATAFLASH_MISO	PB3
#define DATAFLASH_MOSI	PB2
#define DATAFLASH_SCK	PB1
#define DATAFLASH_RESET	PB5

/*Chip Type Settings */
#define DATAFLASH_SIZE		16	//!< In megabits

/*Commands */
#define DATAFLASH_CMD_STATUS 		0xD7
#define DATAFLASH_CMD_PROGRAM		0x83
#define DATAFLASH_CMD_CONTARRAYREAD	0xE8


/* General Defines */
#define	DATAFLASH_BUSY		0
#define	DATAFLASH_READY		1
#define	DATAFLASH_PAGESIZE	528					//!< pages size in bytes
#define	DATAFLASH_BLOCK_COUNT	512				//!< Number of blocks on the chip

#ifdef __cplusplus
extern "C"{
#endif

void dataflash_init();
unsigned char dataflash_checkStatus();
void dataflash_out(unsigned char cData);

void dataflash_program_page(unsigned char * page_buff, unsigned int page_num);
void dataflash_read_block(unsigned char * storage_buff, unsigned long addr, unsigned int size);
void dataflash_erase();
void dataflash_cont_read(unsigned char * storage_buff, unsigned int page_num, unsigned int size);

void dataflash_write_buff(unsigned char * out_data);
void dataflash_read_buff(unsigned char * in_data);
void dataflash_clear_buff();

#ifdef __cplusplus
} // extern "C"
#endif

#endif




