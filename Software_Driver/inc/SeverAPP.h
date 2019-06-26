#ifndef __SEVERAPP_H
#define __SEVERAPP_H

#include "W25QXX.h"

#include "SupCOM.h"
#include "JSON.h"
#include "ESP_AT.h"
#include "ControlAPP.h"
#include "SystemTime.h"


#define SEVER_SEND_COLRTS			"SEND_COLRTS"
#define SEVER_SEND_TASK				"SEND_TASK"
#define SEVER_SEND_TEMP				"SEND_TEMPERATUERE"
#define SEVER_SEND_AI_TEMP			"SEND_AI_TEMPERATUERE"

#define SEVER_COLRTS_GET			"SEND_COLRTS"			,SeverGetUpDataFlag
#define SEVER_TASK_GET				"SEND_TASK"				,SeverReadTASK
#define SEVER_TEMP_GET				"SEND_TEMPERATUERE"		,SeverRead_Temp
#define SEVER_AI_TEMP_GET			"SEND_AI_TEMPERATUERE"	,SeverRead_AI_Temp
#define SEVER_UPDATA_GET			"SEND_VERSION"			,SeverPrintTCP



#define SeverAPP_PringLive	4


struct COLRTSSrt
{
	u8 DC_Flag;
	u8 AI_Flag;

	u8 TASK_Flag;
	u8 TEMP_Flag;
	u8 TEMP_AI_Flag;

	u8 UpDataFlag;
	
};

struct TimeStr
{
	u8 Hour;
	u8 Minute;
};

struct TempStr
{
	float TempData;
	
	u8 TempInt;
	u8 TempDec;
};

struct TempDataStr
{
	struct TempStr Temp_u;
	struct TempStr Temp_l;

	struct TimeStr Time_t;
	struct TimeStr Time_f;

	u8 ContFlag;
	
};

struct TASKDataStr
{
	u8 Switch;
	u8 NumData;

	struct TimeStr Time_Task;

	u8 Status;

	u8 WeekBuff[7];

	u8 Valid;
};

struct PowerStr
{
	u32 Current;
	u32 Watt;

	u32 Kilowatt;

	u8 	Kilowatt_Int;
	u16 Kilowatt_Dec;
	
	u16 Voltage;

	u8 EleTime;

	u8 UpDataFlag;
	u8 SendFlag;

	u32 SumMin;
};



u8 SeverGetDataCallFun( u8 *DataName , void (*FunPtr)(void) );

void SeverSend_Parameter( struct TACK_Par *Pam );

void SeverGetUpDataFlag(void);
void SeverRead_Temp(void);
void SeverReadTASK(void);
void SeverRead_AI_Temp(void);

void SendPower( struct PowerStr *EDataPrt  );
void SeverSendPowerOffLine( void );

void SeverUpDataFun(void);
void CheckenFlashUpData( void );

void SeverPrintTCP( void );

extern struct COLRTSSrt 	Colrt_Prm;
extern struct PowerStr	 	HLW8012;


#endif /* __SEVERAPP_H */
