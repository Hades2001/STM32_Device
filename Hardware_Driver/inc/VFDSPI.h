#ifndef __VFDSPI_H
#define __VFDSPI_H

#include "stm32f10x.h"
#include "delay.h"

#define SPI_CS   GPIOA, GPIO_Pin_5
#define SPI_CLK  GPIOA, GPIO_Pin_6
#define SPI_Data GPIOA, GPIO_Pin_7

#define GPIO_WriteLow        GPIO_ResetBits
#define GPIO_WriteHigh       GPIO_SetBits

void SPI_SendData8Bit(u8 SPI_SendData);
void SPI_SendAllData(u8 *ProData,u8 DataNum);
#endif /* ____VFDSPI_H */
