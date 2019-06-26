#ifndef __CAMOSD_H
#define __CAMOSD_H

#include "System.h"

struct OSDTimeStr
{
	uint8_t Xpos;
	uint8_t YPos;
	uint8_t *pStr;
	uint8_t Times;
	uint8_t	Color;
	uint8_t State;
	uint8_t ID;

	uint8_t Count_Ti;
	uint8_t Count_Str;
	uint8_t RealYPos;

	struct OSDTimeStr *Next;
};

#define OSD_RUN		0
#define OSD_STOP	1
#define OSD_SET		2

#define OSD_FAILED	0
#define OSD_OK		1

#define MAX_ID 		128

#define OSD_Creat(x,y,qs,t)  		OSDFastCreat_Malloc(x,y,qs,sizeof(qs),t)
#define	OSD_AutoPrint( x,y,qs,t,d )	OSD_AutoSet(x,y,qs,sizeof(qs),t,d)

uint8_t OSD_AutoSet( 	uint8_t xPOS , 
						uint8_t yPOS ,
						uint8_t *Str ,
						uint8_t StrSize , 
						uint8_t Time,
						uint8_t *pID );

uint8_t OSD_SetFromID( uint8_t xPOS , uint8_t yPOS ,uint8_t *Str ,uint8_t StrSize , uint8_t Time , uint8_t ID );
uint8_t OSDFastCreat_Malloc( uint8_t xPOS , uint8_t yPOS ,uint8_t *Str ,uint8_t StrSize , uint8_t Time );
void SetOSD_Time( struct OSDTimeStr *pOSDStr );
void OSDInTime1S( void );
void OSDInLoop( void );

#endif
