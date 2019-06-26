#include "SystemTime.h"
#include "ESP_AT.h"
#include "SupCOM.h"


struct SystemTime SysTime_Str , SysRegTime ;

u8 Month_D[12] = {31,28,31,30,31,30,31,31,30,31,30,31};


void InitSysTemTime(void)
{
	u32 RTC_CNTVal = 0 ;
	
	RTC_CNTVal = RTC_GetCounter();

	ntp_get_current_timestamp(RTC_CNTVal);

}


void SysTimeFun(void)
{

	SysTime_Str.SystemTimeSec ++;
	SysTime_Str.S_Secs ++;

	if( SysTime_Str.S_Secs >= 60 )
	{
		SysTime_Str.S_Secs = 0;
		SysTime_Str.S_Mins ++;
		
		SysTime_Str.S_MinsuPFlag = 1;
	}

	if( SysTime_Str.S_Mins >= 60 )
	{
		SysTime_Str.S_Mins = 0;
		SysTime_Str.S_Hour ++;

		SysTime_Str.S_HouruPFlag = 1;
		SysTime_Str.NTP_UpDataFlag = 1;
	}

	if( SysTime_Str.S_Hour >= 24 )
	{
		SysTime_Str.S_Hour = 0;
		SysTime_Str.S_Days ++;
		SysTime_Str.S_Week ++;

		SysTime_Str.S_DayuPFlag = 1;
	}
	if( SysTime_Str.S_Week > 7 )
	{
		SysTime_Str.S_Week = 1;
	}

	if((( SysTime_Str.S_Year % 4 == 0 )&&( SysTime_Str.S_Year % 100 != 0 )) || ( SysTime_Str.S_Year % 400 == 0))
	Month_D[1] = 29;
	else
	Month_D[1] = 28;

	if( SysTime_Str.S_Days >= Month_D[ SysTime_Str.S_Month - 1 ] )
	{
		SysTime_Str.S_Days = 1;
		SysTime_Str.S_Month ++;

		SysTime_Str.S_MonthuPFlag = 1;
	}

	if( SysTime_Str.S_Month > 12 )
	{
		SysTime_Str.S_Month = 1;
		SysTime_Str.S_Year ++;

		SysTime_Str.S_YearuPFlag = 1;
	}
}




u8 ntp_get_current_timestamp(u32 SecsData)
{
	u8 	Month = 1 ,Days = 1 ,Hour = 0 ,Min = 0 ,Sec = 0 ,Week = 0 ;
	u16 Years = 1900; 
	u32 YearsSec;


	Week = (( SecsData / 86400 ) % 7 ) + 1;

	SecsData += TimeZone*3600;
		
	do
	{
		if((( Years % 4 == 0 )&&( Years % 100 != 0 )) || ( Years % 400 == 0))
		YearsSec = 31622400;
		else
		YearsSec = 31536000;

		if( SecsData < YearsSec )
		break;
		else
		{
			SecsData -= YearsSec;
			Years ++;
		}
	}
	while(1);

	if((( Years % 4 == 0 )&&( Years % 100 != 0 )) || ( Years % 400 == 0))
	Month_D[1] = 29;	

	for( Month = 1; Month <= 12 ; Month ++ )
	{
		if( SecsData < ( Month_D[Month-1] * 86400 ))
		break;
		else
		SecsData = SecsData - ( Month_D[Month-1] * 86400 );
	}
	
	Days = ( SecsData / 86400 ) + 1;
	SecsData = SecsData % 86400;

	Hour 		= 	SecsData / 3600;
	SecsData 	= 	SecsData % 3600;
	Min			=	SecsData / 60;	
	Sec			=	SecsData % 60;


	SysTime_Str.S_Year 	= Years;
	SysTime_Str.S_Month = Month;
	SysTime_Str.S_Days 	= Days;
	SysTime_Str.S_Hour 	= Hour;
	SysTime_Str.S_Mins 	= Min;
	SysTime_Str.S_Secs 	= Sec;
	SysTime_Str.S_Week  = Week;
		
	Printf_Num(Years);TxBitData('-');
	Printf_Num(Month);TxBitData('-');
	Printf_Num(Days);
	TxStringData("\r\n");
	Printf_Num(Hour);TxBitData(':');
	Printf_Num(Min);TxBitData(':');
	Printf_Num(Sec);
	TxStringData("\r\n");
	return 0;

}

u8 GetNTPTime(void)
{
	u8 	NTPBuff[50] ;
	u16 TimeOUT_Count_Main , NTP_AttemptCount ;
	u32 NTPData ;

	u8 Res = 0;

	Res = AT_LinkTCP_Sever_IPStr( AT_UDP_TYPE , "cn.ntp.org.cn" , 123 );

	for( TimeOUT_Count_Main = 0; TimeOUT_Count_Main < 48 ; TimeOUT_Count_Main ++ )
	NTPBuff[ TimeOUT_Count_Main ] = 0x00;
	
	NTPBuff[0] = 0xE3;
	NTPBuff[1] = 0x00;
	NTPBuff[2] = 0x06;
	NTPBuff[3] = 0xEC;
	
	NTPBuff[12] = 0x31;
	NTPBuff[13] = 0x4E;
	NTPBuff[14] = 0x31;
	NTPBuff[15] = 0x35;
	
	NTPBuff[17] = 0x06;


	NTP_AttemptCount = 0;

	while( NTP_AttemptCount <= 5 )
	{			
		AT_SendTCPData(48,NTPBuff);
		
		AT_TCP_SetReDataMode();
	
		TimeOUT_Count_Main 	= 0;
		
		while( AT_GetTCPReFlag() != AT_TCP_GetReady && TimeOUT_Count_Main < 1000 )
		{	
			DelayMs( 1 );
	
			TimeOUT_Count_Main ++;
		}
		
		if( TimeOUT_Count_Main >= 1000 )
		{
			TimeOUT_Count_Main 	= 0;
			NTP_AttemptCount	++;
				
			Printf_Num(NTP_AttemptCount);
		}
		else
		{
			AT_CopyTCPBuff(NTPBuff,48);
		
			NTPData = 0;
		
			NTPData |= NTPBuff[40] << 24;
			NTPData |= NTPBuff[41] << 16;
			NTPData |= NTPBuff[42] << 8;
			NTPData |= NTPBuff[43] ;
		
			SysTime_Str.NTP_flag = 1;				
			SysTime_Str.SystemTimeSec = NTPData;


//			RTC_SetCounter(NTPData);//设置RTC计数器的值
//    		RTC_WaitForLastTask();    //等待最近一次对RTC寄存器的写操作完成
    		
			ntp_get_current_timestamp(NTPData);

			Res = 1;
			
			break;
		}
	}

	if( NTP_AttemptCount >= 5)
	Res = 0;
	
	AT_CloseSeverLink();

	return Res;
}

void PrintfTime( u8 *TimeStringBuff , u8 Mode , struct SystemTime *Time )
{
	struct StringStr TimeBuff;
	
	u16 Count_i;

	String_FushBuff( &TimeBuff );

	switch( Mode )
	{
		case TIME_YYYYMMDD:
		ADDNumToStringDataBuff( Time->S_Year , 0 , &TimeBuff );
		ADDCharToStringDataBuff( '-' , &TimeBuff );
		if( Time->S_Month < 10 )
		ADDCharToStringDataBuff( '0' , &TimeBuff );
		ADDNumToStringDataBuff( Time->S_Month , 0 , &TimeBuff );
		ADDCharToStringDataBuff( '-' , &TimeBuff );
		if( Time->S_Days < 10 )
		ADDCharToStringDataBuff( '0' , &TimeBuff );
		ADDNumToStringDataBuff( Time->S_Days , 0 , &TimeBuff );
		break;

		case TIME_YYMMDD:
		ADDNumToStringDataBuff( Time->S_Year % 100 , 0 , &TimeBuff );
		ADDCharToStringDataBuff( '-' , &TimeBuff );
		ADDNumToStringDataBuff( Time->S_Month , 0 , &TimeBuff );
		ADDCharToStringDataBuff( '-' , &TimeBuff );
		ADDNumToStringDataBuff( Time->S_Days , 0 , &TimeBuff );
		break;
		
		case TIME_HHMMSS:
		ADDNumToStringDataBuff( Time->S_Hour , 0 , &TimeBuff );
		ADDCharToStringDataBuff( ':' , &TimeBuff );
		ADDNumToStringDataBuff( Time->S_Mins , 0 , &TimeBuff );
		ADDCharToStringDataBuff( ':' , &TimeBuff );
		ADDNumToStringDataBuff( Time->S_Secs , 0 , &TimeBuff );
		break;
		
		case TIME_HHMM:
		ADDNumToStringDataBuff( Time->S_Hour , 0 , &TimeBuff );
		ADDCharToStringDataBuff( ':' , &TimeBuff );
		ADDNumToStringDataBuff( Time->S_Mins , 0 , &TimeBuff );
		break;

		case TIME_HH:
		ADDNumToStringDataBuff( Time->S_Hour , 0 , &TimeBuff );
		ADDCharToStringDataBuff( ':' , &TimeBuff );
		CopyStringToStringDataBuff( "00" , &TimeBuff );
		break;
	}

	for( Count_i = 0 ; Count_i < TimeBuff.DataNum ; Count_i ++ )
	*( TimeStringBuff + Count_i ) = TimeBuff.DataBuff[Count_i];

	*( TimeStringBuff + Count_i  ) = '\0';

}

void PrintTimeTobuff( u8 *TimeStringBuff , u8 F_Year , u8 F_Month , u8 F_Day )
{
	struct StringStr TimeBuff;
	
	u16 Count_i;

	String_FushBuff( &TimeBuff );

	CopyStringToStringDataBuff( "20" ,&TimeBuff );
	
	if( F_Year < 10 )
	ADDCharToStringDataBuff( '0' , &TimeBuff );
	ADDNumToStringDataBuff( F_Year , 0 , &TimeBuff );
	
	ADDCharToStringDataBuff( '-' , &TimeBuff );
	if( F_Month < 10 )
	ADDCharToStringDataBuff( '0' , &TimeBuff );
	ADDNumToStringDataBuff( F_Month , 0 , &TimeBuff );
	ADDCharToStringDataBuff( '-' , &TimeBuff );
	
	if( F_Day < 10 )
	ADDCharToStringDataBuff( '0' , &TimeBuff );
	ADDNumToStringDataBuff( F_Day , 0 , &TimeBuff );


	for( Count_i = 0 ; Count_i < TimeBuff.DataNum ; Count_i ++ )
	*( TimeStringBuff + Count_i ) = TimeBuff.DataBuff[Count_i];

	*( TimeStringBuff + Count_i  ) = '\0';

}

























