#ifndef __TFT_LCD_DRV_H
#define __TFT_LCD_DRV_H

#include "Tiky_LCD.h" 


void Print_Ascii(u16 x,u16 y,unsigned char Font,u16 FontColor,u16 BuckColor);
void Print_String(u16 x,u16 y,unsigned char *String,u16 FontColor,u16 BuckColor);
void Print_StringTPT(u16 x,u16 y,unsigned char *String,u16 FontColor);
u16 TransparencyCalculation(u16 FrontColor,u16 BackColor,u8 TPT);
void Print_BOXTPT(u16 x,u16 y,u16 xlong,u16 ylong,u16 Color,u8 TPT);
#endif /* __TFT_LCD_DRV_H */
