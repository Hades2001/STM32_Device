#include "IIC.h"

void IIC_Init(void);
void IIC_START(void);
void IIC_STOP(void);
void IIC_ACK(void);
void IIC_WriteBit(u8 IIC_Data);
u8 IIC_RradBit(void);
void IIC_Write_ADDData(u8 IIC_ICADD,u8 IIC_Data);
void IIC_Write_ADDREData(u8 IIC_ICADD,u8 IIC_READD,u8 IIC_Data);
void IIC_InitGPIO(GPIO_TypeDef* IIC_SDA_SetGPIO,uint16_t IIC_SDA_SetGPIO_Pin,GPIO_TypeDef* IIC_SCL_SetGPIO,uint16_t IIC_SCL_SetGPIO_Pin);


GPIO_TypeDef* IIC_SDA_GPIOx;
uint16_t IIC_SDA_GPIO_Pin;
GPIO_TypeDef* IIC_SCL_GPIOx;
uint16_t IIC_SCL_GPIO_Pin;

void IIC_InitGPIO(GPIO_TypeDef* IIC_SDA_SetGPIO,uint16_t IIC_SDA_SetGPIO_Pin,GPIO_TypeDef* IIC_SCL_SetGPIO,uint16_t IIC_SCL_SetGPIO_Pin)
{
	IIC_SDA_GPIOx=IIC_SDA_SetGPIO;
	IIC_SDA_GPIO_Pin=IIC_SDA_SetGPIO_Pin;
	IIC_SCL_GPIOx=IIC_SCL_SetGPIO;
	IIC_SCL_GPIO_Pin=IIC_SCL_SetGPIO_Pin;
}

void GPIO_SET(unsigned char Set_Data)
{
	
}
void IIC_Dleay(unsigned char x)
{
	unsigned char i;
	for(i=0;i<x;i++);
}

void IIC_Init(void)
{
  GPIO_WriteHigh(IIC_SDA);
  GPIO_WriteHigh(IIC_SCL);
}

void IIC_START(void)
{
 	GPIO_WriteHigh(IIC_SDA);
	IIC_Dleay(1);
  GPIO_WriteHigh(IIC_SCL);
	IIC_Dleay(1);
 	GPIO_WriteLow(IIC_SDA);
	IIC_Dleay(1);

}

void IIC_STOP(void)
{
 	GPIO_WriteLow(IIC_SDA);
	IIC_Dleay(1);
  GPIO_WriteHigh(IIC_SCL);
	IIC_Dleay(1);
 	GPIO_WriteHigh(IIC_SDA);
	IIC_Dleay(1);
}

void IIC_ACK(void)
{
  u8 i=0;

  GPIO_WriteHigh(IIC_SCL);
	IIC_Dleay(1);
  while(GPIO_ReadInputPin(IIC_SDA)&&(i<255))
  i++;
  GPIO_WriteLow(IIC_SCL);
  IIC_Dleay(1);
}

void IIC_WriteBit(u8 IIC_Data)
{
  u8 i;
	GPIO_WriteLow(IIC_SCL);
	IIC_Dleay(1);
  for(i=0;i<8;i++)
  {
    if((IIC_Data&0x80)==0x80)
    GPIO_WriteHigh(IIC_SDA);
    else
    GPIO_WriteLow(IIC_SDA);
    IIC_Data<<=1;
    //IIC_Dleay(2);
    GPIO_WriteHigh(IIC_SCL);
    IIC_Dleay(1);
		GPIO_WriteLow(IIC_SCL);
    IIC_Dleay(1);
  }
	GPIO_WriteHigh(IIC_SDA);
	IIC_Dleay(1);
}

u8 IIC_RradBit(void)
{
  u8 i,IIC_Read_Data;
	
	IIC_Dleay(1);
  GPIO_WriteHigh(IIC_SDA);
  IIC_Dleay(1);
  for(i=0;i<8;i++)
  {
    GPIO_WriteHigh(IIC_SCL);
    IIC_Dleay(1);
    IIC_Read_Data<<=1;
    if(GPIO_ReadInputPin(IIC_SDA))
    IIC_Read_Data|=0x01;
    GPIO_WriteLow(IIC_SCL);
    IIC_Dleay(1);
  }
  IIC_Dleay(1);
//  GPIO_Init(IIC_SDA, GPIO_MODE_OUT_PP_HIGH_FAST);
  return (IIC_Read_Data);

}

void IIC_Write_ADDData(u8 IIC_ICADD,u8 IIC_Data)
{
  IIC_START();
  IIC_WriteBit(IIC_ICADD);
  IIC_ACK();
  IIC_WriteBit(IIC_Data);
  IIC_ACK();
  IIC_STOP();
}

void IIC_Write_ADDREData(u8 IIC_ICADD,u8 IIC_READD,u8 IIC_Data)
{
  IIC_START();
  IIC_WriteBit(IIC_ICADD);
  IIC_ACK();
  IIC_WriteBit(IIC_READD);
  IIC_ACK();
  IIC_WriteBit(IIC_Data);
  IIC_ACK();
  IIC_STOP();
}
