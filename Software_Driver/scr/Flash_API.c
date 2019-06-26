#include "Flash_API.h"

extern unsigned int EC11_Count;

FIL fil;
FRESULT res;
UINT bww;
FILINFO filinfo;
DIR dirifo;

u8 Hex_Flash(void);
void TestMenu(void);
void WelcomeMenu(void)
{


}

void TestMenu(void)
{
// 		FIL fil;
// 		FILINFO filinfo;
// 		DIR dirifo;
// 		FRESULT res;
// 		UINT bww;
		u32 total,free;	
	
		TextBoxStr  CMDTXT;
	
		CMDTXT.TextBOXBuff=">                    ";
		CMDTXT.YPos=2;
		CMDTXT.XPos=0;
		CMDTXT.Underline=0;
		CMDTXT.Highlight=0;
		CMDTXT.Hide=0;
		
		OLED_CLS();
		OLED_DisTextBoxStr(&CMDTXT);
	
		TxBitData(0x0c);
		TxBitData(0x0c);
		
		f_mount(fs[0],"0:",1); 			
		
		if(exf_getfree("0",&total,&free))	
		{
				CMDTXT.TextBOXBuff=">SD Card Fatfs Error!";
// 				ErrFlag=1;	
		}		
		else		
		CMDTXT.TextBOXBuff=">SD Card OK    ";
		
		OLED_DisTextBoxStr(&CMDTXT);
		DelayMs(500);
		if(f_opendir(&dirifo,"TXT")!=FR_OK)
		{
				CMDTXT.TextBOXBuff=">OpenDir Error!";
// 				ErrFlag=2;	
		}
		else
		CMDTXT.TextBOXBuff=">OpenDir OK!   ";
		
		OLED_DisTextBoxStr(&CMDTXT);
		
		
		while(1)
		{
				res=f_readdir(&dirifo,&filinfo); 
				if(filinfo.fname[0]==0)	break; 
			
				CMDTXT.TextBOXBuff=filinfo.fname;
		
				OLED_DisTextBoxStr(&CMDTXT);
				TxStringData(filinfo.fname);
				TxStringData(" \r\n");
				DelayMs(500);

		}
}
   
u8 Ascii_Hex(u8 Ascii1,u8 Ascii2)
{
		u8 ChangeData=0;
		if(Ascii1<=0x39)
		ChangeData=((Ascii1-0x30)<<4)&0xf0;
		else
		ChangeData=((Ascii1-0x37)<<4)&0xf0;
		
		if(Ascii2<=0x39)
		ChangeData|=(Ascii2-0x30)&0x0f;
		else
		ChangeData|=(Ascii2-0x37)&0x0f;
		
		return ChangeData;
}

void WriteFlashCMD(u8 Address,u8 DataBuff)
{
		u8 FlashBuff[128];
		W25QXX_Read(FlashBuff,0,128);
 		W25QXX_Erase_Sector(0);
	
		FlashBuff[Address]=DataBuff;
	
		W25QXX_Write_Page(FlashBuff,0,128);
}

#define FlashStartAdd 0x00100000

u8 Hex_Flash(void)
{
// 		FIL fil;
// 		DIR dirifo;
// 		FRESULT res;
// 		UINT bww;
	
		u32 FlashAdd;
		u32 total,free;	
		u8 ErrFlag=0;
		u8 iCount=0;
	
		u8 ReadCMDBuff[4];
		u8 ReadHEXBuff[50];
	
 		u8 WriteHEXBuff[16];

		u8 DataNum;       
		u32 DataCount=0,HexCount=0,HexSize=0;
	
		NumBoxStr  	TestNumBox,TestNumBox2;
		TextBoxStr  DisPlayTXT,CMDTXT,SizeTXT,NewSize;
		ProrsStr 		DisPlayPrors;
	
		DisPlayTXT.TextBOXBuff="> TFCardBootloader <";
		DisPlayTXT.YPos=0;
		DisPlayTXT.XPos=0;
		DisPlayTXT.Underline=0;
		DisPlayTXT.Highlight=0;
		DisPlayTXT.Hide=0;

		CMDTXT.TextBOXBuff=">                    ";
		CMDTXT.YPos=2;
		CMDTXT.XPos=0;
		CMDTXT.Underline=0;
		CMDTXT.Highlight=0;
		CMDTXT.Hide=0;

		SizeTXT.TextBOXBuff=">HEXSize:";
		SizeTXT.YPos=4;
		SizeTXT.XPos=0;
		SizeTXT.Underline=0;
		SizeTXT.Highlight=0;
		SizeTXT.Hide=0;
		
		NewSize.TextBOXBuff=">NowSize:";
		NewSize.YPos=5;
		NewSize.XPos=0;
		NewSize.Underline=0;
		NewSize.Highlight=0;
		NewSize.Hide=0;
	
		TestNumBox.XPos=56;
		TestNumBox.YPos=4;
		TestNumBox.NumVal=0;
		
		TestNumBox2.XPos=56;
		TestNumBox2.YPos=5;
		TestNumBox2.NumVal=0;
		
		DisPlayPrors.XPos=3;
		DisPlayPrors.YPos=7;
		DisPlayPrors.XSize=80;
		DisPlayPrors.DisNum=1;
		DisPlayPrors.Hide=0;
		
		
		OLED_CLS();
		
		WriteFlashCMD(0x06,0x1A);
		
		OLED_DisTextBoxStr(&DisPlayTXT);
		OLED_DisTextBoxStr(&CMDTXT);		
	
		f_mount(fs[0],"0:",1); 			

		
		if(exf_getfree("0",&total,&free))	
		{
				CMDTXT.TextBOXBuff=">SD Card Fatfs Error!";
				ErrFlag=1;	
		}		
		else		
		CMDTXT.TextBOXBuff=">SD Card OK    ";
		OLED_DisTextBoxStr(&CMDTXT);
		DelayMs(500);
		
		if(f_opendir(&dirifo,"UpData/Hex")!=FR_OK)
		{
				CMDTXT.TextBOXBuff=">OpenDir Error!";
				ErrFlag=2;	
		}
		else
		CMDTXT.TextBOXBuff=">OpenDir OK!   ";
		OLED_DisTextBoxStr(&CMDTXT);
		DelayMs(500);
		
		res=f_open (&fil,"0:/UpData/Hex/Eproject.hex", FA_OPEN_EXISTING|FA_READ);	
		
		if(res!=FR_OK)
		{
				CMDTXT.TextBOXBuff=">OpenFill Error!";
				ErrFlag=3;	
		}
		else
		CMDTXT.TextBOXBuff=">OpenFill OK!   ";	
		OLED_DisTextBoxStr(&CMDTXT);
		DelayMs(500);
		
		CMDTXT.TextBOXBuff=">Start Flash    ";	
		OLED_DisTextBoxStr(&CMDTXT);
		
		for(iCount=0;iCount<128;iCount++)
		W25QXX_Erase_Sector(iCount+256);
		
		CMDTXT.TextBOXBuff=">Start Flash  OK";			
		OLED_DisTextBoxStr(&CMDTXT);
		
		OLED_DisTextBoxStr(&SizeTXT);
		OLED_DisTextBoxStr(&NewSize);		
		
		ReadCMDBuff[2]=0x31;
		while(Ascii_Hex(ReadCMDBuff[1],ReadCMDBuff[2])!=0)
		{
				f_read(&fil,ReadCMDBuff,3,&bww);
				DataCount+=3;
				DataNum=Ascii_Hex(ReadCMDBuff[1],ReadCMDBuff[2])*2+8+2;
				DataCount+=DataNum;
				if(Ascii_Hex(ReadCMDBuff[1],ReadCMDBuff[2])!=0)
				{
						f_read(&fil,ReadHEXBuff,DataNum,&bww);    
					
						if(Ascii_Hex(ReadHEXBuff[4],ReadHEXBuff[5])==0)
						{
								HexCount+=Ascii_Hex(ReadCMDBuff[1],ReadCMDBuff[2]);
								
						}				
				}
		}
		HexSize=HexCount;
		TestNumBox.NumVal=HexCount;
		OLED_DisNumBoxStr(&TestNumBox);
		
		DisPlayPrors.ProgressVla=0;
		OLED_DisProrsStr(&DisPlayPrors);
		
		f_close(&fil);
		res=f_open (&fil,"0:/UpData/Hex/Eproject.hex", FA_OPEN_EXISTING|FA_READ);	
		
		ReadCMDBuff[2]=0x31;
		
		DataCount=0;
		HexCount=0;
		FlashAdd=0;
		
 		while(Ascii_Hex(ReadCMDBuff[1],ReadCMDBuff[2])!=0)
		{
// 				OLED_CLS();
			
				f_read(&fil,ReadCMDBuff,3,&bww);
// 				DisPlayTXT.TextBOXBuff=ReadCMDBuff;
		
				DataCount+=3;
// 				OLED_DisTextBoxStr(&DisPlayTXT);
// 				DelayMs(500);
				DataNum=Ascii_Hex(ReadCMDBuff[1],ReadCMDBuff[2])*2+8+2;
				DataCount+=DataNum;
			
// 				TestNumBox.NumVal=DataNum;
// 				OLED_DisNumBoxStr(&TestNumBox);
				if(Ascii_Hex(ReadCMDBuff[1],ReadCMDBuff[2])!=0)
				{
						f_read(&fil,ReadHEXBuff,DataNum,&bww);    
					
						if(Ascii_Hex(ReadHEXBuff[4],ReadHEXBuff[5])==0)
						{
								FlashAdd=(((Ascii_Hex(ReadHEXBuff[0],ReadHEXBuff[1])<<8)&0xff00)|Ascii_Hex(ReadHEXBuff[2],ReadHEXBuff[3]))-0x5000;
								for(iCount=0;(iCount<(DataNum-10)/2);iCount++)
								{
										WriteHEXBuff[iCount]=Ascii_Hex(ReadHEXBuff[6+iCount*2],ReadHEXBuff[7+iCount*2]);
								}
								HexCount+=Ascii_Hex(ReadCMDBuff[1],ReadCMDBuff[2]);
								W25QXX_PageWrite(WriteHEXBuff,FlashAdd+FlashStartAdd,Ascii_Hex(ReadCMDBuff[1],ReadCMDBuff[2]));			
								
						}				
						TestNumBox2.NumVal=HexCount;
						OLED_DisNumBoxStr(&TestNumBox2);
						DisPlayPrors.ProgressVla=HexCount*100/HexSize;
						OLED_DisProrsStr(&DisPlayPrors);
				}
		}
		WriteFlashCMD(0x07,HexCount>>24);
		WriteFlashCMD(0x08,HexCount>>16);
		WriteFlashCMD(0x09,HexCount>>8);
		WriteFlashCMD(0x0A,HexCount);
		
		WriteFlashCMD(0x06,0x1C);
		
		
// 		TestNumBox.NumVal=DataNum;
// 		OLED_DisNumBoxStr(&TestNumBox);
// 		DisPlayTXT.TextBOXBuff=ReadHEXBuff;
// 		OLED_DisTextBoxStr(&DisPlayTXT);
		
// 		DisPlayTXT.TextBOXBuff="ReadFinish";
// 		OLED_DisTextBoxStr(&DisPlayTXT);
		return ErrFlag;
}


u8 Read8x8BMPFileToFlash(unsigned char *FileStr,unsigned long FlashAdd)
{
// 		FIL fil;
// 		FRESULT res;
// 		UINT bww;

		u8 ErrFlag=0;

		u8 j,k;
	
		unsigned char ReadBMPBuff[4]; 	
 		unsigned char BMPDisBuff[8];
 		unsigned char ChangeBuff=0,DataBuffE;
		
		res=f_open (&fil,FileStr, FA_OPEN_EXISTING|FA_READ);	
		if(res!=FR_OK)
		{
				ErrFlag=1;	
		}
 		else
		{
		f_lseek(&fil,0x3E);
		for(j=0;j<8;j++)
		{		
				f_read(&fil,ReadBMPBuff,4,&bww);
				ChangeBuff=ReadBMPBuff[0];
				for(k=0;k<8;k++)
				{
						if((ChangeBuff&0x80)==0x80)
						{			
								DataBuffE=0x80;
								DataBuffE>>=(j);
								BMPDisBuff[k]|=DataBuffE;
						}
						else
						{
								DataBuffE=0x80;
								DataBuffE>>=(j);
								BMPDisBuff[k]&=~DataBuffE;
						}
						ChangeBuff<<=1;
				}
		}
 		W25QXX_PageWrite(BMPDisBuff,FlashAdd,8);

		res=f_close(&fil);
		}
		return ErrFlag;
}


void DisPlay8x8Flash(unsigned long FlashAdd,unsigned char XData,unsigned char YData)
{
		unsigned char n;
		unsigned char BMPDisBuff[8];
		W25QXX_Read(BMPDisBuff,FlashAdd,8);
		OLED_SetPost(XData,YData+n);
		OLED_Write_nData(BMPDisBuff,8);
}

u8 Read16x16BMPFileToFlash(unsigned char *FileStr,unsigned long FlashAdd)
{
// 		FIL fil;
// 		FRESULT res;
// 		UINT bww;

		u8 ErrFlag=0;

		u8 j,k,l,n;
	
		unsigned char ReadBMPBuff[4]; 	
 		unsigned char BMPDisBuff[32];
 		unsigned char ChangeBuff=0,DataBuffE;
		
		res=f_open (&fil,FileStr, FA_OPEN_EXISTING|FA_READ);	
		if(res!=FR_OK)
		{
				ErrFlag=1;	
		}
 		else
		{
		f_lseek(&fil,0x3E);
		for(n=0;n<2;n++)
		{
				for(j=0;j<8;j++)
				{		
						f_read(&fil,ReadBMPBuff,4,&bww);
						for(l=0;l<2;l++)
						{
								ChangeBuff=ReadBMPBuff[l];
								for(k=0;k<8;k++)
								{
										if((ChangeBuff&0x80)==0x80)
										{			
												DataBuffE=0x80;
												DataBuffE>>=(j);
												BMPDisBuff[l*8+k]|=DataBuffE;
										}
										else
										{
												DataBuffE=0x80;
												DataBuffE>>=(j);
												BMPDisBuff[l*8+k]&=~DataBuffE;
										}
										ChangeBuff<<=1;
								}
						} 
				}	
// 		OLED_SetPost(0,1-n);
// 		OLED_Write_nData(BMPDisBuff,16);
 		W25QXX_PageWrite(BMPDisBuff,FlashAdd+(0x10-n*0x10),16);
		}
		res=f_close(&fil);
		}
		return ErrFlag;
}


void DisPlay16x16Flash(unsigned long FlashAdd,unsigned char XData,unsigned char YData)
{
		unsigned char n;
		unsigned char BMPDisBuff[16];
		for(n=0;n<2;n++)
		{
				W25QXX_Read(BMPDisBuff,FlashAdd+(n*0x10),16);
				OLED_SetPost(XData,YData+n);
				OLED_Write_nData(BMPDisBuff,16);
		}
}


u8 Read32x32BMPFileToFlash(unsigned char *FileStr,unsigned long FlashAdd)
{
// 		FIL fil;
// 		FRESULT res;
// 		UINT bww;

		u8 ErrFlag=0;

		u8 j,k,l,n;
	
		unsigned char ReadBMPBuff[4]; 	
 		unsigned char BMPDisBuff[32];
 		unsigned char ChangeBuff=0,DataBuffE;
		
		res=f_open (&fil,FileStr, FA_OPEN_EXISTING|FA_READ);	
	
		if(res!=FR_OK)
		{
				ErrFlag=1;	
		}
 		else
		{
		f_lseek(&fil,0x3E);
		for(n=0;n<4;n++)
		{
				for(j=0;j<8;j++)
				{		
						f_read(&fil,ReadBMPBuff,4,&bww);
						for(l=0;l<4;l++)
						{
								ChangeBuff=ReadBMPBuff[l];
								for(k=0;k<8;k++)
								{
										if((ChangeBuff&0x80)==0x80)
										{			
												DataBuffE=0x80;
												DataBuffE>>=(j);
												BMPDisBuff[l*8+k]|=DataBuffE;
										}
										else
										{
												DataBuffE=0x80;
												DataBuffE>>=(j);
												BMPDisBuff[l*8+k]&=~DataBuffE;
										}
										ChangeBuff<<=1;
								}
						} 
				}
				
 		W25QXX_PageWrite(BMPDisBuff,FlashAdd+(0x60-n*0x20),32);
		}
		res=f_close(&fil);
		}
		return ErrFlag;
}


void DisPlay32x32Flash(unsigned long FlashAdd,unsigned char XData,unsigned char YData)
{
		unsigned char n;
		unsigned char BMPDisBuff[32];
		for(n=0;n<4;n++)
		{
				W25QXX_Read(BMPDisBuff,FlashAdd+(n*0x20),32);
				OLED_SetPost(XData,YData+n);
				OLED_Write_nData(BMPDisBuff,32);
		}
}

void DisPlay32x32Null(unsigned char XData,unsigned char YData)
{
		unsigned char n;
		unsigned char BMPDisBuff[32];
		for(n=0;n<32;n++)
		BMPDisBuff[n]=0x00;
		for(n=0;n<4;n++)
		{
				OLED_SetPost(XData,YData+n);
				OLED_Write_nData(BMPDisBuff,32);
		}
}

void DisHighlightPlay32x32Flash(unsigned long FlashAdd,unsigned char XData,unsigned char YData)
{
		unsigned char n,i;
		unsigned char BMPDisBuff[32];
		for(n=0;n<4;n++)
		{
				W25QXX_Read(BMPDisBuff,FlashAdd+(n*0x20),32);
				for(i=0;i<32;i++)
				BMPDisBuff[i]=~BMPDisBuff[i];
				OLED_SetPost(XData,YData+n);
				OLED_Write_nData(BMPDisBuff,32);
		}
}

u8 Read64x64BMPFileToFlash(unsigned char *FileStr,unsigned long FlashAdd)
{

// 		FIL fil;
// 		FRESULT res;
// 		UINT bww;

		u8 ErrFlag=0;

		u8 i,j,k,l,n;
	
		unsigned char ReadBMPBuff[4]; 	
 		unsigned char BMPDisBuff[64];
 		unsigned char ChangeBuff=0,DataBuffE;
		
		res=f_open (&fil,FileStr, FA_OPEN_EXISTING|FA_READ);	
	
		if(res!=FR_OK)
		{
				ErrFlag=1;	
		}
 		else
		{
		
		f_lseek(&fil,0x3E);
		for(n=0;n<8;n++)
		{
				for(j=0;j<8;j++)
				{		
						for(i=0;i<2;i++)
						{
								f_read(&fil,ReadBMPBuff,4,&bww);
								for(l=0;l<4;l++)
								{
										ChangeBuff=ReadBMPBuff[l];
										for(k=0;k<8;k++)
										{
												if((ChangeBuff&0x80)==0x80)
												{			
														DataBuffE=0x80;
														DataBuffE>>=(j);
														BMPDisBuff[i*4*8+l*8+k]|=DataBuffE;
												}
												else
												{
														DataBuffE=0x80;
														DataBuffE>>=(j);
														BMPDisBuff[i*4*8+l*8+k]&=~DataBuffE;
												}
												ChangeBuff<<=1;
										}
								}
						} 
 				}
// 		OLED_SetPost(0,7-n);
// 		OLED_Write_nData(BMPDisBuff,64);
 		W25QXX_PageWrite(BMPDisBuff,FlashAdd+(0x1C0-n*0x40),64);
		}
		res=f_close(&fil);
		}
		return ErrFlag;
}


void DisPlay64x64Flash(unsigned long FlashAdd,unsigned char XData)
{
		unsigned char n;
		unsigned char BMPDisBuff[64];
		FlashAdd+=1;
		for(n=0;n<8;n++)
		{
				W25QXX_Read(BMPDisBuff,FlashAdd+(n*0x40),64);
				OLED_SetPost(XData,n);
				OLED_Write_nData(BMPDisBuff,64);
		}

}


u8 Read128x64BMPFileToFlash(unsigned char *FileStr,unsigned long FlashAdd)
{

// 		FIL fil;
// 		FRESULT res;
// 		UINT bww;

		u8 ErrFlag=0;

		u8 i,j,k,l,n;
	
		unsigned char ReadBMPBuff[4]; 	
 		unsigned char BMPDisBuff[128];
 		unsigned char ChangeBuff=0,DataBuffE;
		
		res=f_open (&fil,FileStr, FA_OPEN_EXISTING|FA_READ);	
	
		if(res!=FR_OK)
		{
				ErrFlag=1;	
		}
 		else
		{
		
		f_lseek(&fil,0x3E);
		for(n=0;n<8;n++)
		{
				for(j=0;j<8;j++)
				{		
						for(i=0;i<4;i++)
						{
								f_read(&fil,ReadBMPBuff,4,&bww);
								for(l=0;l<4;l++)
								{
										ChangeBuff=ReadBMPBuff[l];
										for(k=0;k<8;k++)
										{
												if((ChangeBuff&0x80)==0x80)
												{			
														DataBuffE=0x80;
														DataBuffE>>=(j);
														BMPDisBuff[i*4*8+l*8+k]|=DataBuffE;
												}
												else
												{
														DataBuffE=0x80;
														DataBuffE>>=(j);
														BMPDisBuff[i*4*8+l*8+k]&=~DataBuffE;
												}
												ChangeBuff<<=1;
										}
								}
						} 
				}
		
 		W25QXX_PageWrite(BMPDisBuff,FlashAdd+(0x380-n*0x80),128);
		}
		res=f_close(&fil);
		}
		return ErrFlag;
}


void DisPlayFlash(unsigned long FlashAdd)
{
		unsigned char n;
		unsigned char BMPDisBuff[128];
		FlashAdd+=1;
		for(n=0;n<8;n++)
		{
				W25QXX_Read(BMPDisBuff,FlashAdd+(n*0x80),128);
				OLED_SetPost(0,n);
				OLED_Write_nData(BMPDisBuff,128);
		}

}

void ReadBMPFile(unsigned char *FileStr)
{
// 		FIL fil;
// 		FRESULT res;
// 		UINT bww;
		
// 		TextBoxStr  CMDTXT;
		u8 ErrFlag=0;

		u16 i,j,k,l,n;
	
		unsigned char ReadBMPBuff[4];
// 	
 		unsigned char BMPDisBuff[128];
 		unsigned char ChangeBuff=0,DataBuffE;
	
// 		CMDTXT.TextBOXBuff=">                    ";
// 		CMDTXT.YPos=2;
// 		CMDTXT.XPos=0;
// 		CMDTXT.Underline=0;
// 		CMDTXT.Highlight=0;
// 		CMDTXT.Hide=0;
		
// 		res=f_open (&fil,"0:/System/BMP/BC1.hex", FA_OPEN_EXISTING|FA_READ);	
	
// 	
// 		if(exf_getfree("0",&total,&free))	
// 		{
// 				CMDTXT.TextBOXBuff=">SD Card Fatfs Error!";
// 				ErrFlag=1;	
// 		}		
// 		else		
// 		CMDTXT.TextBOXBuff=">SD Card OK    ";
// 		OLED_DisTextBoxStr(&CMDTXT);
// 		DelayMs(500);
// 		
// 		if(f_opendir(&dirifo,"SYSTEM/BMP")!=FR_OK)
// 		{
// 				CMDTXT.TextBOXBuff=">OpenDir Error!";
// 				ErrFlag=2;	
// 		}
// 		else
// 		CMDTXT.TextBOXBuff=">OpenDir OK!   ";
// 		OLED_DisTextBoxStr(&CMDTXT);
// 		DelayMs(500);
		
			res=f_open (&fil,FileStr, FA_OPEN_EXISTING|FA_READ);	
		
// 		if(res!=FR_OK)
// 		{
// 				CMDTXT.TextBOXBuff=">OpenFill Error!";
// 				ErrFlag=3;	
// 		}
// 		else
// 		CMDTXT.TextBOXBuff=">OpenFill OK!   ";	
// 		OLED_DisTextBoxStr(&CMDTXT);
// 		DelayMs(500);
		
		f_lseek(&fil,0x3E);
		for(n=0;n<8;n++)
		{
				for(j=0;j<8;j++)
				{		
						for(i=0;i<4;i++)
						{
								f_read(&fil,ReadBMPBuff,4,&bww);
								for(l=0;l<4;l++)
								{
		// 									USART1_SendBit(ReadBMPBuff[l]);
										ChangeBuff=ReadBMPBuff[l];
										for(k=0;k<8;k++)
										{
												if((ChangeBuff&0x80)==0x80)
												{			
														DataBuffE=0x80;
														DataBuffE>>=(j);
														BMPDisBuff[i*4*8+l*8+k]|=DataBuffE;
												}
												else
												{
														DataBuffE=0x80;
														DataBuffE>>=(j);
														BMPDisBuff[i*4*8+l*8+k]&=~DataBuffE;
												}
												ChangeBuff<<=1;
										}
								}
						} 
				}
		
		OLED_SetPost(0,7-n);
		OLED_Write_nData(BMPDisBuff,128);
		}
		
		res=f_close(&fil);
		
// 	
// 		for(j=0;j<64;j++)
// 		{
// 				for(i=0;i<16;i++)
// 				{
// 						ChangeBuff=ReadBMPBuff[j*16+i];
// 						for(k=0;k<8;k++)
// 						{
// 								if((ChangeBuff&0x80)==0x80)
// 								{			
// 										DataBuffE=0x80;
// 										DataBuffE>>=(j%8);
// 										BMPDisBuff[7-(j/8)][i*8+k]|=DataBuffE;
// 								}
// 								else
// 								{
// 										DataBuffE=0x80;
// 										DataBuffE>>=(j%8);
// 										BMPDisBuff[7-(j/8)][i*8+k]&=~DataBuffE;

// 								}
// 						}
// 				}
// 		}
// 		OLED_FillBuff(BMPDisBuff[8]);
}

void PowerOFF(void)
{
		OLED_CLS();

		DisPlay32x32Flash(0x00203000+(36-1)*0x80,20,2);
		DisPlay32x32Flash(0x00203000+(37-1)*0x80,76,2);
	
		while(1)
		{
// 				EC11_Count=100;
// 				while(EC11_Count==100)
		}
}

// unsigned char USART1_GetRxFlag(void);
// unsigned char USART1_ClearRxFlag(void);
// char USART1_ReadRxData(void);
// unsigned char USART2_RxFlag;
// unsigned char USART2_RxData;
//void USART2_SendBit(unsigned char Data)
void Terminal(void)
{
		unsigned char AirTxDataBuff[18]={0xbb,0xaa,0x0f,0x02,0x00,0x7f,0x00,0x7f,0x00,0x7f,0x00,0x7f,0x00,0x7f,0x00,0x7f,0x00,0xef};
		unsigned char AirTxCMD_Buff[7]={0xbb,0xaa,0x04,0x00,0x01,0x00,0xef};
		unsigned char i,Sum_Data;
		DisPlayFlash(0x00218C00);

		Sum_Data=0;
		DelayS(1);
		for(i=0;i<18;i++)
		{
				if(i<=4)
				{
							Sum_Data+=AirTxCMD_Buff[i];
							AirTxCMD_Buff[5]=Sum_Data;

				}
				USART2_SendBit(AirTxCMD_Buff[i]);
		}
		DelayS(1);
		
		while(1)
		{
				Sum_Data=0;
				for(i=0;i<18;i++)
				{
						if(i<=15)
						{
									Sum_Data+=AirTxDataBuff[i];
									AirTxDataBuff[16]=Sum_Data;

						}
						USART2_SendBit(AirTxDataBuff[i]);
				}
				DelayS(2);
		}

}

void GCodeMode(void)
{
		unsigned char i;
		unsigned long FileSize=0,ReadNum,Remainder,FileSizeCount=0;
		unsigned long Count_A;
		unsigned char GCodeBuff[100];
		unsigned char *GCodeBuffPtr;
	
		TextBoxStr 	GCodeTXT[5];
		NumBoxStr  	LineNum[5];
	
		TextBoxStr  TitleTXT;
	
		TitleTXT.YPos=0;
		TitleTXT.XPos=10;
		TitleTXT.Underline=0;
		TitleTXT.Highlight=0;
		TitleTXT.Font=0;
		TitleTXT.Hide=0;
	
		TitleTXT.TextBOXBuff="0:/G-Code/001.nc";
	
		for(i=0;i<5;i++)
		{
				GCodeTXT[i].YPos=2+i;
				GCodeTXT[i].XPos=34;
				GCodeTXT[i].Underline=0;
				GCodeTXT[i].Highlight=0;
				GCodeTXT[i].Font=0;
				GCodeTXT[i].Hide=0;
				GCodeTXT[i].TextBOXBuff="G0 X1.0000";
		}
		
		for(i=0;i<5;i++)
		{
				LineNum[i].XPos=12;
				LineNum[i].YPos=2+i;
				LineNum[i].NumVal=i;
		}
				
		DisPlayFlash(0x00218400);
		
		OLED_DisTextBoxStr(&TitleTXT);
		
		for(i=0;i<5;i++)
		OLED_DisNumBoxStr(&LineNum[i]);
		
		for(i=0;i<5;i++)
		OLED_DisTextBoxStr(&GCodeTXT[i]);
		
		TxBitData(0x0c);
		TxBitData(0x0c);
		
		f_open (&fil,"0:/G-Code/001.nc", FA_OPEN_EXISTING|FA_READ);
		
		FileSize=fil.fsize;
		TxStringData("FileSize:");
		Printf_Num(FileSize);
		TxStringData(" \r\n");
		
		FileSize-=1;
// 		Remainder=FileSize%100;
// 		ReadNum=(FileSize-Remainder)/100;
		
		while(1)
		{
				f_read(&fil,GCodeBuff,100,&bww);
				GCodeBuffPtr=GCodeBuff;
 				while(*GCodeBuffPtr!=0x0A)
				{
						if((*GCodeBuffPtr!=0x0D)&&(*GCodeBuffPtr!=0x0A))
						TxBitData(*GCodeBuffPtr);
						FileSizeCount++;
						GCodeBuffPtr++;
				}
 				FileSizeCount++;
				
				TxStringData("    //ENDLine -");
				Printf_Num(FileSizeCount);
				TxStringData(" \r\n");
				
				if(FileSizeCount>=FileSize)
				{
						TxStringData("Finish");
						TxStringData(" \r\n");
						while(1);
				}
 				f_lseek(&fil,FileSizeCount); 
		}
	
}

void ASMCodeMode(void)
{
	
		DisPlayFlash(0x00218800);
		while(1);
	
}

void Parameter(void)
{
		DisPlayFlash(0x00219400);
		DisPlay16x16Flash(0x00202040,5,5);
		DisPlay16x16Flash(0x00202060,4+16+4,5);
		DisPlay16x16Flash(0x00202080,4+16+4+16+4,5);
		while(1);

}



void CrossMode(void)
{ 
	
		unsigned char *NamePrt;
 		unsigned char RxKeyBoardBuff[50];
		unsigned char i=0,n=0,KeyBoard=0;
	
		TxBitData(0x0c);
		TxBitData(0x0c);
	
// 		f_mount(fs[0],"0:",1); 
// 		f_chdir("0:");  
		if(f_opendir(&dirifo,"0:/")!=FR_OK)
		{
					TxStringData("Open Dir Error \r\n");
					while(1);
		}
		TxStringData("Open Dir OK \r\n");
		while(1)
		{
				i=0;
				KeyBoard=0;
				while(KeyBoard!=0x0D)
				{
						ClearnRxFlag();
						KeyBoard=ReadRxData();
						RxKeyBoardBuff[i]=KeyBoard;
						i++;
				}
				TxStringData(" \r\n");
				
				i-=1;
				
				n=0;
				TxStringData("CDM : ");
				while(RxKeyBoardBuff[n]!=' ')
				{
						if((RxKeyBoardBuff[n]>=97)&&(RxKeyBoardBuff[n]<=122))
						RxKeyBoardBuff[n]-=32;
						if(RxKeyBoardBuff[n]!=0x0D)
						{
								TxBitData(RxKeyBoardBuff[n]);
								n++;
						}
						else
						{
								TxStringData(" \r\n");
								TxStringData("NO CMD \r\n");
								break;
						}
				}
				TxStringData(" \r\n");
				
				if(n>=i)
				{
						TxStringData("NO CMD \r\n");
				}
				else
				{
						while(RxKeyBoardBuff[n]==' ')
						{
								TxBitData('.');
								n++;
						}
						TxStringData(" \r\n");
				}
				
				if(n>=i)
				{
						TxStringData("NO CMD \r\n");
				}
				else
				{
						TxStringData("Data : ");
					
						while(RxKeyBoardBuff[n]!=0x0D)
						{
								TxBitData(RxKeyBoardBuff[n]);
								n++;
						}
						TxStringData(" \r\n");
				}
		}
// 		
// 		while(i!=0)
// 		{
// 				TxBitData(RxKeyBoardBuff[i]);
// 				i--;
// 				TxStringData(" \r\n");
// 		}
		
		while(1)
		{
//  				TxStringData("Test OK \r\n");
				res=f_readdir(&dirifo,&filinfo); 
				if (res != FR_OK)
 				TxStringData("Read Dir Error \r\n");
				else
				{
// 						TxStringData("Test \r\n");
						if(filinfo.fname[0]==0)	break; 
						NamePrt=filinfo.fname;
						TxStringData(NamePrt);
						TxStringData(" \r\n");
				}
// 				DelayMs(100);
		}
// 		while(1);

}

unsigned char TextTXBuff[11]={0xBB,0xAA,0x04,0x0A,0x01,0x00,0x00,0x00,0x00,0x00,0xef};

unsigned char LockTXBuff[11]={0xBB,0xAA,0x20,0x0B,0x01,0x00,0x00,0x00,0x00,0x00,0xef};

unsigned char ParameterTXBuff[6]={0xBB,0xAA,0x0A,0x03,0x00,0xef};

unsigned char ParameterBuff[10][4]=
{
{0x00,0x12,0x00,0x01},
{0x01,0x05,0x00,0x02},
{0x07,0x07,0x00,0x03},
{0x06,0x5A,0x00,0x04},
{0x04,0x10,0x00,0x05},
{0x01,0x7B,0x00,0x06},
{0x03,0x90,0x00,0x07},
{0x05,0xD1,0x00,0x08},
{0x08,0xA8,0x00,0x09},
{0x09,0x01,0x00,0x0A},
};

extern unsigned char Uart_Rx_FSM_Buff[50];
//extern Uart_Rx_FSM_Buff[50];

void ExChange_Mode(void)
{
		unsigned char i=0,k=0,SumData=0;
		unsigned char Check_Flag=0;
//		unsigned int  j=0,Time=100;
		NumBoxStr  	TestNumBox;
	
		TestNumBox.XPos=12;
		TestNumBox.YPos=5;
		TestNumBox.NumVal=0;
	
		OLED_CLS();
		OLED_DisNumBoxStr(&TestNumBox);
//		TxBitData(0xAB);
		while(1)
		{		
				while(Uart_Rx_FSM_Buff[0] != 0x01);
				Uart_Rx_FSM_Buff[0]=0;
			
				if(Uart_Rx_FSM_Buff[2]==0x0A&&Uart_Rx_FSM_Buff[3]==0x02)
				{
						SumData=0;
						TxBitData(0xBB);SumData+=0xbb;
						TxBitData(0xAA);SumData+=0xaa;
						TxBitData(0x0A);SumData+=0x0a;
						TxBitData(0x02);SumData+=0x02;
						TxBitData(0x0A);SumData+=0x0A;
						for(k=0;k<10;k++)
						{
								for(i=0;i<4;i++)
								{
										TxBitData(ParameterBuff[9-k][i]);
										SumData+=ParameterBuff[9-k][i];
								}
								TxBitData(0x00);
						}
						TxBitData(SumData);
						TxBitData(0xef);
				
				}

				if(Uart_Rx_FSM_Buff[2]==0x0A&&Uart_Rx_FSM_Buff[3]==0x03)
				{
						Check_Flag=0;
						for(k=0;k<10;k++)
						{
								if(ParameterBuff[k][0]==Uart_Rx_FSM_Buff[4]&&ParameterBuff[k][1]==Uart_Rx_FSM_Buff[5])
								{
										ParameterBuff[k][2]=Uart_Rx_FSM_Buff[6];
										ParameterBuff[k][3]=Uart_Rx_FSM_Buff[7];							
										TestNumBox.NumVal=Uart_Rx_FSM_Buff[6]*256+Uart_Rx_FSM_Buff[7];
										OLED_DisNumBoxStr(&TestNumBox);
										Check_Flag=1;
								}
						}
						if(Check_Flag==1)
						ParameterTXBuff[4]=0;
						else
						ParameterTXBuff[4]=1;	
						
						for(i=0;i<6;i++)
						TxBitData(ParameterTXBuff[i]);
				}
//				Uart_Rx_FSM_Buff[0]=0;
				
		
		}


}

void Toolkit(void)
{
		unsigned char i=0;
		TxBitData(0x04);
		while(1)
		{		
				while(Uart_Rx_FSM_Buff[0] != 0x01);
				Uart_Rx_FSM_Buff[0]=0;
			
				if(Uart_Rx_FSM_Buff[2]==0x0D&&Uart_Rx_FSM_Buff[3]==0x01)
				{
						TxBitData(0xBB);
						TxBitData(0xAA);
						TxBitData(0x20);
						TxBitData(0x0D);
						TxBitData(0x01);
					
						for(i=0;i<10;i++)
						{
								//TxBitData(i);
								if(i%2==0)
								TxStringData("Test Message Number :");
								else
								TxStringData("²âÊÔÐÅÏ¢ºÅ :");
								TxBitData(i+0x30);
								TxStringData(" \r\n");
						}
						TxStringData("Test Message End \r\n");
						TxBitData(0xef);
				
				}
			
		}


}

unsigned char MenuICO[17]={14,15,16,17,18,20,21,22,24,26,28,29,32,34,35,33,30};
unsigned char MeunTXT[17][11]={
{" Windows "},
{" Download"},
{"CrossMode"},
{" Exchange"},
{"  GCode  "},
{" ASMCode "},
{"Parameter"},
{" Command "},
{"Auto-Mode"},
{" Terminal"},
{" DisPage "},
{"  Files  "},
{" Setting "},
{" Toolkit "},
{" UP Data "},
{" TF Cadr "},
{"Power OFF"},
};
unsigned char MainMenu(void)
{
		unsigned int EC11Data=0;
		static char i;
		unsigned char (*p)[11];
	
		TextBoxStr  TitleTXT;
	
		p=MeunTXT;
// 		i=0;
		OLED_CLS();
	
		TitleTXT.YPos=6;
		TitleTXT.XPos=34;
		TitleTXT.Underline=0;
		TitleTXT.Highlight=0;
		TitleTXT.Font=0;
		TitleTXT.Hide=0;
		DisPlayFlash(0x00218000);
	
		if(i>16)
		i=0;
		
		if(i==0)
		DisPlay32x32Null(0,2);
		else
		DisPlay32x32Flash(0x00203000+(MenuICO[i-1]-1)*0x80,0,2);
		DisPlay32x32Flash(0x00203000+(MenuICO[i+0]-1)*0x80,47,1);
		if(i==16)
		DisPlay32x32Null(95,2);
		else	
		DisPlay32x32Flash(0x00203000+(MenuICO[i+1]-1)*0x80,95,2);

		TitleTXT.TextBOXBuff=*(p+0);
		OLED_DisTextBoxStr(&TitleTXT);
		
		while(1)
		{
				EC11_Count=100;
				while(EC11_Count==100)
				{
						if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10))
						{
								DelayMs(100);
							
								return i;					

								goto EndMenu;
						}
				}
				if(EC11_Count<100)
				{
						if(i<=15)
						i++;	
						else
						i=16;
				}
				else
				{
						if(i>0)
						i--;
						else 
						i=0;
				}
				
				TitleTXT.TextBOXBuff=*(p+i);
				OLED_DisTextBoxStr(&TitleTXT);
				
				if(i>16)
				i=0;
				
				if(i==0)
				DisPlay32x32Null(0,2);
				else
				DisPlay32x32Flash(0x00203000+(MenuICO[i-1]-1)*0x80,0,2);
				DisPlay32x32Flash(0x00203000+(MenuICO[i+0]-1)*0x80,47,1);
				if(i==16)
				DisPlay32x32Null(95,2);
				else	
				DisPlay32x32Flash(0x00203000+(MenuICO[i+1]-1)*0x80,95,2);
				
				DelayMs(100);
		}
		EndMenu:;
}

void FirstUSE(void)
{
// 		FIL fil;
// 		FRESULT res;
// 		UINT bww;
// 		DIR dirifo;
	
		u8 ErrFlag;
		u32 total,free;	
		u32 iConut;
	
		u8 i=0;
	
		unsigned char NameBuff1[]="0:/SYSTEM/BMP/128x64/BC001.bmp";
														 //012345678901234567890123456789
		unsigned char NameBuff2[]="0:/SYSTEM/BMP/32x32/BB001.bmp";
													   //012345678901234567890123456789
		unsigned char NameBuff3[]="0:/SYSTEM/BMP/16x16/BC001.bmp";
														 //012345678901234567890123456789
		unsigned char NameBuff4[]="0:/SYSTEM/BMP/8x8/BA001.bmp";
														 //012345678901234567890123456789
		unsigned char NameBuff5[]="0:/SYSTEM/BMP/64x64/BB001.bmp";
													   //012345678901234567890123456789
	
 		TextBoxStr  CMDTXT[3];
		TextBoxStr  TitleTXT;
		ProrsStr 		DisPlayPrors;
		
		DisWs2812(20,0,0);
	
		TitleTXT.TextBOXBuff="Get ready for 1st Use";
		/*********************---------------------********/
		TitleTXT.YPos=0;
		TitleTXT.XPos=0;
		TitleTXT.Underline=0;
		TitleTXT.Highlight=0;
		TitleTXT.Font=0;
		TitleTXT.Hide=0;
	
		DisPlayPrors.XPos=14;
		DisPlayPrors.YPos=6;
		DisPlayPrors.XSize=100;
		DisPlayPrors.DisNum=0;
		DisPlayPrors.Hide=0;
		
		CMDTXT[0].TextBOXBuff=">                    ";
		CMDTXT[0].YPos=2;
		CMDTXT[0].XPos=0;
		CMDTXT[0].Underline=0;
		CMDTXT[0].Highlight=0;
		CMDTXT[0].Font=0;
		CMDTXT[0].Hide=0;
		
		CMDTXT[1].TextBOXBuff=">                    ";
		CMDTXT[1].YPos=3;
		CMDTXT[1].XPos=0;
		CMDTXT[1].Underline=0;
		CMDTXT[1].Highlight=0;
		CMDTXT[1].Font=0;
		CMDTXT[1].Hide=0;
		
		CMDTXT[2].TextBOXBuff=">                    ";
		CMDTXT[2].YPos=4;
		CMDTXT[2].XPos=0;
		CMDTXT[2].Underline=0;
		CMDTXT[2].Highlight=0;
		CMDTXT[2].Font=0;
		CMDTXT[2].Hide=0;
		
		
		OLED_DisTextBoxStr(&TitleTXT);
		
		CMDTXT[0].TextBOXBuff="Checking the TF card ";
		OLED_DisTextBoxStr(&CMDTXT[0]);
		
// 		CMDTXT[1].TextBOXBuff=">Press the";
												 /*---------------------********/
		f_mount(fs[0],"0:",1); 
// 		if( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0)
// 		{
// 				Welcome();
// 		}
		while(exf_getfree("0",&total,&free))	
		{
				CMDTXT[0].TextBOXBuff=">Please insert TFcard";
				CMDTXT[1].TextBOXBuff=">Press Key1 to retry";
				OLED_DisTextBoxStr(&CMDTXT[0]);
				OLED_DisTextBoxStr(&CMDTXT[1]);
				ErrFlag=1;	
				DisWs2812(0,20,0);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8));

		}		
		CMDTXT[0].TextBOXBuff=">SD Card OK          ";
		CMDTXT[1].TextBOXBuff=">                    ";
												 /*---------------------********/
		OLED_DisTextBoxStr(&CMDTXT[0]);
		OLED_DisTextBoxStr(&CMDTXT[1]);
		DelayMs(100);
		
		while(f_opendir(&dirifo,"SYSTEM")!=FR_OK)
		{
				CMDTXT[0].TextBOXBuff=">Please Check Files ";
				CMDTXT[1].TextBOXBuff=">Press Key1 to retry";
				OLED_DisTextBoxStr(&CMDTXT[0]);
				OLED_DisTextBoxStr(&CMDTXT[1]);
				ErrFlag=1;	
				DisWs2812(0,20,0);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8));
		}
		DisWs2812(20,0,0);
		CMDTXT[0].TextBOXBuff=">Open SYSTEMDir OK!  ";
											/*---------------------********/
		OLED_DisTextBoxStr(&CMDTXT[0]);
		DelayMs(100);
		
		while(f_opendir(&dirifo,"SYSTEM/INF")!=FR_OK)
		{
				CMDTXT[0].TextBOXBuff=">Please Check Files ";
				CMDTXT[1].TextBOXBuff=">Press Key1 to retry";
				OLED_DisTextBoxStr(&CMDTXT[0]);
				OLED_DisTextBoxStr(&CMDTXT[1]);
				ErrFlag=1;	
				DisWs2812(0,20,0);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8));
		}
		DisWs2812(20,0,0);
		CMDTXT[0].TextBOXBuff=">Open INFDir OK!     ";
											/*---------------------********/
		OLED_DisTextBoxStr(&CMDTXT[0]);
		DelayMs(500);
		
		while(f_opendir(&dirifo,"SYSTEM/BMP")!=FR_OK)
		{
				CMDTXT[0].TextBOXBuff=">Please Check Files ";
				CMDTXT[1].TextBOXBuff=">Press Key1 to retry";
				OLED_DisTextBoxStr(&CMDTXT[0]);
				OLED_DisTextBoxStr(&CMDTXT[1]);
				ErrFlag=1;	
				DisWs2812(0,20,0);
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8));
		}
		DisWs2812(20,0,0);
		CMDTXT[0].TextBOXBuff=">Open BMPDir OK!     ";
		CMDTXT[1].TextBOXBuff=">                    ";
		OLED_DisTextBoxStr(&CMDTXT[0]);
		OLED_DisTextBoxStr(&CMDTXT[1]);
		DelayMs(100);
		
// 		W25QXX_Erase_Sector(0x217);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC1.bmp",0x00217000);
// 		CMDTXT[0].TextBOXBuff=">Coppy BC1.bmp       ";
// 		OLED_DisTextBoxStr(&CMDTXT[0]);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC2.bmp",0x00217400);
// 		CMDTXT[0].TextBOXBuff=">Coppy BC2.bmp       ";
// 		OLED_DisTextBoxStr(&CMDTXT[0]);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC3.bmp",0x00217800);
// 		CMDTXT[0].TextBOXBuff=">Coppy BC3.bmp       ";
// 		OLED_DisTextBoxStr(&CMDTXT[0]);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC4.bmp",0x00217C00);
// 		CMDTXT[0].TextBOXBuff=">Coppy BC4.bmp       ";
// 		OLED_DisTextBoxStr(&CMDTXT[0]);
// 		
// 		W25QXX_Erase_Sector(0x218);
// 		W25QXX_Erase_Sector(0x219);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC005.bmp",0x00218000);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC006.bmp",0x00218400);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC007.bmp",0x00218800);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC008.bmp",0x00218c00);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC009.bmp",0x00219000);
// 		Read128x64BMPFileToFlash("0:/SYSTEM/BMP/128x64/BC010.bmp",0x00219400);
// 		OLED_CLS();
// 		ReadBMPFile("0:/SYSTEM/BMP/128x64/BC2.bmp");
// 		DisPlayFlash(0x00217400);
		
// 		W25QXX_Erase_Sector(0x201);
// 		Read8x8BMPFileToFlash("0:/SYSTEM/BMP/8x8/BA1.bmp",0x00201000);
// 		Read8x8BMPFileToFlash("0:/SYSTEM/BMP/8x8/BA2.bmp",0x00201008);
// 		W25QXX_Erase_Sector(0x202);
// 		Read16x16BMPFileToFlash("0:/SYSTEM/BMP/16x16/BB001.bmp",0x00202000);
// 		Read16x16BMPFileToFlash("0:/SYSTEM/BMP/16x16/BB002.bmp",0x00202020);
// 		Read16x16BMPFileToFlash("0:/SYSTEM/BMP/16x16/BB003.bmp",0x00202040);
// 		Read16x16BMPFileToFlash("0:/SYSTEM/BMP/16x16/BB004.bmp",0x00202060);
// 		Read16x16BMPFileToFlash("0:/SYSTEM/BMP/16x16/BB005.bmp",0x00202080);
// 		CMDTXT[0].TextBOXBuff=">1.---------------";
// 		CMDTXT[0].YPos=4;
// 		CMDTXT[0].XPos=8;

// 		OLED_DisTextBoxStr(&CMDTXT[0]);
// 		
// 		OLED_DisProrsStr(&DisPlayPrors);
// 		
// 		
// 		CMDTXT[0].TextBOXBuff=">Coppy BMP Files";
// 		CMDTXT[0].YPos=4;
// 		CMDTXT[0].XPos=8;
// 		
// 		CMDTXT[1].TextBOXBuff=">               ";
// 		CMDTXT[1].YPos=5;
// 		CMDTXT[1].XPos=8;
// 		OLED_DisTextBoxStr(&CMDTXT[0]);
// 		
// 		for(iConut=0;iConut<8;iConut++)
// 		W25QXX_Erase_Sector(0x207+iConut);
// 		
// 		for(iConut=0;iConut<30;iConut++)
// 		{
// 				NameBuff5[22]=(iConut+1)/100%10+0x30;
// 				NameBuff5[23]=(iConut+1)/10%10+0x30;
// 				NameBuff5[24]=(iConut+1)%10+0x30;
// 				CMDTXT[1].TextBOXBuff=NameBuff5+14;
// 				OLED_DisTextBoxStr(&CMDTXT[1]);
// 				Read64x64BMPFileToFlash(NameBuff5,0x00207000+iConut*0x200);
// 		}
// 		OLED_CLS();
// 		
// 		W25QXX_Erase_Sector(0x203);
// 		W25QXX_Erase_Sector(0x204);
// 		W25QXX_Erase_Sector(0x205);
// 		W25QXX_Erase_Sector(0x206);
// 		
// 		for(iConut=0;iConut<37;iConut++)
// 		{
// 				NameBuff2[22]=(iConut+1)/100%10+0x30;
// 				NameBuff2[23]=(iConut+1)/10%10+0x30;
// 				NameBuff2[24]=(iConut+1)%10+0x30;
// 				CMDTXT[1].TextBOXBuff=NameBuff2+14;
// 				OLED_DisTextBoxStr(&CMDTXT[1]);
// 				Read32x32BMPFileToFlash(NameBuff2,0x00203000+iConut*0x80);
// 		}
		
// 		for(iConut=0;iConut<35;iConut++)
// 		{
// 				DisPlay32x32Flash(0x00203000+iConut*0x80,0,0);
// 				DelayMs(200);
// 		}		
// 		for(iConut=0;iConut<10;iConut++)
// 		{
// 				DisPlay64x64Flash(0x00207000+iConut*0x200,32);
// 				DelayMs(70);
// 		}
// 		while(1)
// 		for(iConut=0;iConut<20;iConut++)
// 		{
// 				DisPlay64x64Flash(0x00207000+(iConut+10)*0x200,32);
// 				DelayMs(70);
// 		}
// 		OLED_CLS();
// 		TestMenu();
// 			OLED_Power_OFF();
// 			DelayMs(1000);
// 			OLED_Power_ON();
		while(1)
		{
					i=MainMenu();
					switch(i)
					{
						case 0 : break;
						case 1 : break;
						case 2 : CrossMode();		break;
						case 3 : ExChange_Mode();break;
						case 4 : GCodeMode();		break;
						case 5 : ASMCodeMode();	break;
						case 6 : Parameter();		break;
						case 7 : break;
						case 8 : break;
						case 9 : Terminal();		break;
						case 10: break;
						case 11: break;
						case 12: break;
						case 13: Toolkit();break;
						case 14: break;
						case 15: break;
						case 16: PowerOFF();		break;
					};

		}

}
