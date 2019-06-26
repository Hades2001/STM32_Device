#ifndef __IIC1602_H
#define __IIC1602_H

#include "stm32f10x.h"


extern void IIC1602Init(void);
extern void IIC1602CLR(void);
extern void IIC1602_WString(u8 hang,u8 lie,char *string);
extern void IIC1602_Dis3Num(u8 hang,u8 lie,u16 LCD1602_Num);


#endif /* ____IIC1602_H */
