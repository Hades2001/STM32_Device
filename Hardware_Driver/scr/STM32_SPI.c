#include "STM32_SPI.h"

void SPI1_Setclock(u8 SpeedSet)
{
// 	
// 		SPI_InitTypeDef  SPI_InitStructure;

// 		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
// 		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
// 		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
// 		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
// 		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
// 		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
// 		SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
// 		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
// 		SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
// 		SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
// 		SPI_Cmd(SPI1,ENABLE);
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SpeedSet=(SpeedSet<<3)&0x38;
	SPI1->CR1|=SpeedSet;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); 
} 

void SPI2_Setclock(u8 SPI_Prescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI_Prescaler=(SPI_Prescaler<<3)&0x38;
	SPI2->CR1|=SPI_Prescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI2,ENABLE); 
} 


u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
 		    
}

u8 SPI2_ReadWriteByte(u8 writeData)
{		 			 
   u8 waitnum=0;
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){} //�ȴ���������  
	      {  
				    waitnum++;  
				    if(waitnum>200)return 0;  
				}
	SPI_I2S_SendData(SPI2, writeData);  //ͨ������SPI1����һ���ֽ�
		
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�һ���ֽڽ����� 
        {  
				    waitnum++;  
				    if(waitnum>200)return 0;  
				}
	return SPI_I2S_ReceiveData(SPI2);  //����SPI1���յ�����	
 		    
}


