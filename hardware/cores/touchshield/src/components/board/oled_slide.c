//*******************************************************************************
//*		
//*	
//*	
//*******************************************************************************
//*	Detailed Edit History
//*		PLEASE put comments here every time you work on this code
//*******************************************************************************
//*	Dec 29,	2008	<?> dispBrightness changed to oled_brightness
//*	Dec 29,	2008	<CML> released v0012LW
//*	Jan  2,	2009	<MLS> Changed name to oled_slide.c
//*	Jan 19,	2009	<MLS> Got permission from Chris to start working on cleanin up slide code
//*******************************************************************************

#include	"HardwareDef.h"

#ifdef _TOUCH_SLIDE_

#include	<avr/io.h>
#include	<inttypes.h>

#include	"oled_slide.h"
#include	"bitops.h"
#include	"binary.h"
#include	"graphics.h"

OLED_GAMMA	gGammaTable[BRIGHT_MAX] = {
/* Brightness level */
/* BRIGHT1 50  */ { {0x0800, 0x0980, 0x1300, 0x0C0D, 0x1F13, 0x201C, 0x1A17, 0x1A1C, 0x2517}},
/* BRIGHT2 150 */ { {0x2000, 0x2180, 0x2D80, 0x1512, 0x1C10, 0x2216, 0x1813, 0x2219, 0x2014}},
/* BRIGHT3 175 */ { {0x2380, 0x2480, 0x3100, 0x1612, 0x1B0F, 0x2115, 0x1813, 0x2219, 0x1F13}},
/* BRIGHT4 225 */ { {0x2900, 0x2a00, 0x3700, 0x1912, 0x1B0D, 0x2014, 0x1812, 0x2118, 0x2111}},
/* BRIGHT5 250 */ { {0x2B80, 0x2C80, 0x3980, 0x1912, 0x1A0D, 0x2014, 0x1811, 0x2218, 0x1E11}},
};

//*******************************************************************************
unsigned int oled_status()
{
//unsigned char low, med, high;
unsigned char lB;

volatile unsigned int result;

	OLED_DATA_LOW_DDR = 0x00; //in
	OLED_DATA_MED_DDR = 0x00; //in
	OLED_DATA_HIGH_DDR = 0x00; //in

	CLRBIT(OLED_CTRL_PORT,OLED_DC);
	CLRBIT(OLED_CTRL_PORT,OLED_CS);
	CLRBIT(OLED_CTRL_PORT,OLED_RD);

	lB = OLED_DATA_LOW_PIN >> 3;
	lB |= (OLED_DATA_MED_PIN>>2) << 6;
	//hB = (OLED_DATA_MED_PIN<<1) >> 6;

	SETBIT(OLED_CTRL_PORT,OLED_RD);
	SETBIT(OLED_CTRL_PORT,OLED_CS);
	SETBIT(OLED_CTRL_PORT,OLED_DC);

	OLED_DATA_LOW_DDR = 0xFF; //output
	OLED_DATA_MED_DDR = 0xFF; //output
	OLED_DATA_HIGH_DDR = 0xFF; //output

//result = (hB << 8) + lB;
result = lB;
return result;
}

//*******************************************************************************
void	oled_init()
{
	COLOR black = {0,0,0};
	
	volatile unsigned long x,y;
	
	SETBIT(V5_SHUTDOWN_DDR, V5_SHUTDOWN_PNUM);
	CLRBIT(V5_SHUTDOWN_PORT, V5_SHUTDOWN_PNUM);
	
	OLED_DATA_LOW_DDR = 0xFF; //output
	OLED_DATA_MED_DDR = 0xFF; //output
	OLED_DATA_HIGH_DDR = 0xFF; //output
	
	OLED_CTRL_DDR |=  (1<<OLED_DC) | 
					 (1<<OLED_CS) | 
					 (1<<OLED_RD) |
					 (1<<OLED_WR) |
					 (1<<OLED_RESET);

	for (x=0;x<100;x++)
	{
		/* Reset OLED */
		CLRBIT(OLED_CTRL_PORT,OLED_RESET);
	}

	for (x=0;x<100;x++)
	{
		/* OLED ON */
		SETBIT(OLED_CTRL_PORT,OLED_RESET);
	}

	dispCommand(kOLEDcmd_IFselect);
	
	dispCommand(kOLEDcmd_RGBinterfaceControl);
	dispData(0x0000);
	
	dispCommand(kOLEDcmd_EntryMode);
 //  dispData(0x8030);
	dispData(0x8031);
	
	dispCommand(kOLEDcmd_StandbyMode); //standby off
	dispData(0x0000);
	
	//delay
	for(y=0;y<5000;y++)
	for(x=0;x<10; x++)
	{
		asm("nop");
	}

	SETBIT(OLED_SUPPLY_PORT,OLED_SUPPLY_PNUM);
	SETBIT(OLED_SUPPLY_DDR,OLED_SUPPLY_PNUM); //turn on supply

	for(y=0;y<5000;y++)
	for(x=0;x<10; x++)
	{
		asm("nop");
	}

	dispCommand(kOLEDcmd_DisplayControl1); //display on
	dispData(0x0001);
	
	oled_brightness(1); //low brightness
	
	dispCommand(kOLEDcmd_GRAMread_write); //write / read graphics data
	
	dispColor(black);
	dispClearScreen();

}

//*******************************************************************************
void	oled_brightness(uint8_t brightnessLevel)
{

	/* Range Check Brightness */
	if (brightnessLevel < BRIGHT_MAX)
	{
		unsigned char ii;

		/* Set the Brightness */
		for(ii=0; ii<GREG_MAX; ii++)
		{
			dispCommand(kOLEDcmd_GammaTopBottomControl + ii);
			dispData(gGammaTable[brightnessLevel].gReg[ii]);
		}
	}
}



#endif	//	_TOUCH_SLIDE_