#include "SupCOM.h"


unsigned char GetRxFlag(void);
unsigned char ClearnRxFlag(void);
void TxBitData(unsigned char Data);
void TxStringData(char *String);
char ReadRxData(void);
void ReadStringDR(char *String);
unsigned char ContrastString(char *StringA,char *StringB);
unsigned char ContrastString_nNum(char *StringA,char *StringB,unsigned int n);
void Printf_Num(unsigned long int Num_Data);
void Printf_HEX(unsigned char HEXData);

char RxString[50]={""};

unsigned char GetRxFlag(void)
{
		return USART1_GetRxFlag();
}

unsigned char ClearnRxFlag(void)
{
		USART1_ClearRxFlag();
		return 0;
}

void TxBitData(unsigned char Data)
{
		USART1_SendBit(Data);
}

void TxStringData(char *String)
{
		USART1_SendString(String);
}


char ReadRxData(void)
{
	char RxSavaData;
	while(USART1_GetRxFlag()!=1);
	RxSavaData=USART1_ReadRxData();
	return RxSavaData;
	
}

void ReadStringDR(char *String)
{
	unsigned int Count_Sava_Data=0;
	
	while(*String!='\0')         //Clear Buffer
	{
		*String='\0';
		String++;
		Count_Sava_Data++;
	}
	
	String-=Count_Sava_Data;     //Recovery Ptr
	
	while(1)
	{
		*String=ReadRxData();
		if(*String=='\r')
		break;	
		String++;
	}
}
//�Զ������ַ����Ա�
unsigned char ContrastString(char *StringA,char *StringB)  
{
	unsigned char C_Flag=0;
	while(*StringA!='\0'||*StringB!='\0')
	{
			if(*StringA==*StringB)
			{
				C_Flag=1;
			}
			else
			{
				C_Flag=0;
				break;
			}
			StringA++;
			StringB++;
	
	}
	return C_Flag;
}
//�̶������ַ����Ա�
unsigned char ContrastString_nNum(char *StringA,char *StringB,unsigned int n)
{
	unsigned char C_Flag=0;
	unsigned int Count;
	for(Count=0;Count<n;Count++)
	{
			if(*StringA==*StringB)
			{
				C_Flag=1;
			}
			else
			{
				C_Flag=0;
				break;
			}
			StringA++;
			StringB++;
	
	}
	return C_Flag;
}
//��ӡ���֣��Զ�ʶ�𳤶ȣ����65536
// void Printf_Num(unsigned int Num_Data)
// {
// 	char Num_Buf[5]={0,0,0,0,0};
// 	unsigned char i=0,n=0;
// 	while(Num_Data/10!=0)
// 	{     
// 			Num_Buf[4-i]=Num_Data%10;
// 			Num_Data/=10;
// 			i++;
// 	}
// 	Num_Buf[4-i]=Num_Data%10;
// 	i++;
// 	for(n=0;n<i;n++)
// 	TxBitData(Num_Buf[5-i+n]+0x30);
// }

void Printf_Num(unsigned long int Num_Data)
{
	char Num_Buf[10]={0,0,0,0,0,0,0,0,0,0};
	unsigned char i=0,n=0;
	while(Num_Data/10!=0)
	{     
			Num_Buf[9-i]=Num_Data%10;
			Num_Data/=10;
			i++;
	}
	Num_Buf[9-i]=Num_Data%10;
	i++;
	for(n=0;n<i;n++)
	TxBitData(Num_Buf[10-i+n]+0x30);
}

void Printf_HEX(unsigned char HEXData)
{
		unsigned char Data_H,Data_L;
		Data_L = HEXData&0x0f;
		Data_H = HEXData>>4;

		if(Data_H<10)
		TxBitData(Data_H+0x30);
		else
		TxBitData(Data_H-10+0x41);
		
		if(Data_L<10)
		TxBitData(Data_L+0x30);
		else
		TxBitData(Data_L-10+0x41);	
}

void Printf_HEX16(unsigned long HEXData)
{
		unsigned char DataBuff[4];
		unsigned char i=0;
		for(i=0;i<4;i++)
		{
				DataBuff[7-i]=HEXData&0x0f;
				HEXData>>=4;
		}
		
		for(i=0;i<4;i++)
		{
				if(DataBuff[i]<10)
				TxBitData(DataBuff[i]+0x30);
				else
				TxBitData(DataBuff[i]-10+0x41);	
		}
}

void Printf_HEX32(unsigned long HEXData)
{
		unsigned char DataBuff[8];
		unsigned char i=0;
		for(i=0;i<8;i++)
		{
				DataBuff[7-i]=HEXData&0x0f;
				HEXData>>=4;
		}
		
		for(i=0;i<8;i++)
		{
				if(DataBuff[i]<10)
				TxBitData(DataBuff[i]+0x30);
				else
				TxBitData(DataBuff[i]-10+0x41);	
		}
}



