#ifndef __IIC_H
#define __IIC_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

#define IIC_SDA  IIC_SDA_GPIOx, IIC_SDA_GPIO_Pin
#define IIC_SCL  IIC_SCL_GPIOx, IIC_SCL_GPIO_Pin

#define IIC_OLED_SDA  GPIOA, GPIO_Pin_0
#define IIC_OLED_SCL  GPIOA, GPIO_Pin_1     

#define GPIO_WriteLow        GPIO_ResetBits
#define GPIO_WriteHigh       GPIO_SetBits
#define GPIO_ReadInputPin    GPIO_ReadInputDataBit


extern void IIC_Init(void);
extern void IIC_START(void);
extern void IIC_STOP(void);
extern void IIC_ACK(void);
extern void IIC_WriteBit(u8 IIC_Data);
extern u8 IIC_RradBit(void);
extern void IIC_Write_ADDData(u8 IIC_ICADD,u8 IIC_Data);
extern void IIC_Write_ADDREData(u8 IIC_ICADD,u8 IIC_READD,u8 IIC_Data);
extern void IIC_InitGPIO(GPIO_TypeDef* IIC_SDA_SetGPIO,uint16_t IIC_SDA_SetGPIO_Pin,GPIO_TypeDef* IIC_SCL_SetGPIO,uint16_t IIC_SCL_SetGPIO_Pin);


#endif /* ____IIC_H */
