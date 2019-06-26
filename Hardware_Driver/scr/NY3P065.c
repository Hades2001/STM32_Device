#include "NY3P065.H"
#include "delay.H"

void Set_Sound(unsigned char Sound_Num);
void Set_ACK_Sound(unsigned char Sound_Num);
void Stop_Sound(void);


void Set_Sound(unsigned char Sound_Num)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	DelayUs(200);
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	DelayUs(200);
	while(Sound_Num!=0)
	{
			GPIO_SetBits(GPIOB, GPIO_Pin_4);
			DelayUs(200);
			GPIO_ResetBits(GPIOB, GPIO_Pin_4);
			DelayUs(200);
			Sound_Num--;
	}

}

void Set_ACK_Sound(unsigned char Sound_Num)
{
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)==0);
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	DelayUs(200);
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	DelayUs(200);
	while(Sound_Num!=0)
	{
			GPIO_SetBits(GPIOB, GPIO_Pin_4);
			DelayUs(200);
			GPIO_ResetBits(GPIOB, GPIO_Pin_4);
			DelayUs(200);
			Sound_Num--;
	}
}

void Stop_Sound(void)
{
	
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	DelayUs(200);
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	DelayUs(200);
	
}
