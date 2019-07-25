#include "LED.h"

struct  LEDStr	LEDs[LEDMaxNum]=
{
	[LED1]	= { LED_OFF , GPIOA, GPIO_Pin_11 },
	[LED2]	= { LED_OFF , GPIOA, GPIO_Pin_12 },
	[LED3]	= { LED_OFF , GPIOA, GPIO_Pin_15 },
};
//
// LEDs Low layer API
//

void LEDs_Dis( uint16_t Num )
{
	if( LEDs[Num].LED_State == LED_ON )
	LED_ON_GPIO(LEDs[Num].GPIOx,LEDs[Num].GPIO_Pin);
	else
	LED_OFF_GPIO(LEDs[Num].GPIOx,LEDs[Num].GPIO_Pin);
}

void LEDs_Ctrl( uint16_t Num , uint8_t State )
{
	LEDs[Num].LED_State = State;
	LEDs_Dis( Num );
}

int InitLEDs( void )
{
	uint16_t Count = 0;
	for( Count = 0 ; Count < LEDMaxNum ; Count ++ )
	LEDs_Dis( Count );
	return 0;
}INIT_FS_EXPORT(InitLEDs);
