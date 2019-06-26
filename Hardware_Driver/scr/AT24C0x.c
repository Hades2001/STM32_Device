#include "AT24C0x.h"
#include "IIC.h"
#include "delay.h"

void AT24C0x_Write_1Bit(unsigned int Address,unsigned char Data);
void AT24C0x_Write_nBit(unsigned int Address,unsigned char Digit,unsigned char *Data);
void AT24C0x_Read_nBit(unsigned int Address,unsigned char Digit,unsigned char *Data);
unsigned char AT24C0x_Read_1Bit(unsigned int Address);

void AT24C0x_Write_1Bit(unsigned int Address,unsigned char Data)
{
	unsigned char Address_H,Address_L;
	
	Address = Address & 0x03FF;
	Address_L = Address;
	Address_H = (Address>>8)&0x00ff;
	
	Address_H = Address_H<<1;
	
//	Write_Address = Address_H | 0xA0;
	
	IIC_START();
	IIC_WriteBit(Address_H | 0xA0);
	IIC_ACK();
	IIC_WriteBit(Address_L);
	IIC_ACK();
	IIC_WriteBit(Data);
	IIC_ACK();
	IIC_STOP();
	DelayMs(1);
}

void AT24C0x_Write_nBit(unsigned int Address,unsigned char Digit,unsigned char *Data)
{
	
	unsigned char Address_H,Address_L;
	
	Address = Address & 0x03FF;
	Address_L = Address;
	Address_H = (Address>>8)&0x00ff;
	
	Address_H = Address_H<<1;
	
	IIC_START();
	IIC_WriteBit(Address_H | 0xA0);
	IIC_ACK();
	IIC_WriteBit(Address_L);
	IIC_ACK();
	
	while((Digit--)!=0)
	{
		IIC_WriteBit(*Data);
		IIC_ACK();
		Data++;
	}
	IIC_STOP();
	DelayMs(1);
}

unsigned char AT24C0x_Read_1Bit(unsigned int Address)
{
	unsigned char Address_H,Address_L,Read_Data;
	
	Address = Address & 0x03FF;
	Address_L = Address;
	Address_H = (Address>>8)&0x00ff;
	
	Address_H = Address_H<<1;
	
	//Write_Address = Address_H | 0xA1;
	
	IIC_START();
	IIC_WriteBit(Address_H | 0xA0);
	IIC_ACK();
	IIC_WriteBit(Address_L);
	IIC_ACK();
	IIC_START();
	IIC_WriteBit(Address_H | 0xA1);
	IIC_ACK();
	Read_Data=IIC_RradBit();
	IIC_ACK();
	IIC_STOP();
	DelayUs(50);
	return (Read_Data);
	
}

void AT24C0x_Read_nBit(unsigned int Address,unsigned char Digit,unsigned char *Data)
{
	
	while(Digit!=0)
	{
	*Data=AT24C0x_Read_1Bit(Address);
	Address++;
	Data++;
	Digit--;
	}
	
}
