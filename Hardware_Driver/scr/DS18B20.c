#include "DS18B20.h"


//void Wire_Rest(void)
//{
//	__disable_irq();   // ?????
//    GPIO_ResetBits(DS18B20_Data);
//    DelayUs(600);
//    GPIO_SetBits(DS18B20_Data);
//    DelayUs(50);
//    GPIO_SetBits(DS18B20_Data);
//	__enable_irq();    // ?????
//}

u8 Wire_Rest(void)
{
	u8 	Res 	= 0 ;
	u16 Count	= 0 ;
	
	__disable_irq();   // ?????
    GPIO_ResetBits(DS18B20_Data);
    DelayUs(600);
    GPIO_SetBits(DS18B20_Data);
    DelayUs(60);
    Res =  GPIO_ReadInputDataBit(DS18B20_Data);
	
    while(( GPIO_ReadInputDataBit(DS18B20_Data) != 1 )&&( Count <= 5000 ))
    {
		Count ++;
		DelayUs(5);
    }
	
	__enable_irq();    // ?????

	return Res;
}


void Wire_WriteH(void)
{
    GPIO_ResetBits(DS18B20_Data);
    DelayUs(2);
    GPIO_SetBits(DS18B20_Data);
    DelayUs(50);
}

void Wire_WriteL(void)
{
    GPIO_ResetBits(DS18B20_Data);
    DelayUs(30);
    GPIO_SetBits(DS18B20_Data);
	DelayUs(25);

}

unsigned char Wire_Read(void)
{
    u8 i=0;
	
    GPIO_ResetBits(DS18B20_Data);
	DelayUs(2);
    GPIO_SetBits(DS18B20_Data);
	DelayUs(2);
    if( GPIO_ReadInputDataBit(DS18B20_Data) )
    i=1;
    else
    i=0;
	DelayUs(60);
    GPIO_SetBits(DS18B20_Data);
    return i;
}

unsigned char Wire_ReadByte(void)
{
    u8 i=0,Data=0;
	__disable_irq();   // ?????
    for(i=0;i<8;i++)
    {
        Data >>= 1;
        if(Wire_Read())
        Data |= 0x80;
    } 
	__enable_irq();    // ?????
    return Data;
}

void Wire_SendByte(u8 Data)
{
    u8 i=0;
		__disable_irq();   // ?????
    for(i=0;i<8;i++)
    {
          if(Data & 0x01)
          Wire_WriteH();
          else
          Wire_WriteL();
          
          Data >>= 1;
    }  
		__enable_irq();    // ?????
}

void DS18B20_StartTemp(void)
{
	Wire_Rest();
	Wire_SendByte(0xCC);
	Wire_SendByte(0x44);
}

u16 DS18B20_ReadTemp(void)
{
	u16 MSB = 0 , LSB = 0;
	Wire_Rest();
	Wire_SendByte(0xCC);
	Wire_SendByte(0xBE);
	
	LSB =  Wire_ReadByte();
    MSB =  Wire_ReadByte();

	MSB = ( MSB << 8 ) | LSB;
	
	return MSB;
}

//void Wire_WriteREG8Bit(u8 REGAddress,u8 Data)
//{
//		DS2781_Rest();
//		DS2781SendByte(0xCC);
//		DS2781SendByte(0x6C);
//		DS2781SendByte(REGAddress);
//		DS2781SendByte(Data);
//}

//void DS2781WriteREG16Bit(u8 REGAddress,u16 Data)
//{
//		DS2781_Rest();
//		DS2781SendByte(0xCC);
//		DS2781SendByte(0x6C);
//		DS2781SendByte(REGAddress);
//		DS2781SendByte(Data>>8);
//		DS2781SendByte(Data);
//}

//u8 DS2781Read1Byte(u8 REGAddress)
//{
//		u8 ReData=0;
//	
//    DS2781_Rest();
//    DS2781SendByte(0xCC);
//    DS2781SendByte(0x69);
//		DS2781SendByte(REGAddress);
//    ReData=ReadByte();
//	
//		return ReData;
//}
//u16 DS2781Read16Bit(u8 REGAddress)
//{
//		u16 ReData=0;
//	
//    DS2781_Rest();
//    DS2781SendByte(0xCC);
//    DS2781SendByte(0x69);
//		DS2781SendByte(REGAddress);
//    ReData=ReadByte();
//    ReData <<= 8;
//    ReData |= ReadByte();
//	
//		return ReData;
//}

//void CopyDataEEPROM(u8 REGAddress)
//{
//		DS2781_Rest();
//		DS2781SendByte(0xCC);
//		DS2781SendByte(0x48);
//		DS2781SendByte(REGAddress);
//}

//void CopyDataRAM(u8 REGAddress)
//{
//		DS2781_Rest();
//		DS2781SendByte(0xCC);
//		DS2781SendByte(0xB8);
//		DS2781SendByte(REGAddress);
//}








