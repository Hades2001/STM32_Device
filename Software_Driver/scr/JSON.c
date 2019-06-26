#include "JSON.h"
#include "SystemTime.h"
#include "SeverAPP.h"


void JSON_FushBuff( struct JSON_Data *Prt )
{
	u16 count;
	
	Prt->JSON_DataNum = 0;

	for(count = 0 ; count <JSON_DATABUFFSIZE ; count ++ )
	Prt->JSON_DataBuff[count] = '\0';
}

void CopyStringToJSONDataBuff( u8 *StringData , struct JSON_Data *BuffPrt )
{
	while( *StringData != '\0' )
	{
		BuffPrt->JSON_DataBuff[BuffPrt->JSON_DataNum] = *StringData;
		*StringData ++;
		BuffPrt->JSON_DataNum++;
	}
}

void PrintJSONDataBuff(	struct JSON_Data *BuffPrt )
{
	SupCOMPrintString_Level( "JSON DataNum:" , 3 );
	SupCOM_Printf_Num( BuffPrt->JSON_DataNum , 3 );
	SupCOMPrintString_Level( "\r\n" , 3 );

	SupCOMPrintString_Level( "JSON Data:" , 3 );
	SupCOMPrintString_Level( BuffPrt->JSON_DataBuff , 3 );

	SupCOMPrintString_Level("\r\n" , 3 );
}

void ADDCharToJSONDataBuff( u8 Data , struct JSON_Data *BuffPrt )
{
	BuffPrt->JSON_DataBuff[BuffPrt->JSON_DataNum] = Data;
	BuffPrt->JSON_DataNum++;
}

void ADDHex8ToJSONDataBuff( u8 Data , struct JSON_Data *BuffPrt )
{
	u8 Data_H,Data_L;
	
	Data_L = Data & 0x0f;
	Data_H = Data >> 4;
	
	if(Data_H<10)
	BuffPrt->JSON_DataBuff[BuffPrt->JSON_DataNum] = Data_H+0x30;
	else
	BuffPrt->JSON_DataBuff[BuffPrt->JSON_DataNum] = Data_H-10+0x41;

	BuffPrt->JSON_DataNum++;
		
	if(Data_L<10)
	BuffPrt->JSON_DataBuff[BuffPrt->JSON_DataNum] = Data_L+0x30;
	else
	BuffPrt->JSON_DataBuff[BuffPrt->JSON_DataNum] = Data_L-10+0x41;

	BuffPrt->JSON_DataNum++;

}

void ADDNumToJSONDataBuff( u32 Num_Data ,u8 PointPosition ,struct JSON_Data *BuffPrt )
{
	u8 	Num_Buf[10]={0,0,0,0,0,0,0,0,0,0};
	u8  i = 0 , n = 0 ;

	while(Num_Data/10!=0)
	{     
		Num_Buf[9-i]=Num_Data%10;
		Num_Data/=10;
		i++;
	}
	Num_Buf[9-i]=Num_Data%10;
	i++;


	if( PointPosition >= i )
	i = PointPosition + 1;

	
	for(n=0;n<i;n++)
	{
		if( ( i - n ) == PointPosition )
		{
			BuffPrt->JSON_DataBuff[BuffPrt->JSON_DataNum] = '.';
			BuffPrt->JSON_DataNum++;
		}
		BuffPrt->JSON_DataBuff[BuffPrt->JSON_DataNum] = (Num_Buf[10-i+n]+0x30);
		BuffPrt->JSON_DataNum++;
	}
}



void ADD_StringParJSON(u8 *ParNameString , u8 *ParValString ,struct JSON_Data *BuffPrt )
{
	ADDCharToJSONDataBuff( '\"' , BuffPrt );
	
	CopyStringToJSONDataBuff( ParNameString , BuffPrt );
	
	CopyStringToJSONDataBuff( "\":\"" , BuffPrt );

	CopyStringToJSONDataBuff( ParValString , BuffPrt );
	
	ADDCharToJSONDataBuff( '\"' , BuffPrt );
	
	ADDCharToJSONDataBuff( ',' , BuffPrt );
}

void ADD_NumParJSON(u8 *ParNameString , u32 Num_Data ,u8 PointPosition ,struct JSON_Data *BuffPrt )
{
	ADDCharToJSONDataBuff( '\"' , BuffPrt );
	
	CopyStringToJSONDataBuff( ParNameString , BuffPrt );
	
	CopyStringToJSONDataBuff( "\":\"" , BuffPrt );

	ADDNumToJSONDataBuff( Num_Data , PointPosition , BuffPrt );
	
	ADDCharToJSONDataBuff( '\"' , BuffPrt );
	
	ADDCharToJSONDataBuff( ',' , BuffPrt );
}
 
void ADDMACToJSONData( struct JSON_Data *BuffPrt )
{	
	CopyStringToJSONDataBuff( "\"MAC_ADDRESS\":" , BuffPrt );

	ADDCharToJSONDataBuff( '\"' , BuffPrt );

	ADDHex8ToJSONDataBuff( SATMAC[0] , BuffPrt );
	ADDCharToJSONDataBuff( ':' , BuffPrt );
	ADDHex8ToJSONDataBuff( SATMAC[1] , BuffPrt );
	ADDCharToJSONDataBuff( ':' , BuffPrt );
	ADDHex8ToJSONDataBuff( SATMAC[2] , BuffPrt );
	ADDCharToJSONDataBuff( ':' , BuffPrt );
	ADDHex8ToJSONDataBuff( SATMAC[3] , BuffPrt );
	ADDCharToJSONDataBuff( ':' , BuffPrt );
	ADDHex8ToJSONDataBuff( SATMAC[4] , BuffPrt );
	ADDCharToJSONDataBuff( ':' , BuffPrt );
	ADDHex8ToJSONDataBuff( SATMAC[5] , BuffPrt );
	
	ADDCharToJSONDataBuff( '\"' , BuffPrt );
	ADDCharToJSONDataBuff( ',' , BuffPrt );

}

void ADDHourToJSON( u8 *ParNameString , u8 HourData, u8  MinData , struct JSON_Data *BuffPrt )
{
	ADDCharToJSONDataBuff( '\"' , BuffPrt );
	
	CopyStringToJSONDataBuff( ParNameString , BuffPrt );
	
	CopyStringToJSONDataBuff( "\":\"" , BuffPrt );
	
	if( HourData < 10 )
	ADDCharToJSONDataBuff( '0' , BuffPrt );
	
	ADDNumToJSONDataBuff( HourData , 0 , BuffPrt );
	
	ADDCharToJSONDataBuff( ':' , BuffPrt );
	if( MinData < 10 )
	ADDCharToJSONDataBuff( '0' , BuffPrt );
	
	ADDNumToJSONDataBuff( MinData , 0 , BuffPrt );
	
	ADDCharToJSONDataBuff( '\"' , BuffPrt );
	
	ADDCharToJSONDataBuff( ',' , BuffPrt );
}


void SendJSON_Parameter( struct Parameter *DataPrt,struct JSON_Data *BuffPrt )
{
	JSON_FushBuff( BuffPrt );
	
	ADDCharToJSONDataBuff( '{' , BuffPrt );

	ADD_StringParJSON( "COMMOND" , "SEND_MONITOR" , BuffPrt );

	ADDMACToJSONData( BuffPrt );

	ADD_NumParJSON( "S" 	, DataPrt->Pmr_S 	, 0 , BuffPrt );
	ADD_NumParJSON( "R" 	, DataPrt->Pmr_R 	, 0 , BuffPrt );
	ADD_NumParJSON( "M" 	, DataPrt->Pmr_M 	, 0 , BuffPrt );
	ADD_NumParJSON( "DC" 	, DataPrt->Pmr_DC 	, 0 , BuffPrt );
	ADD_NumParJSON( "AI" 	, DataPrt->Pmr_AI 	, 0 , BuffPrt );
	ADD_NumParJSON( "TW" 	, DataPrt->Pmr_TW 	, 0 , BuffPrt );
	ADD_NumParJSON( "T" 	, DataPrt->Pmr_Temp , 0 , BuffPrt );
	ADD_NumParJSON( "A" 	, DataPrt->Pmr_Cur 	, 4 , BuffPrt );
	ADD_NumParJSON( "W" 	, DataPrt->Pmr_Wat 	, 3 , BuffPrt );

	BuffPrt->JSON_DataNum --;

	CopyStringToJSONDataBuff( "}$$",BuffPrt );

	PrintJSONDataBuff(BuffPrt);
}

void SendRequestJson( u8 *RequestName ,struct JSON_Data *BuffPrt )
{
	JSON_FushBuff( BuffPrt );
	
	ADDCharToJSONDataBuff( '{' , BuffPrt );

	ADD_StringParJSON( "COMMOND" , RequestName , BuffPrt );

	ADDMACToJSONData( BuffPrt );

	BuffPrt->JSON_DataNum --;
	
	CopyStringToJSONDataBuff( "}$$",BuffPrt );

	PrintJSONDataBuff(BuffPrt);
}



void SendRequestFlagJSON( u8 *RequestName , u8 Flag ,struct JSON_Data *BuffPrt )
{
	JSON_FushBuff( BuffPrt );
	
	ADDCharToJSONDataBuff( '{' , BuffPrt );

	ADD_StringParJSON( "COMMOND" , RequestName , BuffPrt );

	ADDMACToJSONData( BuffPrt );

	ADD_NumParJSON( "FLAG" 	, Flag 	, 0 , BuffPrt );

	BuffPrt->JSON_DataNum --;
	
	CopyStringToJSONDataBuff( "}$$",BuffPrt );

	PrintJSONDataBuff(BuffPrt);
}

void SendEletricOnLine( u8 *TimeString , u8 Hour , u32 Kilowatt , u8 EleTime  , struct JSON_Data *BuffPrt )
{

	JSON_FushBuff( BuffPrt );

	ADDCharToJSONDataBuff( '{' , BuffPrt );

	ADD_StringParJSON( "COMMOND" , "SEND_ELECTRIC" , BuffPrt );

	ADDMACToJSONData( BuffPrt );

 	CopyStringToJSONDataBuff( "\"ELECTRIC_DATA\":[",BuffPrt );
	
	ADDCharToJSONDataBuff( '{' , BuffPrt );

	ADD_StringParJSON( "ELECTRIC_DATE" , TimeString , BuffPrt );

	ADDHourToJSON( "TIME_F" , Hour , 0 , BuffPrt );

	ADD_NumParJSON( "ELECTRIC" 	, Kilowatt / 1000 , 3 , BuffPrt );
	
	ADD_NumParJSON( "DURATION" 	, EleTime  , 0 , BuffPrt );

	BuffPrt->JSON_DataNum --;

	ADDCharToJSONDataBuff( '}',BuffPrt );
	
	ADDCharToJSONDataBuff( ']',BuffPrt );
	
	CopyStringToJSONDataBuff( "}$$",BuffPrt );

	PrintJSONDataBuff(BuffPrt);
}


void SendEletricOffLine_Head( struct JSON_Data * BuffPrt )
{
	JSON_FushBuff( BuffPrt );

	ADDCharToJSONDataBuff( '{' , BuffPrt );

	ADD_StringParJSON( "COMMOND" , "SEND_ELECTRIC" , BuffPrt );

	ADDMACToJSONData( BuffPrt );

 	CopyStringToJSONDataBuff( "\"ELECTRIC_DATA\":[",BuffPrt );

	PrintJSONDataBuff(BuffPrt);
}

void SendEletricOffLine_Data(u8 * TimeString, u8 Hour, u32 Kilowatt, u8 EleTime, struct JSON_Data * BuffPrt)
{
	JSON_FushBuff( BuffPrt );
	
	ADDCharToJSONDataBuff( '{' , BuffPrt );

	ADD_StringParJSON( "ELECTRIC_DATE" , TimeString , BuffPrt );

	ADDHourToJSON( "TIME_F" , Hour , 0 , BuffPrt );

	ADD_NumParJSON( "ELECTRIC" 	, Kilowatt/1000, 3 , BuffPrt );
	
	ADD_NumParJSON( "DURATION" 	, EleTime  , 0 , BuffPrt );

	BuffPrt->JSON_DataNum --;

	ADDCharToJSONDataBuff( '}',BuffPrt );

	PrintJSONDataBuff(BuffPrt);
}

void SendEletricOffLine_End( struct JSON_Data * BuffPrt )
{
	JSON_FushBuff( BuffPrt );
	
	ADDCharToJSONDataBuff( ']',BuffPrt );
	
	CopyStringToJSONDataBuff( "}$$",BuffPrt );

	PrintJSONDataBuff(BuffPrt);
}

void SendReUpData( u16 PackNum  , u8 *VresionBuff , struct JSON_Data * BuffPrt )
{
	
	JSON_FushBuff( BuffPrt );
	
	ADDCharToJSONDataBuff( '{' , BuffPrt );

	ADD_StringParJSON( "COMMOND" , "SEND_VERSION" , BuffPrt );

	ADDMACToJSONData( BuffPrt );

	ADD_StringParJSON( "VERSION_NO" , VresionBuff , BuffPrt );

	ADD_NumParJSON( "FILE_NO" 	, PackNum 	, 0 , BuffPrt );

	BuffPrt->JSON_DataNum --;
	
	CopyStringToJSONDataBuff( "}$$",BuffPrt );

	PrintJSONDataBuff(BuffPrt);

}

void SendUpDataFlag( u8 *VresionBuff , u8 Flag , struct JSON_Data * BuffPrt )
{
	
	JSON_FushBuff( BuffPrt );
	
	ADDCharToJSONDataBuff( '{' , BuffPrt );

	ADD_StringParJSON( "COMMOND" , "SEND_VERSION" , BuffPrt );

	ADDMACToJSONData( BuffPrt );

	ADD_NumParJSON( "FLAG" 	, Flag 	, 0 , BuffPrt );

	ADD_StringParJSON( "VERSION_NO" , VresionBuff , BuffPrt );

	BuffPrt->JSON_DataNum --;
	
	CopyStringToJSONDataBuff( "}$$",BuffPrt );

	PrintJSONDataBuff(BuffPrt);

}


u8 JSON_Resolve( u8 *JSON_Buff, u8 *ValName , u8 *JSON_VAL )
{
	u16 Count = 0 , Count_A = 0;
//	AT_TCP.AT_TCPReBuff[0];
	while(( *JSON_Buff != '\0' )&&( Count <= 1023 ))
	{
		if( *JSON_Buff == *( ValName + Count_A ))
		Count_A ++;
		else
		Count_A = 0;

		if( *( ValName + Count_A ) == '\0' )
		break;

		Count ++;
		JSON_Buff ++;
	}

	if( Count < 1023 )
	{
		if(( *( JSON_Buff + 2 ) == ':') && ( *( JSON_Buff + 3 ) == '\"'))
		{
			
			JSON_Buff += 4;
				
			while( *JSON_Buff != '\"' )
			{
				*JSON_VAL = *JSON_Buff;

//				TxBitData( *JSON_Buff );

				JSON_VAL++;
				JSON_Buff ++;
			}
		}

		*JSON_VAL = '\0';
		
		return JSON_Ana_OK;
	}
	else
	{
		TxStringData("JSON VAL Name Not Found \r\n");
		return JSON_Ana_NOFOUND;
	}
}

u8 JSON_ResolveNUm( u8 *JSON_Buff ,u8 *ValName, u32 *Num )
{
	u8 NumBuff[64];
	u8 Res;
	
	Res = JSON_Resolve( JSON_Buff , ValName , NumBuff );
	
	if( Res == JSON_Ana_OK )
	(*Num) = SupCOM_StringToNum( NumBuff , 0 );

	return Res;
}

u8 JSON_COMMONDStrToVal( u8 *JSON_VAL )
{
	u8 ReturnData = 0;

	if( ContrastString( (char*)JSON_VAL , "SEND_MONITOR") == 1 )
	ReturnData = 1;
	if( ContrastString( (char*)JSON_VAL , "SEND_DC") == 1 )
	ReturnData = 2;
	if( ContrastString( (char*)JSON_VAL , "SEND_AI") == 1 )
	ReturnData = 3;
	if( ContrastString( (char*)JSON_VAL , "SEND_ELECTRIC") == 1 )
	ReturnData = 4;
	if( ContrastString( (char*)JSON_VAL , "SEND_TASK") == 1 )
	ReturnData = 5;
	if( ContrastString( (char*)JSON_VAL , "SEND_TEMPERATUERE") == 1 )
	ReturnData = 6;
	if( ContrastString( (char*)JSON_VAL , "SEND_AI_TEMPERATUERE") == 1 )
	ReturnData = 7;

	TxStringData( "COMMOND Num : " );
	Printf_Num( ReturnData );
	TxStringData(" \r\n");
	
	return ReturnData;
}

//u8 JSOM_FlagStrToVal( u8 *JSON_VAL )
//{
//	u8 Data = 0;
//	
//	if( *JSON_VAL == '0' )
//	Data = 0;
//	else if( *JSON_VAL == '1' )
//	Data = 1;

//	return Data;
//}

//u8 JSON_TEMPStrToval( u8 *JSON_VAL )
//{
//	u8 Temp_Val = 0;

//	Temp_Val  = (( *(JSON_VAL) )-0x30)*10;
//	Temp_Val += (( *(JSON_VAL + 1) )-0x30);

//	return Temp_Val;
//}

void JSON_F_String( u8 *StringB , struct JSON_FindStr *p , struct JSON_VARStr *VAR )
{
	u16 Count_i = 0 , Count_A = 0;

	VAR->FindNum = 0;
	p->FindState = JSON_Ana_NULL;
	
	while(( *( p->JSON_Buff_Ptr + Count_i + p->offset ) != '\0' )&&( Count_i <= p->JSON_Buff_Size ))
	{
		if( *( p->JSON_Buff_Ptr + Count_i + p->offset ) ==  *( StringB + Count_A ) )
		Count_A ++;
		else
		Count_A = 0;
		
		if( *( StringB + Count_A ) == '\0' )
		{
			if( VAR->FindMode == JSON_F_FIRST )
			{
				VAR->FindNum = 1;
				VAR->StartOffset[0] 	= Count_i - Count_A + 1;
				VAR->EndOffset[0] 	= Count_i ;
				break;
			}
			else
			{
				VAR->StartOffset[VAR->FindNum] 	= Count_i - Count_A + 1;
				VAR->EndOffset[VAR->FindNum] 	= Count_i ;
				VAR->FindNum ++;
				Count_A = 0;
			}
		}
		Count_i ++;
	}

	if( VAR->FindNum != 0 )
	{
		
//		TxStringData("FindNum : ");
//		Printf_Num( VAR->FindNum );
//		TxStringData("\r\n");
//		for( Count_A = 0 ; Count_A < VAR->FindNum ; Count_A++ )
//		{
//			TxStringData("StartOffset : ");
//			Printf_Num( VAR->StartOffset[Count_A] );
//			TxStringData("  ENDOffset : ");
//			Printf_Num( VAR->EndOffset[Count_A] );
//			TxStringData("\r\n");
//		}
		p->FindState = JSON_Ana_OK;
	}
	else
	{
//		TxStringData("Not Found \r\n");
		p->FindState = 	JSON_Ana_ERROR;
	}

}

void JSON_ReadVal( u8 VarNum , u8 *ValBuff ,struct JSON_FindStr *p ,struct JSON_VARStr *VAR )
{
	u32 Count_i = 0 , Count_a = 0;

	while( *( p->JSON_Buff_Ptr + VAR->EndOffset[VarNum] + Count_i ) != ':' )
	Count_i ++;
	
	while( *( p->JSON_Buff_Ptr + VAR->EndOffset[VarNum] + Count_i ) != '\"' )
	Count_i ++;

	Count_i = Count_i + 1;

	while( *( p->JSON_Buff_Ptr + VAR->EndOffset[VarNum] + Count_i ) != '\"' )
	{
		*( ValBuff + Count_a ) = *( p->JSON_Buff_Ptr + VAR->EndOffset[VarNum] + Count_i );
			
		Count_i ++ ;
		Count_a ++ ;
	}

	*( ValBuff + Count_a ) = '\0';

//	TxStringData(ValBuff);
//	TxStringData("\r\n");
	
}


