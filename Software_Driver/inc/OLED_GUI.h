/***************************************************
*
*			Function List:
*
*
*
*
*
*
****************************************************/
#ifndef __OLED_GUI_H
#define __OLED_GUI_H

#include "stm32f10x.h"
#include "OLED_I2C.h"

#define TextBoxStr 			struct TextBOX
#define NumBoxStr				struct NumBox
#define ProrsStr 				struct ProrsBar
#define ICOBoxStr 			struct ICOBox
#define MenuListStr			struct MenuList	
#define RollMenuStr 		struct RollMenu		
#define RollTextBoxStr 	struct RollTextBox

#define Font8x6					0
#define Font16x8				1

#define HideEnable			1
#define HideDisable			0

extern const unsigned char F6x8[][6];
extern const unsigned char F8X16[];
#if		DisPlayBuff	== 1

#if		DisPlayMode	== 0
extern unsigned char DisPlayBuff_Size[DisPlayYLimit][DisPlayXLimit/(8/DisPlayBuff)];
#endif
#if		DisPlayMode	== 1
extern unsigned char DisPlayBuff_Size[DisPlayYLimit/(8/DisPlayBuff)][DisPlayXLimit];
#endif
void CleanDisPlayBuff(void);
void DisPlayPrintBuff(void);


#endif

//文本显示结构体

TextBoxStr
{
	unsigned int XPos;
	unsigned int YPos;
	
	unsigned char* TextBOXBuff;
	
	unsigned char Underline;					//下划线
	unsigned char Highlight;					//高亮（反色）
	
	unsigned char Font;								//
	
	unsigned char Hide;								//隐藏 
};


RollTextBoxStr
{
	unsigned int XPos;
	unsigned int YPos;
	
	unsigned char MemberNum;							//显示的菜单数量
	
	unsigned char* TextBOXBuff;
	
	unsigned char *String[20];	
	
	unsigned char Underline;					//下划线
	unsigned char Highlight;					//高亮（反色）
	
	unsigned char Font;								//
	
	unsigned char Hide;								//隐藏 
};

//8*8 Block 显示结构体

ICOBoxStr
{
	unsigned int XPos;
	unsigned int YPos;
	
	unsigned char* ICOBOXBuff;

	unsigned char Highlight;				//高亮
	
	unsigned char Hide;							//隐藏
};

NumBoxStr
{
	unsigned int XPos;
	unsigned int YPos;
	
	unsigned char Underline;					//下划线
	unsigned char Highlight;					//高亮（反色）
	
	unsigned char Font;								//
	
	long NumVal;
	
	unsigned char Hide;								//隐藏 

};

//进度条机构体
ProrsStr
{
	unsigned int XPos;
	unsigned int YPos;

	unsigned int XSize;								//进度条最大宽度
	
	unsigned char ProgressVla;				//当前进度条的百分比 0-100
	
	unsigned char DisNum;							//是否显示百分比
	
	unsigned char Hide;								//隐藏
};

//菜单结构体
MenuListStr
{
	unsigned int XPos;
	unsigned int YPos;

	unsigned int XSize;										//菜单的最大宽度
	
	unsigned char MemberNum;							//显示的菜单数量
	
	unsigned char DisMAXMemberNum;				//最大菜单数量
	
	unsigned char Pointer;								//高亮指示
	
	unsigned char *String[100];						//菜单的字符串
	
	unsigned char MenuVla;								//显示的第一条字符串
	
	unsigned char Hide;										//隐藏
};

RollMenuStr
{
	
	unsigned int XPos;
	unsigned int YPos;
	
	unsigned int XSize;										//菜单的最大宽度
	
	unsigned char DisMAXMemberNum;				//最大菜单数量
	
	unsigned char *String[20];						//菜单的字符串
	
	unsigned char Pointer;								//高亮指示
	
	unsigned char MenuVla;								//显示的第一条字符串
	
	unsigned char Hide;										//隐藏
};


extern void OLED_Power_OFF(void);
extern void OLED_Power_ON(void);
extern void OLED_DisString(unsigned int Startx,unsigned int Starty,unsigned char *BuffData);
extern void OLED_DisHightString(unsigned int Startx,unsigned int Starty,unsigned char *BuffData);
extern void OLED_DisTextBoxStr(TextBoxStr *TextBoxPrt);
extern void OLED_DisICOBoxStr(ICOBoxStr *AltBoxPrt);
extern void OLED_DisNumBoxStr(NumBoxStr *AltBoxPrt);
extern void DisRollTextBoxStr(RollTextBoxStr *AltBoxPrt);
extern void OLED_DisProrsStr(ProrsStr *AltBoxPrt);
extern void OLED_DisMenuList(MenuListStr *AltBoxPrt);
extern void OLED_Write_nData(unsigned char *Data,unsigned int DataNum);
extern void OLED_SetPost(unsigned int x,unsigned int y);

#endif /* ____OLED_GUI_H */
