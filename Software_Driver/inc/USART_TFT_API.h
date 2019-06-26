//USART_TFT_API.h
#ifndef __USART_TFT_API_H
#define __USART_TFT_API_H

extern void USART_TFT_SendCMD(char *CMD_Data);
extern void USART_RxHandle(void);
extern unsigned char USARTRx_Data[20];
extern void Main_Con(void);
//extern void WriteUserStrToEEPROM(struct UserStr *Ptr,unsigned int Start_Add);
//extern void ReadEEPROMToUserStr(struct UserStr *Ptr,unsigned int Start_Add);
/**************************************************/


#endif /* __USART_TFT_API_H */
