#include "TFT_LCD_Drv.h"
#include "font_ASCII.h"


void Print_Ascii(u16 x,u16 y,unsigned char Font,u16 FontColor,u16 BuckColor)
{
		u8 i,j,Fout_Data;
		BlockWrite(x,x+5,y,y+11);
		for(i=0;i<12;i++)
		{
			Fout_Data=Assic6x12[Font-32][i];
			for(j=0;j<6;j++)
			{
					if(Fout_Data&0x01)
					*(__IO u16 *) (Bank1_LCD_D) = FontColor;
					else
					*(__IO u16 *) (Bank1_LCD_D) = BuckColor;
					Fout_Data>>=1;
			}
		}
}

void Print_AsciiTPT(u16 x,u16 y,unsigned char Font,u16 FontColor)
{
		u8 i,j,Fout_Data;
		BlockWrite(x,x+5,y,y+11);
		for(i=0;i<12;i++)
		{
			Fout_Data=Assic6x12[Font-32][i];
			for(j=0;j<6;j++)
			{
					if(Fout_Data&0x01)
					*(__IO u16 *) (Bank1_LCD_D) = FontColor;
					else
					*(__IO u16 *) (Bank1_LCD_D) = GetPoint(x+j,y+i);
					Fout_Data>>=1;
			}
		}
}


void Print_String(u16 x,u16 y,unsigned char *String,u16 FontColor,u16 BuckColor)
{
	u16 Str_x;
	while(*String!='\0')
	{
		Print_Ascii(x+(Str_x*6),y,*String,FontColor,BuckColor);
		String++;
		Str_x++;
	}
}

void Print_StringTPT(u16 x,u16 y,unsigned char *String,u16 FontColor)
{
	u16 Str_x;
	while(*String!='\0')
	{
// 		Print_Ascii(x+(Str_x*6),y,*String,FontColor,GetPoint(x+(Str_x*6),y));
		Print_AsciiTPT(x+(Str_x*6),y,*String,FontColor);
		String++;
		Str_x++;
	}
}

u16 TransparencyCalculation(u16 FrontColor,u16 BackColor,u8 TPT)
{
		u16 FrontColor_R,FrontColor_G,FrontColor_B,BackColor_R,BackColor_G,BackColor_B;
		u16 ResultColor=0x0000;
	
		FrontColor_R=FrontColor&0x001f;
		FrontColor_G=(FrontColor>>5)&0x003f;
		FrontColor_B=(FrontColor>>11)&0x001f;

		BackColor_R=BackColor&0x001f;
		BackColor_G=(BackColor>>5)&0x003f;
		BackColor_B=(BackColor>>11)&0x001f;
	
 		BackColor_R=((BackColor_R*TPT)/100+(FrontColor_R*(100-TPT)/100))&0x001f;
 		BackColor_G=((BackColor_G*TPT)/100+(FrontColor_G*(100-TPT)/100))&0x003f;
 		BackColor_B=((BackColor_B*TPT)/100+(FrontColor_B*(100-TPT)/100))&0x001f;

		ResultColor|= BackColor_R;
		ResultColor|=(BackColor_G<<5);
		ResultColor|=(BackColor_B<<11);
	
		return ResultColor;
}

void Print_BOXTPT(u16 x,u16 y,u16 xlong,u16 ylong,u16 Color,u8 TPT)
{
	u16 X_Data,Y_Data;
	BlockWrite(x,x+xlong,y,y+ylong);
	for(Y_Data=0;Y_Data<=ylong;Y_Data++)
	for(X_Data=0;X_Data<=xlong;X_Data++)
	*(__IO u16 *) (Bank1_LCD_D) = Color;
// 	*(__IO u16 *) (Bank1_LCD_D) = TransparencyCalculation(Color,GetPoint(x+X_Data,y+Y_Data),TPT);
}

