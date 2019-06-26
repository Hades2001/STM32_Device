#include "ControlAPP.h"


struct TACK_Par OperaPar;

#define K_Delay_S		300

#define RELAY_OFF		0
#define RELAY_ON_ADD	1
#define RELAY_ON_SUB	2

#define Mode_Refrigeration		0
#define Mode_Heading			1

const u8 OLEDSysICO_CloseBMP[8]	= {0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00};
const u8 OLEDSysICO_OpenBMP[8]	= {0x00,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x00};


void RelayFun( u8 Relay_State )
{
	switch( Relay_State )
	{
		case RELAY_OFF :
			
		GPIO_ResetBits( GPIOB , GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 );
		
		break;
		
		case RELAY_ON_SUB :
			
		GPIO_ResetBits( GPIOB , GPIO_Pin_6 | GPIO_Pin_7 );
		GPIO_SetBits( 	GPIOB , GPIO_Pin_5 | GPIO_Pin_8 );
	
		break;

		case RELAY_ON_ADD :

		GPIO_SetBits( 	GPIOB , GPIO_Pin_6 | GPIO_Pin_7 );
		GPIO_ResetBits( GPIOB , GPIO_Pin_5 | GPIO_Pin_8 );

		break;

		default:
			
		GPIO_ResetBits( GPIOB , GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 );

		break;
	}

}

void TempReadMode(void)
{
	if( OperaPar.Temp_A > OperaPar.Temp_B )
	{
		OperaPar.L = Mode_Heading;
	}
	else
	{
		OperaPar.L = Mode_Refrigeration;
	}
}

void ReadDoppler( void )
{
	if(( OperaPar.AI == 1 )&&( OperaPar.D == 1 )&&( OperaPar.DC == 0 )&&( OperaPar.TW == 0 ))
	{
		if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0 )
		OFF_KLight();
	}
	
	OperaPar.D = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
}


void ControlRelay(void)
{
	if( OperaPar.K == 1 )
	{
		RelayFun( RELAY_OFF );
		OLED_ShowString( 64	, 6 , "RELAY_OFF" );
		LED2_ON();
	}							   
	else
	{
		if( OperaPar.Temp_A > OperaPar.Temp_B )
		{
			RelayFun( RELAY_ON_SUB );
			OLED_ShowString( 64	, 6 , "RELAY_SUB" );
		}
		else
		{
			RelayFun( RELAY_ON_ADD );
			OLED_ShowString( 64	, 6 , "RELAY_ADD" );
		}
		LED2_OFF();
	}
}

void Open_KLight( void )
{
	OperaPar.K = 1;
	OperaPar.K_State = 1;
}

void OFF_KLight( void )
{
	OperaPar.K_State = 0;
}

void KLightTimer1SFun( void )
{
	if(( OperaPar.K_StateSave != OperaPar.K_State )&&( OperaPar.K_State == 0 ))
	{
		OperaPar.K_Count = 30;
	}
	
	OperaPar.K_StateSave = OperaPar.K_State ;

	if(( OperaPar.K_Count != 0 )&&( OperaPar.K_State == 0 ))
	{
		OperaPar.K_Count -- ;
		if( OperaPar.K_Count == 0 )
		OperaPar.K = 0;
	}
}

void DisFlag( u8 Xsize , u8 YSize , u8 Flag )
{
	if( Flag == 1 )
	OLED_ShowBlock( Xsize , YSize , (u8*)OLEDSysICO_OpenBMP );
	else
	OLED_ShowBlock( Xsize , YSize , (u8*)OLEDSysICO_CloseBMP );
}

void DisControlAPP( void )
{
	OLED_ShowString( 0  , 7 , "AI" );
	OLED_ShowString( 24 , 7 , "DC" );
	OLED_ShowString( 48 , 7 , "TW" );
	OLED_ShowString( 72 , 7 , " D" );
	OLED_ShowString( 96 , 7 , " L" );
	

	DisFlag( 12 , 7 , OperaPar.AI );
	DisFlag( 36 , 7 , OperaPar.DC );
	DisFlag( 60 , 7 , OperaPar.TW );
	DisFlag( 84 , 7 , OperaPar.D );
	DisFlag( 108 , 7 , OperaPar.L );

	OLED_ShowString( 0  , 6 , "K " );
	DisFlag( 		12  , 6 , OperaPar.K );
	OLED_ShowNumPoint( 32 , 6 , OperaPar.K_Count ,0 );

	if(( OperaPar.AI == 1 )&&( OperaPar.D == 1 )&&( OperaPar.DC == 0 )&&( OperaPar.TW == 0 ))
	{
		if( OperaPar.AI_temp_Con == 1 )
		{
			OLED_ShowString( 0 , 4 , "AI T+:" );
			OLED_ShowString( 64 , 4 , "AI T-:" );
			OLED_ShowNumPoint( 40 , 4 , OperaPar.AI_Temp_up , 0 );
			OLED_ShowNumPoint( 104 , 4 , OperaPar.AI_Temp_Low , 0 );
		}
	}
	else
	{
		OLED_ShowString( 0 , 4 , "TEMP+:" );
		OLED_ShowString( 64 , 4 , "TEMP-:" );
		OLED_ShowNumPoint( 40 , 4 , OperaPar.Temp_up , 0 );
		OLED_ShowNumPoint( 104 , 4 , OperaPar.Temp_Low , 0 );
	}
	
	OLED_ShowString( 0  , 5 , "TempA:" );
	OLED_ShowString( 64 , 5 , "TempB:" );

	OLED_ShowNumPoint( 42 , 5 , OperaPar.Temp_A , 0 );
	OLED_ShowNumPoint( 106 , 5 , OperaPar.Temp_B , 0 );

	
	if( ESP_Srt.ESP_State == ESP_AP_LINK )
	OLED_ShowString( 0  , 0 , "WIFI OK   " );
	else
	OLED_ShowString( 0  , 0 , "WIFI ERROR" );

	if( AT_TCP.TCP_LinkState == TCP_LINK_OK )
	OLED_ShowString( 64  , 0 , "TCK OK   " );
	else
	OLED_ShowString( 64  , 0 , "TCP ERROR" );
		
}

void ControlLoop( void )
{

	if(( OperaPar.AI == 1 )&&( OperaPar.D == 1 )&&( OperaPar.DC == 0 )&&( OperaPar.TW == 0 ))
	{
		if( OperaPar.AI_temp_Con == 1 )
		{
			if( OperaPar.L == Mode_Heading )
			{
				if( OperaPar.Temp_B < OperaPar.AI_Temp_up )
				Open_KLight();
				else
				OFF_KLight();
			}
			else
			{
				if( OperaPar.Temp_B > OperaPar.AI_Temp_Low )
				Open_KLight();
				else
				OFF_KLight();
			}
		}
		else
		{
			Open_KLight();
		}
	}
	
	else
	{
		if( OperaPar.L == Mode_Heading )
		{
			if( OperaPar.Temp_B < OperaPar.Temp_Low )
			OperaPar.TW = 1;
			else
			OperaPar.TW = 0;
			
		}
		else
		{
			if( OperaPar.Temp_B > OperaPar.Temp_up )
			OperaPar.TW = 1;
			else
			OperaPar.TW = 0;
		}
	}

	if(( OperaPar.DC == 1 ) || ( OperaPar.TW == 1 )||( OperaPar.StandbyFlag == 1 ))
	{
		Open_KLight();
	}
	else if(( OperaPar.DC == 0 ) && ( OperaPar.TW == 0 ) && ( OperaPar.AI != 1 )&&( OperaPar.StandbyFlag == 0 ))
	{
		OFF_KLight();
	}
	
	ControlRelay();

	DisControlAPP();

//	OLED_ShowStr( 0 , 4 ,"AI : " , 2 );
//	OLED_ShowStr( 0 , 6 ,"DC : " , 2 );
//	
//	OLED_ShowStr( 64, 4 ,"TW : " , 2 );
//	OLED_ShowStr( 64, 6 ,"D  : " , 2 );

//	OLED_ShowNum(	32	,4,OperaPar.AI	,1 ,16 );	
//	OLED_ShowNum(	32	,6,OperaPar.DC	,1 ,16 );

//	OLED_ShowNum(	96	,4,OperaPar.TW	,1 ,16 );
//	OLED_ShowNum(	96	,6,OperaPar.D	,1 ,16 );
//	

//	OLED_ShowStr( 0 , 0 ,"K  : " , 2 );
//	OLED_ShowStr( 0 , 2 ,"K T: " , 2 );

//	OLED_ShowNum(	32	,0,OperaPar.K	,1 ,16 );	
//	OLED_ShowNum(	32	,2,OperaPar.K_Count	,1 ,16 );

//	OLED_ShowStr( 64 , 0 ,"TEMP A " , 2 );
//	OLED_ShowStr( 64 , 2 ,"TEMP B " , 2 );

//	OLED_ShowNum( 100	,0,OperaPar.Temp_A	,2 ,16 );	
//	OLED_ShowNum( 100	,2,OperaPar.Temp_B	,2 ,16 );

}

void DisPlayControl( void )
{



}

#define Preinstall_Temp_up		35	
#define Preinstall_Temp_low		18

#define Preinstall_AI_Temp_up	35
#define Preinstall_AI_Temp_low	18


void TASK_ReadFlash(void)
{
	u8 	DataBuff[5];
	u32 Address = 0;

	//温度设置

	Address = 0x020000;
	Address = Address + ( (u32)SysTime_Str.S_Hour * 4 );
	
	W25QXX_Read( DataBuff , Address , 4 );
	
	if( DataBuff[0] == 0x10 )
	{
		OperaPar.Temp_up	= 	DataBuff[1];
		OperaPar.Temp_Low	= 	DataBuff[2];
		OperaPar.Temp_con	=	DataBuff[3];

		SupCOMPrintString_Level( "---Memory Temp Set ---\r\n" , 0 );

		SupCOMPrintString_Level( "| Temp up  | " , 0 );	SupCOM_Printf_Num( OperaPar.Temp_up , 0);
		SupCOMPrintString_Level( " |\r\n" , 0 );
		SupCOMPrintString_Level( "| Temp Low | " , 0 );	SupCOM_Printf_Num( OperaPar.Temp_Low , 0);
		SupCOMPrintString_Level( " |\r\n" , 0 );
		SupCOMPrintString_Level( "| Temp con | " , 0 );	SupCOM_Printf_Num( OperaPar.Temp_con, 0);
		SupCOMPrintString_Level( "  |\r\n" , 0 );
		
		SupCOMPrintString_Level( " \r\n" , 0 );
	}
	else
	{
		OperaPar.Temp_up	= Preinstall_Temp_up;

		OperaPar.Temp_Low	= Preinstall_Temp_low;

		OperaPar.Temp_con = 0;

	}

	//AI温度设置

	Address = 0x021000;
	Address = Address + ( (u32)SysTime_Str.S_Hour * 4 );
	
	W25QXX_Read( DataBuff , Address , 4 );
	if( DataBuff[0] == 0x1A )
	{
		OperaPar.AI_Temp_up		=	DataBuff[1];
		OperaPar.AI_Temp_Low	=	DataBuff[2];
		OperaPar.AI_temp_Con	=	DataBuff[3];
		
		SupCOMPrintString_Level( "--Memory AI Temp Set --\r\n" , 0 );

		SupCOMPrintString_Level( "| AI_Temp up  | " , 0 );	SupCOM_Printf_Num( OperaPar.AI_Temp_up , 0);
		SupCOMPrintString_Level( " |\r\n" , 0 );
		SupCOMPrintString_Level( "| AI_Temp Low | " , 0 );	SupCOM_Printf_Num( OperaPar.AI_Temp_Low , 0);
		SupCOMPrintString_Level( " |\r\n" , 0 );
		SupCOMPrintString_Level( "| AI_Temp con | " , 0 );	SupCOM_Printf_Num( OperaPar.AI_temp_Con, 0);
		SupCOMPrintString_Level( "  |\r\n" , 0 );
		
		SupCOMPrintString_Level( "\r\n" , 0 );
	}
	else
	{
		OperaPar.AI_temp_Con = 0;
	}

	Address = 0x010 + ( (u32)SysTime_Str.S_Week - 1 );
	Address = Address << 12;
	Address = Address + ( (u32)SysTime_Str.S_Hour * 5 );

	W25QXX_Read( DataBuff , Address , 5 );
	
	if(( DataBuff[0] == 0x1C )&&( DataBuff[4] == 1 ))
	{
		if(( OperaPar.AI == 1 )&&( DataBuff[2] == 2 )&&( DataBuff[3] == 0 ))
		{
			OFF_KLight();
		}
	
		if( DataBuff[2] == 1 )
		OperaPar.DC = DataBuff[3];
		else
		OperaPar.AI = DataBuff[3];
		
		SupCOMPrintString_Level( "--- Memory TASK Set ---\r\n" , 0 );

		SupCOMPrintString_Level( "| DC  | " , 0 );	SupCOM_Printf_Num( OperaPar.DC , 0);
		SupCOMPrintString_Level( " |\r\n" , 0 );
		
		SupCOMPrintString_Level( "| AI  | " , 0 );	SupCOM_Printf_Num( OperaPar.AI , 0);
		SupCOMPrintString_Level( " |\r\n" , 0 );
		
		SupCOMPrintString_Level( "\r\n" , 0 );
	}

}





