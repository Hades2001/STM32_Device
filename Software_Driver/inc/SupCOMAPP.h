#ifndef __SUPCOMAPP_H
#define __SUPCOMAPP_H

#include "delay.h"
#include "SupCOM.h"
#include "SystemTime.h"

#include "W25QXX.h"


#define SupCOM_Version		"V0.1.1"

#define SupCOM_NULL			0x0000
#define SupCOM_MENU			0x1000
#define SupCOM_CMDM			0x2000
#define SupCOM_EnterFUM		0xA000

#define SupCOM_FUN_INIT		0x00
#define SupCOM_FUN_RUN		0x01
#define SupCOM_FUN_END		0x0E

#define S_CMD_SUPCOM		0x654789EC
#define S_CMD_MENU			0x5846D000
#define S_CMD_SET			0x64498000
#define S_CMD_INF			0x50D28000
#define S_CMD_REST			0x62494C00


#define S_CMD_CHECK			0x44720940

#define S_CMD_echo			0x8823B800


#define Menu_Init 			0x01
#define Menu_Chrose 		0x02
#define Menu_ESC			0x03

#define InYes				0x01
#define InNo				0x02
#define InError				0x0E

enum EXEATT
{
	MenuStr,

	BlockingFun,
	NonBlockingFun,
	
	TimerFun,
	EnterTri,
	SemTri,
};

struct SupCOMUI
{
	u8 	SupCOM_Mode;	
	u16 SupCOM_State; 

	u8 	SupCom_Fun_State;
	
	void (*SupCOM_Enter_FunPrt)();
	
};


struct SupCOM_EXEStr
{
	void (*FunPrt)();
	
	struct SupCOM_MENU_Str *PreviousMenu;

	enum EXEATT EXEATT_P;
};


struct SupCOM_MENU_Str
{
	u8 State;

	void (*MenuInitFun)();

	void (*MenuExecuteFun[10])();

	void (*MeunESCFun)();

	struct SupCOM_EXEStr ExeStr[10];

	u8	MenuNum;

	const u8 *MeunTitel;
	const u8 *TabName[10];

	struct SupCOM_MENU_Str *PreviousMenu;
};



#define SC_IND_NULL		0x00
#define SC_IND_CMD		0x01
#define SC_IND_NUM		0x02
#define SC_IND_STR		0x03
#define SC_IND_YorN		0x04


struct SupCOM_InData
{
	u8 InData_State;

	u8 InDataString[32];

	u32 InDataNum;

	u32 CMD_Data;

	u8 InDataYorN;
};


struct SysTemUserInf
{
	u8 Name[32];

	u8 PasswordBuff[32];

	u8 Class;
};


struct SupCOMEnter
{
	u8 ProcessNum;

	void (*InitFun)();
	

};


extern void SupCOM_UI(void);
extern void SupCOM_Init(void);

extern const u8 ASCIIImageBuff[];                                                           



#endif /* ____ESP_AT_H */

