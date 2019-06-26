#ifndef __SYSTEMTIME_H
#define __SYSTEMTIME_H

#include "stm32f10x.h"

#define TimeZone 8

#define TIME_YYYYMMDD	1
#define TIME_YYMMDD		2
#define TIME_HHMMSS		3
#define TIME_HHMM		4
#define TIME_HH			5



struct SystemTime
{
	u32 SystemTimeSec;
	
	u16 S_Year;
	u8	S_Month;
	u8	S_Days;
	u8	S_Hour;
	u8	S_Mins;
	u8	S_Secs;
	u8 	S_Week;

	u8 	S_HouruPFlag;
	u8 	S_MinsuPFlag;
	u8 	S_SecuPFlag;
	
	u8 	S_YearuPFlag;
	u8 	S_MonthuPFlag;
	u8 	S_DayuPFlag;
	
	u8	NTP_flag;
	u8	Time_UpDataFlag;
	u8	NTP_UpDataFlag;
	
};

extern void PrintfTime( u8 *TimeStringBuff , u8 Mode , struct SystemTime *Time );
extern void PrintTimeTobuff( u8 *TimeStringBuff , u8 F_Year , u8 F_Month , u8 F_Day );
extern void SysTimeFun(void);
extern u8 ntp_get_current_timestamp(u32 SecsData);
extern u8 GetNTPTime(void);

extern void InitSysTemTime(void);


extern struct SystemTime SysTime_Str,SysRegTime;


#endif /* __SYSTEMTIME_H */

