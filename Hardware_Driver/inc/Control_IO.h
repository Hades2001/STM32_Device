#ifndef __Control_IO_H
#define __Control_IO_H

#include "stm32f10x.h"

#define Control_IO1  GPIOA, GPIO_Pin_6
#define Control_IO2  GPIOA, GPIO_Pin_7
#define Control_IO3  GPIOA, GPIO_Pin_4
#define Control_IO4  GPIOA, GPIO_Pin_5

#define GPIO_WriteLow        GPIO_ResetBits
#define GPIO_WriteHigh       GPIO_SetBits

extern void Control_IO1_ON(void);
extern void Control_IO2_ON(void);
extern void Control_IO3_ON(void);
extern void Control_IO4_ON(void);

extern void Control_IO1_OFF(void);
extern void Control_IO2_OFF(void);
extern void Control_IO3_OFF(void);
extern void Control_IO4_OFF(void);

#endif /* ____Control_IO_H */

