#include "stm32f10x.h"

#include "LCD12864.h"
#include "CODETAB.h"

//sbit LCD_CS		=	P2^2;
//sbit LCD_RST	=	P2^3;
//sbit LCD_RS		=	P2^4;
//sbit LCD_SCK	=	P2^6;
//sbit LCD_SID	=	P2^5;

#define	LCD_CS			GPIOC , GPIO_Pin_3
#define	LCD_RST			GPIOC , GPIO_Pin_2
#define	LCD_RS			GPIOC , GPIO_Pin_1
#define	LCD_SCK			GPIOC , GPIO_Pin_0
#define	LCD_SID			GPIOC , GPIO_Pin_4



#define uchar 	unsigned char
#define uint 		unsigned int
#define ulong 	unsigned long

#define LCD_RS_DATA 	1
#define LCD_RS_CMD 		0

void LCD_12864_Init(void);
void LCD_WriteData(unsigned char X_Data,unsigned char Y_Data,unsigned char LCD_Data);
void LCD_DisChar(unsigned char X_Data,unsigned char Y_Data,unsigned char LCD_Data);
void LCD_DisSTring(unsigned char X_Data,unsigned char Y_Data,unsigned char *LCD_StringDataPrt);
void LCD_DisHEX(unsigned char X_Data,unsigned char Y_Data,unsigned char LCD_HEX_Data);
void LCD_DisnSTring(unsigned char X_Data,unsigned char Y_Data,unsigned char *LCD_StringDataPrt,unsigned char String_Num);
void LCD_ClearDis(void);

void Delay_LCD(unsigned char i)
{
		while( i != 0 )
		i--;
}


//******************************************************************
//
//		void Wirite_DataLCD(unsigned char RS_Data,unsigned char Data)
//
//		RS_Data = 0  --->   CMD
//		RS_Data = 1  --->   DATA
//
//******************************************************************
void Wirite_DataLCD(unsigned char RS_Data,unsigned char Data)
{
		unsigned char Count_i;

		GPIO_ResetBits( LCD_CS );//		LCD_CS = 0;
//		
		if( RS_Data == 1 )
		GPIO_SetBits( LCD_RS );
		else
		GPIO_ResetBits( LCD_RS );	
//		LCD_RS = RS_Data;
	
		for(Count_i=0;Count_i<8;Count_i++)
		{
				if( Data & 0x80 )
				GPIO_SetBits( LCD_SID );//				LCD_SID = 1;
				else
				GPIO_ResetBits( LCD_SID );//				LCD_SID = 0;
				
				Data <<= 1;
				
				GPIO_ResetBits( LCD_SCK );//				LCD_SCK = 0;
				Delay_LCD(5);
				GPIO_SetBits( LCD_SCK );//				LCD_SCK = 1;
				Delay_LCD(5);
		}
		
		GPIO_SetBits( LCD_CS );//		LCD_CS = 1;
		Delay_LCD(5);
}

void LCD_12864_Init(void)
{
		GPIO_ResetBits( LCD_RST );//		LCD_RST = 0;
		Delay_LCD(200);
		GPIO_SetBits( LCD_RST );//		LCD_RST = 1;
		Delay_LCD(200);

		Wirite_DataLCD(LCD_RS_CMD,0xE2);
		Delay_LCD(10);
	
		Wirite_DataLCD(LCD_RS_CMD,0x2C);
		Delay_LCD(10);
	
		Wirite_DataLCD(LCD_RS_CMD,0x2E);
		Delay_LCD(10);
	
		Wirite_DataLCD(LCD_RS_CMD,0x2F);
		Delay_LCD(10);
	
		Wirite_DataLCD(LCD_RS_CMD,0x23);
		Delay_LCD(10);
	
		Wirite_DataLCD(LCD_RS_CMD,0x81);
		Delay_LCD(10);
		
		Wirite_DataLCD(LCD_RS_CMD,0x15);
		Delay_LCD(10);
		
		Wirite_DataLCD(LCD_RS_CMD,0xA2);
		Delay_LCD(10);
		
		Wirite_DataLCD(LCD_RS_CMD,0xC8);
		Delay_LCD(10);
		
		Wirite_DataLCD(LCD_RS_CMD,0xA0);
		Delay_LCD(10);
		
		Wirite_DataLCD(LCD_RS_CMD,0x40);
		Delay_LCD(10);
		
		Wirite_DataLCD(LCD_RS_CMD,0xAF);
		Delay_LCD(10);

}

void LCD_SetADDR(unsigned char X_Data,unsigned char Y_Data)
{
		Wirite_DataLCD(LCD_RS_CMD,0xB0+Y_Data);

		Wirite_DataLCD(LCD_RS_CMD, (( X_Data>>4 )& 0x0F)+0x10 );
		Wirite_DataLCD(LCD_RS_CMD,	X_Data & 0x0F );
}

void LCD_WriteData(unsigned char X_Data,unsigned char Y_Data,unsigned char LCD_Data)
{
		LCD_SetADDR( X_Data , Y_Data );
		Wirite_DataLCD( LCD_RS_DATA , LCD_Data );
}


void LCD_ClearDis(void)
{
		unsigned char Count_X,Count_Y;

		for(Count_Y = 0 ; Count_Y < 9 ; Count_Y++ )
		{
				LCD_SetADDR( 0 , Count_Y );
			
				for(Count_X = 0 ; Count_X < 132 ; Count_X++ )
				Wirite_DataLCD( LCD_RS_DATA , 0x00 );
		}
}

void LCD_DisChar(unsigned char X_Data,unsigned char Y_Data,unsigned char LCD_Data)
{
		unsigned char Count_X;
		LCD_SetADDR( X_Data , Y_Data );
	
		for(Count_X = 0 ; Count_X < 6 ; Count_X++ )
		Wirite_DataLCD(  LCD_RS_DATA , F6x8[LCD_Data-0x20][Count_X] );
}

void LCD_DisSTring(unsigned char X_Data,unsigned char Y_Data,unsigned char *LCD_StringDataPrt)
{

		unsigned char Count_X;
		LCD_SetADDR( X_Data , Y_Data );
	
		while(*LCD_StringDataPrt != '\0')
		{
				for(Count_X = 0 ; Count_X < 6 ; Count_X++ )
				Wirite_DataLCD(  LCD_RS_DATA , F6x8[*LCD_StringDataPrt-0x20][Count_X] );
			
				LCD_StringDataPrt++;

		}
}

void LCD_DisnSTring(unsigned char X_Data,unsigned char Y_Data,unsigned char *LCD_StringDataPrt,unsigned char String_Num)
{

		unsigned char Count_X;
		LCD_SetADDR( X_Data , Y_Data );
	
		while(String_Num != 0 )
		{
				for(Count_X = 0 ; Count_X < 6 ; Count_X++ )
				Wirite_DataLCD(  LCD_RS_DATA , F6x8[*LCD_StringDataPrt-0x20][Count_X] );
			
				LCD_StringDataPrt++;
			
				String_Num --;

		}
}

void LCD_DisHEX(unsigned char X_Data,unsigned char Y_Data,unsigned char LCD_HEX_Data)
{
	
		unsigned char Data_H,Data_L;
		Data_L = LCD_HEX_Data & 0x0f;
		Data_H = LCD_HEX_Data >> 4;
	
		LCD_DisChar(X_Data,Y_Data,'0');
		LCD_DisChar(X_Data+6,Y_Data,'x');
	
		if(Data_H<10)
		LCD_DisChar(X_Data+12,Y_Data,Data_H+0x30);
		else
		LCD_DisChar(X_Data+12,Y_Data,Data_H-10+0x41);
		
		if(Data_L<10)
		LCD_DisChar(X_Data+18,Y_Data,Data_L+0x30);
		else
		LCD_DisChar(X_Data+18,Y_Data,Data_L-10+0x41);	
		LCD_DisChar(X_Data+24,Y_Data,' ');
}

void LCD_DisHEX32(unsigned char X_Data,unsigned char Y_Data,unsigned long LCD_HEX_Data)
{
		unsigned char Data[8];
		unsigned char i;
	
		for(i=0;i<8;i++)
		{
				Data[7-i]=LCD_HEX_Data&0x0f;
				LCD_HEX_Data >>= 4;
		}

		LCD_DisChar(X_Data,Y_Data,'0');
		LCD_DisChar(X_Data+6,Y_Data,'x');
		
		for(i=0;i<8;i++)
		{
				if(Data[i]<10)
				LCD_DisChar(X_Data+12+(i*6),Y_Data,Data[i]+0x30);
				else
				LCD_DisChar(X_Data+12+(i*6),Y_Data,Data[i]-10+0x41);
		}
}
