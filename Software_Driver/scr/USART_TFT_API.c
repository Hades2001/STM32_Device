//USART_TFT_API.c
#include "USART_TFT_API.h"
#include "Hardware_System.h"
#include "AT24C0x.h"
#include "delay.h"
#include "USART.h"
#include "NY3P065.h"
#include "Finger.h"

unsigned char USARTRx_Data[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char Test_Data[10]={0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x00,0x00};

extern unsigned char FingerLink_CMD[24];
extern unsigned char Closs_System_Flag;
extern unsigned char Error_Flag;

extern	struct UserStr{
									unsigned char UserNum;
									unsigned char UserApply;
									unsigned char UserMode;
	
									unsigned char UserFingerFlag;									
									unsigned char UserFingerID;
	
									unsigned char UserPasswordFlag;	
									unsigned char UserPassword[10];
									
									unsigned char CRC_Bit;
	
							}Region1[10],Region2[10],Region3[10],Region4[10],*UserPtr;


							
void WriteUserStrToEEPROM(struct UserStr *Ptr,unsigned int Start_Add);
void ReadEEPROMToUserStr(struct UserStr *Ptr,unsigned int Start_Add);
void Password_MCSE(void);
void Main_Con(void);

void Admin_Password(void);
void Administration(void);
void USER_Set_Page(void);
void Change_User_Password(void);
void Inter_Finder_User(void);
							
void USART_TFT_SendData(unsigned char USART_Send_Data)
{
	
	USART1_SendBit(USART_Send_Data);
	
}


void USART_TFT_SendString(char *USART_SendString)
{
	
	USART1_SendString(USART_SendString);
	
}

void USART_RxHandle(void)
{
	;
}

void USART_TFT_SendCMD(char *CMD_Data)
{
	
	USART_TFT_SendString(CMD_Data);
	USART_TFT_SendData(0xff);
	USART_TFT_SendData(0xff);
	USART_TFT_SendData(0xff);

}

void Main_Con(void)
{
	
	if(USARTFingerRx_Data[0]==1||USARTRx_Data[0]==1)
	{
		if(USARTRx_Data[0]==1)
		{	
				USARTRx_Data[0]=0;
				switch(USARTRx_Data[1])
				{
					case(0xa1):Password_MCSE();break;
					case(0xa2):Administration();break;
					case(0xa3):USER_Set_Page();break;
					case(0xa4):break;
					case(0xa5):break;
					case(0xa6):break;
					case(0xa7):break;
					case(0xa8):break;
					case(0xa9):break;
					case(0xab):Admin_Password();break;
					//case(0xaf):break;
				}
				while(USARTRx_Data[0]!=1);
		}
		if(USARTFingerRx_Data[0]==1)
		{
			USARTFingerRx_Data[0]=0;
			if(USARTFingerRx_Data[2]==0x17)
			{
					Check_Again:
					if(USARTFingerRx_Data[5]==0)
					{
							if((AT24C0x_Read_1Bit(((USARTFingerRx_Data[6]-1)*17)+0x0032+2))==0x01)
							{						
									USART_TFT_SendCMD("page 8");
									Control_IO3_ON();
									Set_ACK_Sound(10);
									Set_ACK_Sound(13);
									Closs_System_Flag=0;
									DelayS(8);
									Closs_System_Flag=0;
									FingerCheckCMD();
									Closs_System_Flag=6;
									Control_IO3_OFF();
									
									USART_TFT_SendCMD("page 1");
							}
//							else
//							{
//									Set_ACK_Sound(8);
//									FingerCheckCMD();	
//									DelayS(2);
//							}
					}
//					else if(USARTFingerRx_Data[5]==0x21)
//					{
//							while(USARTFingerRx_Data[0]!=1);
//							goto Check_Again;
//					}
//					else if(USARTFingerRx_Data[5]==0x20)
//					{
//							while(USARTFingerRx_Data[0]!=1);
//							goto Check_Again;
//					}
					else if(USARTFingerRx_Data[5]==0x01)
					{
							if(USARTFingerRx_Data[6]!=0x0b)
							{
							Set_ACK_Sound(8);
							DelayS(2);
							Closs_System_Flag=0;
							FingerCheckCMD();	
							}
							if(USARTFingerRx_Data[6]==0x0b)
							{
							FingerCheckCMD();		
							}
					}
			}
//			FingerCheckCMD();
		
		}
		
	}
	if(Error_Flag==3)
	{
			End_ERROR_Page:;
			AT24C0x_Write_1Bit(0x0002,3);
			Set_ACK_Sound(14);
			Control_IO4_ON();
			USART_TFT_SendCMD("page 2");
			USART_TFT_SendCMD("p0.pic=10");
			DelayS(8);
			Closs_System_Flag=0;
			Control_IO4_OFF();
			USART_TFT_SendCMD("page 3");
			while(1)
			{
						unsigned char i;
						while(USARTRx_Data[0]!=1);
						USARTRx_Data[0]=0;
						if(USARTRx_Data[1]==0xac)
						{
								goto End_ERROR_Page;
						}
						if(USARTRx_Data[1]==0xa7)
						{
								i=0;
								while(i!=9)
								{
									if(AT24C0x_Read_1Bit(16+i)!=USARTRx_Data[i+2])
									{
											goto End_ERROR_Page;
									}
									i++;
								}
								if(i==9)
								{
										Error_Flag=0;
										AT24C0x_Write_1Bit(0x0002,Error_Flag);
										USART_TFT_SendCMD("page 2");
										USART_TFT_SendCMD("p0.pic=9");
										Set_ACK_Sound(10);
										DelayS(3);
										USART_TFT_SendCMD("page 1");
										goto End_Error_End;
								}
						}
				}	
				End_Error_End:;
	}
}

void USER_Set_Page(void)
{
		unsigned char Sava_Data;
		unsigned int Read_ADD;
		if(USARTRx_Data[2]==0xaa)
		goto ALL_USER_Set_Page;
		Sava_Data=USARTRx_Data[2];
		UserPtr=UserPtr+(USARTRx_Data[2]-0x31);
		USART3_SendBit(UserPtr->UserNum);
		Read_ADD=((UserPtr->UserNum)*17)+0x0032;
		ReadEEPROMToUserStr(UserPtr,Read_ADD);
		switch(UserPtr->UserMode)
		{
			case(0x00):USART_TFT_SendCMD("bt0.val=0");USART_TFT_SendCMD("bt1.val=0");break;
			case(0x01):USART_TFT_SendCMD("bt0.val=1");USART_TFT_SendCMD("bt1.val=0");break;
			case(0x02):USART_TFT_SendCMD("bt0.val=0");USART_TFT_SendCMD("bt1.val=1");break;
			case(0x03):USART_TFT_SendCMD("bt0.val=1");USART_TFT_SendCMD("bt1.val=1");break;
		}
		if(UserPtr->UserPasswordFlag==1)
		{
			USART_TFT_SendCMD("q0.picc=16");
		}
		if(UserPtr->UserFingerFlag==1)
		{
			USART_TFT_SendCMD("q1.picc=16");
		}
	
//		USART3_SendBit(UserPtr->UserNum);
		while(1)
		{
				USART3_SendBit(0xA6);
				while(USARTRx_Data[0]!=1);
				USARTRx_Data[0]=0;
				if(USARTRx_Data[1]==0xa4)
				{
						if(USARTRx_Data[2]==0xaa)
						{
//								USART3_SendBit(UserPtr->UserNum);
								WriteUserStrToEEPROM(UserPtr,0x0032+((UserPtr->UserNum)*17));
								goto END_USER_Set_Page;
						}
						if(USARTRx_Data[2]==0xa1)
						{
								Change_User_Password();
						}
						if(USARTRx_Data[2]==0xa2)
						{
							USART3_SendBit(0xAa);
								Inter_Finder_User();
						
						}
						
						if(USARTRx_Data[2]==0xa3)
						{
								if(USARTRx_Data[3]==0)
								(UserPtr->UserMode)=(UserPtr->UserMode)&(~0x01);
								else
								(UserPtr->UserMode)=(UserPtr->UserMode)|(0x01);
//								USART3_SendBit(UserPtr->UserMode);
						}
						if(USARTRx_Data[2]==0xa4)
						{
								if(USARTRx_Data[3]==0)
								(UserPtr->UserMode)=(UserPtr->UserMode)&(~0x02);
								else
								(UserPtr->UserMode)=(UserPtr->UserMode)|(0x02);
//								USART3_SendBit(UserPtr->UserMode);
						}
						
						
				}

		}
		END_USER_Set_Page:;
	  UserPtr-=(Sava_Data-0x31);
		ALL_USER_Set_Page:;
}

void Inter_Finder_User(void)
{
		unsigned char i=0,j=0;
//		UserPtr->UserNum;
		for(i=2;i<24;i++)
		USARTFingerTx_Data[i]=0;
		Delete_USER_ID:;
		USARTFingerTx_Data[3]=0x11;
		USARTFingerTx_Data[5]=0x04;
		USARTFingerTx_Data[6]=UserPtr->UserNum+1;
		FingerSendCMD(USARTFingerTx_Data);
		USARTFingerRx_Data[0]=0;
		while(USARTFingerRx_Data[0]!=1);
		USARTFingerRx_Data[0]=0;
		if(USARTFingerRx_Data[2]!=0x11)
		goto Delete_USER_ID;
				USART3_SendBit(0xAB);
				DelayMs(10);
				while(1)
				{
						if(USARTRx_Data[0]==1)
						{
								USARTRx_Data[0]=0;
								if(USARTRx_Data[2]==0xac)
								{	
									Set_ACK_Sound(22);
									goto End_Finder_T;
								}
						}
					Closs_System_Flag=0;
					Finger_CMD_Again:	
					USARTFingerRx_Data[0]=0;
					USARTFingerTx_Data[3]=0x16;
					USARTFingerTx_Data[5]=0x04;
					USARTFingerTx_Data[6]=UserPtr->UserNum+1;
					FingerSendCMD(USARTFingerTx_Data);

					Finger_C_Again:	
					while(USARTFingerRx_Data[0]!=1)
					{
					Closs_System_Flag=0;
					if(USARTRx_Data[0]==1)
						{
								USARTRx_Data[0]=0;
								if(USARTRx_Data[2]==0xac)
								{	
									Set_ACK_Sound(22);
									
									goto End_Finder_T;
								}
						}
					}
					USARTFingerRx_Data[0]=0;	
					if(USARTFingerRx_Data[2]!=0x16)
					goto Finger_CMD_Again;	
					if(USARTFingerRx_Data[5]==0x00)	
					{
							Set_ACK_Sound(23);
							Set_ACK_Sound(3);
							goto End_Finder_T;
					}
					else if(USARTFingerRx_Data[5]==0x20)
					{
							Set_ACK_Sound(4);
							goto Finger_C_Again;
					}
					else if(USARTFingerRx_Data[5]==0x21)
					{
							Set_ACK_Sound(5);
							goto Finger_C_Again;
					}			
					else
					{
							Set_ACK_Sound(22);
							goto Finger_CMD_Again;
					}
						
				}			
				
		End_Finder_T:;
		DelayS(1);
		USART_TFT_SendCMD("page 6");
		UserPtr->UserApply=1;
		UserPtr->UserPasswordFlag=1;
		switch(UserPtr->UserMode)
		{
		case(0x00):USART_TFT_SendCMD("bt0.val=0");USART_TFT_SendCMD("bt1.val=0");break;
		case(0x01):USART_TFT_SendCMD("bt0.val=1");USART_TFT_SendCMD("bt1.val=0");break;
		case(0x02):USART_TFT_SendCMD("bt0.val=0");USART_TFT_SendCMD("bt1.val=1");break;
		case(0x03):USART_TFT_SendCMD("bt0.val=1");USART_TFT_SendCMD("bt1.val=1");break;
		}
		if(UserPtr->UserPasswordFlag==1)
		{
				USART_TFT_SendCMD("q0.picc=16");
		}
		if(UserPtr->UserFingerFlag==1)
		{
				USART_TFT_SendCMD("q1.picc=16");
		}		
}

void Change_User_Password(void)
{
		unsigned char User_Password_Data[10],i=0;
		while(1)
		{
					New_Password_Input:;
					USART_TFT_SendCMD("page 3");
					USART_TFT_SendCMD("t1.txt=\"请输入新密码\"");
					//USART_TFT_SendCMD("t1.txt=\"请输入新密码\"");
					while(USARTRx_Data[0]!=1);
					USARTRx_Data[0]=0;
					if(USARTRx_Data[1]==0xac)
					{
							USART_TFT_SendCMD("page 6");
							goto END_Change_User_Password;
					}
					if(USARTRx_Data[1]==0xa7)
					{
							for(i=0;i<10;i++)
							{
									User_Password_Data[i]=USARTRx_Data[i+2];
							}
							i=0;
							USART_TFT_SendCMD("page 3");
							USART_TFT_SendCMD("t1.txt=\"请再次输入密码\"");
							while(USARTRx_Data[0]!=1);
							USARTRx_Data[0]=0;
							if(USARTRx_Data[1]==0xac)
							{
									USART_TFT_SendCMD("page 6");
									goto END_Change_User_Password;
							}
							while(i!=9)
							{
									if(User_Password_Data[i]!=USARTRx_Data[i+2])
									{
											i=0;
											USART_TFT_SendCMD("t1.txt=\"两次输入密码不同\"");
											DelayS(1);
											goto New_Password_Input;
									}
									i++;
							}
							if(i==9)
							{
									i=0;
									USART_TFT_SendCMD("t1.txt=\"设置成功!\"");
									for(i=0;i<10;i++)
									{
											UserPtr->UserPassword[i]=User_Password_Data[i];
//											USART3_SendBit(User_Password_Data[i]);
									}
									DelayS(1);
									USART_TFT_SendCMD("page 6");
									UserPtr->UserApply=1;
									UserPtr->UserPasswordFlag=1;
									switch(UserPtr->UserMode)
									{
											case(0x00):USART_TFT_SendCMD("bt0.val=0");USART_TFT_SendCMD("bt1.val=0");break;
											case(0x01):USART_TFT_SendCMD("bt0.val=1");USART_TFT_SendCMD("bt1.val=0");break;
											case(0x02):USART_TFT_SendCMD("bt0.val=0");USART_TFT_SendCMD("bt1.val=1");break;
											case(0x03):USART_TFT_SendCMD("bt0.val=1");USART_TFT_SendCMD("bt1.val=1");break;
									}
									if(UserPtr->UserPasswordFlag==1)
									{
											USART_TFT_SendCMD("q0.picc=16");
									}
									if(UserPtr->UserFingerFlag==1)
									{
											USART_TFT_SendCMD("q1.picc=16");
									}	
									goto END_Change_User_Password;
							}
					}	
		}

		END_Change_User_Password:;


}


void Administration(void)
{
		unsigned char i=0;
		unsigned char Administration_Set_Data[10];
		switch(USARTRx_Data[2])
		{
				case(0x30):goto Administration_Flag1;break;
				case(0x31):goto Administration_Flag2;break;
				case(0x32):goto Administration_Flag3;break;
				case(0x33):goto Administration_Flag4;break;
				case(0x34):goto Administration_Flag5;break;
		}
		goto End_Administration;
		Administration_Flag1:;
		{
			while(1)
			{
					while(USARTRx_Data[0]!=1);
					USARTRx_Data[0]=0;
					if(USARTRx_Data[1]==0xac)
					{
							USART_TFT_SendCMD("page 4");
							goto End_Passwprd_Input;
					}
					if(USARTRx_Data[1]==0xa7)
					{
							while(i!=9)
							{
									if(AT24C0x_Read_1Bit(16+i)!=USARTRx_Data[i+2])
									{
											i=0;
											USART_TFT_SendCMD("page 2");
											goto End_Passwprd_Input;
									}
									i++;
							}
							if(i==9)
							{
									i=0;
									goto Next_AdministrationPass;
							}
					}					
				
			}
			while(1)
			{
					Next_AdministrationPass:;
					USART_TFT_SendCMD("page 3");
					USART_TFT_SendCMD("t1.txt=\"请输入新密码\"");
					while(USARTRx_Data[0]!=1);
					USARTRx_Data[0]=0;
					if(USARTRx_Data[1]==0xac)
					{
							USART_TFT_SendCMD("page 4");
							goto End_Passwprd_Input;
					}
					if(USARTRx_Data[1]==0xa7)
					{
							for(i=0;i<10;i++)
							{
									Administration_Set_Data[i]=USARTRx_Data[i+2];
							}
							i=0;
							USART_TFT_SendCMD("page 3");
							USART_TFT_SendCMD("t1.txt=\"请再次输入密码\"");
							while(USARTRx_Data[0]!=1);
							USARTRx_Data[0]=0;
							if(USARTRx_Data[1]==0xac)
							{
									USART_TFT_SendCMD("page 4");
									goto End_Passwprd_Input;
							}
							while(i!=9)
							{
									if(Administration_Set_Data[i]!=USARTRx_Data[i+2])
									{
											i=0;
											USART_TFT_SendCMD("t1.txt=\"两次输入密码不同\"");
											DelayS(1);
											goto Next_AdministrationPass;
									}
									i++;
							}
							if(i==9)
							{
									i=0;
									USART_TFT_SendCMD("t1.txt=\"设置成功!\"");
									AT24C0x_Write_nBit(16,10,Administration_Set_Data);
									DelayS(1);
									USART_TFT_SendCMD("page 4");
									goto End_Passwprd_Input;
							}
					}	
			
			
			}
			End_Passwprd_Input:;
			goto End_Administration;
		}
		Administration_Flag2:;
		{
			UserPtr=Region1;
		
			goto End_Administration;
		}
		Administration_Flag3:;
		{
			UserPtr=Region2;
		
			goto End_Administration;
		}
		Administration_Flag4:;
		{
			UserPtr=Region3;
		
			goto End_Administration;
		}
		Administration_Flag5:;
		{
			UserPtr=Region4;
		
			goto End_Administration;
		}
		End_Administration:;
}

void Admin_Password(void)
{
	unsigned int i=0;
	while(1)
	{
		while(USARTRx_Data[0]!=1);
		USARTRx_Data[0]=0;
		if(USARTRx_Data[1]==0xac)
		{
				USART_TFT_SendCMD("page 1");
				Control_IO1_ON();
				DelayMs(50);
				FingerSendCMD(FingerLink_CMD);
				goto End_ThisAmind;
		}
		if(USARTRx_Data[1]==0xa7)
		{
				while(i!=9)
				{
					if(AT24C0x_Read_1Bit(16+i)!=USARTRx_Data[i+2])
					{
							i=0;
							USART_TFT_SendCMD("page 2");
							goto End_ThisAmind;
					}
					i++;
				}
				if(i==9)
				{
					USART_TFT_SendCMD("page 4");
					goto End_ThisAmind;
				}
		}

	}
			End_ThisAmind:;
}

void Password_MCSE(void)
{
		unsigned char i=0,j;
		UserPtr=Region1;
		for(j=0;j<10;j++)
		{
			ReadEEPROMToUserStr(UserPtr,0x0032+(j*17));
			if((UserPtr->UserApply==1)&&(((UserPtr->UserMode)&0x02)==0x02))
			{
				while(i!=9)
				{
					if(UserPtr->UserPassword[i]!=USARTRx_Data[i+2])
					{
						UserPtr++;
						i=0;
						goto NextOne;
					}
					i++;
				}
				if(i==9)
				{
//				USART3_SendBit(UserPtr->UserNum);
				USART_TFT_SendCMD("page 8");
				Error_Flag=0;
				AT24C0x_Write_1Bit(0x0002,Error_Flag);
				goto End_Password_MCSE;
				}
			}
			else
			UserPtr++;	
			NextOne:;
		}
		
		i=0;
		UserPtr=Region2;
		for(j=0;j<10;j++)
		{
			ReadEEPROMToUserStr(UserPtr,0x0032+((j+10)*17));
			if((UserPtr->UserApply==1)&&(((UserPtr->UserMode)&0x02)==0x02))
			{
				while(i!=9)
				{
					if(UserPtr->UserPassword[i]!=USARTRx_Data[i+2])
					{
						UserPtr++;
						i=0;
						goto NextOne2;
					}
					i++;
				}
				if(i==9)
				{
//				USART3_SendBit(UserPtr->UserNum);
				USART_TFT_SendCMD("page 8");
				Error_Flag=0;
				AT24C0x_Write_1Bit(0x0002,Error_Flag);
				goto End_Password_MCSE;
				}
			}
			else
			UserPtr++;	
			NextOne2:;
		}
		
		
		UserPtr=Region3;
		for(j=0;j<10;j++)
		{
			ReadEEPROMToUserStr(UserPtr,0x0032+((j+20)*17));
			if((UserPtr->UserApply==1)&&(((UserPtr->UserMode)&0x02)==0x02))
			{
				while(i!=9)
				{
					if(UserPtr->UserPassword[i]!=USARTRx_Data[i+2])
					{
						UserPtr++;
						i=0;
						goto NextOne3;
					}
					i++;
				}
				if(i==9)
				{
//				USART3_SendBit(UserPtr->UserNum);
				USART_TFT_SendCMD("page 8");
				Error_Flag=0;
				AT24C0x_Write_1Bit(0x0002,Error_Flag);
				goto End_Password_MCSE;
				}
			}
			else
			UserPtr++;	
			NextOne3:;
		}
		
		
		UserPtr=Region4;
		for(j=0;j<10;j++)
		{
			ReadEEPROMToUserStr(UserPtr,0x0032+((j+30)*17));
			if((UserPtr->UserApply==1)&&(((UserPtr->UserMode)&0x02)==0x02))
			{
				while(i!=9)
				{
					if(UserPtr->UserPassword[i]!=USARTRx_Data[i+2])
					{
						UserPtr++;
						i=0;
						goto NextOne4;
					}
					i++;
				}
				if(i==9)
				{
//				USART3_SendBit(UserPtr->UserNum);
				USART_TFT_SendCMD("page 8");
				Error_Flag=0;
				AT24C0x_Write_1Bit(0x0002,Error_Flag);
				goto End_Password_MCSE;
				}
			}
			else
			UserPtr++;	
			NextOne4:;
		}
		Set_ACK_Sound(9);
		USART_TFT_SendCMD("page 2");
		Error_Flag++;
		AT24C0x_Write_1Bit(0x0002,Error_Flag);
		goto ALL_End;
		End_Password_MCSE:;
		if(UserPtr->UserMode==0x03)                  //*****************************************************************************
		{
			while(1)
			{
				USART_TFT_SendCMD("page 7");
				USARTFingerRx_Data[0]=0;	
				Send_FingerCMD_Again:;
				FingerCheckCMD();
				Check_Again:;
				USARTRx_Data[0]=0;
				Closs_System_Flag=0;
				while(USARTFingerRx_Data[0]!=1)
				if(USARTRx_Data[0]==1)
				{
						USARTRx_Data[0]=0;
						if(USARTRx_Data[2]==0xac)
						{	
							Set_ACK_Sound(8);
							USART_TFT_SendCMD("page 1");
							FingerCheckCMD();
							goto ALL_End;
						}
				}
				USARTFingerRx_Data[0]=0;	
				if(USARTFingerRx_Data[2]==0x17)
				{
					if(USARTFingerRx_Data[5]==0x20)
					{
							Set_ACK_Sound(4);
							Closs_System_Flag=0;
							goto Check_Again;
					}
					else if(USARTFingerRx_Data[5]==0x21)
					{
							Set_ACK_Sound(5);
							Closs_System_Flag=0;
							goto Check_Again;
					}
					else if(USARTFingerRx_Data[5]==0x00)
					{
								if((USARTFingerRx_Data[6]-1)==(UserPtr->UserNum))
								{
										USART_TFT_SendCMD("page 8");
										goto END_Finger_Check;
								}
								else
								{
										
										Set_ACK_Sound(8);
										goto Send_FingerCMD_Again;	
								}
					}
					else
					{
						if(USARTFingerRx_Data[6]!=0x0b)
						{
						Set_ACK_Sound(8);
						goto Send_FingerCMD_Again;
						}
						if(USARTFingerRx_Data[6]==0x0b)
						goto Send_FingerCMD_Again;
					
					}	
				}
				else
				goto Send_FingerCMD_Again;
			}
		}
		END_Finger_Check:;
		Control_IO3_ON();
		Set_ACK_Sound(10);
		DelayS(8);
		Control_IO3_OFF();
		FingerCheckCMD();
		USART_TFT_SendCMD("page 1");
		ALL_End:;
}

void WriteUserStrToEEPROM(struct UserStr *Ptr,unsigned int Start_Add)
{
		unsigned i;
		//AT24C0x_Write_1Bit(Start_Add,Ptr->UserNum); 
		AT24C0x_Write_1Bit(Start_Add+1,Ptr->UserApply);
		AT24C0x_Write_1Bit(Start_Add+2,Ptr->UserMode);
		AT24C0x_Write_1Bit(Start_Add+3,Ptr->UserFingerFlag);
		AT24C0x_Write_1Bit(Start_Add+4,Ptr->UserFingerID);
		AT24C0x_Write_1Bit(Start_Add+5,Ptr->UserPasswordFlag);
		for(i=0;i<10;i++)
		AT24C0x_Write_1Bit(Start_Add+6+i,Ptr->UserPassword[i]);
		AT24C0x_Write_1Bit(Start_Add+16,Ptr->CRC_Bit);
}

void ReadEEPROMToUserStr(struct UserStr *Ptr,unsigned int Start_Add)
{
		unsigned i;
		Ptr->UserNum					=	AT24C0x_Read_1Bit(Start_Add);
		Ptr->UserApply				=	AT24C0x_Read_1Bit(Start_Add+1);
		Ptr->UserMode					=	AT24C0x_Read_1Bit(Start_Add+2);
		Ptr->UserFingerFlag		=	AT24C0x_Read_1Bit(Start_Add+3);
		Ptr->UserFingerID			=	AT24C0x_Read_1Bit(Start_Add+4);
		Ptr->UserPasswordFlag	=	AT24C0x_Read_1Bit(Start_Add+5);
		for(i=0;i<10;i++)
		Ptr->UserPassword[i]=AT24C0x_Read_1Bit(Start_Add+6+i);
		Ptr->CRC_Bit=AT24C0x_Read_1Bit(Start_Add+16);
}


void Set_Password(void)
{


}

