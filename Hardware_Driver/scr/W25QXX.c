#include "W25QXX.h"
 
u16 W25QXX_ID;	 
													 
//��ʼ��SPI FLASH��IO��
void W25QXX_Init(void)
{ 
	SPI2_ReadWriteByte(0xff);//��������	
	SPI2_Setclock(SPI_BaudRatePrescaler_2); 
	W25QXX_ID=W25QXX_ReadID();	              //��ȡW25QXX_ID.
}  
/****************************************************************************
* ��    ��: u8 W25QXX_ReadSR(void) 
* ��    �ܣ���ȡW25QXX��״̬�Ĵ���
* ��ڲ�������
* ���ز�����״̬�Ĵ�����ֵ
* ˵    ���� 		     
****************************************************************************/
u8 W25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                                  //ʹ������   
	SPI2_ReadWriteByte(W25X_CMD_ReadStatusReg);   //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI2_ReadWriteByte(0Xff);                //��ȡһ���ֽ�  
	GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;                                  //ȡ��Ƭѡ     
	return byte;   
} 
/****************************************************************************
* ��    ��: void W25QXX_Write_SR(u8 sr)
* ��    �ܣ�дW25QXX״̬�Ĵ���
* ��ڲ�����д���ֵ
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Write_SR(u8 sr)   
{   
	GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                                 //ʹ������   
	SPI2_ReadWriteByte(W25X_CMD_WriteStatusReg); //����дȡ״̬�Ĵ�������    
	SPI2_ReadWriteByte(sr);                      //д��һ���ֽ�  
	GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;                                 //ȡ��Ƭѡ     	      
} 

//W25QXXдʹ��	 
void W25QXX_Write_Enable(void)   
{
	GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                               //ʹ������   
  SPI2_ReadWriteByte(W25X_CMD_WriteEnable);  //����дʹ��  
	GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;                               //ȡ��Ƭѡ     	      
} 
//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);      //�ȴ�BUSYλ���
} 
//W25QXXд��ֹ	 
void W25QXX_Write_Disable(void)   
{  
	GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                                //ʹ������   
  SPI2_ReadWriteByte(W25X_CMD_WriteDisable);  //����д��ָֹ��    
	GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;                                //ȡ��Ƭѡ     	      
} 
/****************************************************************************
* ��    ��: u16 W25QXX_ReadID(void)
* ��    �ܣ���ȡоƬID
* ��ڲ�������
* ���ز�����оƬID
* ˵    ����0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
            0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
            0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
            0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
            0XEF17,��ʾоƬ�ͺ�ΪW25Q128      
****************************************************************************/  
u16 W25QXX_ReadID(void)
{
	u16 IDnum = 0;	  
	GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;				    
	SPI2_ReadWriteByte(0x90); //���Ͷ�ȡID����	    
	SPI2_ReadWriteByte(0x00); 	    
	SPI2_ReadWriteByte(0x00); 	    
	SPI2_ReadWriteByte(0x00); 	 			   
	IDnum|=SPI2_ReadWriteByte(0xFF)<<8;  
	IDnum|=SPI2_ReadWriteByte(0xFF);	 
	GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;				    
	return IDnum;
} 
/****************************************************************************
* ��    ��: void W25QXX_Erase_Chip(void) 
* ��    �ܣ���������оƬ		  
* ��ڲ�������
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                   
    W25QXX_Wait_Busy();   
  	GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                             //ʹ������   
    SPI2_ReadWriteByte(W25X_CMD_ChipErase);  //����Ƭ��������  
	  GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;                             //ȡ��Ƭѡ     	      
	  W25QXX_Wait_Busy();   				           //�ȴ�оƬ��������
}  
/****************************************************************************
* ��    ��: void W25QXX_Erase_Sector(u32 First_Addr)
* ��    �ܣ�����ĳ���������׵�ַ	  
* ��ڲ�����First_Addr:������ַ
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Erase_Sector(u32 First_Addr)   
{    
 	  First_Addr*=4096;
    W25QXX_Write_Enable();                  
    W25QXX_Wait_Busy();   
  	GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                                  //ʹ������   
    SPI2_ReadWriteByte(W25X_CMD_SectorErase);     //������������ָ�� 
    SPI2_ReadWriteByte((u8)((First_Addr)>>16));   //���͵�ַ    
    SPI2_ReadWriteByte((u8)((First_Addr)>>8));   
    SPI2_ReadWriteByte((u8)First_Addr);  
	  GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;                                  //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				                //�ȴ��������
}  
//�������ģʽ
void W25QXX_PowerDown(void)   
{ 
  	GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                             //ʹ������   
    SPI2_ReadWriteByte(W25X_CMD_PowerDown);  //���͵�������  
	  GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;                             //ȡ��Ƭѡ     	      
                         
}   
//����
void W25QXX_WAKEUP(void)   
{  
  	GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                                      //ʹ������   
    SPI2_ReadWriteByte(W25X_CMD_ReleasePowerDown);    //���ͻ�������
	  GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;                                      //ȡ��Ƭѡ     	                                         
}   

u8 W25QXX_ReadByte(u32 StartAddress)
{
		u8 Data;
	
	  GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                                 //ʹ������   
    SPI2_ReadWriteByte(W25X_CMD_ReadData);     //���Ͷ�ȡ����   
    SPI2_ReadWriteByte((u8)((StartAddress)>>16));  //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((StartAddress)>>8));   
    SPI2_ReadWriteByte((u8)StartAddress);   
	
		Data = SPI2_ReadWriteByte(0XFF);
		GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1; 

		return Data;
}

/****************************************************************************
* ��    ��: void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
* ��    �ܣ�ָ����ַ��ʼ��ȡָ�����ȵ�����
* ��ڲ�����DataBuffer:���ݴ洢��
            StartAddress:��ʼ��ȡ�ĵ�ַ(24bit)
            ByteCount:Ҫ��ȡ���ֽ���(���65535)
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Read(u8* DataBuffer,u32 StartAddress,u16 ByteCount)   
{  										    
	  GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                                 //ʹ������   
    SPI2_ReadWriteByte(W25X_CMD_ReadData);     //���Ͷ�ȡ����   
    SPI2_ReadWriteByte((u8)((StartAddress)>>16));  //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((StartAddress)>>8));   
    SPI2_ReadWriteByte((u8)StartAddress);   
	
			while (ByteCount--) 
			{
					*DataBuffer = SPI2_ReadWriteByte(0XFF);
					DataBuffer++;
			}		
	GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;  				    	      
}  
/****************************************************************************
* ��    ��: void W25QXX_Write_Page(u8* DataBuffer,u32 StartAddress,u16 ByteCount)
* ��    �ܣ���һҳ��д������256���ֽڵ�����  
* ��ڲ�����DataBuffer:���ݴ洢��
            StartAddress:��ʼд��ĵ�ַ(24bit)
            ByteCount:Ҫд����ֽ���(���256)
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Write_Page(u8* DataBuffer,u32 StartAddress,u16 ByteCount)
{  
	  W25QXX_Write_Enable();                   
	  GPIO_ResetBits(SPI_W25Q_nCS);//W25QXX_CS=0;                                   //ʹ������   
    SPI2_ReadWriteByte(W25X_CMD_PageProgram);      //����дҳ����   
    SPI2_ReadWriteByte((u8)((StartAddress)>>16));  //���͵�ַ    
    SPI2_ReadWriteByte((u8)((StartAddress)>>8));   
    SPI2_ReadWriteByte((u8)StartAddress);   
		while (ByteCount--)
			{
			  SPI2_ReadWriteByte(*DataBuffer);
			  DataBuffer++;
			}	
	GPIO_SetBits(SPI_W25Q_nCS);//W25QXX_CS=1;                   //ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   //�ȴ�д�����
}
/****************************************************************************
* ��    ��: void W25QXX_PageWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount) 
* ��    �ܣ���ҳд������
* ��ڲ�����DataBuffer:���ݴ洢��
            StartAddress:��ʼд��ĵ�ַ(24bit)
            ByteCount:Ҫд����ֽ���(���256)
* ���ز�������
* ˵    �������Զ���ҳ���ܣ���Ϊ�ú���û����д֮ǰ�ж�Ҫд��ĵ�ַ�ϵ������Ƿ�Ϊ
            �գ�������д��֮ǰ����ȷ���õ�ַ�ϵ�����Ϊ��0xFF������д��ʧ�� 		     
****************************************************************************/
void W25QXX_PageWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount)   
{
  u8 NumOfPage = 0, NumOfSingle = 0, Surplus_Addr = 0, Surplus_count = 0, midtemp = 0;

  Surplus_Addr = StartAddress % 256;
  Surplus_count = 256 - Surplus_Addr;
  NumOfPage =  ByteCount / 256;
  NumOfSingle = ByteCount % 256;

  if (Surplus_Addr == 0) //��ʼд�ĵ�ַ�պ���ҳ��ʼ�ĵ�ַ
  {
    if (NumOfPage == 0)  // ByteCount < 256��һҳ�ܵ��ֽ�����  
    {
      W25QXX_Write_Page(DataBuffer, StartAddress, ByteCount);
    }
    else                 // ByteCount > 256��һҳ�ܵ��ֽ�����  
    {
      while (NumOfPage--)
      {
        W25QXX_Write_Page(DataBuffer, StartAddress, 256);
        StartAddress +=  256;
        DataBuffer += 256;
      }
      W25QXX_Write_Page(DataBuffer, StartAddress, NumOfSingle);
    }
  }
  else ///��ʼд�ĵ�ַ����ҳ���׵�ַ��
  {
    if (NumOfPage == 0) // ByteCount < 256��һҳ�ܵ��ֽ����� 
    {
      if (NumOfSingle > Surplus_count)  
      {
        midtemp = NumOfSingle - Surplus_count;
        W25QXX_Write_Page(DataBuffer, StartAddress, Surplus_count);
        StartAddress +=  Surplus_count;
        DataBuffer += Surplus_count;
        W25QXX_Write_Page(DataBuffer, StartAddress, midtemp);
      }
      else
      {
        W25QXX_Write_Page(DataBuffer, StartAddress, ByteCount);
      }
    }
    else //ByteCount > 256��һҳ�ܵ��ֽ�����  
    {
      ByteCount -= Surplus_count;
      NumOfPage =  ByteCount / 256;
      NumOfSingle = ByteCount % 256;

      W25QXX_Write_Page(DataBuffer, StartAddress, Surplus_count);
      StartAddress +=  Surplus_count;
      DataBuffer += Surplus_count;
      while (NumOfPage--)
      {
        W25QXX_Write_Page(DataBuffer, StartAddress, 256);
        StartAddress +=  256;
        DataBuffer += 256;
      }
      if (NumOfSingle != 0)
      {
        W25QXX_Write_Page(DataBuffer, StartAddress, NumOfSingle);
      }
    }
  }
} 
/****************************************************************************
* ��    ��: void W25QXX_SectorWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount)
* ��    �ܣ�������д������
* ��ڲ�����DataBuffer:���ݴ洢��
            StartAddress:��ʼд��ĵ�ַ(24bit)
            ByteCount:Ҫд����ֽ���(���65536)
* ���ز�������
* ˵    ����д��֮ǰ�жϸĵ�ַ�ϵ������Ƿ�Ϊ�գ������Ȳ�����д�룬�ú����ο����ϴ���http://www.openedv.com      
****************************************************************************/
// u8 TS_BUFFER[4096];	
// void W25QXX_SectorWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount)   
// { 
// 	u32 secaddr;
// 	u16 secused;
// 	u16 Surplus_count;	   
//  	u16 i=0; 
// 	
// 	u8 * Cache_BUF;	   
//   Cache_BUF=TS_BUFFER;	  
// 	
//  	secaddr=StartAddress/4096;//������ַ  
// 	secused=StartAddress%4096;//д��ĵ�ַ�������ڵ�ƫ�ƣ��Ӹ��������׵�ַ��ʼ��ʹ�õ��ֽ�����
// 	Surplus_count=4096-secused;//������ʣ��ռ��С   

//  	if(ByteCount<=Surplus_count)Surplus_count=ByteCount;//������4096���ֽ�
// 	while(1) 
// 	{	
// 		W25QXX_Read(Cache_BUF,secaddr*4096,4096);//������������������
// 		      i=0;
// 		while(Cache_BUF[secused+i]==0XFF)
// 				{
// 					i++;
// 					if(i==Surplus_count)break;
// 				}
// 		if(i<Surplus_count)//��Ҫ����
// 				{
// 					W25QXX_Erase_Sector(secaddr);//�����������
// 					for(i=0;i<Surplus_count;i++)	   //����
// 					{
// 						Cache_BUF[i+secused]=DataBuffer[i];	  
// 					}
// 					W25QXX_PageWrite(Cache_BUF,secaddr*4096,4096);//д����������  

// 				}
// 		else
// 			  W25QXX_PageWrite(DataBuffer,StartAddress,Surplus_count);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
// 		if(ByteCount==Surplus_count)break;//д�������
// 		else//д��δ����
// 				{
// 					secaddr++;//������ַ��1
// 					secused=0;//ƫ��λ��Ϊ0 	 

// 					DataBuffer+=Surplus_count;  //ָ��ƫ��
// 					StartAddress+=Surplus_count;//д��ַƫ��	   
// 					ByteCount-=Surplus_count;				//�ֽ����ݼ�
// 					if(ByteCount>4096)Surplus_count=4096;	//��һ����������д����
// 					else Surplus_count=ByteCount;			//��һ����������д����
// 				}	 
// 	}
// }
