#ifndef LCD12864_H
#define LCD12864_H


extern void LCD_12864_Init(void);
extern void LCD_WriteData(unsigned char X_Data,unsigned char Y_Data,unsigned char LCD_Data);
extern void LCD_DisChar(unsigned char X_Data,unsigned char Y_Data,unsigned char LCD_Data);
extern void LCD_DisSTring(unsigned char X_Data,unsigned char Y_Data,unsigned char *LCD_StringDataPrt);
extern void LCD_DisnSTring(unsigned char X_Data,unsigned char Y_Data,unsigned char *LCD_StringDataPrt,unsigned char String_Num);
extern void LCD_DisHEX(unsigned char X_Data,unsigned char Y_Data,unsigned char LCD_HEX_Data);
extern void LCD_DisHEX32(unsigned char X_Data,unsigned char Y_Data,unsigned long LCD_HEX_Data);
extern void LCD_ClearDis(void);


#endif 
