#include "IIC.h"
#include "IIC1602.h"

void IIC1602_Dleay(unsigned char x)
{
	unsigned char i,j;
	for(i=0;i<x;i++)
	for(j=0;j<255;j++);
}

void IIC1602_WriteCMD(u8 Data)
{
    u8 SData=0x00,DataH,DataL;
    DataL=Data;
    DataH=Data;
    
    DataL<<=4;
    DataH&=0xf0;
    
    SData|=0x08;  //LED=1
    SData&=~0x01; //RS=0 CDM
    SData&=~0x02;  //RW=0
    IIC_Write_ADDData(0x4e,SData);
    IIC1602_Dleay(2);
    SData|=0x04;  //EN=1
    SData|=DataH;
    IIC_Write_ADDData(0x4e,SData);
    IIC1602_Dleay(2);
    SData&=~0x04;  //EN=0
    IIC_Write_ADDData(0x4e,SData);
    IIC1602_Dleay(2);  
    SData&=0x0f;
    SData|=0x04;  //EN=1
    SData|=DataL;
    IIC_Write_ADDData(0x4e,SData);
    IIC1602_Dleay(2);
    SData&=~0x04;  //EN=0
    IIC_Write_ADDData(0x4e,SData);
    IIC1602_Dleay(2);
}

void IIC1602_WriteData(u8 Data)
{
    u8 SData=0x00,DataH,DataL;
    DataL=Data;
    DataH=Data;
    
    DataL<<=4;
    DataH&=0xf0;
    
    SData|=0x08;  //LED=1
    SData|=0x01; //RS=1 Data
    SData&=~0x02;  //RW=0
    IIC1602_Dleay(2);
    IIC_Write_ADDData(0x4e,SData);
    SData|=0x04;  //EN=1
    SData|=DataH;
    IIC_Write_ADDData(0x4e,SData);
    IIC1602_Dleay(2);
    SData&=~0x04;  //EN=0
    IIC_Write_ADDData(0x4e,SData);
    IIC1602_Dleay(2);  
    SData&=0x0f;
    SData|=0x04;  //EN=1
    SData|=DataL;
    IIC_Write_ADDData(0x4e,SData);
    IIC1602_Dleay(2);
    SData&=~0x04;  //EN=0
    IIC_Write_ADDData(0x4e,SData);
    IIC1602_Dleay(2);
}

void IIC1602Init(void)
{
  
  IIC1602_WriteCMD(0x28);
  IIC1602_WriteCMD(0x28);
  IIC1602_WriteCMD(0x28);
  
  IIC1602_WriteCMD(0x01);
  IIC1602_WriteCMD(0x02);
  IIC1602_WriteCMD(0x06);
  IIC1602_WriteCMD(0x0c);
}

void IIC1602CLR(void)
{
  IIC1602_WriteCMD(0x01);
}

void IIC1602_WChar(u8 hang,u8 lie,u8 Data)
{
  u8 IIC1602ACMD;
  if(hang==1)
  IIC1602ACMD=0x80+lie-1;  
  if(hang==2)
  IIC1602ACMD=0xC0+lie-1;  

  IIC1602_WriteCMD(IIC1602ACMD);
  IIC1602_WriteData(Data);
}

void IIC1602_WString(u8 hang,u8 lie,char *string)
{
  u8 IIC1602ACMD;
  if(hang==1)
  IIC1602ACMD=0x80+lie-1;  
  if(hang==2)
  IIC1602ACMD=0xC0+lie-1; 
  IIC1602_WriteCMD(IIC1602ACMD);
  while(*string!='\0')
  {
    IIC1602_WriteData(*string);
    string++;
  }
}

void IIC1602_Dis3Num(u8 hang,u8 lie,u16 LCD1602_Num)
{
  IIC1602_WChar(hang,lie,  LCD1602_Num/100%10+0x30);
  IIC1602_WChar(hang,lie+1,LCD1602_Num/10%10+0x30);
  IIC1602_WChar(hang,lie+2,LCD1602_Num%10+0x30);
}

