#ifndef __AT24C0x_H
#define __AT24C0x_H

#include "stm32f10x.h"


extern void AT24C0x_Write_1Bit(unsigned int Address,unsigned char Data);
extern unsigned char AT24C0x_Read_1Bit(unsigned int Address);
extern void AT24C0x_Write_nBit(unsigned int Address,unsigned char Digit,unsigned char *Data);
extern void AT24C0x_Read_nBit(unsigned int Address,unsigned char Digit,unsigned char *Data);

#endif /* ____AT24C0x_H */

