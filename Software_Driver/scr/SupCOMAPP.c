#include "SupCOMAPP.h"
#include "stddef.h"
#include "ESP_AT.h"

#define SupCOM_APP_PrintLever	-1

void SupCOM_SavaSupCOM_PClass(void);
void SupCOM_ReadSupCOM_PClass(void);


u32 SupCOM_CMD(void);
void SupCOM_Home( u32 CMDDATA );

void SupCOM_MenuStrInit(void);
void SupCOMMenu_Fun( struct SupCOM_MENU_Str *p );
void SupCOM_InDataFun(void);
void SupCOM_CMDParser(void);

void SupCOM_DisPose( void );

u32 SupCOMStringToNum( u8 *StringBuff );
void SupCOM_CopyMemory( u8 *StringBuffA ,u8 *StringBuffB );
void SupCOM_InYorN( void );

void SupCOM_MenuPrint( struct SupCOM_MENU_Str *p );

void SupCOM_MenuEXE( struct SupCOM_MENU_Str *p );
void SupCOM_MenuRun(  struct SupCOM_MENU_Str *p  , u8 Num );

void SupCOM_SetLevel(void);

void SetPassWord(void);
void SetTime(void);
void SetSeverIP(void);
void InitDevice(void);


void TCPTransfer(void);




void CreateSysMenu(void);
void CreateSysSET(void);

struct SupCOMUI SupCOM_P;
struct SupCOM_InData	InDataStr;
struct SupCOM_MENU_Str	*Menu_P;

struct SupCOM_MENU_Str SysTemMenu , SysTemSet , SysTemINF;

struct SysTemUserInf UserROOT ={"root","fuckstc",0};

extern struct SupCOMRxData RxDataStr;


//=====================================================================\r\n\
//     _______  _______                                                \r\n\
//*   /  ___  \/  ___  \   _____   ____  _____   ____                 *\r\n\
//   |  /   |_|  /   |_|  /  ___) / __ \|  __ \ / ___\ ___  __  __     \r\n\
//*  | |   ___| |   ___  |  (___ | |  | | (__) | |    / _ \/  \/  \   *\r\n\
//   | |  |_  | |  |_  |  \___  \| |  | |  ___/| |   | | | | |\/| |    \r\n\
//*  |  \___| |  \___| |  ____)  | |__| | |    | |___| |_| | |  | |   *\r\n\
//    \_______/\_______/ (______/ \____/|_|     \____/\___/|_|  |_|    \r\n\
//*                                                                   *\r\n\
//=====================================================================\r\n\


const u8 ASCIIImageBuff[]={                      
"\
=====================================================================\r\n\
     _______  _______                                                \r\n\
*   /  ___  \\/  ___  \\   _____  _    _ _____   ____                 *\r\n\
   |  /   |_|  /   |_|  /  ___)| |  | |  __ \\ / ___\\ ___  __  __     \r\n\
*  | |   ___| |   ___  |  (___ | |  | | (__) | |    / _ \\/  \\/  \\   *\r\n\
   | |  |_  | |  |_  |  \\___  \\| |  | |  ___/| |   | | | | |\\/| |    \r\n\
*  |  \\___| |  \\___| |  ____)  | |__| | |    | |___| |_| | |  | |   *\r\n\
    \\_______/\\_______/ (______/ \\____/|_|     \\____/\\___/|_|  |_|    \r\n\
*                                                                   *\r\n\
=====================================================================\r\n\
"};

void SupCOM_Init(void)
{
	
	SupCOM_ReadSupCOM_PClass();

	
	RxDataStr.IRQ_Count = 0;
	RxDataStr.RxFlag = 0;
	RxDataStr.TxFlag = 1;
	
	InDataStr.InData_State 	= 	SC_IND_CMD;
	
	SupCOM_P.SupCOM_State	=	SupCOM_NULL;
	SupCOM_P.SupCOM_Enter_FunPrt	= NULL;
	

	CreateSysMenu();
	CreateSysSET();
	
	CLEAR_SUPCOM();

	SupCOM_ChanreColor(SupCOM_Color_GB);

	SupCOMPrintString_Level( (u8*)ASCIIImageBuff , SupCOM_APP_PrintLever);

	SupCOMPrintString_Level( "GGSupCOM(Super com) Version : " , SupCOM_APP_PrintLever);

	SupCOMPrintString_Level( SupCOM_Version , SupCOM_APP_PrintLever );

	SupCOMPrintString_Level( "\r\n" , SupCOM_APP_PrintLever);

	SupCOMPrintString_Level( "Welcome to GGSupCOM \r\n" , SupCOM_APP_PrintLever);

	SupCOM_ChanreColor(SupCOM_Color_WB);
}

void SupCOMTimer10ms()
{




}

void SupCOM_Rest()
{
	__set_FAULTMASK(1); 
	NVIC_SystemReset();
}

void SupCOM_SavaSupCOM_PClass(void)
{
	u8 DataBuff[4];
	
	W25QXX_Erase_Sector( 0x002 );

	 DataBuff[0] = SupCOM_PClass.Level;
	 DataBuff[1] = SupCOM_PClass.ColorMode;
	 DataBuff[2] = SupCOM_PClass.ForeColor;
	 DataBuff[3] = SupCOM_PClass.BackColor;

	W25QXX_Write_Page( DataBuff , 0x002001 , 4 );

//	W25QXX_Write_Byte( SupCOM_PClass.Level		,0x002001 );
//	W25QXX_Write_Byte( SupCOM_PClass.ColorMode	,0x002002 );
//	W25QXX_Write_Byte( SupCOM_PClass.ForeColor	,0x002003 );
//	W25QXX_Write_Byte( SupCOM_PClass.BackColor	,0x002004 );
	
//	W25QXX_ReadByte

}

void SupCOM_ReadSupCOM_PClass(void)
{
	u8 DataBuff[4];

	W25QXX_Read( DataBuff , 0x002001 , 4 );

	SupCOM_PClass.Level 	=	DataBuff[0];
	SupCOM_PClass.ColorMode =	DataBuff[1];
	SupCOM_PClass.ForeColor =	( enum SupCOMColor )DataBuff[2];
	SupCOM_PClass.BackColor =	( enum SupCOMColor )DataBuff[3];
}

void SupCOM_UI(void)
{
	SupCOM_InDataFun();

	SupCOM_DisPose();
}


void SupCOM_InDataFun(void)
{
	switch( InDataStr.InData_State )
	{
		case SC_IND_CMD : 
 		InDataStr.CMD_Data = SupCOM_CMD();

		break;
		
		case SC_IND_NUM : 
		InDataStr.InDataNum = SupCOMStringToNum( RxDataStr.RxString );

		break;

		case SC_IND_STR : 
		SupCOM_CopyMemory( RxDataStr.RxString , InDataStr.InDataString );

		break;
		case SC_IND_YorN :
			
		SupCOM_InYorN();

		break;

		default :
		InDataStr.InData_State = SC_IND_CMD;

		break;
	}
}

void SupCOM_DisPose( void )
{
	switch( SupCOM_P.SupCOM_State )
	{
		case SupCOM_NULL :
		SupCOM_CMDParser();

		break;

		case SupCOM_MENU :
		SupCOMMenu_Fun( Menu_P );

		break;

		case SupCOM_CMDM :

		break;

		case SupCOM_EnterFUM:
		SupCOM_P.SupCOM_Enter_FunPrt();
		
		break;

		default:
		SupCOM_P.SupCOM_State = SupCOM_NULL;
		
		break;
	}
}

void SupCOM_CMDParser(void)
{
	if( InDataStr.InData_State == SC_IND_CMD )
	{
		switch(	InDataStr.CMD_Data )
		{
			case S_CMD_MENU :
			SupCOM_PClass.Level = -1;
			Menu_P = &SysTemMenu;
			SupCOM_P.SupCOM_State = SupCOM_MENU;
			
			SupCOM_MenuPrint( &SysTemMenu );
			
			Menu_P->State = Menu_Chrose;
			
			break;
			
			case S_CMD_SET :

			break;
			
			case S_CMD_REST :
			SupCOM_Rest();

			break;
			
			default :
			TxStringData("NO Fount CMD :");
			TxStringData( (char *)RxDataStr.RxString);
			TxStringData("  CMD Num:");
			Printf_HEX32( InDataStr.CMD_Data );
			TxStringData("\r\n");
			break;
		}
	}
}

void SupCOMMenu_Fun( struct SupCOM_MENU_Str *p )
{
	switch( p->State )
	{
		case Menu_Init :
		p->MenuInitFun();

		break;

		case Menu_Chrose :
		SupCOM_MenuEXE( p );
//		p->MenuExecuteFun();

		break;

		case Menu_ESC :
		p->MeunESCFun();
		if( p->PreviousMenu != NULL )
		{

		}
		
		break;
	}
}


//void PrintFCUKSTC(void)
//{
//	CLEAR_SUPCOM();
//	
//	TxStringData("FUCK STC \r\n");
//}

void SupCOM_MenuEXE( struct SupCOM_MENU_Str *p )
{
	if(( RxDataStr.RxString[0] == 'e' ) || ( RxDataStr.RxString[0] == 'E' ))
	{
		if( Menu_P->PreviousMenu == NULL )
		{
			SupCOMPrintString_Level( "End of Menu--------------------------->CMD \r\n",  SupCOM_APP_PrintLever );

			SupCOM_P.SupCOM_State = SupCOM_NULL;
			InDataStr.InData_State	=	SC_IND_CMD;

			CLEAR_SUPCOM();
			SupCOM_ChanreColor(SupCOM_Color_GB);
			SupCOMPrintString_Level( (u8*)ASCIIImageBuff , SupCOM_APP_PrintLever);
			SupCOM_ChanreColor(SupCOM_Color_WB);
			
			SupCOM_ReadSupCOM_PClass();
		}
		else
		{
			Menu_P = Menu_P->PreviousMenu;
			
			SupCOM_MenuPrint( Menu_P );

			Menu_P->State = Menu_Chrose;
		}

	}
	else
	{
//		Printf_HEX32( InDataStr.InDataNum );
//		TxStringData("\r\n");
		if( InDataStr.InDataNum < p->MenuNum )
		{
			SupCOM_MenuRun( p , InDataStr.InDataNum );
//			p->MenuExecuteFun[InDataStr.InDataNum]();	

		}
		else
		{
			TxStringData("NO Fount :");
			Printf_HEX32( InDataStr.InDataNum );
			TxStringData("\r\n");
		}
	}
}


void SupCOM_MenuRun(  struct SupCOM_MENU_Str *p  , u8 Num )
{
	switch( p->ExeStr[Num].EXEATT_P )
	{
		case MenuStr:
			
		Menu_P = p->ExeStr[Num].PreviousMenu;

		SupCOM_P.SupCOM_State = SupCOM_MENU;
		
		SupCOM_MenuPrint( Menu_P );
		
		Menu_P->State = Menu_Chrose;

		break;

		case BlockingFun:

		p->ExeStr[Num].FunPrt();
		
		SupCOM_MenuPrint( Menu_P );

		break;

		case NonBlockingFun:

//		p->ExeStr[Num].FunPrt;

		break;

		case TimerFun:

		break;

		case EnterTri:

		SupCOM_P.SupCOM_Enter_FunPrt = p->ExeStr[Num].FunPrt;

		SupCOM_P.SupCom_Fun_State = SupCOM_FUN_INIT;
		
		p->ExeStr[Num].FunPrt();

		SupCOM_P.SupCOM_State = SupCOM_EnterFUM;
		
		SupCOM_P.SupCom_Fun_State = SupCOM_FUN_RUN;

		break;

		case SemTri:

		break;
	}

}


void SupCOM_MenuCallBuck(void)
{
	SupCOM_P.SupCOM_State = SupCOM_MENU;
	
	SupCOM_MenuPrint( Menu_P );
	
	Menu_P->State = Menu_Chrose;

	SupCOM_P.SupCom_Fun_State = SupCOM_FUN_END;
}

void SupCOM_MenuPrint( struct SupCOM_MENU_Str *p )
{
	u8 Count_i , Count_x = 0;

	CLEAR_SUPCOM();

	Count_i = 0;

	while( *( ( p->MeunTitel ) + Count_i ) != '\0' )
	Count_i ++;

	Count_x = ( 45 - Count_i ) / 2;

	SupCOM_ChanreColor(SupCOM_Color_WB);

	SupCOMPrintNbitChar_Level( '=' , 47 , SupCOM_APP_PrintLever );
	SupCOMPrintString_Level( " \r\n", SupCOM_APP_PrintLever);
	SupCOMPrintString_Level("*                                             *\r\n" ,SupCOM_APP_PrintLever );
	
	SupCOMPrintChar_Level( '*' , SupCOM_APP_PrintLever );

	SupCOMPrintNbitChar_Level( ' ' , Count_x , SupCOM_APP_PrintLever );
	SupCOMPrintString_Level( (u8 *)(p->MeunTitel) , SupCOM_APP_PrintLever);
	Count_x = 45 - Count_x - Count_i;
	SupCOMPrintNbitChar_Level( ' ' , Count_x , SupCOM_APP_PrintLever );
	SupCOMPrintChar_Level( '*' , SupCOM_APP_PrintLever );
	SupCOMPrintString_Level( " \r\n", SupCOM_APP_PrintLever);
	
	SupCOMPrintString_Level("*                                             *\r\n" , SupCOM_APP_PrintLever);	
	SupCOMPrintNbitChar_Level( '=' , 47 ,SupCOM_APP_PrintLever );
	SupCOMPrintString_Level( " \r\n", SupCOM_APP_PrintLever);

	for( Count_i = 0 ; Count_i < p->MenuNum ; Count_i++ )
	{
		SupCOM_Printf_Num( Count_i , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level( ". " , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level( (u8 *)p->TabName[Count_i] , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level( " \r\n", SupCOM_APP_PrintLever);
	}
	SupCOMPrintString_Level( "E.-ESC \r\n",  SupCOM_APP_PrintLever);

	InDataStr.InData_State = SC_IND_NUM;

}


const u8 SysMenuTitel[] = "SysTemMeun";

const u8 SysMenuTab0[] = "Display system information";
const u8 SysMenuTab1[] = "GG-SupCOM Settings";
const u8 SysMenuTab2[] = "General Settings";
const u8 SysMenuTab3[] = "Projcet Settings";
const u8 SysMenuTab4[] = "System Update";
const u8 SysMenuTab5[] = "Factory Reset";
const u8 SysMenuTab6[] = "TCP transfer";


void CreateSysMenu(void)
{

	SysTemMenu.MeunESCFun		= NULL;
	SysTemMenu.MenuInitFun		= NULL;
//	SysTemMenu.MenuExecuteFun	= NULL;

	SysTemMenu.MenuNum 			= 7;

	SysTemMenu.MeunTitel 		= SysMenuTitel;
	
	SysTemMenu.TabName[0] 		= SysMenuTab0;
	SysTemMenu.TabName[1] 		= SysMenuTab1;
	SysTemMenu.TabName[2] 		= SysMenuTab2;
	SysTemMenu.TabName[3] 		= SysMenuTab3;
	SysTemMenu.TabName[4] 		= SysMenuTab4;
	SysTemMenu.TabName[5] 		= SysMenuTab5;
	SysTemMenu.TabName[6] 		= SysMenuTab6;

	SysTemMenu.State			= Menu_Init;

	SysTemMenu.PreviousMenu		= NULL;

//	SysTemMenu.ExeStr[0].EXEATT_P		= BlockingFun;
//	SysTemMenu.ExeStr[0].FunPrt 		= PrintFCUKSTC;

	SysTemMenu.ExeStr[3].EXEATT_P		= MenuStr;
	SysTemMenu.ExeStr[3].PreviousMenu 	= &SysTemSet;

	SysTemMenu.ExeStr[4].EXEATT_P		= BlockingFun;
	SysTemMenu.ExeStr[4].FunPrt 		= YModem;

	SysTemMenu.ExeStr[5].EXEATT_P		= EnterTri;
	SysTemMenu.ExeStr[5].FunPrt 		= InitDevice;

	SysTemMenu.ExeStr[6].EXEATT_P		= EnterTri;
	SysTemMenu.ExeStr[6].FunPrt 		= TCPTransfer;

	

}

const u8 SysSetTitel[] 	= "Projcet Settings";

const u8 SysSetTab0[]	= "Set system time";	
const u8 SysSetTab1[]	= "Set Log-IN Password";	
const u8 SysSetTab2[]	= "Set polling interval";	
const u8 SysSetTab3[]	= "Set ServerIP & Port";	
const u8 SysSetTab4[]	= "Enter ESP-TOUCH Mode";	
const u8 SysSetTab5[]	= "Enter JSON Monitoring Mode";	
const u8 SysSetTab6[]	= "Set SupCOM Display Level";	


void CreateSysSET(void)
{
	
	SysTemSet.MenuInitFun 		= NULL;
	SysTemSet.MeunESCFun		= NULL;

	SysTemSet.MenuNum 			= 7;

	SysTemSet.MeunTitel			= SysSetTitel;

	SysTemSet.TabName[0]		= SysSetTab0;
	SysTemSet.TabName[1]		= SysSetTab1;
	SysTemSet.TabName[2]		= SysSetTab2;
	SysTemSet.TabName[3]		= SysSetTab3;
	SysTemSet.TabName[4]		= SysSetTab4;
	SysTemSet.TabName[5]		= SysSetTab5;
	SysTemSet.TabName[6]		= SysSetTab6;

	SysTemSet.State				= Menu_Init;

	SysTemSet.PreviousMenu		= &SysTemMenu;

	SysTemSet.ExeStr[0].EXEATT_P	= EnterTri;
	SysTemSet.ExeStr[0].FunPrt 		= SetTime;

//	SysTemSet.ExeStr[1].EXEATT_P	= EnterTri;
//	SysTemSet.ExeStr[1].FunPrt 		= SetPassWord;

	SysTemSet.ExeStr[3].EXEATT_P	= EnterTri;
	SysTemSet.ExeStr[3].FunPrt 		= SetSeverIP;

	
	SysTemSet.ExeStr[6].EXEATT_P	= EnterTri;
	SysTemSet.ExeStr[6].FunPrt 		= SupCOM_SetLevel;

	
}




void CreateSysINF(void)
{
	SysTemINF.MeunESCFun		= NULL;
//	SysTemINF.MenuExecuteFun	= NULL;

	SysTemINF.State			= Menu_Init;

	SysTemINF.PreviousMenu		= &SysTemMenu;
}


u32 SupCOM_CMD(void)
{
	u32 CMDData = 0;
	u8 	Count_i = 0;
	
	if( RxDataStr.RxString[0] == '-')
	{
		Count_i ++;
		
		while( RxDataStr.RxString[Count_i] != '\0')
		{
			if(( RxDataStr.RxString[Count_i] >= 'A' ) && ( RxDataStr.RxString[Count_i] <= 'Z' ))
			CMDData |= (( RxDataStr.RxString[Count_i] - 'A' ) & 0x1f) << ( 25 - ( ( Count_i - 1 ) * 5) );
			else if(( RxDataStr.RxString[Count_i] >= 'a' ) && ( RxDataStr.RxString[Count_i] <= 'z' ))
			CMDData |= (( RxDataStr.RxString[Count_i] - 'a' ) & 0x1f) << ( 25 - ( ( Count_i - 1 ) * 5) );
			
			Count_i ++;
			if( Count_i >= 7 )
			break;
		}
		
		CMDData |= 0x40000000;
	}
	else if( RxDataStr.RxString[0] == ':')
	{
		Count_i ++;
		
		while( RxDataStr.RxString[Count_i] != '\0')
		{
			if(( RxDataStr.RxString[Count_i] >= 'a' ) && ( RxDataStr.RxString[Count_i] <= 'z' ))
			CMDData |= (( RxDataStr.RxString[Count_i] - 'a' ) & 0x1f) << ( 25 - ( Count_i * 5) );	

			Count_i ++;
			if( Count_i >= 7 )
			break;
		}
		CMDData |= 0xC0000000;
	}
	else
	{
		while( RxDataStr.RxString[Count_i] != '\0')
		{
			if(( RxDataStr.RxString[Count_i] >= 'a' ) && ( RxDataStr.RxString[Count_i] <= 'z' ))
			CMDData |= (( RxDataStr.RxString[Count_i] - 'a' ) & 0x1f) << ( 25 - ( Count_i * 5) );	

			Count_i ++;

			if( Count_i >= 6 )
			break;
		}
		CMDData |= 0x80000000;
	}

	return CMDData;
}


u32 SupCOMStringToNum( u8 *StringBuff )
{

	u32 NumData = 0;
	u8 Count_i;

	while(( *StringBuff != '\0' )&&( Count_i < 32 ))
	{
		if(( *StringBuff >= '0' )&&( *StringBuff <= '9' ))
		{
			NumData *= 10;
			NumData = NumData + ( *StringBuff - 0x30 ); 
		}
		else 
		break;

		StringBuff ++;
		Count_i ++;
	}

	
//	Printf_Num(NumData);
//	TxStringData( " \r\n");
	
	return NumData;

}

void SupCOM_CopyMemory( u8 *StringBuffA ,u8 *StringBuffB )
{
	u8 Count_i = 0;
	
	while(( *StringBuffA != '\0' )&&( Count_i < 32 ))
	{
		*StringBuffB = *StringBuffA;
		
		StringBuffA ++;
		StringBuffB ++;
		
		Count_i ++;
	}

	*StringBuffB = '\0';
}

void SupCOM_InYorN( void )
{
	if(( RxDataStr.RxString[0] == 'Y' )||( RxDataStr.RxString[0] == 'y' ))
	{
		InDataStr.InDataYorN = InYes;
	}
	else if(( RxDataStr.RxString[0] == 'N' )||( RxDataStr.RxString[0] == 'n' ))
	{
		InDataStr.InDataYorN = InNo;
	}
	else
	{
		InDataStr.InDataYorN = 0x0E;
	}
}


void SupCOM_SetLevel(void)
{
	static u8 SetLevelState;
	
	u8 DataBuff[4];
	
	if( SupCOM_P.SupCom_Fun_State == SupCOM_FUN_INIT )
	{
			SupCOMPrintString_Level("=============================================== \r\n" , SupCOM_APP_PrintLever);	
			SupCOMPrintString_Level("*                                             * \r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("*           Set SupCOM Printf Level           * \r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("*                                             * \r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("=============================================== \r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("*  Level 0  |  Only printf Super COM Inf      *\r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("*  Level 1  |  Add Super COM prompt message   *\r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("*  Level 2  |  Level of freedom               *\r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("*  Level 3  |  Level of freedom               *\r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("*  Level 4  |  Level of freedom               *\r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("*  Level 5  |  Level of freedom               *\r\n" , SupCOM_APP_PrintLever);
			SupCOMPrintString_Level("=============================================== \r\n" , SupCOM_APP_PrintLever);
	
			SetLevelState = 0;

			W25QXX_Read( DataBuff , 0x002001 , 4 );

	//		InDataStr.InData_State = SC_IND_NUM;
			SupCOMPrintString_Level("SupCOM Level  : " ,SupCOM_APP_PrintLever );
			SupCOM_Printf_Num( DataBuff[0] ,SupCOM_APP_PrintLever );
			SupCOMPrintString_Level(" \r\n" ,SupCOM_APP_PrintLever );
			
			SupCOMPrintString_Level("Enter Number : \r\n" ,SupCOM_APP_PrintLever );

			InDataStr.InData_State = SC_IND_NUM;
		
	}
	else
	{
		switch( SetLevelState )
		{
			case 0 :

			if( InDataStr.InDataNum <= 5 )
			{
				SupCOM_PClass.Level = InDataStr.InDataNum;

				SupCOM_SavaSupCOM_PClass();
				
				SupCOMPrintString_Level("SupCOM Set Level successful , Level : " ,SupCOM_APP_PrintLever );
				SupCOM_Printf_Num( SupCOM_PClass.Level ,SupCOM_APP_PrintLever );
				SupCOMPrintString_Level(" \r\n" ,SupCOM_APP_PrintLever );
				SupCOMPrintString_Level("Press any key to return. \r\n" , SupCOM_APP_PrintLever );
				
				SupCOM_PClass.Level = SupCOM_APP_PrintLever;
					
				SetLevelState = 1;
			}
			else
			{
				SupCOMPrintString_Level("Input Error ,Please Enter Number 0 to 5 \r\n",SupCOM_APP_PrintLever);
				SetLevelState = 0;
			}
			break;
	
			case 1:
			SupCOM_MenuCallBuck();
			SetLevelState = 0;
			break;

		}	

	}
	
}

void TCPTransfer(void)
{
	static u8 TCPTransferState;
	static u8 TCP_RES = 0 , SeverState_Sava;
	
	static struct StringStr TCPString;
	
	if( SupCOM_P.SupCom_Fun_State == SupCOM_FUN_INIT )
	{
	
		CLEAR_SUPCOM();
			
		SupCOMPrintString_Level("===============================================\r\n" , SupCOM_APP_PrintLever);	
		SupCOMPrintString_Level("*                                             *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("*                 TCPTransfer                 *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("*                                             *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("===============================================\r\n" , SupCOM_APP_PrintLever);

		String_FushBuff( &TCPString );

		if( Server.State == 1 )
		{
			SupCOMPrintString_Level( "Server IP : " 	, SupCOM_APP_PrintLever);
			SupCOMPrintString_Level( ServerIP 			, SupCOM_APP_PrintLever );
			SupCOMPrintString_Level( "---->Port : " 	, SupCOM_APP_PrintLever);
			SupCOM_Printf_Num( ServerPort 				, SupCOM_APP_PrintLever );		
			SupCOMPrintString_Level( "\r\n" 			, SupCOM_APP_PrintLever);
		}
		else
		{
			SupCOMPrintString_Level( "Server IP : " , SupCOM_APP_PrintLever);
			
			SupCOM_Printf_Num(Server.IP[0],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
			SupCOM_Printf_Num(Server.IP[1],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
			SupCOM_Printf_Num(Server.IP[2],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
			SupCOM_Printf_Num(Server.IP[3],SupCOM_APP_PrintLever);

			SupCOMPrintString_Level( "---->Port : " , SupCOM_APP_PrintLever);
			SupCOM_Printf_Num( Server.Port 			, SupCOM_APP_PrintLever );	
			SupCOMPrintString_Level( "\r\n" 		, SupCOM_APP_PrintLever);
		}

		SupCOMPrintString_Level("* Chrose User IP \r\n" 		, SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("* 1.Memory server IP \r\n" 	, SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("* 2.Enter New server IP \r\n" 	, SupCOM_APP_PrintLever);

		
		AT_TCP.TCP_State	= TCP_SupCOM_Transfer;

		InDataStr.InData_State = SC_IND_NUM;

//		InDataStr.InData_State = SC_IND_STR;
		
//		AT_LinkTCP_Sever();
		AT_TCP_SetReDataMode();

		TCPTransferState = 0;
		
	}
	else
	{
		switch( TCPTransferState )
		{
			case 0:
			if(( InDataStr.InDataNum <= 2 )&&( InDataStr.InDataNum != 0))
			{
				if( InDataStr.InDataNum == 1 )
				{
					TCP_RES = AT_LinkTCP_Sever();
					
					InDataStr.InData_State = SC_IND_STR;
					TCPTransferState = 7;

					AT_SendCMD( "AT+CIPCLOSE" );
				}
				else
				{
					TxStringData("Enter Sever IP (xxx.xxx.xxx.xxx) : \r\n" );

					InDataStr.InData_State = SC_IND_STR;
					TCPTransferState = 1;
				}
			}
			else
			{
					TxStringData("Input Error ,Please Enter Number 1 to 2 \r\n");
					TCPTransferState = 0;
			}
			break;

			case 1 :
			if( AT_StringtoIP( RxDataStr.RxString , Server.IP ) == 1 )
			{
				TxStringData("Enter Sever Port : \r\n" );
				InDataStr.InData_State = SC_IND_NUM;
				TCPTransferState = 2;
			}
			else
			{
				TxStringData("Formal error !!  Enter Sever IP (xxx.xxx.xxx.xxx) : \r\n" );
				TCPTransferState = 1;
			}
			break;

			case 2 :
			Server.Port = InDataStr.InDataNum;		
			SupCOMPrintString_Level( "New server IP & Port : " , SupCOM_APP_PrintLever);
			
			SupCOM_Printf_Num(Server.IP[0],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
			SupCOM_Printf_Num(Server.IP[1],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
			SupCOM_Printf_Num(Server.IP[2],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
			SupCOM_Printf_Num(Server.IP[3],SupCOM_APP_PrintLever);
			
			SupCOMPrintString_Level( "---->Port : " 	, SupCOM_APP_PrintLever);
			SupCOM_Printf_Num( Server.Port 				, SupCOM_APP_PrintLever );	
			SupCOMPrintString_Level( "\r\n" 			, SupCOM_APP_PrintLever);
				
			SupCOMPrintString_Level( "New server Y/N :" , SupCOM_APP_PrintLever);
			InDataStr.InData_State = SC_IND_YorN;
			TCPTransferState = 3;
			
			break;
			case 3 :
			if( InDataStr.InDataYorN != InError )
			{
				if( InDataStr.InDataYorN == InYes )
				{
					SeverState_Sava = Server.State ;
					
					Server.State = 0;
					
					TCP_RES = AT_LinkTCP_Sever();
					if( TCP_RES == AT_OK )
					{
						SupCOMPrintString_Level("Link TCP OK! \r\n" ,SupCOM_APP_PrintLever );
						AT_TCP_SetReDataMode();
						InDataStr.InData_State = SC_IND_STR;
						TCPTransferState = 7;
					}
					else
					{
						SupCOMPrintString_Level("Link TCP Error! \r\n" ,SupCOM_APP_PrintLever );				
						SupCOMPrintString_Level("Press any key to Retry ! \r\n" ,SupCOM_APP_PrintLever );
					}
				}
				else
				{
					SupCOMPrintString_Level("Re Enter Sever IP ?  : Y/N \r\n" ,SupCOM_APP_PrintLever );
					TCPTransferState = 4;
				}
			}
			else
			{
				SupCOMPrintString_Level( "Plese Enter Y or N  Y/N :" , SupCOM_APP_PrintLever);
				TCPTransferState = 3;
			}
			break;

			case 4 :
			if( InDataStr.InDataYorN != InError )
			{
				if( InDataStr.InDataYorN == InYes )
				{
					SupCOMPrintString_Level("Enter Sever IP (xxx.xxx.xxx.xxx) : \r\n" ,SupCOM_APP_PrintLever);

					InDataStr.InData_State = SC_IND_STR;
					TCPTransferState = 1;
				}
				else
				{
					SupCOMPrintString_Level( "Quit TCP Transfer \r\n" , SupCOM_APP_PrintLever);
					SupCOMPrintString_Level("Press any key to return. \r\n" ,SupCOM_APP_PrintLever);
					TCPTransferState = 8;
				}
			}
			else
			{
				SupCOMPrintString_Level( "Plese Enter Y or N  Y/N :" , SupCOM_APP_PrintLever);
				TCPTransferState = 3;
			}
			break;
			case 7 :
				if(( RxDataStr.RxString[0] == '!' )&&( RxDataStr.RxString[1] == 'q' ))
				{
					SupCOMPrintString_Level( "Quit TCP Transfer \r\n" , SupCOM_APP_PrintLever);
					SupCOMPrintString_Level("Press any key to return. \r\n" ,SupCOM_APP_PrintLever);
					TCPTransferState = 8;
					
					Server.State = SeverState_Sava;
				}
				else
				{
					String_FushBuff( &TCPString );
					CopyStringToStringDataBuff( RxDataStr.RxString , &TCPString );
					AT_SendTCPData( TCPString.DataNum ,TCPString.DataBuff );
					String_FushBuff( &TCPString );
					TCPTransferState = 7;
				}
			break;
			case 8 :
				AT_SendCMD( "AT+CIPCLOSE" );
			
				ESP_ReadIPPort();
				SupCOM_MenuCallBuck();
			break;
		}

	}
}

void SetSeverIP(void)
{
	static u8 SetSeverIPState;
	
	if( SupCOM_P.SupCom_Fun_State == SupCOM_FUN_INIT )
	{
		
		CLEAR_SUPCOM();
		SupCOMPrintString_Level("===============================================\r\n" , SupCOM_APP_PrintLever);	
		SupCOMPrintString_Level("*                                             *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("*              Set Sever IP & Port            *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("*                                             *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("===============================================\r\n" , SupCOM_APP_PrintLever);


		SupCOMPrintString_Level( "Default server IP : " , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level( ServerIP 				, SupCOM_APP_PrintLever );
		SupCOMPrintString_Level( "---->Port : " 		, SupCOM_APP_PrintLever);
		SupCOM_Printf_Num( ServerPort 					, SupCOM_APP_PrintLever );		
		SupCOMPrintString_Level( "\r\n" 				, SupCOM_APP_PrintLever);
		

		SupCOMPrintString_Level( "UserSet server IP : " , SupCOM_APP_PrintLever);
		
		SupCOM_Printf_Num(Server.IP[0],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
		SupCOM_Printf_Num(Server.IP[1],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
		SupCOM_Printf_Num(Server.IP[2],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
		SupCOM_Printf_Num(Server.IP[3],SupCOM_APP_PrintLever);

		SupCOMPrintString_Level( "---->Port : " , SupCOM_APP_PrintLever);
		SupCOM_Printf_Num( Server.Port 			, SupCOM_APP_PrintLever );	
		SupCOMPrintString_Level( "\r\n" 		, SupCOM_APP_PrintLever);


		SupCOMPrintString_Level("* 1.Chrose User IP \r\n" 			, SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("* 2.Set UserSet server IP \r\n" 	, SupCOM_APP_PrintLever);

		
		SupCOMPrintString_Level("Please Num to choose \r\n" 		, SupCOM_APP_PrintLever);

		SetSeverIPState = 7;
		InDataStr.InData_State = SC_IND_NUM;
//		TxStringData("Enter Sever IP (xxx.xxx.xxx.xxx) : \r\n" );
		
	}
	else
	{
		switch( SetSeverIPState )
		{
			case 7:
			if(( InDataStr.InDataNum <= 2 )&&( InDataStr.InDataNum != 0))
			{
				if( InDataStr.InDataNum == 1)
				{
					SupCOMPrintString_Level("* Chrose User IP \r\n" 			, SupCOM_APP_PrintLever);
					SupCOMPrintString_Level("* 1.User Default server IP \r\n" 	, SupCOM_APP_PrintLever);
					SupCOMPrintString_Level("* 2.Uesr UserSet server IP \r\n" 	, SupCOM_APP_PrintLever);
					
					InDataStr.InData_State = SC_IND_NUM;
					
					SetSeverIPState = 2;

				}
				else
				{
					TxStringData("Enter Sever IP (xxx.xxx.xxx.xxx) : \r\n" );

					InDataStr.InData_State = SC_IND_STR;
					SetSeverIPState = 0;
				}

			}
			else
			{
					TxStringData("Input Error ,Please Enter Number 1 to 2 \r\n");
					SetSeverIPState = 7;
			}
			break;
			case 0 :
			if( AT_StringtoIP( RxDataStr.RxString , Server.IP ) == 1 )
			{
				TxStringData("Enter Sever Port : \r\n" );
				InDataStr.InData_State = SC_IND_NUM;
				SetSeverIPState = 1;
			}
			else
			{
				TxStringData("Formal error !!  Enter Sever IP (xxx.xxx.xxx.xxx) : \r\n" );
				SetSeverIPState = 0;
			}
			break;
			case 1 :
				Server.Port = InDataStr.InDataNum;
				
				SupCOMPrintString_Level( "New server IP & Port : " , SupCOM_APP_PrintLever);

				SupCOM_Printf_Num(Server.IP[0],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
				SupCOM_Printf_Num(Server.IP[1],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
				SupCOM_Printf_Num(Server.IP[2],SupCOM_APP_PrintLever);SupCOMPrintChar_Level( '.' ,SupCOM_APP_PrintLever);
				SupCOM_Printf_Num(Server.IP[3],SupCOM_APP_PrintLever);

				SupCOMPrintString_Level( "---->Port : " , SupCOM_APP_PrintLever	);
				SupCOM_Printf_Num( Server.Port 			, SupCOM_APP_PrintLever );	
				SupCOMPrintString_Level( "\r\n" 		, SupCOM_APP_PrintLever	);

//				SupCOMPrintString_Level("* Chrose User IP \r\n" , 0);
//				SupCOMPrintString_Level("* 1.User Default server IP \r\n" , 0);
//				SupCOMPrintString_Level("* 2.Uesr UserSet server IP \r\n" , 0);

//				InDataStr.InData_State = SC_IND_NUM;

				SupCOMPrintString_Level( "Sava Y/N :" 	, SupCOM_APP_PrintLever);
				InDataStr.InData_State = SC_IND_YorN;
				SetSeverIPState = 3;
//				Server.State = InDataStr.InDataNum;
			break;
			case 2 :
				if(( InDataStr.InDataNum <= 2 )&&( InDataStr.InDataNum != 0))
				{
					SupCOMPrintString_Level( "Sava Y/N :" , SupCOM_APP_PrintLever);
					InDataStr.InData_State = SC_IND_YorN;
					SetSeverIPState = 3;
					Server.State = InDataStr.InDataNum;
				}
				else
				{
					TxStringData("Input Error ,Please Enter Number 1 to 2 \r\n");
					SetSeverIPState = 2;
				}
			break;
			case 3 :
			if( InDataStr.InDataYorN != InError )
			{
				if( InDataStr.InDataYorN == InYes )
				{
					SupCOMPrintString_Level( "Sava ------------------------>" , SupCOM_APP_PrintLever);
					ESP_WriteServerData();
					TxStringData("End \r\n Press any key to return. \r\n");
					SetSeverIPState = 4;

				}
				else
				{
					SupCOMPrintString_Level( "Esc. \r\n" , SupCOM_APP_PrintLever);
					TxStringData("Press any key to return. \r\n");
					SetSeverIPState = 4;
				}
			}
			else
			{
				SupCOMPrintString_Level( "Plese Enter Y or N  Y/N :" , SupCOM_APP_PrintLever );
				SetSeverIPState = 3;
			}
			break;
			case 4:
			SetSeverIPState = 0;
			SupCOM_MenuCallBuck();

			break;
		}
	}

}

extern struct SystemTime SysTime_Str;

void SetTime(void)
{
	static u8 SetTimeState;

	if( SupCOM_P.SupCom_Fun_State == SupCOM_FUN_INIT )
	{

		CLEAR_SUPCOM();
		SupCOMPrintString_Level("===============================================\r\n" , SupCOM_APP_PrintLever);	
		SupCOMPrintString_Level("*                                             *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("*                Set SysTem Time              *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("*                                             *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("===============================================\r\n" , SupCOM_APP_PrintLever);

		Printf_Num(SysTime_Str.S_Year);	TxBitData('-');
		Printf_Num(SysTime_Str.S_Month);TxBitData('-');
		Printf_Num(SysTime_Str.S_Days);
		TxStringData("\r\n");
		Printf_Num(SysTime_Str.S_Hour);TxBitData(':');
		Printf_Num(SysTime_Str.S_Mins);TxBitData(':');
		Printf_Num(SysTime_Str.S_Secs);
		TxStringData("\r\n");

		SupCOMPrintString_Level("* 1.Manual input \r\n" 				, SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("* 2.Get time from the network \r\n" 	, SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("Please Num to choose \r\n" 			, SupCOM_APP_PrintLever);

		SetTimeState = 8;

		InDataStr.InData_State = SC_IND_NUM;

//		TxStringData("Please enter the year : \r\n");
//		TxStringData("Year : ");

		
	}
	else
	{
		switch( SetTimeState )
		{
			case 8 :
			if(( InDataStr.InDataNum <= 2 )&&( InDataStr.InDataNum != 0))
			{
				if( InDataStr.InDataNum == 1)
				{
					TxStringData("Please enter the year : \r\n");
					TxStringData("Year : ");
					InDataStr.InData_State = SC_IND_NUM;
					SetTimeState = 0;

				}
				else
				{
					GetNTPTime();
					
					Printf_Num(SysTime_Str.S_Year); TxBitData('-');
					Printf_Num(SysTime_Str.S_Month);TxBitData('-');
					Printf_Num(SysTime_Str.S_Days);
					TxStringData("\r\n");
					Printf_Num(SysTime_Str.S_Hour);TxBitData(':');
					Printf_Num(SysTime_Str.S_Mins);TxBitData(':');
					Printf_Num(SysTime_Str.S_Secs);
					TxStringData("\r\n");
					
					TxStringData("Press any key to return. \r\n");
					InDataStr.InData_State = SC_IND_STR;
					SetTimeState = 6;
				}

			}
			else
			{
					TxStringData("Input Error ,Please Enter Number 1 to 2 \r\n");
					SetTimeState = 8;
			}
			break;
		
			case 0 :
			if(( InDataStr.InDataNum >= 1997)&&( InDataStr.InDataNum <= 9999 ))
			{
				SysTime_Str.S_Year = InDataStr.InDataNum;
				
				TxStringData("Please enter the Month : \r\n");
				TxStringData("Month : ");

				SetTimeState = 1;

			}
			else
			{
				TxStringData("Please enter the correct year \r\n");
				
				TxStringData("Year : ");
			}

			break;

			case 1 :
			if(( InDataStr.InDataNum >= 1 )&&( InDataStr.InDataNum <= 12 ))
			{
				SysTime_Str.S_Month = InDataStr.InDataNum;
				
				TxStringData("Please enter the Days : \r\n");
				TxStringData("Days : ");

				SetTimeState = 2;

			}
			else
			{
				TxStringData("Please enter the correct month \r\n");
			
				TxStringData("Month : ");
			}

			break;

			case 2 :
			if(( InDataStr.InDataNum >= 1)&&( InDataStr.InDataNum <= 31 ))
			{
				SysTime_Str.S_Days = InDataStr.InDataNum;
				
				TxStringData("Please enter the Hour : \r\n");
				TxStringData("Hour : ");

				SetTimeState = 3;

			}
			else
			{
				TxStringData("Please enter the correct Days \r\n");

				TxStringData("Days : ");
			}

			break;

			case 3 :
			if( InDataStr.InDataNum <= 23 )
			{
				SysTime_Str.S_Hour = InDataStr.InDataNum;
				
				TxStringData("Please enter the Mins : \r\n");
				TxStringData("Mins : ");

				SetTimeState = 4;

			}
			else
			{
				TxStringData("Please enter the correct Hour \r\n");

				TxStringData("Hour : ");
			}

			break;

			case 4 :
			if( InDataStr.InDataNum <= 59 )
			{
				SysTime_Str.S_Mins = InDataStr.InDataNum;
				
				TxStringData("Please enter the Sec : \r\n");
				TxStringData("Sec : ");

				SetTimeState = 5;

			}
			else
			{
				TxStringData("Please enter the correct Mins \r\n");

				TxStringData("Mins : ");
			}

			break;

			case 5 :
			if( InDataStr.InDataNum <= 59 )
			{
				SysTime_Str.S_Secs = InDataStr.InDataNum;
				
				Printf_Num(SysTime_Str.S_Year); TxBitData('-');
				Printf_Num(SysTime_Str.S_Month);TxBitData('-');
				Printf_Num(SysTime_Str.S_Days);
				TxStringData("\r\n");
				Printf_Num(SysTime_Str.S_Hour);TxBitData(':');
				Printf_Num(SysTime_Str.S_Mins);TxBitData(':');
				Printf_Num(SysTime_Str.S_Secs);
				TxStringData("\r\n");
				
				SetTimeState = 6;
				TxStringData("Press any key to return \r\n");

			}
			else
			{
				TxStringData("Please enter the correct Sec \r\n");
				TxStringData("Sec : ");
			}

			break;
			case 6:

				SupCOM_MenuCallBuck();
				SetTimeState = 0;

			break;

		};
	}
}

void InitDevice(void)
{
	u8  count_i = 0;
	
	if( SupCOM_P.SupCom_Fun_State == SupCOM_FUN_INIT )
	{

		CLEAR_SUPCOM();
		SupCOMPrintString_Level("===============================================\r\n" , SupCOM_APP_PrintLever);	
		SupCOMPrintString_Level("*                                             *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("*             Initialization equipment        *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("*                                             *\r\n" , SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("===============================================\r\n" , SupCOM_APP_PrintLever);

		SupCOM_ChanreColor(SupCOM_Color_RB);

		SupCOMPrintString_Level("The initialization device is irreversible. \r\n" 	, SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("Please confirm. \r\n" 	, SupCOM_APP_PrintLever);

		SupCOMPrintString_Level("Initialization equipment ? \r\n" 	, SupCOM_APP_PrintLever);
		SupCOMPrintString_Level("Press Y/N \r\n" 					, SupCOM_APP_PrintLever);

		SupCOM_ChanreColor(SupCOM_Color_WB);

		InDataStr.InData_State = SC_IND_YorN;

	}
	else
	{
		if( InDataStr.InDataYorN != InError )
		{
			if( InDataStr.InDataYorN == InYes )
			{
				SupCOM_ChanreColor(SupCOM_Color_YB);
			
				SupCOMPrintString_Level( "Erase Flash Chip , This step will last 1-2 minutes.\r\n" , SupCOM_APP_PrintLever);
				
				SupCOMPrintString_Level( "--Erase \r\n" , SupCOM_APP_PrintLever);
				
				W25QXX_Erase_Chip_NACK();
				
				while((W25QXX_ReadSR()&0x01)==0x01)
				{
					DelayS(1);
					SupCOMPrintChar_Level( '.', SupCOM_APP_PrintLever );
					count_i ++;
					if( count_i >= 180 )
					{
						SupCOM_ChanreColor(SupCOM_Color_RB);
						SupCOMPrintString_Level( "Erase Flash Chip TimeOut!\r\n" , SupCOM_APP_PrintLever);
						SupCOMPrintString_Level( "Please check hardware!!!\r\n" , SupCOM_APP_PrintLever);
						SupCOM_ChanreColor(SupCOM_Color_YB);
						break;
					}
				}
				
				SupCOMPrintString_Level( "\r\n" , 0);
				
				if( count_i < 180 )
				{
					SupCOMPrintString_Level( "Erase Flash Chip Successful.\r\n" , SupCOM_APP_PrintLever);
					
					SupCOM_ChanreColor(SupCOM_Color_RB);
					
					SupCOMPrintString_Level( "Restart device Now... Don't turn off the power.\r\n" , SupCOM_APP_PrintLever);
					
					__set_FAULTMASK(1); 
					NVIC_SystemReset();
					
					SupCOM_ChanreColor(SupCOM_Color_WB);
				}
				
			}
			else
			{
					SupCOMPrintString_Level( "Esc. \r\n" , SupCOM_APP_PrintLever);
					TxStringData("Press any key to return. \r\n");
			}
		}
		else
		{
			TxStringData("Press Y or N. \r\n");
		}

	}



}

