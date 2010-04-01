//*******************************************************************************
//*	
//*		Keyboard input
//*		for TouchShield library
//*		by Mark Sproul  msproul@jove.rutgers.edu
//*******************************************************************************
//*	Detailed Edit history
//*	<MLS>		=	Mark Sproul <msproul@jove.rutgers.edu>
//*******************************************************************************
//*	Mar  1,	2009	<MLS> Stared on keyboard input
//*******************************************************************************
//*	<MLS> Programming style
//*			Never use 1 letter variable names, use LONG names, 
//*			{ and } always go on their own line
//*			Never put 2 lines of code on the same line
//*			Use TAB instead of multiple spaces
//*			A comment line of "****"s above the start of each routine
//*			Leave a space before and after each operator "a + b"  not "a+b"
//*		these next few rules have to be broken to keep compatibility
//*			Routine names should start with a CAPITAL letter
//*			Variable names should always start with lower case,
//*			Global variables should start with a lower case "g" followed by an UPPER case char
//*			Constants (#define )  should start with a lower case "k" followed by an UPPER case char
//*			UnderLine "_" is an ok char to use in variable names, routine names, and constants
//*			Changes should include comments with your initials
//*			Macros should have some comments explaining what they do and why
//*******************************************************************************


#include	<avr/io.h>
#include	<avr/pgmspace.h>
#include	<inttypes.h>
#include	<math.h>
#include	<stdlib.h>
#include	"wiring.h"

#ifndef _HARDWARE_DEF_H_
	#include	"HardwareDef.h"
#endif

#ifndef _SUBP_OPTIONS_H_
	#include	"SubPOptions.h"
#endif
#ifndef SUBPGRAPHICS_H
	#include	"SubPGraphics.h"
#endif
#include	"QuickDraw.h"
#ifndef GRAPHICS_H
	#include	"graphics.h"
#endif
#ifndef _FONT_H_
	#include	"font.h"
#endif


typedef struct 
{
	
	RECT	keyButton;
	char	keyChar;
} KEYBOARD;

#define	kKeysAcross	14
#define	kKeyRows	5

#define	kTotalKeys	(kKeyRows * kKeysAcross)


//*******************************************************************************
static void	DrawOneKeyboardKey(KEYBOARD *theKey)
{
#ifdef _ENABLE_HERSHEY_FONTS_
	char	charString[4];
	int		yLoc;
	char	fontSize;
#endif

	if (theKey->keyChar > 0)
	{
		setbcolor(255,255,255);
		EraseRect(&theKey->keyButton);
		FrameRect(&theKey->keyButton);
		
	#ifdef _ENABLE_HERSHEY_FONTS_
		charString[0]	=	theKey->keyChar;
		charString[1]	=	0;
		yLoc			=	(theKey->keyButton.top + theKey->keyButton.bottom) / 2;
		fontSize		=	(theKey->keyButton.bottom - theKey->keyButton.top) - 12;
	#endif
		if (theKey->keyChar >= 0x21)
		{
		#ifdef _ENABLE_HERSHEY_FONTS_
			HersheyDrawCString(0, theKey->keyButton.left + 10, yLoc, charString, fontSize, 0, 1);
		#else
			text(	theKey->keyChar,
					theKey->keyButton.left + 6,
					theKey->keyButton.top + 7);
		#endif
		}
		else if (theKey->keyChar == 0x08)
		{
			text(	"BS",
					theKey->keyButton.left + 3,
					theKey->keyButton.top + 7);
		}
		else if (theKey->keyChar == 0x09)
		{
			text(	"TAB",
					theKey->keyButton.left + 3,
					theKey->keyButton.top + 7);
		}
		else if (theKey->keyChar == 0x0D)
		{
			text(	"RET",
					theKey->keyButton.left + 3,
					theKey->keyButton.top + 7);
		}
		else if (theKey->keyChar == 0x20)
		{
			text(	"SPC",
					theKey->keyButton.left + 3,
					theKey->keyButton.top + 7);
		}
	}
}

//*******************************************************************************
static void	DrawEnteredText(RECT *textRect, char *enteredText)
{
int		yLoc;
char	fontSize;

	setbcolor(200,200,200);
	EraseRect(textRect);
	FrameRect(textRect);
#ifdef _ENABLE_HERSHEY_FONTS_
	yLoc		=	(textRect->top + textRect->bottom) / 2;
	fontSize	=	(textRect->bottom - textRect->top) - 12;
	HersheyDrawCString(0, textRect->left + 10, yLoc, enteredText, fontSize, 0, 1);
#else
	text(	enteredText,
			textRect->left + 6,
			textRect->top + 7);
#endif
}

//*******************************************************************************
int	GetKeyIndex(KEYBOARD *theKeyboard, Point *mousePoint)
{
int		ii;
int		keyIndex;


	//*	figure out which button was pressed
	keyIndex	=	-1;
	for (ii=0; ii<kTotalKeys; ii++)
	{
		if (PtInRect(*mousePoint, &(theKeyboard[ii].keyButton)))
		{
			keyIndex		=	ii;
		}
	}
	return(keyIndex);
}

//*******************************************************************************
char	GetQWERTYkeyboardInput(void)
{
char			returnChar;
signed char		buttonSize;
signed char		ii, jj;
short			xx, yy;
KEYBOARD		theKeyboard[kTotalKeys];
RECT			keyboardRect;
RECT			enteredTextRect;
Point			mousePoint;
byte			keyIndex;
byte			prevKeyIndex;
char			enteredText[48];
byte			charCount;


	returnChar	=	-1;
	buttonSize	=	gWidth / kKeysAcross;
	xx			=	0;
	yy			=	gHeight - (kKeyRows * buttonSize) - 2;
	SetRect(&keyboardRect,		0, yy - 5, gWidth-1, gHeight-1);
	SetRect(&enteredTextRect,	0, yy - buttonSize, gWidth-1, yy-1);
	keyIndex	=	0;
	for (ii=0; ii<kKeyRows; ii++)
	{
		for (jj=0; jj<kKeysAcross; jj++)
		{
			theKeyboard[keyIndex].keyButton.left	=	xx;
			theKeyboard[keyIndex].keyButton.top		=	yy;
			theKeyboard[keyIndex].keyButton.right	=	xx + buttonSize;
			theKeyboard[keyIndex].keyButton.bottom	=	yy + buttonSize;
			theKeyboard[keyIndex].keyChar			=	0;
			
			keyIndex++;
			
			xx	+=	buttonSize;
			xx	+=	1;
		}
		yy	+=	buttonSize;
		yy	+=	1;
		
		//switch(ii) 
		//{
		//	case	0:	xx	=	buttonSize / 2;	break;
		//	case	1:	xx	=	(buttonSize / 2) + (buttonSize / 5);	break;
		//	case	2:	xx	=	buttonSize;	break;
		//	case	3:	xx	=	0;	break;
		//}
		xx	=	0;
	}
	jj	=	0;
	theKeyboard[jj++].keyChar	=	'~';
	theKeyboard[jj++].keyChar	=	'!';
	theKeyboard[jj++].keyChar	=	'@';
	theKeyboard[jj++].keyChar	=	'#';
	theKeyboard[jj++].keyChar	=	'$';
	theKeyboard[jj++].keyChar	=	'%';
	theKeyboard[jj++].keyChar	=	'^';
	theKeyboard[jj++].keyChar	=	'&';
	theKeyboard[jj++].keyChar	=	'*';
	theKeyboard[jj++].keyChar	=	'(';
	theKeyboard[jj++].keyChar	=	')';
	theKeyboard[jj++].keyChar	=	'_';
	theKeyboard[jj++].keyChar	=	'+';
	theKeyboard[jj++].keyChar	=	0x08;
	
	jj	=	1 * kKeysAcross;
	theKeyboard[jj++].keyChar	=	'`';
	theKeyboard[jj++].keyChar	=	'1';
	theKeyboard[jj++].keyChar	=	'2';
	theKeyboard[jj++].keyChar	=	'3';
	theKeyboard[jj++].keyChar	=	'4';
	theKeyboard[jj++].keyChar	=	'5';
	theKeyboard[jj++].keyChar	=	'6';
	theKeyboard[jj++].keyChar	=	'7';
	theKeyboard[jj++].keyChar	=	'8';
	theKeyboard[jj++].keyChar	=	'9';
	theKeyboard[jj++].keyChar	=	'0';
	theKeyboard[jj++].keyChar	=	'-';
	theKeyboard[jj++].keyChar	=	'=';
	theKeyboard[jj++].keyChar	=	'|';
	
	jj	=	2 * kKeysAcross;
	theKeyboard[jj++].keyChar	=	0x09;
	theKeyboard[jj++].keyChar	=	'Q';
	theKeyboard[jj++].keyChar	=	'W';
	theKeyboard[jj++].keyChar	=	'E';
	theKeyboard[jj++].keyChar	=	'R';
	theKeyboard[jj++].keyChar	=	'T';
	theKeyboard[jj++].keyChar	=	'Y';
	theKeyboard[jj++].keyChar	=	'U';
	theKeyboard[jj++].keyChar	=	'I';
	theKeyboard[jj++].keyChar	=	'O';
	theKeyboard[jj++].keyChar	=	'P';
	theKeyboard[jj++].keyChar	=	'[';
	theKeyboard[jj++].keyChar	=	']';
	theKeyboard[jj++].keyChar	=	'\\';

	jj	=	3 * kKeysAcross;
	theKeyboard[jj++].keyChar	=	'<';
	theKeyboard[jj++].keyChar	=	'A';
	theKeyboard[jj++].keyChar	=	'S';
	theKeyboard[jj++].keyChar	=	'D';
	theKeyboard[jj++].keyChar	=	'F';
	theKeyboard[jj++].keyChar	=	'G';
	theKeyboard[jj++].keyChar	=	'H';
	theKeyboard[jj++].keyChar	=	'J';
	theKeyboard[jj++].keyChar	=	'K';
	theKeyboard[jj++].keyChar	=	'L';
	theKeyboard[jj++].keyChar	=	';';
	theKeyboard[jj++].keyChar	=	'\'';
	theKeyboard[jj++].keyChar	=	0x0D;

	jj	=	4 * kKeysAcross;
	theKeyboard[jj++].keyChar	=	'>';
	theKeyboard[jj++].keyChar	=	'Z';
	theKeyboard[jj++].keyChar	=	'X';
	theKeyboard[jj++].keyChar	=	'C';
	theKeyboard[jj++].keyChar	=	'V';
	theKeyboard[jj++].keyChar	=	'B';
	theKeyboard[jj++].keyChar	=	'N';
	theKeyboard[jj++].keyChar	=	'M';
	theKeyboard[jj++].keyChar	=	',';
	theKeyboard[jj++].keyChar	=	'.';
	theKeyboard[jj++].keyChar	=	'/';
	theKeyboard[jj++].keyChar	=	'?';
	theKeyboard[jj++].keyChar	=	0x20;

	EraseRect(&keyboardRect);
	for (ii=0; ii<kTotalKeys; ii++)
	{
		DrawOneKeyboardKey(&theKeyboard[ii]);
	}

	charCount		=	0;
	enteredText[0]	=	0;
	DrawEnteredText(&enteredTextRect, enteredText);
	
	mouseY			=	gHeight;
	
	while (mouseY > (gHeight / 2))
	{
		if (gettouch())
		{
			keyIndex	=	-1;

			//*	figure out which button was pressed
			mousePoint.h	=	mouseX;
			mousePoint.v	=	mouseY;
			keyIndex		=	GetKeyIndex(theKeyboard, &mousePoint);

			//*	do we have a valid key rect
			if (keyIndex >= 0)
			{
				prevKeyIndex	=	keyIndex;
				//*	does it contain a real char
				if (theKeyboard[keyIndex].keyChar > 0)
				{
					//*	invert the rect for the user
					FillRect(&theKeyboard[keyIndex].keyButton);
					while (gettouch())
					{
						mousePoint.h	=	mouseX;
						mousePoint.v	=	mouseY;
						keyIndex		=	GetKeyIndex(theKeyboard, &mousePoint);
						if (keyIndex != prevKeyIndex)
						{
							//*	put the old key back to the way it should be
							DrawOneKeyboardKey(&theKeyboard[prevKeyIndex]);
							
							if (theKeyboard[keyIndex].keyChar > 0)
							{
								//*	invert the new key
								FillRect(&theKeyboard[keyIndex].keyButton);
							}
							
							prevKeyIndex	=	keyIndex;
						}
					}
					DrawOneKeyboardKey(&theKeyboard[keyIndex]);
					if (keyIndex == prevKeyIndex)
					{
					char	theChar;
					
						theChar						=	theKeyboard[keyIndex].keyChar;
						if (theChar == 0x08)
						{
							charCount--;
							if (charCount < 0)
							{
								charCount	=	0;
							}
							enteredText[charCount]		=	0;
						}
						else if (theChar == 0x0d)
						{
							charCount	=	0;
							enteredText[charCount]		=	0;
						}
						else
						{
							enteredText[charCount++]	=	theChar;
							enteredText[charCount]		=	0;
						}
						DrawEnteredText(&enteredTextRect, enteredText);
					}
				}
			}
		}
	}
	
	
	return(returnChar);
}


	




