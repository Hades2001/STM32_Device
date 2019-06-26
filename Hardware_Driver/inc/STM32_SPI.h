#ifndef __STM32_SPI_H
#define __STM32_SPI_H		

#include "stm32f10x.h"

#define SPI1_CS1 GPIOA, GPIO_Pin_4

void SPI1_Setclock(u8 SPI_Prescaler); //设置SPI1时钟速度  
void SPI2_Setclock(u8 SPI_Prescaler);
u8 SPI1_ReadWriteByte(u8 writeData);
u8 SPI2_ReadWriteByte(u8 writeData);


#endif
