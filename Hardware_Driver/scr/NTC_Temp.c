#include "NTC_Temp.h"

struct NTC_TEMP	NTC_RT1 , NTC_RT2 ;


const u32 NTC_Data[141] = { 149240,140710,132740,125270,118270,111710,105560,99788,94372,89285,
							84507,80015,75791,71818,68079,64558,61242,58117,55172,52394,
							49773,47299,44964,42759,40675,38705,36843,35081,33414,31837,
							30343,28928,27587,26317,25112,23970,22886,21857,20881,19954,
							19073,18236,17440,16684,15965,15281,14630,14010,13420,12858,
							12323,11327,11327,10864,10422,10000,9598,9214,8847,8498,
							8163,7840,7539,7247,6969,6702,6447,6203,5970,5746,
							5532,5328,5131,4943,4763,4591,4425,4267,4115,3969,
							3829,3694,3565,3442,3323,3209,3099,2994,2892,2795,
							2701,2611,2525,2442,2362,2285,2210,2139,2070,2004,
							1940,1879,1820,1763,1708,1655,1604,1555,1507,1461,
							1417,1374,1333,1294,1255,1218,1182,1148,1115,1082,
							1051,1021,992,964,937,910,885,860,836,813,
							791,769,749,728,709,690,671,654,636,620,603,};


u16 GetRT1ADVal(void)
{
	return ADC_GetConversionValue(ADC1);
}

u16 GetRT2ADVal(void)
{
	return ADC_GetConversionValue(ADC2);
}


u32 ChanegNTC( u16 ADC_Data )
{
	u32 NTC_VAL = 0;
	u32 ADC_Data_U32 = ADC_Data;
	
	NTC_VAL = (( ADC_Data_U32 * 10000 *100 )/( 4096 - ADC_Data_U32 ));

	NTC_VAL /= 100;

	return NTC_VAL;

}


double TempChange( u32 NTCData )
{

	double K_ValData = 0 , B_ValData = 0 ,TempVal = 0;
	u16 Count_i;
	s16 Temp;

	for( Count_i = 0 ; Count_i < 141 ; Count_i ++ )
	{
		if(( NTCData <= NTC_Data[Count_i] )&&( NTCData > NTC_Data[Count_i+1] ))
		{

			K_ValData = NTC_Data[Count_i] - NTC_Data[Count_i + 1 ];

			K_ValData = -( 1 / K_ValData);

			Temp = Count_i;

			Temp = Temp - 30;

			B_ValData = Temp - NTC_Data[Count_i]*K_ValData;
		
			TempVal = K_ValData * NTCData + B_ValData;

//			TempVal -= 30;
			break;
		}
	}
	return TempVal; 
}

void ADC_Sampling(void)
{
	static u32 ADC1_Sum , ADC2_Sum;

	static u8 Count;

	ADC1_Sum += GetRT1ADVal();
	ADC2_Sum += GetRT2ADVal();

	Count ++;
	
	if( Count == 64)
	{
		Count = 0;
		NTC_RT1.TempData = ADC1_Sum>>6;
		NTC_RT2.TempData = ADC2_Sum>>6;

		ADC1_Sum = 0;
		ADC2_Sum = 0;

		NTC_RT1.TempReady = 1;
		NTC_RT2.TempReady = 1;
	}
	else if( Count > 64 )
	{
		Count = 0;
		ADC1_Sum = 0;
		ADC2_Sum = 0;
	}
}





