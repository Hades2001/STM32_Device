#include "ESP_AT.h"
#include "stddef.h"
#include "W25QXX.h"
//#include "LED.h"

#define WIFIBEBUGLEVEL			3

#define PrintWifiDebug(...)		SupCOMPrintString_Level("WIFI_DB : ",WIFIBEBUGLEVEL);	\
								SupCOMPrintString_Level(__VA_ARGS__,WIFIBEBUGLEVEL);\
								SupCOMPrintString_Level(" \r\n",WIFIBEBUGLEVEL);


struct 	AT_SETStr 	AT_Str;
struct	AT_TCPStr	AT_TCP;
struct 	ESP_Wifi	ESP_Srt;
struct  ServerSrt	Server;


u8 SATMAC[6];

u32 ASCII_Num( u8 *DataBuff , u8 DataNum );

void AT_TCP_SetReDataMode(void);
u8 AT_StringtoIP( u8 *IPStringBuff , u8 *IPBuff );

u8 AT_SendCMD( u8 *String );




void SendATChar( u8 CharData )
{
	USART2_SendBit( (char)CharData);
}

void SendATString( u8 *StringData )
{
	USART2_SendString( (char*)StringData);
}

void SendNbyte( u8 *DataBuff , u16 DataNum )
{
	u16 Count_i;

	for( Count_i = 0; Count_i < DataNum ; Count_i ++)
	{
		USART2_SendBit(*(DataBuff+Count_i));
	}

}


void Send_Num(u32 Num_Data)
{
	char Num_Buf[10]={0,0,0,0,0,0,0,0,0,0};
	unsigned char i=0,n=0;

	while(Num_Data/10!=0)
	{     
			Num_Buf[9-i]=Num_Data%10;
			Num_Data/=10;
			i++;
	}
	Num_Buf[9-i]=Num_Data%10;
	i++;
	for(n=0;n<i;n++)
	{
		USART2_SendBit(Num_Buf[10-i+n]+0x30);
	}
}

void SendAT( u8 *AT_String )
{
	SendATString(AT_String);
	SendATString("\r\n");
}

void Flush_Buff(void)
{
	u8 Count_i,Count_j;

	for( Count_i = 0; Count_i < 16 ;Count_i ++)
	for( Count_j = 0; Count_j < 64 ;Count_j ++)
	AT_Str.AT_ReDataBuff[Count_i][Count_j] = '\0';

}

void SendBuffData(void)
{
	u8 Count_i,Count_j = 0;

	for( Count_i = 0; Count_i < AT_Str.AT_DataNum ; Count_i ++)
	{
		Count_j = 0;

		while( AT_Str.AT_ReDataBuff[Count_i][Count_j] != '\0' )
		{
			TxBitData(AT_Str.AT_ReDataBuff[Count_i][Count_j]);
			Count_j ++;
		}

		TxBitData( '\r' );
		TxBitData( '\n' );
	}

}

void AT_CopyCMDBuff( u8 *objectiveBuff, u8 objectiveoffset, u8 Member ,u8 Sourceoffset,u8 Length )
{
	u8 Count = 0;

	if( Length == 0 )
	{
		while( AT_Str.AT_ReDataBuff[Member][ Sourceoffset + Count ] != '\0' )
		{
			*(objectiveBuff + objectiveoffset + Count) = AT_Str.AT_ReDataBuff[Member][ Sourceoffset + Count ];
			Count++;
		}
		*( objectiveBuff + objectiveoffset + Count ) = '\0';
	}
	else
	{
		for( Count = 0; Count < Length ; Count ++ )
		{
			*(objectiveBuff + objectiveoffset + Count) = AT_Str.AT_ReDataBuff[Member][ Sourceoffset + Count ];
		}
	}
}

void ESP_ReadIPPort(void)
{
	u8 DataBuff[5];
	
	W25QXX_Read( Server.IP , 0x003000 , 4 );
	W25QXX_Read( DataBuff , 0x003004 , 5 );

	Server.Port = 0;

	Server.Port |= DataBuff[0];
	Server.Port <<= 8;
	Server.Port |= DataBuff[1];
	Server.Port <<= 8;
	Server.Port |= DataBuff[2];
	Server.Port <<= 8;
	Server.Port |= DataBuff[3];

	Server.State = DataBuff[4] ;

}

void ESP_WriteServerData( void )
{
	u8 DataBuff[5];

	DataBuff[0] = Server.Port >> 24 ;
	DataBuff[1] = Server.Port >> 16 ;
	DataBuff[2] = Server.Port >> 8 ;
	DataBuff[3] = Server.Port  ;

	DataBuff[4] = Server.State  ;

	W25QXX_Erase_Sector(0x003);

	W25QXX_Write_Page( Server.IP , 0x003000 , 4 );
	W25QXX_Write_Page( DataBuff , 0x003004 , 5 );
}

void ESP_Init(void)
{
	u8 Count_i = 0;

	AT_TCP.TCP_State		=	TCP_Null;
	AT_TCP.TCP_LinkState	=	TCP_LINK_NULL;

	PrintWifiDebug("AT+CIFSR");
	AT_Send_ReCall("AT+CIFSR",AT_GetSATAMAC,NULL,NULL);

	PrintWifiDebug("AT+CIPSTATUS");
	AT_Send_ReCall("AT+CIPSTATUS",AT_GETNetSta,NULL,NULL);

	if( ESP_Srt.ESP_AP_State == 5 )
	{
		while( ESP_Srt.ESP_AP_State == 5  )
		{
			DelayS( 1 );
			AT_Send_ReCall("AT+CIPSTATUS",AT_GETNetSta,NULL,NULL);
			Count_i ++;
		
			SupCOM_Printf_Num( Count_i ,0 );
			SupCOMPrintChar_Level( '.' ,0);
		
			if( Count_i >= 10 )
			{
				break;
			}
		}
		
		if( Count_i < 10 )
		{
			ESP_Srt.ESP_State = ESP_AP_LINK;
			SupCOMPrintString_Level( "GET AP OK " , 0);
			
			SupCOMPrintString_Level( "Restart device Now... " , 0);
		
			__set_FAULTMASK(1); 
			NVIC_SystemReset();
		}
		else
		{
			ESP_Srt.ESP_State = ESP_AP_UNUNITED;
			
			SupCOM_ChanreColor(SupCOM_Color_RB);
			SupCOMPrintString_Level( "GET AP Time OUT " , 0);
		}
	}
	else
	{
		ESP_Srt.ESP_State = ESP_AP_LINK;
		
		PrintWifiDebug("AT+CWJAP_DEF?");
		AT_Send_ReCall("AT+CWJAP_DEF?",AT_APName,NULL,NULL);
	}
	
	
	AT_SendCMD("AT+CIPMUX=0");

	ESP_ReadIPPort();

	AT_TCP_SetReDataMode();

}

void AT_Query_connection(void)
{
	u8 Res;

	SupCOMPrintString_Level("Check the AP connection. " ,0 );
	AT_Send_ReCall("AT+CIPSTATUS",AT_GETNetSta,NULL,NULL);

	if( ESP_Srt.ESP_AP_State != 5 )
	{
		ESP_Srt.ESP_State = ESP_AP_LINK;
		SupCOMPrintString_Level( "GET AP OK \r\n" , 0);
		
		SupCOMPrintString_Level( "Check the TCP connection. " , 0);
		Res = AT_LinkTCP_Sever();

		if( Res == AT_OK )
		{
			AT_TCP.TCP_LinkState	= 	TCP_LINK_OK;
			SupCOMPrintString_Level( "Link TCP OK \r\n" , 0);
			AT_SendCMD( "AT+CIPCLOSE" );
		}
		else
		{
			AT_TCP.TCP_LinkState	= 	TCP_LINK_ERROR;
			SupCOMPrintString_Level( "Link TCP Error \r\n" , 0);
		}
	}
	else
	{
		ESP_Srt.ESP_State = ESP_AP_UNUNITED;
		SupCOMPrintString_Level( "GET AP Time OUT \r\n" , 0);
	}

}

void AT_Test(void)
{
	AT_Str.AT_Start = AT_ATARE_A;
	SendAT("AT");
}


u8 AT_SendCMD( u8 *String )
{
	u8 ReState = 0;

	AT_Str.AT_Start = AT_ATARE_A;
	AT_Str.AT_ReFlag = AT_NULL;
	AT_Str.TimeOUT_Count = 0;

	Flush_Buff();
	
	SendAT(String);
	
	while(( AT_Str.AT_ReFlag == AT_NULL )&&( AT_Str.TimeOUT_Count < AT_TimeOUT));
	{
		if( AT_Str.TimeOUT_Count <= AT_TimeOUT )
		{
			if( AT_Str.AT_ReFlag == AT_OK )
			{
				PrintWifiDebug("AT_OK");
				ReState = AT_OK;
			}
			else if( AT_Str.AT_ReFlag == AT_ERROR )
			{
				PrintWifiDebug("AT_ERROR");	
				ReState = AT_ERROR;
			}
		}
		else
		{
			PrintWifiDebug("Time Out");	
			ReState = AT_TOut;
		}
	}
	
	return ReState;
}

u8 AT_Send_ReCall( u8 *String , void (*AT_OK_FUN)() , void (*AT_ERROR_FUN)() , void(*AT_TIME_OUT_FUN)())
{
	AT_Str.AT_Start = AT_ATARE_A;
	AT_Str.AT_ReFlag = AT_NULL;
	AT_Str.TimeOUT_Count = 0;

	Flush_Buff();
	
	SendAT(String);
	while(( AT_Str.AT_ReFlag == AT_NULL )&&( AT_Str.TimeOUT_Count < AT_TimeOUT));
	{
		if( AT_Str.TimeOUT_Count <= AT_TimeOUT )
		{
			if( AT_Str.AT_ReFlag == AT_OK )
			{
				PrintWifiDebug("AT_OK");	
				(*AT_OK_FUN)();
				return AT_OK;

			}
			else if( AT_Str.AT_ReFlag == AT_ERROR )
			{
				PrintWifiDebug("AT_ERROR");	
				(*AT_ERROR_FUN)();			
				return AT_ERROR;
			}
		}
		else
		{
			PrintWifiDebug("Time Out");	
			(*AT_TIME_OUT_FUN)();			
			return AT_TOut;
		}
	}
	return AT_ERROR;
}

u8 AT_LinkTCP_Sever(void)
{
	struct StringStr ESPSend;

	u8 Res = AT_NULL;
	
	String_FushBuff( &ESPSend );
	
	CopyStringToStringDataBuff( "AT+CIPSTART=\"TCP\",\"" , &ESPSend );
	
//PrintSYSDebug("AT+CIPSTART=\"TCP\",\"120.79.92.192\",9001");

	if( Server.State == 1 )
	{
		CopyStringToStringDataBuff( ServerIP , &ESPSend );
		CopyStringToStringDataBuff( "\"," , &ESPSend );
		ADDNumToStringDataBuff( ServerPort , 0 , &ESPSend );
	}
	else
	{
		ADDNumToStringDataBuff( Server.IP[0] , 0 , &ESPSend );
		ADDCharToStringDataBuff( '.' , &ESPSend );
		ADDNumToStringDataBuff( Server.IP[1] , 0 , &ESPSend );
		ADDCharToStringDataBuff( '.' , &ESPSend );
		ADDNumToStringDataBuff( Server.IP[2] , 0 , &ESPSend );
		ADDCharToStringDataBuff( '.' , &ESPSend );
		ADDNumToStringDataBuff( Server.IP[3] , 0 , &ESPSend );
		
		CopyStringToStringDataBuff( "\"," , &ESPSend );
		ADDNumToStringDataBuff( Server.Port , 0 , &ESPSend );

	}
	
//	PrintStringDataBuff( &ESPSend );

	Res = AT_SendCMD( ESPSend.DataBuff );

	return Res;

}

u8 AT_LinkTCP_Sever_IPStr( u8 LinkType , u8 *IPString , u32 PortNum )
{
	struct StringStr ESPSend;
	
	u8 Res = AT_NULL;
		
	String_FushBuff( &ESPSend );

	if( LinkType == AT_TCP_TYPE)
	CopyStringToStringDataBuff( "AT+CIPSTART=\"TCP\",\"" , &ESPSend );
	else if( LinkType == AT_UDP_TYPE )
	CopyStringToStringDataBuff( "AT+CIPSTART=\"UDP\",\"" , &ESPSend );

	CopyStringToStringDataBuff( IPString , &ESPSend );
	CopyStringToStringDataBuff( "\"," , &ESPSend );
	ADDNumToStringDataBuff( PortNum , 0 , &ESPSend );

	Res = AT_SendCMD( ESPSend.DataBuff );
		
	return Res;

}

u8 AT_CloseSeverLink(void)
{
	u8 Res = AT_NULL;
	
	Res = AT_SendCMD( "AT+CIPCLOSE" );

	return Res;
}


u8 AT_WaitTCPData( u32 TimeOUTMs )
{
	u32 TimeOUT_Count_Main = 0;
	
	while( ( AT_GetTCPReFlag() != AT_TCP_GetReady ) && ( TimeOUT_Count_Main < TimeOUTMs ) )
	{
		DelayMs(1);
		TimeOUT_Count_Main ++;
	}

	if( TimeOUT_Count_Main >= TimeOUTMs )
	{
		
		SupCOM_ChanreColor(SupCOM_Color_RB);
		SupCOMPrintString_Level("TCP Time OUT \r\n",0);
		SupCOM_ChanreColor(SupCOM_Color_WB);
		return AT_TOut;
	}
	else
	{
		SupCOM_ChanreColor(SupCOM_Color_GB);
		SupCOMPrintString_Level("Sever response time :",0);
		SupCOM_ChanreColor(SupCOM_Color_YB);
		SupCOM_Printf_Num(TimeOUT_Count_Main,0);
		SupCOMPrintString_Level("ms \r\n",0);
		SupCOM_ChanreColor(SupCOM_Color_WB);

		
		SupCOM_ChanreColor(SupCOM_Color_YB);
		SupCOMPrintString_Level("TCP ReData Num :",3);
		SupCOM_ChanreColor(SupCOM_Color_WB);
		SupCOM_Printf_Num(AT_TCP.AT_TCPReDataNum,3);
		SupCOMPrintString_Level(" \r\n",3);

		
		SupCOM_ChanreColor(SupCOM_Color_YB);
		SupCOMPrintString_Level("TCP ReData : ",3);
		SupCOM_ChanreColor(SupCOM_Color_WB);
		SupCOMPrintString_Level(AT_TCP.AT_TCPReBuff,3);
		SupCOMPrintString_Level(" \r\n",3);

		AT_TCP_SetReDataMode();

		return AT_OK;
	}

}

void AT_DisTPCData(void)
{
	if(( AT_Str.AT_Start == AT_TCP_END )&&( AT_TCP.AT_TCP_ReFlag == AT_TCP_GetReady ))
	{
		SupCOMPrintString_Level("TCP ReData Num :",0);
		SupCOM_Printf_Num(AT_TCP.AT_TCPReDataNum,0);
		SupCOMPrintString_Level(" \r\n",0);
		
		SupCOMPrintString_Level("TCP ReData : ",0);
		SupCOMPrintString_Level(AT_TCP.AT_TCPReBuff,0);
		SupCOMPrintString_Level(" \r\n",0);
	
		AT_TCP_SetReDataMode();
	}
}

//u8 DataBuffStringCompare( u8 DataBuffNum ,u8 *StringData )
//{
//	u8 Count_i = 0 , ReFlag = 0;

//	while( *StringData != '' )
//	{
//		if( AT_Str.AT_ReDataBuff[DataBuffNum][Count_i] == *StringData )
//		{
//			ReFlag = 1;
//		}
//		else
//		{
//			ReFlag = 0;
//			break;
//		}
//		StringData ++;
//	}

//	return ReFlag;
//	
//}

u8 AT_SendTCPData(u16 DataNum ,u8 *DataBuff)
{

	AT_Str.AT_Start = AT_ATARE_A;
	AT_Str.AT_ReFlag = AT_NULL;
	AT_Str.TimeOUT_Count = 0;

	Flush_Buff();
	
	SendATString("AT+CIPSEND=");
	Send_Num(DataNum);
	SendATString("\r\n");
	
	while(( AT_Str.AT_ReFlag == AT_NULL )&&( AT_Str.TimeOUT_Count < AT_TimeOUT));
	{
		if( AT_Str.TimeOUT_Count <= AT_TimeOUT )
		{
			if( AT_Str.AT_ReFlag == AT_OK )
			{
				PrintWifiDebug("AT_OK");	
//				SendBuffData();

				AT_Str.AT_Start = AT_TCP_Send;
				AT_TCP.AT_TCPFlag = AT_TCP_NULL;

				SendNbyte(DataBuff,DataNum);
				while(( AT_TCP.AT_TCPFlag == AT_TCP_NULL )&&( AT_Str.TimeOUT_Count < AT_TimeOUT));
				if( AT_TCP.AT_TCPFlag == AT_TCP_OK )
				{
					AT_TCP_SetReDataMode();
//					PrintWifiDebug("AT_TCP_Send_OK");	
				}
				else
				{
//					PrintWifiDebug("AT_TCP_Send_ERROR");	
				}
				return 0;				
			}
			else if( AT_Str.AT_ReFlag == AT_ERROR )
			{
				PrintWifiDebug("AT_ERROR");	
				return 0;	
				
			}
		}
		else
		{
			PrintWifiDebug("Time Out");	
			return 0;
		}
	}
	return 1;
}

void AT_TCP_SetReDataMode(void)
{
	
	AT_Str.AT_Start = AT_TCP_ReData ;
	AT_TCP.AT_TCP_ReFlag = AT_TCP_NULL;
//	AT_TCP.AT_TCPReDataNum = 0;
}

u8 AT_GetAT_Start(void)
{
	return AT_Str.AT_Start;
}

u8 AT_GetTCPReFlag(void)
{
	return AT_TCP.AT_TCP_ReFlag;
}

void AT_CopyTCPBuff( u8 *Databuff , u16 DataNum )
{
	u16 Count_i;
	if( DataNum == 0)
	{
		for( Count_i = 0; Count_i < AT_TCP.AT_TCPReDataNum ;Count_i ++)
		*(Databuff + Count_i ) = AT_TCP.AT_TCPReBuff[Count_i];
	}
	else
	{
		for( Count_i = 0; Count_i < DataNum ;Count_i ++)
		*(Databuff + Count_i ) = AT_TCP.AT_TCPReBuff[Count_i];
	}
}

void AT_FUM( u8 Data )
{
	static u16 Count_Data;

//	USART1_SendBit(Data);

	switch (AT_Str.AT_Start)
	{
		case AT_ATARE_A:
		if( Data == 'A' )
		AT_Str.AT_Start = AT_ATARE_T;
		else
		AT_Str.AT_Start = AT_ATARE_A;
		break;

		case AT_ATARE_T:
		if( Data == 'T' )
		{
			AT_Str.AT_Start = AT_ATARE_CMD;
			Count_Data = 0;
		}
		else
		AT_Str.AT_Start = AT_ATARE_A;
		break;

		case AT_ATARE_CMD:
		if(( Data == '\r' )||( Data == '\n' ))
		{
			Count_Data = 0;
			AT_Str.AT_DataNum = 0;
			AT_Str.AT_Start = AT_ATARE_DATA;	
		}
		else
		{
//			USART1_SendBit(Data);
			if( Data == AT_Str.AT_CMDString[2+Count_Data] )
			{

			}
			else
			{


			}
		}
		break;

		case AT_ATARE_DATA:

		if( Data == '>' )
		{
			AT_Str.AT_Start = AT_TCP_SendReady;
		}

		if(( Data == '\r' )||( Data == '\n' ))
		{
			if( Count_Data != 0 )
			{
				Count_Data = 0;
				AT_Str.AT_DataNum ++;
			}
		}
		else
		{
			AT_Str.AT_ReDataBuff[AT_Str.AT_DataNum][Count_Data] = Data;

			if( Count_Data == 1 )
			{
				if(( AT_Str.AT_ReDataBuff[AT_Str.AT_DataNum][0] == 'O' )&&( AT_Str.AT_ReDataBuff[AT_Str.AT_DataNum][1] == 'K'  ))
				{
					AT_Str.AT_ReFlag = AT_OK;
				}
				else if(( AT_Str.AT_ReDataBuff[AT_Str.AT_DataNum][0] == 'E' )&&( AT_Str.AT_ReDataBuff[AT_Str.AT_DataNum][1] == 'R' ))
				{
					AT_Str.AT_ReFlag = AT_ERROR;
				}
			}
			Count_Data ++ ;
		}
		break;

		// TCP

		case AT_TCP_Send:
		if( Data == 'S')
		{
			AT_Str.AT_Start = AT_TCP_SendRe;
			AT_Str.AT_ReDataBuff[0][0] = 'S';
			Count_Data = 0;
		}
		break;

		case AT_TCP_SendRe:
		{
			if(( Data == '\r' )||( Data == '\n' ))
			{
				if( Count_Data != 0 )
				{
					if	(( AT_Str.AT_ReDataBuff[0][2]  == 'N' ) && \
						( AT_Str.AT_ReDataBuff[0][3]  == 'D' ))
					{
						if	(( AT_Str.AT_ReDataBuff[0][5]  == 'O' ) && \
						( AT_Str.AT_ReDataBuff[0][6]  == 'K' ))
						AT_TCP.AT_TCPFlag = AT_TCP_OK;
						else
						AT_TCP.AT_TCPFlag = AT_TCP_ERROR;	
					}
				}
			}
			else
			{
				AT_Str.AT_ReDataBuff[0][ Count_Data + 1 ] = Data;
				Count_Data ++;
			}
		}
		break;

		case AT_TCP_ReData:
		if( Data == '+' )
		AT_Str.AT_Start = AT_TCP_Re_I;
		break;

		case AT_TCP_Re_I :
		if( Data == 'I')
		AT_Str.AT_Start = AT_TCP_Re_D;
		else
		AT_Str.AT_Start = AT_TCP_ReData;
		break;

		case AT_TCP_Re_D :
		if( Data == 'P')
		AT_Str.AT_Start = AT_TCP_Re_P;
		else
		AT_Str.AT_Start = AT_TCP_ReData;
		break;

		case AT_TCP_Re_P :
		if( Data == 'D')
		AT_Str.AT_Start = AT_TCP_Re_F;
		else
		AT_Str.AT_Start = AT_TCP_ReData;
		break;

		case AT_TCP_Re_F :
		if( Data == ',')
		{
			Count_Data = 0;
			AT_TCP.AT_TCP_ReFlag = AT_TCP_Receive;
			AT_Str.AT_Start = AT_TCP_ReNum;
		}
		else
		AT_Str.AT_Start = AT_TCP_ReData;
		break;

		case AT_TCP_ReNum :
		if(  Data == ':' )
		{
			AT_TCP.AT_TCPReDataNum = ASCII_Num( AT_TCP.AT_TCPNumBuff , Count_Data );
			AT_Str.AT_Start = AT_TCP_Data;
			AT_TCP.AT_TCP_ReFlag = AT_TCP_NEmpty;
			Count_Data = 0;
		}
		else
		{
			AT_TCP.AT_TCPNumBuff[Count_Data] = Data;
			Count_Data ++;
		}
		break;

		case AT_TCP_Data:
		if( Count_Data < AT_TCP.AT_TCPReDataNum )
		{
			AT_TCP.AT_TCPReBuff[ Count_Data ] = Data;
			Count_Data ++;
			
			if( Count_Data >= AT_TCP.AT_TCPReDataNum )
			{
				AT_TCP.AT_TCPReBuff[ Count_Data ] = '\0';
				AT_Str.AT_Start = AT_TCP_END;
				AT_TCP.AT_TCP_ReFlag = AT_TCP_GetReady;
			}
		}
		break;



		/*		ESP_Touch		*/
		case AT_ESP_T_0 :
		if(( Data == '\r' )||( Data == '\n' ))
		{
			if( ESP_Srt.ESP_FUM_Count != 0 )
			{
				ESP_Srt.ESP_FUM_Count = 0;
				AT_Str.AT_DataNum ++;
			}
		}
		else
		{
			AT_Str.AT_ReDataBuff[AT_Str.AT_DataNum][ESP_Srt.ESP_FUM_Count] = Data;
			ESP_Srt.ESP_FUM_Count ++ ;
		}
	}
}

u8 AT_ASCIIHex( u8 CharData_A , u8 CharData_B )
{
	u8 ReData = 0;
	
	if( CharData_A <= '9' )
	{
		ReData = CharData_A - '0';
	}
	else if(( CharData_A > '9' )&&( CharData_A <= 'F'))
	{
		ReData = ( CharData_A - 'A' ) + 0x0A;
	}
	else if(( CharData_A > 'F' )&&( CharData_A <= 'f'))
	{
		ReData = ( CharData_A - 'a' ) + 0x0A;
	}

	ReData <<= 4;

	if( CharData_B <= '9' )
	{
		ReData |= CharData_B - '0';
	}
	else if(( CharData_B > '9' )&&( CharData_B <= 'F'))
	{
		ReData |= ( CharData_B - 'A' ) + 0x0A;
	}
	else if(( CharData_B > 'F' )&&( CharData_B <= 'f'))
	{
		ReData |= ( CharData_B - 'a' ) + 0x0A;
	}

	return ReData;
}

u32 ASCII_Num( u8 *DataBuff , u8 DataNum )
{
	static u8 	Count_i;
	static u32	ReData;

	ReData = 0;

	for( Count_i = 0 ;Count_i < DataNum ; Count_i ++ )
	{
		ReData *= 10;
		ReData += ((*(DataBuff + Count_i)) - 0x30 );
	}

	return ReData;
}

u8 AT_StringtoIP( u8 *IPStringBuff , u8 *IPBuff )
{
	
	u8 Count_i = 0 , Count_j = 0 , NumData = 0 , Res = 0 ;

	while(1)
	{

		NumData = NumData * 10;
		NumData = NumData + ( *( IPStringBuff + Count_i ) - 0x30 ); 

		Count_i++;
			
		if(*( IPStringBuff + Count_i ) == '.')
		{
			*( IPBuff + Count_j ) = NumData;
			NumData = 0;
			Count_j ++;
			Count_i	++;
		}

		if( *( IPStringBuff + Count_i ) == '\0' )
		{
			if( Count_j == 3 )
			{
				*( IPBuff + Count_j ) = NumData;
				Res = 1;
				break;
			}
			else
			{
				Res = 0;
				break;
			}
		}
	}

	return Res;
//	SupCOM_Printf_Num(*( IPBuff + 0 ),WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '.' ,WIFIBEBUGLEVEL);
//	SupCOM_Printf_Num(*( IPBuff + 1 ),WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '.' ,WIFIBEBUGLEVEL);
//	SupCOM_Printf_Num(*( IPBuff + 2 ),WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '.' ,WIFIBEBUGLEVEL);
//	SupCOM_Printf_Num(*( IPBuff + 3 ),WIFIBEBUGLEVEL);
//	SupCOMPrintString_Level("\r\n",WIFIBEBUGLEVEL);

}

u8 AT_FindStringFromBuff( u8 *stringBuff , struct AT_SETStr *p , u8 *FindResBuff )
{
	u8 Count_i = 0 , Count_j = 0 , Count_a = 0 ;

	while( Count_i <  p->AT_DataNum )
	{
		while( p->AT_ReDataBuff[ Count_i ][ Count_j ] != '\0' )
		{
			if( p->AT_ReDataBuff[ Count_i ][ Count_j ] == *( stringBuff + Count_a ) )
			Count_a ++;
			else
			Count_a = 0; 

			if( *( stringBuff + Count_a ) == '\0' )
			{
//				SupCOMPrintString_Level("Find ",0);
//				SupCOMPrintString_Level(stringBuff,0);
//				SupCOMPrintString_Level(" OK \r\n",0);
				*( FindResBuff + 0 ) = Count_i;
				*( FindResBuff + 1 ) = Count_j;
				return 1;
			}

			Count_j ++;
		}
		Count_i ++;
		Count_a = 0;
		Count_j = 0;
	}

	return 0;
}

void AT_ExtractIP(void)
{
	u8 Count_i = 0 , Count_j = 0 , NumData = 0;

	while(1)
	{

		NumData = NumData * 10;
		NumData = NumData + ( ESP_Srt.ESP_IPString[Count_i] - 0x30 ); 

		Count_i++;
			
		if(ESP_Srt.ESP_IPString[Count_i] == '.')
		{
			ESP_Srt.ESP_IP[Count_j] = NumData;
			NumData = 0;
			Count_j ++;
			Count_i	++;
		}

		if( ESP_Srt.ESP_IPString[Count_i] == '\0' )
		{
			ESP_Srt.ESP_IP[Count_j] = NumData;
			break;
		}
	}

	SupCOM_Printf_Num(ESP_Srt.ESP_IP[0],WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '.' ,WIFIBEBUGLEVEL);
	SupCOM_Printf_Num(ESP_Srt.ESP_IP[1],WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '.' ,WIFIBEBUGLEVEL);
	SupCOM_Printf_Num(ESP_Srt.ESP_IP[2],WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '.' ,WIFIBEBUGLEVEL);
	SupCOM_Printf_Num(ESP_Srt.ESP_IP[3],WIFIBEBUGLEVEL);
	SupCOMPrintString_Level("\r\n",WIFIBEBUGLEVEL);
}

void AT_GetSATAMAC(void)
{
	u8 Count_i = 0 ,Count_j = 0;
//	+CIFSR:STAMAC,"60:01:94:83:3a:54"
//	01234567890123456789012345678901

	for( Count_i = 0; Count_i < AT_Str.AT_DataNum ; Count_i ++)
	{
		if( ( AT_Str.AT_ReDataBuff[Count_i][0] 	== '+')&& \
			( AT_Str.AT_ReDataBuff[Count_i][1] 	== 'C')&& \
			( AT_Str.AT_ReDataBuff[Count_i][2] 	== 'I')&& \
			( AT_Str.AT_ReDataBuff[Count_i][3] 	== 'F')&& \
			( AT_Str.AT_ReDataBuff[Count_i][10] == 'M')&& \
			( AT_Str.AT_ReDataBuff[Count_i][11] == 'A')&& \
			( AT_Str.AT_ReDataBuff[Count_i][12] == 'C'))
		{
			PrintWifiDebug("MAC:");

			SATMAC[0] = AT_ASCIIHex( AT_Str.AT_ReDataBuff[Count_i][15] ,AT_Str.AT_ReDataBuff[Count_i][16] );
			SupCOM_Printf_HEX(SATMAC[0],WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '-' ,WIFIBEBUGLEVEL);
			
			SATMAC[1] = AT_ASCIIHex( AT_Str.AT_ReDataBuff[Count_i][18] ,AT_Str.AT_ReDataBuff[Count_i][19] );
			SupCOM_Printf_HEX(SATMAC[1],WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '-' ,WIFIBEBUGLEVEL);
			
			SATMAC[2] = AT_ASCIIHex( AT_Str.AT_ReDataBuff[Count_i][21] ,AT_Str.AT_ReDataBuff[Count_i][22] );
			SupCOM_Printf_HEX(SATMAC[2],WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '-' ,WIFIBEBUGLEVEL);
			
			SATMAC[3] = AT_ASCIIHex( AT_Str.AT_ReDataBuff[Count_i][24] ,AT_Str.AT_ReDataBuff[Count_i][25] );
			SupCOM_Printf_HEX(SATMAC[3],WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '-' ,WIFIBEBUGLEVEL);
			
			SATMAC[4] = AT_ASCIIHex( AT_Str.AT_ReDataBuff[Count_i][27] ,AT_Str.AT_ReDataBuff[Count_i][28] );
			SupCOM_Printf_HEX(SATMAC[4],WIFIBEBUGLEVEL);SupCOMPrintChar_Level( '-' ,WIFIBEBUGLEVEL);
			
			SATMAC[5] = AT_ASCIIHex( AT_Str.AT_ReDataBuff[Count_i][30] ,AT_Str.AT_ReDataBuff[Count_i][31] );
			SupCOM_Printf_HEX(SATMAC[5],WIFIBEBUGLEVEL);

			SupCOMPrintString_Level("\r\n",WIFIBEBUGLEVEL);

			break;
		}
		else if(( AT_Str.AT_ReDataBuff[Count_i][0] 	== '+')&& \
				( AT_Str.AT_ReDataBuff[Count_i][1] 	== 'C')&& \
				( AT_Str.AT_ReDataBuff[Count_i][2] 	== 'I')&& \
				( AT_Str.AT_ReDataBuff[Count_i][3] 	== 'F')&& \
				( AT_Str.AT_ReDataBuff[Count_i][10] == 'I')&& \
				( AT_Str.AT_ReDataBuff[Count_i][11] == 'P'))
		{
			Count_j = 0;
			
			while(1)
			{
				ESP_Srt.ESP_IPString[Count_j] = AT_Str.AT_ReDataBuff[Count_i][ 14 + Count_j ];
				Count_j ++;
				if( AT_Str.AT_ReDataBuff[Count_i][ 14 + Count_j ] == '\"')
				{
					ESP_Srt.ESP_IPString[Count_j] = '\0';

					PrintWifiDebug("ESP_IP :");
//					USART1_SendString(ESP_Srt.ESP_IPString);
//					USART1_SendString("\r\n");
					AT_ExtractIP();
					break;
				}
			}
			
		}

	}
}

void AT_GETNetSta(void)
{
	if(	( AT_Str.AT_ReDataBuff[0][0] == 'S')&&	\
		( AT_Str.AT_ReDataBuff[0][1] == 'T')&&	\
		( AT_Str.AT_ReDataBuff[0][2] == 'A')&&	\
		( AT_Str.AT_ReDataBuff[0][3] == 'T')&&	\
		( AT_Str.AT_ReDataBuff[0][4] == 'U')&&	\
		( AT_Str.AT_ReDataBuff[0][5] == 'S'))
	{
		ESP_Srt.ESP_AP_State = AT_Str.AT_ReDataBuff[0][7] - 0x30;

		PrintWifiDebug("ESP_AP_State :");
		SupCOM_Printf_Num( ESP_Srt.ESP_AP_State ,WIFIBEBUGLEVEL);

		switch(ESP_Srt.ESP_AP_State)
		{
			case 2:
			SupCOMPrintString_Level("--Connected AP,Get IP \r\n",WIFIBEBUGLEVEL);
			break;
			case 3:
			SupCOMPrintString_Level("--Established TCP or UDP \r\n",WIFIBEBUGLEVEL);
			break;
			case 4:
			SupCOMPrintString_Level("--Disconnect \r\n",WIFIBEBUGLEVEL);
			break;
			case 5:
			SupCOMPrintString_Level("--ununited AP \r\n",WIFIBEBUGLEVEL);
			break;
		}
	}
}

//SupCOMPrintString_Level(" \r\n",WIFIBEBUGLEVEL);

void AT_APName( void )
{
//	AT+CWJAP_DEF?
	u8 Count_i = 0 ,Count_j = 0 , Count_x = 0;

//	u8 Res = AT_NULL;

	for( Count_i = 0; Count_i < AT_Str.AT_DataNum ; Count_i ++)
	{
		if( ( AT_Str.AT_ReDataBuff[Count_i][0] 	== '+')&& \
			( AT_Str.AT_ReDataBuff[Count_i][1] 	== 'C')&& \
			( AT_Str.AT_ReDataBuff[Count_i][2] 	== 'W')&& \
			( AT_Str.AT_ReDataBuff[Count_i][3] 	== 'J')&& \
			( AT_Str.AT_ReDataBuff[Count_i][4] 	== 'A')&& \
			( AT_Str.AT_ReDataBuff[Count_i][5] 	== 'P'))
		{
			PrintWifiDebug("Found AP");

//			SupCOMPrintString_Level( "AP Name :" , 0 );
			
			while(( AT_Str.AT_ReDataBuff[Count_i][Count_j] != '\"')&&( Count_j <= 64 ))
			Count_j	++;

			Count_j ++;

			Count_x = 0;

			while(( AT_Str.AT_ReDataBuff[Count_i][Count_j] != '\"')&&( Count_j <= 128 ))
			{
				ESP_Srt.ESP_APName[ Count_x ] = AT_Str.AT_ReDataBuff[ Count_i ][ Count_j ];
				Count_x ++ ;
				Count_j ++ ;
			}
			ESP_Srt.ESP_APName[ Count_x ] = '\0';

//			SupCOMPrintString_Level( ESP_Srt.ESP_APName , 0 );
//			SupCOMPrintString_Level( " \r\n" , 0 );

//			Res = AT_OK;

		}
		else if(( AT_Str.AT_ReDataBuff[Count_i][0] 	== 'N')&& \
				( AT_Str.AT_ReDataBuff[Count_i][1] 	== 'o')&& \
				( AT_Str.AT_ReDataBuff[Count_i][2] 	== ' ')&& \
				( AT_Str.AT_ReDataBuff[Count_i][3] 	== 'A')&& \
				( AT_Str.AT_ReDataBuff[Count_i][4] 	== 'P'))
		{
			PrintWifiDebug("No AP");
//			Res = AT_ERROR;
		}
	}

//	return Res;

}

u8 ESP_Touch(void)
{
	u8 ESP_Touch_State 	= 0;
	u8 Time_Out_Count	= 0;
	u8 Res = 0;
	
	u8 FindResBuff[2];

	PrintWifiDebug("AT+CWMODE_DEF=1");
	if(AT_SendCMD("AT+CWMODE_DEF=1") != AT_OK )
	{
		return AT_ERROR;
	}

	PrintWifiDebug("AT+CWAUTOCONN=1");
	if(AT_SendCMD("AT+CWAUTOCONN=1") != AT_OK )
	{
		return AT_ERROR;
	}

	PrintWifiDebug("AT+CWSTARTSMART=3");
	if(AT_SendCMD("AT+CWSTARTSMART=3") != AT_OK )
	{
		return AT_ERROR;
	}

	Flush_Buff();
	
//	AT_Str.AT_Start		= AT_ESP_T_0;
	AT_Str.AT_DataNum	= 0;
	ESP_Srt.ESP_FUM_Count = 0;
	ESP_Srt.ESP_TouchState	=	ESP_NULL;

	while(1)
	{
		if( AT_Str.AT_DataNum == 4)
		{
		
		}
		else if( AT_Str.AT_DataNum >= 6 )
		{
			ESP_Srt.ESP_TouchState = ESP_OK;
				
			SendBuffData();

			
			Res = 0;
			Res = AT_FindStringFromBuff("ssid:" , &AT_Str , FindResBuff );
			if( Res == 1 )
			{
				AT_CopyCMDBuff( ESP_Srt.ESP_APName , 0 , FindResBuff[0] , FindResBuff[1] + 1 , 0  );
				SupCOMPrintString_Level("ESP Touch AP Name : " , 0);
				SupCOMPrintString_Level(ESP_Srt.ESP_APName , 0 );
				SupCOMPrintString_Level("\r\n" , 0);
			}


			Res = 0;
			Res = AT_FindStringFromBuff("WIFI GOT IP" , &AT_Str , FindResBuff );
			if( Res == 1 )
			{
				SupCOMPrintString_Level("ESP Touch GOT IP \r\n" , 0);
				SupCOMPrintString_Level("ESP Touch Set Successful! \r\n" , 0);
				break;
			}

		}
		DelayS(2);
		Time_Out_Count ++;

		if( Time_Out_Count >= 60 )
		{
			ESP_Srt.ESP_TouchState = ESP_TIMEOUT;
			
			SupCOMPrintString_Level("ESP Touch Time OUT" , 0 );
			break;
		}
	}

	PrintWifiDebug("AT+CWSTOPSMART");
	if(AT_SendCMD("AT+CWSTOPSMART") != AT_OK )
	{
		return AT_ERROR;
	}
	SendBuffData();

	return ESP_Touch_State;
}
