#ifndef __CAMAPP_H
#define __CAMAPP_H

#include "System.h"

extern struct   CAMData 		s_ReadData;
extern struct   CamStateStr 	CamState;
extern uint8_t  DiskID;

extern uint8_t Str_UserA[32];
extern uint8_t Str_UserB[32];

void    SetAWC(void);
void    ChangeGAIN(void);
s8      ReadIPCamacity(void);
void    ReadIPStateLED(void);
void    DisSceneLED( uint8_t SecneNum );
s8      ReadAndSetCamInit(void);


#endif
