#ifndef __KEYSCAN_H
#define __KEYSCAN_H

#include "stm32f10x.h"

#define	LED_Scene1  0x80
#define	LED_Scene2  0x40
#define	LED_Scene3  0x20
#define	LED_USB  	0x10

#define	LED_Photo	0x04
#define	LED_REC		0x02

#define Key_SCENE   0x01
#define Key_FREEZE  0x02
#define Key_SEL     0x04
#define Key_AWC     0x08
#define Key_GAIN    0x10
#define Key_PHOTO   0x20
#define Key_REC     0x40
#define Key_DRANGE  0x80

#define Key_MENU    0x02
#define Key_UP      0x08
#define Key_DOWN    0x80
#define Key_LEFT    0x20
#define Key_RIGHT   0x40

#define KEY_PRESS	0x1
#define KEY_RELEASE	0x0

#define	UMOUNT		0x0
#define MOUNTING	0x1
#define	MOUNTED		0x2

struct KeyScanStr
{
	uint8_t 	ScanCount;

	uint8_t		LEDState;
	uint8_t 	KeyState;

	uint16_t 	KeyPassCount[8];
	uint32_t	KeyRAWData;
	uint16_t	KeyPressShortCountLimt;

	uint8_t		KeyHandle;
	uint16_t	KeyHandleCount;
	uint16_t	KeyHandleCountLimt;

	uint8_t		IPLEDMountState;
	uint8_t		IPLEDMountStateSava;
	uint16_t	IPLEDMountCount;

	uint8_t		IPLEDRecState;
};

extern struct KeyScanStr SysKLStr;

extern void Key_Time1mS( void );


#endif //__KEYSCAN_H
