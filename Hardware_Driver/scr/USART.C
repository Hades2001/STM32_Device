#include "USART.h"

void USART_SendBit_S(unsigned char Data , USART_TypeDef* USARTx);
void USART_SendString_S(char *nString , USART_TypeDef* USARTx);

void USART1_SendBit(unsigned char Data);
void USART2_SendBit(unsigned char Data);
void USART3_SendBit(unsigned char Data);

void USART1_SendString(char *nString);
void USART2_SendString(char *nString);
void USART3_SendString(char *nString);

void USART1_RxOver_Interrupt(void);
void USART2_RxOver_Interrupt(void);
void USART3_RxOver_Interrupt(void);

unsigned char USART1_GetRxFlag(void);
unsigned char USART1_ClearRxFlag(void);
char USART1_ReadRxData(void);

unsigned char USART2_GetRxFlag(void);
unsigned char USART2_ClearRxFlag(void);
char USART2_ReadRxData(void);

extern unsigned char USARTFingerRx_Data[20];
extern unsigned char USARTFingerTx_Data[24];

unsigned char Uart_Rx_FSM_Buff[50];
unsigned char Uart_Rx_FSM_DataLength;

unsigned char USARTRx_Data[2];

extern unsigned char Closs_System_Flag;
extern unsigned char Page0_Flag;

void Uart_FSM_S(unsigned char Uart_Rx_Data , struct CamUartFsm *USART_S);
void Uart_FSM(unsigned char Uart_Rx_Data);
void Uart1_FSM(unsigned char Data);


void USART_SendBit_S(unsigned char Data , USART_TypeDef* USARTx)
{
	USART_SendData(USARTx, Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)!=1);
}

void USART_SendString_S(char *nString , USART_TypeDef* USARTx)
{
	while(*nString!='\0')
	{
		USART_SendBit_S(*nString,USARTx);
		nString++;
	}
}

void USART1_SendBit(unsigned char Data)
{
		USART_SendData(USART1, Data);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)!=1);
}

void USART2_SendBit(unsigned char Data)
{
		USART_SendData(USART2, Data);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)!=1);
}

void USART3_SendBit(unsigned char Data)
{
		USART_SendData(USART3, Data);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE)!=1);
}

void USART1_SendString(char *nString)
{
	while(*nString!='\0')
	{
		USART1_SendBit(*nString);
		nString++;
	}
}

void USART2_SendString(char *nString)
{
	while(*nString!='\0')
	{
		USART2_SendBit(*nString);
		nString++;
	}
}
void USART3_SendString(char *nString)
{
	while(*nString!='\0')
	{
		USART3_SendBit(*nString);
		nString++;
	}

}

unsigned char USART1_RxFlag;
unsigned char USART1_RxData;


unsigned char USART1_GetRxFlag(void)
{
		return USART1_RxFlag;
}

unsigned char USART1_ClearRxFlag(void)
{
		USART1_RxFlag=0;
		return 0;
}

char USART1_ReadRxData(void)
{
		USART1_RxFlag=0;
		return USART1_RxData;
}


unsigned char USART2_RxFlag;
unsigned char USART2_RxData;


unsigned char USART2_GetRxFlag(void)
{
		return USART2_RxFlag;
}

unsigned char USART2_ClearRxFlag(void)
{
		USART2_RxFlag=0;
		return 0;
}

char USART2_ReadRxData(void)
{
		USART2_RxFlag=0;
		return USART2_RxData;
}

unsigned char USART3_RxFlag;
unsigned char USART3_RxData;

struct CamUartFsm s_CamUART,s_IPUAR;


void USART1_RxOver_Interrupt(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		USART1_RxFlag=1;
		USART1_RxData=USART_ReceiveData(USART1);
		Uart1_FSM(USART1_RxData);
		//USART2_SendBit(USART1_RxData);
	}
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


void USART2_RxOver_Interrupt(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		USART2_RxFlag=1;
		USART2_RxData=USART_ReceiveData(USART2);
		//USART1_SendBit(USART2_RxData);
		Uart_FSM_S(USART2_RxData,&s_CamUART);

	}
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void USART3_RxOver_Interrupt(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		USART3_RxFlag=1;
		USART3_RxData=USART_ReceiveData(USART3);
		//USART1_SendBit(USART3_RxData);
		Uart_FSM_S(USART3_RxData,&s_IPUAR);
	}
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}


void Uart_FSM_S(unsigned char Uart_Rx_Data , struct CamUartFsm *USART_S)
{
		switch(USART_S->FSM_State)
		{
			case 0:	
				if(Uart_Rx_Data==0x02)
				USART_S->FSM_State = 1;
				else
				USART_S->FSM_State = 0;
			break;
			case 1:	
				if(Uart_Rx_Data=='%'){
					USART_S->Count_i = 0;
					USART_S->FSM_State = 2;
				}
				else
				USART_S->FSM_State = 0;
			break;
			case 2:
				if((Uart_Rx_Data==':')&&( USART_S->Count_i <= 4 ))
				{
					USART_S->FSM_State = 3;
					USART_S->CMDBuff[ USART_S->Count_i ] = 0;
					USART_S->Count_i = 0;
				}
				else{
					USART_S->CMDBuff[ USART_S->Count_i ] = Uart_Rx_Data;
					USART_S->Count_i++;

					if( USART_S->Count_i > 5 )
					{
						USART_S->FSM_State = 0;
						USART_S->Count_i = 0;
					}
				}
			break;
			case 3:
				if( Uart_Rx_Data != 0x03 ){
					USART_S->DataBuff[ USART_S->Count_i ] = Uart_Rx_Data;
					USART_S->Count_i++;	
				}
				else{
					USART_S->Length = USART_S->Count_i;
					USART_S->DataBuff[ USART_S->Count_i ] = 0;
					USART_S->FSM_State = 0;
					USART_S->DataFlag = 1;
				}
			break;	
			default :	USART_S->FSM_State=0; break;
		};
}
void Uart_FSM(unsigned char Uart_Rx_Data)
{
		static char Count_i;

		switch(s_CamUART.FSM_State)
		{
			case 0:	
				if(Uart_Rx_Data==0x02)
				s_CamUART.FSM_State = 1;
				else
				s_CamUART.FSM_State = 0;
			break;
			case 1:	
				if(Uart_Rx_Data=='%'){
					Count_i = 0;
					s_CamUART.FSM_State = 2;
				}
				else
				s_CamUART.FSM_State = 0;
			break;
			case 2:
				if((Uart_Rx_Data==':')&&( Count_i <= 4 ))
				{
					s_CamUART.FSM_State = 3;
					s_CamUART.CMDBuff[ Count_i ] = 0;
					Count_i = 0;
				}
				else{
					s_CamUART.CMDBuff[ Count_i ] = Uart_Rx_Data;
					Count_i++;

					if( Count_i > 5 )
					{
						s_CamUART.FSM_State = 0;
						Count_i = 0;
					}
				}
			break;
			case 3:
				if( Uart_Rx_Data != 0x03 ){
					s_CamUART.DataBuff[ Count_i ] = Uart_Rx_Data;
					Count_i++;	
				}
				else{
					s_CamUART.Length = Count_i;
					s_CamUART.DataBuff[ Count_i ] = 0;
					s_CamUART.FSM_State = 0;
					s_CamUART.DataFlag = 1;
				}
			break;	
			default :	s_CamUART.FSM_State=0; break;
		};
}

struct   USART_Line s_UART1;

void Uart1_FSM(unsigned char Data)
{
	static uint8_t SumData;

	switch( s_UART1.FSM_State )
	{
	case 0 : if( Data == 0xAA ) { s_UART1.FSM_State = 1; SumData = 0x00 ; } break;
	case 1 : if( Data == 0xCC ) s_UART1.FSM_State = 2; break;
	case 2 :
		s_UART1.CMD = Data ;
		s_UART1.FSM_State = 3 ;
		break;
	case 3 :
		s_UART1.ID = Data;
		s_UART1.ID <<= 8;
		s_UART1.FSM_State = 4 ;
		break;
	case 4 :
		s_UART1.ID |= Data;
		s_UART1.FSM_State = 5 ;
		break;
	case 5 :
		s_UART1.Length = Data;
		s_UART1.Length <<= 8;
		s_UART1.FSM_State = 6 ;
		break;
	case 6 :
		s_UART1.Length |= Data;
		s_UART1.Count_i = 0;
		s_UART1.FSM_State = 7 ;
		break;
	case 7 :
		*(s_UART1.DataBuff + s_UART1.Count_i ) = Data;
		s_UART1.Count_i ++;
		if( s_UART1.Count_i >= s_UART1.Length )
		s_UART1.FSM_State = 8 ;
		break;
	case 8 :
		s_UART1.Sum 	= SumData;
		s_UART1.ReSum 	= Data;
		s_UART1.FSM_State = 9 ;
		break;
	case 9 :
		if( Data == 0xee ){
			s_UART1.ReFlag = 1;
		}
		s_UART1.FSM_State = 0;
		break;
	default:
		s_UART1.FSM_State = 0;break;
	}

	SumData += Data ;

}
