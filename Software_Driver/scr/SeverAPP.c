#include "SeverAPP.h"
#include "LED.h"

struct COLRTSSrt 	Colrt_Prm;
struct PowerStr	 	HLW8012;

void BuffToTimeStr( u8 *DataBuff , struct TimeStr *Time );
void BuffToTEMP( u8 *DataBuff , struct TempStr *Temp );
void BuffToWeekBuff( u8 *DataBuff , u8 *WeekBuff );

#define ModeYEAR		1
#define ModeHOUR		2

extern struct TACK_Par OperaPar;

void SeverQuery(void)
{



}

void SeverPringTime( u8 Mode , struct SystemTime *Time )
{
	
	SupCOM_ChanreColor(SupCOM_Color_BB);
	
	if( Mode == ModeYEAR )
	{
		SupCOM_Printf_Num( Time->S_Year ,0 );
		SupCOMPrintChar_Level( '-' ,0 );
		SupCOM_Printf_Num( Time->S_Month ,0 );
		SupCOMPrintChar_Level( '-' ,0 );
		SupCOM_Printf_Num( Time->S_Days ,0 );
		SupCOMPrintChar_Level( '-' ,0 );
	}

	SupCOM_Printf_Num( Time->S_Hour ,0 );
	SupCOMPrintChar_Level( ':' ,0 );
	SupCOM_Printf_Num( Time->S_Mins ,0 );
	SupCOMPrintChar_Level( ':' ,0 );
	SupCOM_Printf_Num( Time->S_Secs ,0 );
	SupCOMPrintChar_Level( '>' ,0 );

	SupCOM_ChanreColor(SupCOM_Color_WB);

}


void SeverPrintTCP( void )
{
	SupCOMPrintString_Level("TCP ReData Num :",0);
	SupCOM_Printf_Num(AT_TCP.AT_TCPReDataNum,0);
	SupCOMPrintString_Level(" \r\n",0);
	
	SupCOMPrintString_Level("TCP ReData : ",0);
	SupCOMPrintString_Level(AT_TCP.AT_TCPReBuff,0);
	SupCOMPrintString_Level(" \r\n",0);
	
	AT_TCP_SetReDataMode();
}

u8 SeverGetDataCallFun( u8 *DataName , void (*FunPtr)(void) )
{
	struct JSON_Data 	JSONBuff;

	u8 Res = AT_NULL ;

	SeverPringTime( ModeHOUR , &SysTime_Str );

	SupCOM_ChanreColor(SupCOM_Color_YB);
	SupCOMPrintString_Level("Sever Get ", 0 );
	SupCOMPrintString_Level(DataName, 0 );
	SupCOMPrintString_Level("\r\n", 0 );
	SupCOM_ChanreColor(SupCOM_Color_WB);
	
	Res = AT_LinkTCP_Sever();

	if( Res == AT_OK )
	{
		LinkLEDON(5);
		SendRequestJson( DataName, &JSONBuff);
		
		AT_SendTCPData(JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff);
		
		AT_TCP_SetReDataMode();
		
		Res = AT_WaitTCPData( 300 );
		
		if( Res == AT_OK )
		{
			LinkLEDON(5);
			(*FunPtr)();
			SendRequestFlagJSON( DataName , 1 , &JSONBuff );
			AT_SendTCPData( JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff );
		}
	}
	AT_CloseSeverLink();

	return Res;
}


u8 SeverGetData( u8 *DataName )
{
	
	struct JSON_Data 	JSONBuff;

	u8 Res = AT_NULL ;

	SeverPringTime( ModeHOUR , &SysTime_Str );

	SupCOM_ChanreColor(SupCOM_Color_YB);
	SupCOMPrintString_Level("Sever Get ", 0 );
	SupCOMPrintString_Level(DataName, 0 );
	SupCOMPrintString_Level("\r\n", 0 );
	SupCOM_ChanreColor(SupCOM_Color_WB);
	
	Res = AT_LinkTCP_Sever();

	if( Res == AT_OK )
	{
		LinkLEDON(5);
		SendRequestJson( DataName, &JSONBuff);
		
		AT_SendTCPData(JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff);
		
		AT_TCP_SetReDataMode();
		
		Res = AT_WaitTCPData( 100 );
		
		if( Res == AT_OK )
		{
			LinkLEDON(5);
			SendRequestFlagJSON( DataName , 1 , &JSONBuff );
			AT_SendTCPData( JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff );
		}
	}
	
	AT_CloseSeverLink();

	return Res;

}

void SendPower( struct PowerStr *EDataPrt  )
{
	struct JSON_Data	JSONBuff;
	
	u8 TimeBuff[32];
	
	u8 Res = AT_NULL ;
	u8 HourData;
	u8 SendFlag = 0 ;

	SupCOM_ChanreColor(SupCOM_Color_GB);
	SupCOMPrintString_Level("Sever Send Ele-Power \r\n", 0 );
	SupCOM_ChanreColor(SupCOM_Color_WB);

	LinkLEDON(5);
	
	Res = AT_LinkTCP_Sever();
	
	if( Res == AT_OK )
	{
	
		LinkLEDON(5);
		
		PrintfTime( TimeBuff , TIME_YYYYMMDD , &SysTime_Str );

		if( SysTime_Str.S_Hour == 0 )
		HourData = 23;
		else
		HourData = SysTime_Str.S_Hour - 1;

		SendEletricOnLine( TimeBuff , HourData , EDataPrt->Kilowatt , EDataPrt->EleTime , &JSONBuff );
		
		AT_SendTCPData( JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff );
		
		SupCOM_ChanreColor(SupCOM_Color_YB);

		AT_TCP_SetReDataMode();

		Res = AT_WaitTCPData( 100 );

		if( Res == AT_OK )
		{
			LinkLEDON(5);
	
			Res = JSON_ResolveNUm( AT_TCP.AT_TCPReBuff , "FLAG" , (u32*)&SendFlag );
			if( Res == JSON_Ana_ERROR )
			SendFlag = 0 ;
			else
			{
				if( SendFlag == 1 )
				{
					SupCOM_ChanreColor(SupCOM_Color_GB);
					SupCOMPrintString_Level("Send Ele-Power Successful\r\n", 0 );
					SupCOM_ChanreColor(SupCOM_Color_WB);	

					EDataPrt->SendFlag = 1;
				}
				else
				{
					EDataPrt->SendFlag = 2;
				}
			}

		AT_CloseSeverLink();
			
		}
	}
}

void SeverSendPowerOffLine( void )
{
	struct JSON_Data	JSONBuff;

	u8 TimeBuff[32];

	u8 Res = AT_NULL ;
	u8 Count_i = 0;

	u8 PowerBuff[16];

	u8 	RecordNum = 0;
	u32 Address = 0; 
	u32 PowerK;

	u8 SendFlag = 0;
	

	SupCOM_ChanreColor(SupCOM_Color_GB);
	SupCOMPrintString_Level("Sever Send Ele-Power OffLine Data \r\n", 0 );
	SupCOM_ChanreColor(SupCOM_Color_WB);

	LinkLEDON(5);
	
	Res = AT_LinkTCP_Sever();

	if( Res == AT_OK )
	{
		LinkLEDON(5);

		SendEletricOffLine_Head( &JSONBuff );
		AT_SendTCPData( JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff );

		W25QXX_Read( PowerBuff , 0x022000 , 16 );
		

		RecordNum = PowerBuff[3];
			
		Address = 0;

		Address |= PowerBuff[4];
		Address <<= 8;

		Address |= PowerBuff[5];
		Address <<= 8;

		Address |= PowerBuff[6];
		Address <<= 8;

		Address |= PowerBuff[7];

		Address = Address - (( RecordNum - 1 ) * 10 );

		SupCOMPrintString_Level("Hour Num : ",0);
		SupCOM_Printf_Num( RecordNum , 0 );
		SupCOMPrintString_Level("\r\n",0);

		for( Count_i = 0 ; Count_i < RecordNum ; Count_i++ )
		{
			W25QXX_Read( PowerBuff , Address , 10 );

			SupCOMPrintString_Level("Flash Addr :",0);
			SupCOM_PrintfHex32( Address , 0 );
			SupCOMPrintString_Level("\r\n",0);

			PowerK = 0;

			PowerK |= PowerBuff[5];
			PowerK <<= 8;
			PowerK |= PowerBuff[6];
			PowerK <<= 8;
			PowerK |= PowerBuff[7];
			PowerK <<= 8;
			PowerK |= PowerBuff[8];

			if( PowerK != 0 )
			{
				PrintTimeTobuff( TimeBuff , PowerBuff[1] , PowerBuff[2] , PowerBuff[3] );

				SendEletricOffLine_Data( TimeBuff , PowerBuff[4] , PowerK , PowerBuff[9] , &JSONBuff );
			
				AT_SendTCPData( JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff );
			}

			Address += 10;

		}

		SendEletricOffLine_End( &JSONBuff );	
		AT_SendTCPData( JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff );

		AT_TCP_SetReDataMode();

		Res = AT_WaitTCPData( 500 );

		if( Res == AT_OK )
		{
			LinkLEDON(5);
			
			Res = JSON_ResolveNUm( AT_TCP.AT_TCPReBuff , "FLAG" , (u32*)&SendFlag );
			if( Res == JSON_Ana_ERROR )
			Colrt_Prm.DC_Flag = 0 ;
			else
			{
				if( SendFlag == 1 )
				{
					SupCOMPrintString_Level("Send OK!",0);
					W25QXX_Erase_Sector( 0x023 );
					W25QXX_Erase_Sector( 0x022 );

					HLW8012.SendFlag = 0;
				}
				else
				{
					SupCOMPrintString_Level("SendError!",0);
				}
			}
		}
		AT_CloseSeverLink();
	}
}

void SeverSend_Parameter( struct TACK_Par *Pam )
{
	
	u8 Res = AT_NULL ;
	
	struct Parameter 	Par;
	struct JSON_Data 	JSONBuff;
	
	Par.Pmr_DC 		= 	OperaPar.DC;
	Par.Pmr_AI 		= 	OperaPar.AI;
	Par.Pmr_M		= 	OperaPar.L;
	Par.Pmr_R		= 	1;
	Par.Pmr_S		= 	OperaPar.D;
	Par.Pmr_Temp 	=	OperaPar.Temp_A ;
	Par.Pmr_TW		= 	OperaPar.TW;
	Par.Pmr_Wat		= 	HLW8012.Watt    ;
	Par.Pmr_Cur		= 	HLW8012.Current ;

	SeverPringTime( ModeHOUR , &SysTime_Str );
	
	SupCOM_ChanreColor(SupCOM_Color_GB);
	SupCOMPrintString_Level("Sever Send Parameter \r\n", 0 );
	SupCOM_ChanreColor(SupCOM_Color_WB);
	
	Res = AT_LinkTCP_Sever();

	if( Res == AT_OK )
	{
		LinkLEDON(5);
		SendJSON_Parameter( &Par , &JSONBuff );
		
		AT_SendTCPData( JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff );
		
		SupCOM_ChanreColor(SupCOM_Color_YB);
		
		AT_TCP_SetReDataMode();
	}
	else
	{
		SupCOM_ChanreColor(SupCOM_Color_YB);
		SupCOMPrintString_Level("--Link Fail\r\n", 0 );
		SupCOM_ChanreColor(SupCOM_Color_WB);
	}

	AT_CloseSeverLink();


}

void SeverReadTASK(void)
{
	u8 Count_i = 0 , Count_j = 0 ;

	u8 DataBuff[32];

	u32 FlashAddr = 0;

	struct JSON_FindStr	JSONSrt;
	
	struct JSON_VARStr 	TASK_SWITCH_FindSrt;
	struct JSON_VARStr 	TASK_NO_FindSrt;
	struct JSON_VARStr 	TASK_TIME_FindSrt;
	struct JSON_VARStr 	TASK_STATUS_FindSrt;
	struct JSON_VARStr 	TASK_RECYCLE_FindSrt;
	struct JSON_VARStr 	TASK_VALID_FindSrt;
	
	struct TASKDataStr	CONSrt;
	
	JSONSrt.JSON_Buff_Ptr 	= AT_TCP.AT_TCPReBuff;
	JSONSrt.JSON_Buff_Size 	= AT_TCP.AT_TCPReDataNum;
	JSONSrt.offset 			= 0;

	TASK_SWITCH_FindSrt.FindMode	= JSON_F_ALL;
	TASK_NO_FindSrt.FindMode		= JSON_F_ALL;
	TASK_TIME_FindSrt.FindMode		= JSON_F_ALL;
	TASK_STATUS_FindSrt.FindMode	= JSON_F_ALL;
	TASK_RECYCLE_FindSrt.FindMode	= JSON_F_ALL;
	TASK_VALID_FindSrt.FindMode		= JSON_F_ALL;
	
	JSON_F_String( "TASK_SWITCH" 	, &JSONSrt , &TASK_SWITCH_FindSrt 	);
	JSON_F_String( "TASK_NO" 		, &JSONSrt , &TASK_NO_FindSrt 		);
	JSON_F_String( "TASK_TIME" 		, &JSONSrt , &TASK_TIME_FindSrt 	);
	JSON_F_String( "TASK_STATUS" 	, &JSONSrt , &TASK_STATUS_FindSrt 	);
	JSON_F_String( "TASK_RECYCLE" 	, &JSONSrt , &TASK_RECYCLE_FindSrt 	);
	JSON_F_String( "TASK_VALID" 	, &JSONSrt , &TASK_VALID_FindSrt 	);

	
	if( JSONSrt.FindState == JSON_Ana_OK )
	{
		W25QXX_Erase_Sector( 0x010 );
		W25QXX_Erase_Sector( 0x011 );
		W25QXX_Erase_Sector( 0x012 );
		W25QXX_Erase_Sector( 0x013 );
		W25QXX_Erase_Sector( 0x014 );
		W25QXX_Erase_Sector( 0x015 );
		W25QXX_Erase_Sector( 0x016 );
	
		for( Count_i = 0 ; Count_i < TASK_VALID_FindSrt.FindNum ; Count_i ++ )
		{
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TASK_SWITCH_FindSrt );
			CONSrt.Switch = SupCOM_StringToNum( DataBuff , 0 );

			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TASK_NO_FindSrt );
			CONSrt.NumData = SupCOM_StringToNum( DataBuff , 0 );
			
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TASK_TIME_FindSrt );
			BuffToTimeStr( DataBuff , &CONSrt.Time_Task );
					
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TASK_STATUS_FindSrt );
			CONSrt.Status = SupCOM_StringToNum( DataBuff , 0 );

			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TASK_RECYCLE_FindSrt );
			BuffToWeekBuff( DataBuff , CONSrt.WeekBuff );
				
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TASK_VALID_FindSrt );
			CONSrt.Valid = SupCOM_StringToNum( DataBuff , 0 );
			
			SupCOM_Printf_Num( Count_i , SeverAPP_PringLive - 2 );
			SupCOMPrintChar_Level('.', SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level("Switch :", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Switch , SeverAPP_PringLive - 2 );
			SupCOMPrintString_Level(" ", SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level(" NumData :", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.NumData , SeverAPP_PringLive - 2 );
			SupCOMPrintString_Level(" ", SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level(" Time :", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Time_Task.Hour , SeverAPP_PringLive - 2 );
			SupCOMPrintString_Level(":", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Time_Task.Minute , SeverAPP_PringLive - 2 );
			SupCOMPrintString_Level(" ", SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level(" Status :", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Status , SeverAPP_PringLive - 2 );
			SupCOMPrintString_Level(" ", SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level(" Valid :", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Valid , SeverAPP_PringLive - 2 );
			SupCOMPrintString_Level(" ", SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level("\r\n", SeverAPP_PringLive - 2 );

			for( Count_j = 0 ; Count_j < 7 ; Count_j ++ )
			{
				FlashAddr = 0x010 + (u32)Count_j ;
				FlashAddr = FlashAddr << 12 ;
				
				if( CONSrt.WeekBuff[ Count_j ] == 1 )
				{
					FlashAddr = FlashAddr + ( (u32)CONSrt.Time_Task.Hour * 5 );
					
					DataBuff[0] = 0x1C ;
					DataBuff[1] = CONSrt.Time_Task.Minute ;
					DataBuff[2] = CONSrt.Switch ;
					DataBuff[3] = CONSrt.Status ;
					DataBuff[4] = CONSrt.Valid ;
			
					W25QXX_Write_Page( DataBuff , FlashAddr , 5 );
					
					SupCOMPrintString_Level( "ADDR-0x" , SeverAPP_PringLive );
					SupCOM_PrintfHex32( FlashAddr , SeverAPP_PringLive );
					
					SupCOMPrintString_Level( " Write 5 Byte " , SeverAPP_PringLive );

					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );SupCOM_Printf_HEX( DataBuff[0] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );SupCOM_Printf_HEX( DataBuff[1] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );SupCOM_Printf_HEX( DataBuff[2] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );SupCOM_Printf_HEX( DataBuff[3] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );SupCOM_Printf_HEX( DataBuff[4] ,SeverAPP_PringLive );

					SupCOMPrintString_Level( "\r\n" , SeverAPP_PringLive );
				}
			}

		}

	}
}


void SeverRead_Temp(void)
{
	u8 Count_i = 0 , Count_j = 0 ;

	u8 DataBuff[32];

	u32 FlashAddr = 0;

	struct JSON_FindStr	JSONSrt;
	
	struct JSON_VARStr 	TEMP_UP_FindSrt;
	struct JSON_VARStr 	TEMP_LOW_FindSrt;
	struct JSON_VARStr 	TEMP_CONTROLL_FindSrt;
	struct JSON_VARStr 	TIME_F_FindSrt;
	struct JSON_VARStr 	TIME_T_FindSrt;

	struct TempDataStr	CONSrt;

//	u8 ContFlag = 0;
	
	JSONSrt.JSON_Buff_Ptr 	= AT_TCP.AT_TCPReBuff;
	JSONSrt.JSON_Buff_Size 	= AT_TCP.AT_TCPReDataNum;
	JSONSrt.offset 			= 0;

	TEMP_UP_FindSrt.FindMode 		= JSON_F_ALL;
	TEMP_LOW_FindSrt.FindMode 		= JSON_F_ALL;
	TEMP_CONTROLL_FindSrt.FindMode 	= JSON_F_ALL;
	TIME_F_FindSrt.FindMode 		= JSON_F_ALL;
	TIME_T_FindSrt.FindMode 		= JSON_F_ALL;

	JSON_F_String( "TEMP_UP" 		, &JSONSrt , &TEMP_UP_FindSrt );
	JSON_F_String( "TEMP_LOW" 		, &JSONSrt , &TEMP_LOW_FindSrt );
	JSON_F_String( "TEMP_CONTROLL" 	, &JSONSrt , &TEMP_CONTROLL_FindSrt );
	JSON_F_String( "TIME_F" 		, &JSONSrt , &TIME_F_FindSrt );
	JSON_F_String( "TIME_T" 		, &JSONSrt , &TIME_T_FindSrt );

	if( JSONSrt.FindState == JSON_Ana_OK )
	{

		W25QXX_Erase_Sector( 0x020 );
		
		for( Count_i = 0 ; Count_i < TEMP_UP_FindSrt.FindNum ; Count_i ++ )
		{
			
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TEMP_UP_FindSrt );
			BuffToTEMP( DataBuff , &CONSrt.Temp_u );

			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TEMP_LOW_FindSrt );
			BuffToTEMP( DataBuff , &CONSrt.Temp_l );

			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TIME_F_FindSrt );
			BuffToTimeStr( DataBuff , &CONSrt.Time_f );

			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TIME_T_FindSrt );
			BuffToTimeStr( DataBuff , &CONSrt.Time_t );

			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TIME_T_FindSrt );
			BuffToTimeStr( DataBuff , &CONSrt.Time_t );
			
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TEMP_CONTROLL_FindSrt );
			CONSrt.ContFlag = SupCOM_StringToNum( DataBuff , 0 );

//			TxStringData("FindNum : ");
			SupCOM_Printf_Num( Count_i , SeverAPP_PringLive - 2 );
			SupCOMPrintChar_Level('.', SeverAPP_PringLive - 2 );
			
			SupCOMPrintString_Level("Temp_Up:", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Temp_u.TempInt , SeverAPP_PringLive - 2 );
			
			SupCOMPrintString_Level("  Temp_Low:", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Temp_l.TempInt , SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level("  Time_F:", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Time_f.Hour , SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level("  Time_T:", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Time_t.Hour , SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level("  ContFlag:", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.ContFlag , SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level("\r\n", SeverAPP_PringLive - 2 );

			for( Count_j = 0 ; Count_j < 24 ; Count_j ++ )
			{
				FlashAddr = 0x020000 +  ((u32)Count_j * 4);
//				FlashAddr = FlashAddr << 12 ;

				if(( Count_j >= CONSrt.Time_f.Hour )&&( Count_j <= CONSrt.Time_t.Hour ))
				{
					DataBuff[0] = 0x10;
					DataBuff[1] = CONSrt.Temp_u.TempInt ;
					DataBuff[2] = CONSrt.Temp_l.TempInt ; 
					DataBuff[3] = CONSrt.ContFlag;
					
					W25QXX_Write_Page( DataBuff , FlashAddr , 4 );

					SupCOMPrintString_Level( "ADDR-0x" , SeverAPP_PringLive );
					SupCOM_PrintfHex32( FlashAddr , SeverAPP_PringLive );
					
					SupCOMPrintString_Level( " Write 4 Byte " , SeverAPP_PringLive );

					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );SupCOM_Printf_HEX( DataBuff[0] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );SupCOM_Printf_HEX( DataBuff[1] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );SupCOM_Printf_HEX( DataBuff[2] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );SupCOM_Printf_HEX( DataBuff[3] ,SeverAPP_PringLive );

					SupCOMPrintString_Level( "\r\n" , SeverAPP_PringLive );
					
				}
				
			}
			
		}
		
	}
	
}

void SeverRead_AI_Temp(void)
{
	u8 Count_i = 0 , Count_j = 0 ;
	
	u8 DataBuff[32];
	
	u32 FlashAddr = 0;
	
	struct JSON_FindStr JSONSrt;
		
	struct JSON_VARStr	TEMP_UP_FindSrt;
	struct JSON_VARStr	TEMP_LOW_FindSrt;
	struct JSON_VARStr	TEMP_CONTROLL_FindSrt;
	struct JSON_VARStr	TIME_F_FindSrt;
	struct JSON_VARStr	TIME_T_FindSrt;
	
	struct TempDataStr	CONSrt;
	
//	u8 ContFlag = 0;
		
	JSONSrt.JSON_Buff_Ptr	= AT_TCP.AT_TCPReBuff;
	JSONSrt.JSON_Buff_Size	= AT_TCP.AT_TCPReDataNum;
	JSONSrt.offset			= 0;
	
	TEMP_UP_FindSrt.FindMode		= JSON_F_ALL;
	TEMP_LOW_FindSrt.FindMode		= JSON_F_ALL;
	TEMP_CONTROLL_FindSrt.FindMode	= JSON_F_ALL;
	TIME_F_FindSrt.FindMode 		= JSON_F_ALL;
	TIME_T_FindSrt.FindMode 		= JSON_F_ALL;
	
	JSON_F_String( "TEMP_UP"		, &JSONSrt , &TEMP_UP_FindSrt );
	JSON_F_String( "TEMP_LOW"		, &JSONSrt , &TEMP_LOW_FindSrt );
	JSON_F_String( "TEMP_CONTROLL"	, &JSONSrt , &TEMP_CONTROLL_FindSrt );
	JSON_F_String( "TIME_F" 		, &JSONSrt , &TIME_F_FindSrt );
	JSON_F_String( "TIME_T" 		, &JSONSrt , &TIME_T_FindSrt );
	
	if( JSONSrt.FindState == JSON_Ana_OK )
	{
		
		W25QXX_Erase_Sector( 0x021 );
			
		for( Count_i = 0 ; Count_i < TEMP_UP_FindSrt.FindNum ; Count_i ++ )
		{
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TEMP_UP_FindSrt );
			BuffToTEMP( DataBuff , &CONSrt.Temp_u );
	
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TEMP_LOW_FindSrt );
			BuffToTEMP( DataBuff , &CONSrt.Temp_l );
	
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TIME_F_FindSrt );
			BuffToTimeStr( DataBuff , &CONSrt.Time_f );
	
			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TIME_T_FindSrt );
			BuffToTimeStr( DataBuff , &CONSrt.Time_t );

			JSON_ReadVal( Count_i , DataBuff , &JSONSrt , &TEMP_CONTROLL_FindSrt );
			CONSrt.ContFlag = SupCOM_StringToNum( DataBuff , 0 );
	
	//			TxStringData("FindNum : ");
			SupCOM_Printf_Num( Count_i , SeverAPP_PringLive - 2 );
			SupCOMPrintChar_Level('.', SeverAPP_PringLive - 2 );
				
			SupCOMPrintString_Level("Temp_Up:", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Temp_u.TempInt , SeverAPP_PringLive - 2 );
				
			SupCOMPrintString_Level("	Temp_Low:", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Temp_l.TempInt , SeverAPP_PringLive - 2 );
	
			SupCOMPrintString_Level("	Time_F:", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Time_f.Hour , SeverAPP_PringLive - 2 );
			SupCOMPrintChar_Level(':', SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Time_f.Minute , SeverAPP_PringLive - 2 );
			
			SupCOMPrintString_Level("	Time_T:", SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Time_t.Hour , SeverAPP_PringLive - 2 );
			SupCOMPrintChar_Level(':', SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.Time_f.Minute , SeverAPP_PringLive - 2 );

			SupCOMPrintString_Level("  ContFlag:" , SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( CONSrt.ContFlag , SeverAPP_PringLive - 2 );
			
			SupCOMPrintString_Level("\r\n" , SeverAPP_PringLive - 2 );
			
			
			for( Count_j = 0 ; Count_j < 24 ; Count_j ++ )
			{
				FlashAddr = 0x021000 +  ((u32)Count_j * 4);
			
				if(( Count_j >= CONSrt.Time_f.Hour )&&( Count_j <= CONSrt.Time_t.Hour ))
				{
					DataBuff[0] = 0x1A;
					DataBuff[1] = CONSrt.Temp_u.TempInt ;
					DataBuff[2] = CONSrt.Temp_l.TempInt ; 
					DataBuff[3] = CONSrt.ContFlag;
						
					W25QXX_Write_Page( DataBuff , FlashAddr , 4 );
			
					SupCOMPrintString_Level( "ADDR-0x" , SeverAPP_PringLive );
					SupCOM_PrintfHex32( FlashAddr , SeverAPP_PringLive );
					
					SupCOMPrintString_Level( " Write 4 Byte " , SeverAPP_PringLive );
					
					SupCOMPrintString_Level( "0x" , SeverAPP_PringLive );	SupCOM_Printf_HEX( DataBuff[0] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );	SupCOM_Printf_HEX( DataBuff[1] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );	SupCOM_Printf_HEX( DataBuff[2] ,SeverAPP_PringLive );
					SupCOMPrintString_Level( " 0x" , SeverAPP_PringLive );	SupCOM_Printf_HEX( DataBuff[3] ,SeverAPP_PringLive );
					
					SupCOMPrintString_Level( "\r\n" , SeverAPP_PringLive );
				}
			}	
		}
	
	}

}

void SeverGetUpDataFlag(void)
{	
	u8 Res = JSON_Ana_NULL; 

	Colrt_Prm.UpDataFlag = 0;

	Res = JSON_ResolveNUm( AT_TCP.AT_TCPReBuff , "DC_FLAG" , (u32*)&Colrt_Prm.DC_Flag );
	if( Res == JSON_Ana_ERROR )
	Colrt_Prm.DC_Flag = 0 ;
	else
	{
		if( Colrt_Prm.DC_Flag != 2 )
		{
			OperaPar.DC =	Colrt_Prm.DC_Flag;
				
			SupCOMPrintString_Level( "| DC_Flag 	 |	" , 0 );
			SupCOM_Printf_Num( OperaPar.DC , 0 );
			SupCOMPrintString_Level( "	|\r\n" , 0 );
		}
	
	}
	
	Res = JSON_ResolveNUm( AT_TCP.AT_TCPReBuff , "AI_FLAG" , (u32*)&Colrt_Prm.AI_Flag );
	if( Res == JSON_Ana_ERROR )
	Colrt_Prm.AI_Flag = 0 ;
	else
	{
		if( Colrt_Prm.AI_Flag != 2 )
		{
			if(( OperaPar.AI == 1 )&&( Colrt_Prm.AI_Flag == 0 ))
			{
				OFF_KLight();
			}

			OperaPar.AI = Colrt_Prm.AI_Flag;
			
			SupCOMPrintString_Level( "| AI_Flag 	 |	" , SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( OperaPar.AI , SeverAPP_PringLive - 2);
			SupCOMPrintString_Level( "	|\r\n" , SeverAPP_PringLive - 2 );
		}
	}
		
	Res = JSON_ResolveNUm( AT_TCP.AT_TCPReBuff , "TASK_FLAG" , (u32*)&Colrt_Prm.TASK_Flag );
	if( Res == JSON_Ana_ERROR )
	Colrt_Prm.TASK_Flag = 0 ;
	else
	{
		if( Colrt_Prm.TASK_Flag == 1 )
		{
			SupCOMPrintString_Level( "| TASK_Flag    |  " , SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( Colrt_Prm.TASK_Flag , SeverAPP_PringLive - 2 );
			SupCOMPrintString_Level( "  |\r\n" , SeverAPP_PringLive - 2 );
		}
	}
		
	Res = JSON_ResolveNUm( AT_TCP.AT_TCPReBuff , "TEMP_FLAG" , (u32*)&Colrt_Prm.TEMP_Flag );
	if( Res == JSON_Ana_ERROR )
	Colrt_Prm.TEMP_Flag = 0 ;
	else
	{
		if(  Colrt_Prm.TEMP_Flag == 1 )
		{
			SupCOMPrintString_Level( "| TEMP_Flag    |  " , SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( Colrt_Prm.TEMP_Flag , SeverAPP_PringLive - 2);
			SupCOMPrintString_Level( "  |\r\n" , SeverAPP_PringLive - 2 );
		}
	}
		
	Res = JSON_ResolveNUm( AT_TCP.AT_TCPReBuff , "TEMP_AI_FLAG" , (u32*)&Colrt_Prm.TEMP_AI_Flag );
	if( Res == JSON_Ana_ERROR )
	Colrt_Prm.TEMP_AI_Flag = 0 ;
	else
	{
		if( Colrt_Prm.TEMP_AI_Flag == 1 )
		{
			SupCOMPrintString_Level( "| TEMP_AI_Flag |  " , SeverAPP_PringLive - 2 );
			SupCOM_Printf_Num( Colrt_Prm.TEMP_AI_Flag , SeverAPP_PringLive - 2);
			SupCOMPrintString_Level( "  |\r\n" , SeverAPP_PringLive - 2 );
		}
	}
	
	Res = JSON_ResolveNUm( AT_TCP.AT_TCPReBuff , "VERSION_FLAG" , (u32*)&Colrt_Prm.UpDataFlag );
	if( Res == JSON_Ana_ERROR )
	Colrt_Prm.UpDataFlag = 0 ;
	else
	{
		if( Colrt_Prm.UpDataFlag == 1 )
		{
			SupCOMPrintString_Level( "| UpDataFlag   |  " , 0 );
			SupCOM_Printf_Num( Colrt_Prm.UpDataFlag , 0);
			SupCOMPrintString_Level( "  |\r\n" , 0 );
		}
	}

}


void DisHexData( u8 *DataBuff , u16 DataNum )
{
	u16 Count_i;
	
	for (Count_i = 0; Count_i < DataNum; ++Count_i)
	{
		if( Count_i % 16 == 0 )
		{
			TxStringData("\r\n");
			TxStringData("0x");
			Printf_HEX32(Count_i&0x0000ffff);
			TxStringData(": ");
		}
		TxBitData(' ');
		Printf_HEX( *( DataBuff + Count_i ) );
	}
	TxStringData("\r\n");
}

void CheckenFlashUpData( void )
{
	struct JSON_Data 	JSONBuff;
	
	u8 Res = AT_NULL ;
	
	u8 FlashDataBuff[6];
	u8 VersionBuff[16];
	
	W25QXX_Read( FlashDataBuff , 0x02f000 , 6 );
	W25QXX_Read( VersionBuff   , 0x02f006 , 16 );

	if(( FlashDataBuff[0] == 0x1A )&&( FlashDataBuff[1] == 0x03 ))
	{
		
		SupCOMPrintString_Level( "UpData Successful" , 0 );
		
		Res = AT_LinkTCP_Sever();

		if( Res == AT_OK )
		{
			LinkLEDON(5);
			
			SendUpDataFlag( VersionBuff , 1 , &JSONBuff );

			AT_SendTCPData(JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff);

			Res = AT_WaitTCPData( 100 );

			if( Res == AT_OK )
			{

				W25QXX_Erase_Sector(0x02f);

				FlashDataBuff[1] = 0x00;

				W25QXX_PageWrite( FlashDataBuff , 0x02f000 , 6	);
				W25QXX_PageWrite( VersionBuff	, 0x02f006 , 16 );

			}
			
		}

		AT_CloseSeverLink();

	}
	else if(( FlashDataBuff[0] == 0x1A )&&( FlashDataBuff[1] == 0x04 ))
	{

		SupCOMPrintString_Level( "UpData Error!" , 0 );
		
		Res = AT_LinkTCP_Sever();

		if( Res == AT_OK )
		{
			LinkLEDON(5);
			
			SendUpDataFlag( VersionBuff , 0 , &JSONBuff );

			AT_SendTCPData(JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff);

			Res = AT_WaitTCPData( 100 );
		}

		AT_CloseSeverLink();
	}
}

void SeverUpDataFun(void)
{
	struct JSON_Data 	JSONBuff;

	u8 Res = AT_NULL ;
	u32 FileSize = 0 , ReDataSize = 0;
	u32 Count_i = 0 , FilePackNum = 0;

	u8 VressionBuff[16];
	u8 FlashDataBuff[8];

	u32 Address = 0;
	u16 PackDataNum;

	SeverPringTime( ModeHOUR , &SysTime_Str );

	SupCOM_ChanreColor(SupCOM_Color_YB);
	SupCOMPrintString_Level("Sever Get ", 0 );
	SupCOMPrintString_Level("SEND_VERSION", 0 );
	SupCOMPrintString_Level("\r\n", 0 );
	SupCOM_ChanreColor(SupCOM_Color_WB);

	for( Count_i = 0 ; Count_i < 33 ; Count_i ++ )
	W25QXX_Erase_Sector( 0x02F + Count_i );
	
	Res = AT_LinkTCP_Sever();

	if( Res == AT_OK )
	{
		LinkLEDON(5);
		
		SendRequestJson( "SEND_VERSION", &JSONBuff);
		
		AT_SendTCPData(JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff);
		
		AT_TCP_SetReDataMode();

		AT_TCP.AT_TCPReDataNum = 0;
		
		Res = AT_WaitTCPData( 100 );
		
		if( Res == AT_OK )
		{
			LinkLEDON(5);

			Res = JSON_ResolveNUm( AT_TCP.AT_TCPReBuff , "FILESIZE" , (u32*)&FileSize );
			if( Res == JSON_Ana_ERROR )
			FileSize = 0 ;
			else
			{
					SupCOMPrintString_Level( "UpData FlieSize : " 	, 0 );
					SupCOM_Printf_Num( 		FileSize 				, 0);
					SupCOMPrintString_Level( "\r\n" 				, 0 );
			}

			
			Res = JSON_Resolve( AT_TCP.AT_TCPReBuff , "VERSION_NO" , VressionBuff );
			if( Res == JSON_Ana_ERROR )
			FileSize = 0 ;
			else
			{
					SupCOMPrintString_Level( "Version : " 	, 0 );
					SupCOMPrintString_Level( VressionBuff 	, 0 );
					SupCOMPrintString_Level( "\r\n" 		, 0 );
			}

			AT_TCP.AT_TCPReDataNum = 0;

			FilePackNum = ( FileSize / 1024 ) + 1 ; 

			SupCOMPrintString_Level( "FilePackNum : "	, 0 );
			SupCOM_Printf_Num(		FilePackNum			, 0);
			SupCOMPrintString_Level( "\r\n" 			, 0 );

			ReDataSize = 0;

			Address = 0x030000;

			for( Count_i = 0 ; Count_i < FilePackNum ; Count_i ++ )
			{
				SendReUpData( Count_i + 1 , VressionBuff , &JSONBuff );

				Res = AT_SendTCPData(JSONBuff.JSON_DataNum ,JSONBuff.JSON_DataBuff);

				AT_TCP_SetReDataMode();
				
				AT_TCP.AT_TCPReDataNum = 0;
				
				Res = AT_WaitTCPData( 100 );
//				
				if( Res == AT_OK )
				{

					PackDataNum = AT_TCP.AT_TCPReDataNum;

					ReDataSize += PackDataNum;
					
					AT_TCP.AT_TCPReDataNum = 0;

					SupCOMPrintString_Level( "Now FlieSize : "	, 0 );
					SupCOM_Printf_Num(		ReDataSize				, 0);
					SupCOMPrintString_Level( "\r\n" 				, 0 );

					SupCOMPrintString_Level("Write Flash Addr :",0);
					SupCOM_PrintfHex32( Address , 0 );
					SupCOMPrintString_Level("\r\n",0);
					
					W25QXX_PageWrite( AT_TCP.AT_TCPReBuff , Address , PackDataNum );

					Address += PackDataNum;
				}
				else
				{
					Count_i --;
				}
			}

			if( ReDataSize == FileSize )
			{
				SupCOMPrintString_Level( "UpData ReData Successful"	, 0 );
				
				FlashDataBuff[0] = 0x1A;
				FlashDataBuff[1] = 0x02;
				FlashDataBuff[2] = FileSize >> 24;
				FlashDataBuff[3] = FileSize >> 16;
				FlashDataBuff[4] = FileSize >> 8 ;
				FlashDataBuff[5] = FileSize;

				W25QXX_PageWrite( FlashDataBuff , 0x02f000 , 6 	);
				W25QXX_PageWrite( VressionBuff 	, 0x02f006 , 16 );

				SupCOMPrintString_Level( "Restart device Now... " , 0);
				
				__set_FAULTMASK(1); 
				NVIC_SystemReset();

				while(1);
			}
		}
	}
	AT_CloseSeverLink();
}

void BuffToTimeStr( u8 *DataBuff , struct TimeStr *Time )
{
	u8 Count_i = 0;

	u8 NumData = 0;
	
	while( *(DataBuff + Count_i ) != '\0' )
	{
		if(( *(DataBuff + Count_i ) >= '0' )&&( *(DataBuff + Count_i ) <= '9' ))
		{
			NumData = NumData * 10; 
			NumData = NumData + *(DataBuff + Count_i ) - 0x30 ;
		}
		
		if( *(DataBuff + Count_i ) == ':' )
		{
			Time->Hour = NumData;
			NumData = 0;
		}
		Count_i ++;
	}

	Time->Minute = NumData;
}

void BuffToTEMP( u8 *DataBuff , struct TempStr *Temp )
{
	u8 Count_i = 0;

	u8 NumData = 0;
	
	while( *(DataBuff + Count_i ) != '\0' )
	{
		if(( *(DataBuff + Count_i ) >= '0' )&&( *(DataBuff + Count_i ) <= '9' ))
		{
			NumData = NumData * 10; 
			NumData = NumData + *(DataBuff + Count_i ) - 0x30 ;
		}
		
//		if( *(DataBuff + Count_i ) == '.' )
//		{
//			Temp->TempInt = NumData;
//			NumData = 0;
//		}
		Count_i ++;
	}

	Temp->TempInt = NumData;
}

void BuffToWeekBuff( u8 *DataBuff , u8 *WeekBuff )
{
	u8 Count_i = 0;
	u8 NumData = 0;

	for( Count_i = 0 ; Count_i < 7 ; Count_i ++ )
	*( WeekBuff + Count_i ) = 0;

	Count_i = 0;

	while( *(DataBuff + Count_i ) != '\0' )
	{
		if(( *(DataBuff + Count_i ) >= '0' )&&( *(DataBuff + Count_i ) <= '9' ))
		{
			NumData =  *(DataBuff + Count_i ) - 0x30 ;
			*( WeekBuff + NumData - 1 ) = 1;
		}

		Count_i ++;
	}
}



