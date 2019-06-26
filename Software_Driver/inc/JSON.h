#ifndef __JSON_H
#define __JSON_H


#include "USART.h"
#include "SupCOM.h"
#include "ESP_AT.h"

#define JSON_DATABUFFSIZE	512

#define COM_SEND_MONITOR			1
#define COM_SEND_DC					2
#define COM_SEND_AI					3
#define COM_SEND_ELECTRIC			4
#define COM_SEND_TASK				5
#define COM_SEND_TEMPERATUERE		6
#define COM_SEND_AI_TEMPERATUERE	7

#define JSON_Ana_NULL				0
#define JSON_Ana_OK					1
#define JSON_Ana_ERROR				2
#define JSON_Ana_NOFOUND			3

#define JSON_F_ALL					1
#define JSON_F_FIRST				2

struct JSON_Data
{
	u16	JSON_DataNum;
	u8 	JSON_DataBuff[JSON_DATABUFFSIZE];
};


struct JSON_VARStr
{
	u8 *ValName;

	u8 	FindNum;
	
	u32 StartOffset[32];
	u32 EndOffset[32];
	
	u8 	FindMode;
};


struct JSON_FindStr
{
	u8 *JSON_Buff_Ptr;
	u32 JSON_Buff_Size;

	u32 offset;
	
	u32 FindState;
};


//struct JSON_EData
//{
//	u32 FlashAddr;
//	u16 DataNum;
//};

struct Parameter
{
	u8 Pmr_S;
	u8 Pmr_R;
	u8 Pmr_M;
	
	u8 Pmr_DC;
	u8 Pmr_AI;
	u8 Pmr_TW;

	u32	Pmr_Temp;
	u32 Pmr_Cur;
	u32 Pmr_Wat;
};

extern void SendEletricOnLine( u8 *TimeString , u8 Hour , u32 Kilowatt , u8 EleTime  , struct JSON_Data *BuffPrt );

extern void CopyStringToJSONDataBuff( u8 *StringData , struct JSON_Data *BuffPrt );
extern void JSON_FushBuff( struct JSON_Data *Prt );
extern void PrintJSONDataBuff(	struct JSON_Data *BuffPrt );
extern void ADDNumToJSONDataBuff( u32 Num_Data ,u8 PointPosition ,struct JSON_Data *BuffPrt );

extern void SendJSON_Parameter( struct Parameter *DataPrt,struct JSON_Data *BuffPrt );
extern void SendJSON_DeviceControl( struct JSON_Data *BuffPrt , u8 DataKind );
extern void SendRequestJson( u8 *RequestName ,struct JSON_Data *BuffPrt );
extern void SendRequestFlagJSON( u8 *RequestName , u8 Flag ,struct JSON_Data *BuffPrt );

extern u8 JSON_Resolve( u8 *JSON_Buff, u8 *ValName , u8 *JSON_VAL );
extern u8 JSON_COMMONDStrToVal( u8 *JSON_VAL );
extern u8 JSOM_FlagStrToVal( u8 *JSON_VAL );
extern u8 JSON_ResolveNUm( u8 *JSON_Buff ,u8 *ValName, u32 *Num );

extern void JSON_F_String( u8 *StringB , struct JSON_FindStr *p , struct JSON_VARStr *VAR );
extern void JSON_ReadVal( u8 VarNum , u8 *ValBuff ,struct JSON_FindStr *p ,struct JSON_VARStr *VAR);

extern void SendEletricOffLine_Head( struct JSON_Data * BuffPrt );
extern void SendEletricOffLine_Data(u8 * TimeString, u8 Hour, u32 Kilowatt, u8 EleTime, struct JSON_Data * BuffPrt);
extern void SendEletricOffLine_End( struct JSON_Data * BuffPrt );

extern void SendReUpData( u16 PackNum  , u8 *VresionBuff , struct JSON_Data * BuffPrt );
extern void SendUpDataFlag( u8 *VresionBuff , u8 Flag , struct JSON_Data * BuffPrt );


//extern void TestJSON( struct JSON_ANA_Str *p );



#endif /* __JSON_H */
