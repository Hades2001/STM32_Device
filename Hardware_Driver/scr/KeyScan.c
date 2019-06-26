#include "KeyScan.h"

struct KeyScanStr SysKLStr;

void Key_Time1mS( void )
{
	uint8_t Count_i = 0 , KeyNum;

	if(( SysKLStr.ScanCount == 1 ) || ( SysKLStr.ScanCount == 3 ))
	{
		SysKLStr.KeyRAWData =  ( GPIOD->IDR & 0x000000f0 ) >> 4  ;
		for( Count_i = 0 ; Count_i < 4 ; Count_i ++ )
		{
			KeyNum = (( SysKLStr.ScanCount / 3 ) * 4 ) + Count_i;
			if( SysKLStr.KeyRAWData & ( 0x00000008 >> Count_i ))
			{
				SysKLStr.KeyPassCount[KeyNum] ++;
				if( SysKLStr.KeyPassCount[KeyNum] > SysKLStr.KeyPressShortCountLimt )
				{
					SysKLStr.KeyState |=  0x01 << KeyNum;
				}
			}
			else
			{
				SysKLStr.KeyPassCount[KeyNum] = 0; 
				SysKLStr.KeyState &= ~( 0x01 << KeyNum );
			}
		}
	}

	SysKLStr.ScanCount ++;
	if( SysKLStr.ScanCount >= 4 )
		SysKLStr.ScanCount = 0;

	GPIOD->BRR	|= 0x0000000f;
	GPIOD->BSRR |= 1U << SysKLStr.ScanCount ;

	switch( SysKLStr.ScanCount )
	{
		case 0: 
			GPIOD->CRL  &= 0x0000ffff;
			GPIOD->CRL  |= 0x77770000;
			GPIOD->ODR  |= 0x000000f0;
			GPIOD->ODR  &= ~(( SysKLStr.LEDState ) & 0xf0 );
			break;
		case 1:
			GPIOD->CRL  &= 0x0000ffff;
			GPIOD->CRL  |= 0x88880000;
			GPIOD->ODR  &= 0xffffff0f;			
			break;
		case 2:
			GPIOD->CRL  &= 0x0000ffff;
			GPIOD->CRL  |= 0x77770000;
			GPIOD->ODR  |= 0x000000f0;
			GPIOD->ODR  &= ~(( SysKLStr.LEDState << 4 ) & 0xf0 );
			break;
		case 3:
			GPIOD->CRL  &= 0x0000ffff;
			GPIOD->CRL  |= 0x88880000;
			GPIOD->ODR  &= 0xffffff0f;
			break;
	}
	
	if( GPIO_ReadInputDataBit( GPIOC , GPIO_Pin_4 ) == 0 )
	{
		if( SysKLStr.KeyHandleCount > SysKLStr.KeyHandleCountLimt )
		{
			SysKLStr.KeyHandle = KEY_PRESS;
		}
		else
		{
			SysKLStr.KeyHandle = KEY_RELEASE;
			SysKLStr.KeyHandleCount++;
		}
	}
	else
	{
		SysKLStr.KeyHandleCount = 0;
		SysKLStr.KeyHandle = KEY_RELEASE;
	}

	if( GPIO_ReadInputDataBit( GPIOE , GPIO_Pin_11 ) == 0 )
	{
		if( SysKLStr.IPLEDMountCount > 1500 )
		{
			SysKLStr.IPLEDMountState = MOUNTED;
		}
		else
		{
			SysKLStr.IPLEDMountState = MOUNTING;
			SysKLStr.IPLEDMountCount += 2;
		}
	}
	else
	{
		if( SysKLStr.IPLEDMountCount > 0 )
		{
			SysKLStr.IPLEDMountCount--;
		}
		else
		{
			SysKLStr.IPLEDMountCount = 0;
			SysKLStr.IPLEDMountState = UMOUNT;
		}
	}
}
