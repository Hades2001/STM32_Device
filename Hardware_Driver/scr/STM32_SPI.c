#include "STM32_SPI.h"

void SPI1_Setclock(u8 SpeedSet)
{
// 	
// 		SPI_InitTypeDef  SPI_InitStructure;

// 		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
// 		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
// 		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
// 		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
// 		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
// 		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
// 		SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet;		//定义波特率预分频的值:波特率预分频值为256
// 		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
// 		SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
// 		SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
// 		SPI_Cmd(SPI1,ENABLE);
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SpeedSet=(SpeedSet<<3)&0x38;
	SPI1->CR1|=SpeedSet;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); 
} 

void SPI2_Setclock(u8 SPI_Prescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI_Prescaler=(SPI_Prescaler<<3)&0x38;
	SPI2->CR1|=SPI_Prescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 
} 


u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
 		    
}

u8 SPI2_ReadWriteByte(u8 writeData)
{		 			 
   u8 waitnum=0;
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){} //等待发送区空  
	      {  
				    waitnum++;  
				    if(waitnum>200)return 0;  
				}
	SPI_I2S_SendData(SPI2, writeData);  //通过外设SPI1发送一个字节
		
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //等待一个字节接收完 
        {  
				    waitnum++;  
				    if(waitnum>200)return 0;  
				}
	return SPI_I2S_ReceiveData(SPI2);  //返回SPI1接收的数据	
 		    
}


