#ifndef __CONTROLAPP_H
#define __CONTROLAPP_H

#include "BSP_System.h"

#include "SupCOM.h"
#include "OLED_I2C.h"
#include "SupCOMAPP.h"
#include "SystemTime.h"
#include "System_APP.h"


struct TACK_Par
{
	u8 DC;
	u8 AI;

	u8 L;
	u8 K;
	u8 TW;
	u8 D;

	u8 Temp_up;
	u8 Temp_Low;	
	u8 Temp_con;

	u8 AI_Temp_up;
	u8 AI_Temp_Low;
	u8 AI_temp_Con;
	
	u16 Temp_A;
	u16 Temp_B;

	u8  K_State;
	u8  K_StateSave;
	u16 K_Count;

	u8 	StandbyFlag;
	
};

void TASK_ReadFlash(void);
void TempReadMode(void);
void ControlRelay(void);
void ControlLoop( void );

void ReadDoppler( void );
void KLightTimer1SFun( void );

void Open_KLight( void );
void OFF_KLight( void );

#endif /* __CONTROLAPP_H */

