//*******************************************************************************
//*	Detailed Edit history
//*	<MLS>	=	Mark Sproul, msproul@jove.rutgers.edu
//*******************************************************************************
//*	Dec 27,	2008	<MLS> Changed RECT_T to RECT
//*	Dec 28,	2008	<MLS> Changed SCREEN_T to SCREEN
//*	Dec 31,	2008	<MLS> Got 0012LW from Chris, making previous changes to this new version
//*	Dec 29,	2008	<CML> RECT and SCREEN moved to graphics.h
//*	Jan  2,	2009	<MLS> Changed name to oled_slide.h
//*******************************************************************************

#ifndef _OLED_SLIDE_H_
#define _OLED_SLIDE_H_


#ifndef _PINDEFS_H_
	#include	"pindefs.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif

#define POWER_HOLD_PORT PORTG
#define POWER_HOLD_DDR  DDRG
#define POWER_HOLD_PIN  PING
#define POWER_HOLD_PNUM PG5

#define POWER_BUTTON_PORT  PORTE
#define POWER_BUTTON_DDR   DDRE
#define POWER_BUTTON_PIN   PINE
#define POWER_BUTTON_PNUM  PE2

#define OLED_SUPPLY_PORT   PORTL
#define OLED_SUPPLY_DDR    DDRL
#define OLED_SUPPLY_PIN    PINL
#define OLED_SUPPLY_PNUM   PL1

#define V5_SHUTDOWN_PORT   PORTE
#define V5_SHUTDOWN_DDR    DDRE
#define V5_SHUTDOWN_PIN    PINE
#define V5_SHUTDOWN_PNUM   PE5

#define ARDUINO_RESET_PORT PORTE	
#define ARDUINO_RESET_DDR  DDRE
#define ARDUINO_RESET_PIN  PINE
#define ARDUINO_RESET_PNUM PE4

#define TOUCH_PGM_PORT  PORTE
#define TOUCH_PGM_DDR   DDRE
#define TOUCH_PGM_PIN   PINE
#define TOUCH_PGM_PNUM  PE7

#define RGB_LED_PORT       PORTD
#define RGB_LED_DDR        DDRD
#define RGB_LED_BLUE_PNUM  PD6
#define RGB_LED_RED_PNUM   PD4
#define RGB_LED_GREEN_PNUM PD5

//---------------------------------------


#define OLED_DC		PL7
#define OLED_CS		PL2
#define OLED_RD		PL4   
#define OLED_WR		PL5
#define OLED_RESET	PL6

#define OLED_CTRL_PORT	PORTL
#define OLED_CTRL_DDR	DDRL
#define OLED_CTRL_PIN   PINL

#define OLED_DATA_LOW 	PORTJ
#define OLED_DATA_MED	PORTA
#define OLED_DATA_HIGH	PORTC

#define OLED_DATA_LOW_DDR	DDRJ
#define OLED_DATA_MED_DDR	DDRA
#define OLED_DATA_HIGH_DDR	DDRC

#define OLED_DATA_LOW_PIN	PINJ
#define OLED_DATA_MED_PIN	PINA
#define OLED_DATA_HIGH_PIN	PINC

#define GREG_MAX  9

#define BRIGHT_MAX 5

typedef struct
{
   unsigned int gReg[GREG_MAX];
} OLED_GAMMA;


#define	kOLEDcmd_RGBinterfaceControl	0x02
#define	kOLEDcmd_EntryMode				0x03
#define	kOLEDcmd_DisplayControl1		0x05
#define	kOLEDcmd_StandbyMode			0x10
#define	kOLEDcmd_GRAMaddressSetX		0x20
#define	kOLEDcmd_GRAMaddressSetY		0x21
#define	kOLEDcmd_GRAMread_write			0x22
#define	kOLEDcmd_IFselect				0x23
#define	kOLEDcmd_VerticalScrollCtrlHi	0x30
#define	kOLEDcmd_VerticalScrollCtrlLo	0x31
#define	kOLEDcmd_VerticalScrollCtrl2	0x32

#define	kOLEDcmd_VerticalRamAddrHi		0x35
#define	kOLEDcmd_VerticalRamAddrLow		0x36
#define	kOLEDcmd_HorizontalRamAddr		0x37
#define	kOLEDcmd_GammaTopBottomControl	0x70


void oled_brightness(uint8_t brightnessLevel);
void oled_init();
unsigned int oled_status();
#ifdef __cplusplus
} // extern "C"
#endif
#endif
