#include "VFDSPI.h"


// #define SPI_CS   GPIOA, GPIO_Pin_5
// #define SPI_CLK  GPIOA, GPIO_Pin_6
// #define SPI_Data GPIOA, GPIO_Pin_7
// #define GPIO_WriteLow        GPIO_ResetBits
// #define GPIO_WriteHigh       GPIO_SetBits

void SPI_SendData8Bit(u8 SPI_SendData)
{
		unsigned char i;
		GPIO_WriteHigh(SPI_CLK);
		for(i=0;i<8;i++)
		{
				if((SPI_SendData&0x80)==0x80)
				GPIO_WriteHigh(SPI_Data);
				else
				GPIO_WriteLow(SPI_Data);
				SPI_SendData<<=1;
				GPIO_WriteLow(SPI_CLK);
				GPIO_WriteHigh(SPI_CLK);
		}
}

void SPI_SendAllData(u8 *ProData,u8 DataNum)
{
		unsigned int i;
		GPIO_WriteHigh(SPI_CS);
		GPIO_WriteLow(SPI_CS);
		for(i=0;i<DataNum;i++)
		{
				SPI_SendData8Bit(*ProData);
				ProData++;
		}
		GPIO_WriteHigh(SPI_CS);
}
