#ifndef __SupCOM_H
#define __SupCOM_H


#include "USART.h"

extern		unsigned char GetRxFlag(void);
extern		unsigned char ClearnRxFlag(void);
extern		void TxBitData(unsigned char Data);
extern		void TxStringData(char *String);
extern		char ReadRxData(void);
extern		void ReadStringDR(char *String);
extern		unsigned char ContrastString(char *StringA,char *StringB);
extern		unsigned char ContrastString_nNum(char *StringA,char *StringB,unsigned int n);
extern    void Printf_Num(unsigned long int Num_Data);
extern		void Printf_HEX(unsigned char HEXData);


#endif /* ____SupCOM_H */

