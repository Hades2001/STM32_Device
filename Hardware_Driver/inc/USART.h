#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

extern void USART_SendBit_S(unsigned char Data , USART_TypeDef* USARTx);
extern void USART_SendString_S(char *nString , USART_TypeDef* USARTx);

extern void USART1_SendBit(unsigned char Data);
extern void USART2_SendBit(unsigned char Data);
extern void USART3_SendBit(unsigned char Data);

extern void USART1_SendString(char *nString);
extern void USART2_SendString(char *nString);
extern void USART3_SendString(char *nString);

extern void USART1_RxOver_Interrupt(void);
extern void USART2_RxOver_Interrupt(void);
extern void USART3_RxOver_Interrupt(void);

extern unsigned char USART1_GetRxFlag(void);
extern unsigned char USART1_ClearRxFlag(void);
extern char USART1_ReadRxData(void);

extern unsigned char USART2_GetRxFlag(void);
extern unsigned char USART2_ClearRxFlag(void);
extern char USART2_ReadRxData(void);

struct CamUartFsm{

    unsigned char FSM_State;
    unsigned char CMDBuff[2];
    unsigned char DataBuff[64];
    unsigned char Length;
    unsigned char DataFlag;

    unsigned char Count_i;
};

struct USART_Line
{
    unsigned char FSM_State;

    unsigned char   CMD;
    unsigned int    ID;
    unsigned int    Length;
    unsigned char   *DataBuff;

    unsigned char   Sum;
    unsigned char   ReSum;
    unsigned char   ReFlag;

    unsigned int    Count_i;
};

extern struct   USART_Line s_UART1;
extern struct   CamUartFsm s_CamUART,s_IPUAR;



#endif /* ____USART_H */
