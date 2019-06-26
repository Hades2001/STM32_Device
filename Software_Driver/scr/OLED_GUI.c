/*****************************************************
*
*				Copyright (c), 2016, Hades2001.
*				All rights reserved.
*				Email:my_hades2001@163.com
*
* 			File name: OLED_GUI.C
*				Processor: STM32F103C8T6
* 			Compiler : MDK fo ARM
*
* 			Author : Hades2001
* 			Version: 1.00
* 			Date   : 2016.12.28
* 			Email  : my_hades2001@163.com
* 			Modification: none
*
*				Description:
*
*
*
*				Vession information:
*
*
*
*
*
****************************************************/
#include "OLED_GUI.h"

#define DisPlayXLimit  128
#define DisPlayYLimit	 64
#define DisPlay1Point  1

#define Block_XSize		8
#define Block_YSize		8

#define DisPlayMode 1
#define DisPlayBuff 1

unsigned char OLEDSysICO_Arrow_Down[8]={0x00,0x06,0x1E,0x7E,0x7E,0x1E,0x06,0x00};
unsigned char OLEDSysICO_Arrow_UP[8]={0x00,0x60,0x78,0x7E,0x7E,0x78,0x60,0x00};
unsigned char OLEDSysICO_Arrow_Right[8]={0x00,0x7E,0x7E,0x3C,0x3C,0x18,0x18,0x00};
unsigned char OLEDSysICO_Arrow_Blank[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char OLED_BlockBuff[Block_XSize];

void OLED_Write_nData(unsigned char *Data,unsigned int DataNum);
void OLED_Power_OFF(void);
void OLED_Power_ON(void);
void OLED_SetPost(unsigned int x,unsigned int y);
void OLED_DisString(unsigned int Startx,unsigned int Starty,unsigned char *BuffData);
void OLED_DisHightString(unsigned int Startx,unsigned int Starty,unsigned char *BuffData);
void OLED_DisTextBoxStr(TextBoxStr *TextBoxPrt);
void OLED_DisICOBoxStr(ICOBoxStr *AltBoxPrt);
void OLED_DisNumBoxStr(NumBoxStr *AltBoxPrt);
void DisRollTextBoxStr(RollTextBoxStr *AltBoxPrt);
void OLED_DisProrsStr(ProrsStr *AltBoxPrt);
void OLED_DisMenuList(MenuListStr *AltBoxPrt);

/**************    Application program interface        ***********/


void OLED_Power_OFF(void)
{
		OLED_OFF();
}

void OLED_Power_ON(void)
{
		OLED_ON();
}

/***************************************************
*
*			Name:	OLED_DisPlayInit
*			
*			Parameter:			Nothing
*			Return value:	0:Init Success	1:Fail
*			Notes:					Init LCD Function 
*
***************************************************/


unsigned char OLED_DisPlayInit(void)
{
 	OLED_Init();

 	return 0;
}

/***************************************************
*
*			Name:	OLED_DisPlayClean
*			
*			Parameter:			Nothing
*			Return value:	0:Success	1:Fail
*			Notes:					Clean LCD Display Function 
*
***************************************************/

unsigned char OLED_DisPlayClean(void)
{
 	OLED_CLS();	

 	return 0;
}

/***************************************************
*
*			Name:	OLED_DisPlayPoint
*			
*			Parameter:			x:Display X-coordinate,y:Display Y-coordinate
*			Return value:	Nothing
*			Notes:					Display a point 
*
***************************************************/

void OLED_DisPlayPoint(unsigned int x,unsigned int y)
{
	;
}

/***************************************************
*
*			Name:	OLED_SetPost
*			
*			Parameter:			x:Display X-coordinate,y:Display Y-coordinate
*			Return value:	Nothing
*			Notes:					Set LCD display positions 
*
***************************************************/
void OLED_SetPost(unsigned int x,unsigned int y)
{
 		OLED_SetPos(x,y);
}

/***************************************************
*
*			Name:	OLED_Write_OneData
*			
*			Parameter:			Data:Write in Data
*			Return value:	Nothing
*			Notes:					Display one Data
*
***************************************************/
void OLED_Write_OneData(unsigned char Data)
{
 		WriteDat(Data);
}


/***************************************************
*
*			Name:	OLED_Write_nData
*			
*			Parameter:			*Data: Data Pointer  DataNum: Number of data
*			Return value:	Nothing
*			Notes:					Display n Datas
*
***************************************************/

void OLED_Write_nData(unsigned char *Data,unsigned int DataNum)
{
		while(DataNum!=0)
		{
				OLED_Write_OneData(*Data);
				Data++;
				DataNum--;
		}
}

void OLEDFillBuff(unsigned char *Data)
{
		unsigned char i,j;
	
		for(i=0;i<8;i++)
		{
				OLED_SetPost(0,i);
				for(j=0;i<128;j++)
				{
						OLED_Write_OneData(*Data);
						Data++;
				}

		}
}

/***************************************************
*
*			Name:	OLED_WriteBlock
*			
*			Parameter:			Startx:Display X-coordinate,Starty:Display Y-coordinate
*											*BuffData: Data Pointer 
*			Return value:	Nothing
*			Notes:					Display n Datas
*
***************************************************/

void OLED_WriteBlock(unsigned int Startx,unsigned int Starty,unsigned char *BuffData)
{
		OLED_SetPost(Startx,Starty);
		OLED_Write_nData(BuffData,Block_XSize);
// 		for(i=0;i<Block_XSize;i++)
// 		{
// 				Transfer_data(*BuffData);
// 				BuffData++;
// 		}
}

void OLED_DisChar(unsigned int Startx,unsigned int Starty,unsigned char BuffData)
{
			unsigned char i=0;
			OLED_SetPost(Startx,Starty);
			for(i=0;i<6;i++)
			OLED_Write_OneData((F6x8[BuffData-32][i]));
}

void OLED_DisString(unsigned int Startx,unsigned int Starty,unsigned char *BuffData)
{
 		unsigned char i=0;
// 		unsigned int  SavaStartx;
// 	
// 		SavaStartx=Startx;
	
		while(*BuffData!='\0')
		{	
// 				if(*BuffData==0x0D)
// 				BuffData++;
// 				else if(*BuffData==0x0A)
// 				{
// 						Starty++;
// 						Startx=SavaStartx;
// 						OLED_SetPost(Startx,Starty);
// 				}
				
				OLED_SetPost(Startx,Starty);
				for(i=0;i<6;i++)
				OLED_Write_OneData((F6x8[*BuffData-32][i]));
				Startx+=6;
				BuffData++;
		}
}
void OLED_DisHightString(unsigned int Startx,unsigned int Starty,unsigned char *BuffData)
{
 		unsigned char i=0;
// 		unsigned int  SavaStartx;
// 		SavaStartx=Startx;
		while(*BuffData!='\0')
		{	
// 				if(*BuffData=='\r')
// 				BuffData++;
// 				else if(*BuffData=='\n')
// 				{
// 						Starty++;
// 						Startx=SavaStartx;
// 				}
				OLED_SetPost(Startx,Starty);
				for(i=0;i<6;i++)
				OLED_Write_OneData(~(F6x8[*BuffData-32][i]));
				Startx+=6;
				BuffData++;
		}
}


/****************************************API Function*********************************************/

/***************************************************
*
*			Name:	OLED_WriteBlock
*			
*			Parameter:			*TextBoxPrt: Structure pointer
*			Return value:	Nothing
*			Notes:					Display TextBox
*
***************************************************/
void OLED_DisTextBoxStr(TextBoxStr *TextBoxPrt)
{
 		unsigned char i=0,j=0;
		unsigned int Startx,Starty;
	
		Startx=TextBoxPrt->XPos;
		Starty=TextBoxPrt->YPos;
	
		while(*TextBoxPrt->TextBOXBuff!='\0')
		{	
				if(TextBoxPrt->Font==0)
				{
						OLED_SetPost(Startx,Starty);
						for(i=0;i<6;i++)
						{
								if(TextBoxPrt->Hide!=1)
								{
										if(TextBoxPrt->Highlight==1)
										OLED_Write_OneData(~(F6x8[*TextBoxPrt->TextBOXBuff-32][i]));						
										else if((TextBoxPrt->Underline==1)&&(TextBoxPrt->Highlight!=1))
										OLED_Write_OneData((F6x8[*TextBoxPrt->TextBOXBuff-32][i])|0x80);	
										else
										OLED_Write_OneData((F6x8[*TextBoxPrt->TextBOXBuff-32][i]));										
								}
								else
								OLED_Write_OneData(0x00);	
						}
						Startx+=6;
						TextBoxPrt->TextBOXBuff++;
						j++;
				}
				else if(TextBoxPrt->Font==1)
				{
						OLED_SetPost(Startx,Starty);
						for(i=0;i<8;i++)
						{
								if(TextBoxPrt->Hide!=1)
								{
										if(TextBoxPrt->Highlight==1)
										OLED_Write_OneData(~(F8X16[(*TextBoxPrt->TextBOXBuff-32)*16+i]));						
										else
										OLED_Write_OneData((F8X16[(*TextBoxPrt->TextBOXBuff-32)*16+i]));										
								}
								else
								OLED_Write_OneData(0x00);	
						}
						OLED_SetPost(Startx,Starty+1);
						for(i=0;i<8;i++)
						{
								if(TextBoxPrt->Hide!=1)
								{
										if(TextBoxPrt->Highlight==1)
										OLED_Write_OneData(~(F8X16[(*TextBoxPrt->TextBOXBuff-32)*16+i+8]));						
										else if((TextBoxPrt->Underline==1)&&(TextBoxPrt->Highlight!=1))
										OLED_Write_OneData((F8X16[(*TextBoxPrt->TextBOXBuff-32)*16+i+8])|0x80);	
										else
										OLED_Write_OneData((F8X16[(*TextBoxPrt->TextBOXBuff-32)*16+i+8]));										
								}
								else
								OLED_Write_OneData(0x00);	
						}						
						Startx+=8;
						TextBoxPrt->TextBOXBuff++;
						j++;
				}
				
		}
		TextBoxPrt->TextBOXBuff-=j;
}

/***************************************************
*
*			Name:	OLED_DisICOBoxStr
*			
*			Parameter:			*AltBoxPrt: Structure pointer
*			Return value:	Nothing
*			Notes:					Display ICOBox
*
***************************************************/

void OLED_DisICOBoxStr(ICOBoxStr *AltBoxPrt)
{
 		unsigned char i=0;
		unsigned int Startx,Starty;
	
		Startx=AltBoxPrt->XPos;
		Starty=AltBoxPrt->YPos;
	
		OLED_SetPost(Startx,Starty);
		for(i=0;i<8;i++)
		{
				if(AltBoxPrt->Hide!=1)
				{
						if(AltBoxPrt->Highlight==1)
						OLED_Write_OneData(~*AltBoxPrt->ICOBOXBuff);
						else
						OLED_Write_OneData(*AltBoxPrt->ICOBOXBuff);
				}
				else
				OLED_Write_OneData(0x00);	
		
				AltBoxPrt->ICOBOXBuff++;
		}
		AltBoxPrt->ICOBOXBuff-=8;
}

// NumBoxStr
// {
// 	unsigned int XPos;
// 	unsigned int YPos;
// 	
// 	unsigned char Underline;					//下划线
// 	unsigned char Highlight;					//高亮（反色）
// 	
// 	unsigned char Font;								//
// 	
// 	long NumVal;
// 	
// 	unsigned char Hide;								//隐藏 

// };
// 	char Num_Buf[10]={0,0,0,0,0,0,0,0,0,0};
// 	unsigned char i=0,n=0;
// 	while(Num_Data/10!=0)
// 	{     
// 			Num_Buf[9-i]=Num_Data%10;
// 			Num_Data/=10;
// 			i++;
// 	}
// 	Num_Buf[9-i]=Num_Data%10;
// 	i++;
// 	for(n=0;n<i;n++)
// 	TxBitData(Num_Buf[10-i+n]+0x30);

void OLED_DisNumBoxStr(NumBoxStr *AltBoxPrt)
{
 		unsigned char i=0,n=0,NumFlag=0;
		long Num_Data;
		unsigned int Startx,Starty;
		unsigned char Num_Buf[10]={0,0,0,0,0,0,0,0,0,0};

		Startx=AltBoxPrt->XPos;
		Starty=AltBoxPrt->YPos;
		OLED_SetPost(Startx,Starty);
		
		OLED_DisString(Startx,Starty,"          ");

		Num_Data=AltBoxPrt->NumVal;
		if(Num_Data<0)
		{
				Num_Data=-Num_Data;
				NumFlag=1;
		}

		while(Num_Data/10!=0)
		{     
				Num_Buf[9-i]=Num_Data%10;
				Num_Data/=10;
				i++;
		}
		Num_Buf[9-i]=Num_Data%10;
		i++;
		
		if(NumFlag==1)
		{
				OLED_DisChar(Startx,Starty,'-');
				Startx+=6;
		}
		
		for(n=0;n<i;n++)
		OLED_DisChar(Startx+n*6,Starty,Num_Buf[10-i+n]+0x30);
		

}
// RollTextBoxStr
// {
// 	unsigned int XPos;
// 	unsigned int YPos;
// 	
// 	unsigned char MemberNum;							//显示的菜单数量
// 	
// 	unsigned char* TextBOXBuff;
// 	
// 	unsigned char *String[20];	
// 	
// 	unsigned char Underline;					//下划线
// 	unsigned char Highlight;					//高亮（反色）
// 	
// 	unsigned char Font;								//
// 	
// 	unsigned char Hide;								//隐藏 
// };
void DisRollTextBoxStr(RollTextBoxStr *AltBoxPrt)
{
		unsigned int Startx,Starty;
		unsigned char i=0;
	
		Startx=AltBoxPrt->XPos;
		Starty=AltBoxPrt->YPos;
		OLED_SetPost(Startx,Starty);
	
// 		while(*BuffData!='\0')
// 		{	
// 				if(*BuffData=='\r')
// 				BuffData++;
// 				else if(*BuffData=='\n')
// 				Starty++;
// 				OLED_SetPost(Startx,Starty);
// 				for(i=0;i<6;i++)
// 				OLED_Write_OneData((F6x8[*BuffData-32][i]));
// 				Startx+=6;
// 				BuffData++;
// 		}

}

/***************************************************
*
*			Name:	OLED_DisProrsStr
*			
*			Parameter:			*AltBoxPrt: Structure pointer
*			Return value:	Nothing
*			Notes:					Display DisPrors
*
***************************************************/

void OLED_DisProrsStr(ProrsStr *AltBoxPrt)
{	
 		unsigned char i=0;
		unsigned int Startx,Starty;
		Startx=AltBoxPrt->XPos;
		Starty=AltBoxPrt->YPos;
		OLED_SetPost(Startx,Starty);
		
		for(i=0;i<=AltBoxPrt->XSize;i++)
		{
				if(AltBoxPrt->Hide!=1)
				{
						if(i==0||(i==AltBoxPrt->XSize)||(i<=((AltBoxPrt->XSize*AltBoxPrt->ProgressVla)/100)))
						OLED_Write_OneData(0xff);
						else 
						{
								OLED_Write_OneData(0x81);	
						}
				}
				else
				OLED_Write_OneData(0x00);	
		}
		if(AltBoxPrt->DisNum==1)
		{
				if(AltBoxPrt->Hide!=1)
				{
						OLED_DisChar(Startx+AltBoxPrt->XSize+6,Starty,AltBoxPrt->ProgressVla/100%10+0x30);
						OLED_DisChar(Startx+AltBoxPrt->XSize+12,Starty,AltBoxPrt->ProgressVla/10%10+0x30);
						OLED_DisChar(Startx+AltBoxPrt->XSize+18,Starty,AltBoxPrt->ProgressVla/1%10+0x30);
						OLED_DisChar(Startx+AltBoxPrt->XSize+24,Starty,'%');
				}
				else
				OLED_DisString(Startx+AltBoxPrt->XSize+6,Starty,"    ");
		}
		
}

/***************************************************
*
*			Name:	OLED_DisMenuList
*			
*			Parameter:			*AltBoxPrt: Structure pointer
*			Return value:	Nothing
*			Notes:					Display MenuList
*
***************************************************/

void OLED_DisMenuList(MenuListStr *AltBoxPrt)
{
 		unsigned char i=0;
		unsigned int Startx,Starty;
		Startx=AltBoxPrt->XPos;
		Starty=AltBoxPrt->YPos;
		for(i=0;i<(AltBoxPrt->DisMAXMemberNum);i++)
		{
				if(AltBoxPrt->Pointer==i)
				OLED_DisHightString(Startx+10,Starty+i,AltBoxPrt->String[i+AltBoxPrt->MenuVla]);
				else
				OLED_DisString(Startx+10,Starty+i,AltBoxPrt->String[i+AltBoxPrt->MenuVla]);
				
		}
		
		if(AltBoxPrt->MenuVla<(AltBoxPrt->MemberNum-AltBoxPrt->DisMAXMemberNum))
		OLED_WriteBlock(Startx,Starty+AltBoxPrt->DisMAXMemberNum-1,OLEDSysICO_Arrow_Down);
		else
		OLED_WriteBlock(Startx,Starty+AltBoxPrt->DisMAXMemberNum-1,OLEDSysICO_Arrow_Blank);
		
		if(AltBoxPrt->MenuVla!=0)
		OLED_WriteBlock(Startx,Starty,OLEDSysICO_Arrow_UP);
		else
		OLED_WriteBlock(Startx,Starty,OLEDSysICO_Arrow_Blank);
		
}
// RollMenuStr
// {
// 	
// 	unsigned int XPos;
// 	unsigned int YPos;
// 	
// 	unsigned int XSize;										//???????
// 	
// 	unsigned char DisMAXMemberNum;				//??????
// 	
// 	unsigned char *String[20];						//??????
// 	
// 	unsigned char Pointer;								//????
// 	
// 	unsigned char MenuVla;								//?????????
// 	
// 	unsigned char Hide;										//??
// }

unsigned char RollMenuDisBuff[2][DisPlayXLimit];

void OLED_RollMenu(RollMenuStr *AltBoxPrt)
{
		;

}
/**************                 End                    ***********/


#if		DisPlayBuff	== 1

#if		DisPlayMode	== 0
unsigned char DisPlayBuff_Size[DisPlayYLimit][DisPlayXLimit/(8/DisPlayBuff)];


#endif

#if		DisPlayMode	== 1
unsigned char DisPlayBuff_Size[DisPlayYLimit/(8/DisPlayBuff)][DisPlayXLimit];


#endif
void CleanDisPlayBuff(void)
{
		unsigned int x,y;
		for(y=0;y<DisPlayYLimit;y++)
		for(x=0;x<DisPlayXLimit;x++)
		DisPlayBuff_Size[y][x]=0x00;
}

void DisPlayPrintBuff(void)
{	
// 		OLED_FillBuff(DisPlayBuff_Size);
// 		unsigned char x=0,y=0;
// 		for(y=0;y<DisPlayYLimit/(8/DisPlayBuff);y++)
// 		{
// 				OLED_SetPos(0,y);
// 				for(x=0;x<DisPlayXLimit;x++)
// 				WriteDat(DisPlayBuff_Size[y][x]);
// 		}
}

#endif

