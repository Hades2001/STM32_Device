#include "stm32f10x.h"
#include "WS2812.h"

void DisWs2812(unsigned char GData,unsigned char RData,unsigned char BData);

void Delay_WS2812(unsigned char n)
{
		while(n!=0)
		n--;
}

void WS2812_S(void)
{
		GPIOB->BSRR=0x0008;
		Delay_WS2812(2);
		GPIOB->BRR=0x0008;
		Delay_WS2812(1);
}

void WS2812_R(void)
{
		GPIOB->BSRR=0x0008;
		Delay_WS2812(1);
		GPIOB->BRR=0x0008;
		Delay_WS2812(2);
}

void DisWs2812(unsigned char GData,unsigned char RData,unsigned char BData)
{
		unsigned char i;
		__disable_irq();
		for(i=0;i<8;i++)
		{
				if((GData&0x80)==0x80)
				WS2812_S();
				else
				WS2812_R();
				GData<<=1;
		}
		for(i=0;i<8;i++)
		{
				if((RData&0x80)==0x80)
				WS2812_S();
				else
				WS2812_R();
				RData<<=1;
		}		
		for(i=0;i<8;i++)
		{
				if((BData&0x80)==0x80)
				WS2812_S();
				else
				WS2812_R();
				BData<<=1;
		}
		
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
		Delay_WS2812(200);
		__enable_irq();
// 		GPIO_SetBits(GPIOB,GPIO_Pin_3);
// 		DelayUs(100);
}


