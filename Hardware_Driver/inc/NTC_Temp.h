#ifndef __NTC_TEMP_H
#define	__NTC_TEMP_H

#include "stm32f10x.h"

struct NTC_TEMP
{
	u16 TempData;

	u8 TempReady;
};

u32 ChanegNTC( u16 ADC_Data );
double TempChange( u32 NTCData );
void ADC_Sampling(void);

extern struct NTC_TEMP	NTC_RT1 , NTC_RT2 ;


#endif

