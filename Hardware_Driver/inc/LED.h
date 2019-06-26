#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define LED1  GPIOE, GPIO_Pin_3
#define LED2  GPIOE, GPIO_Pin_2

#define GPIO_WriteLow        GPIO_ResetBits
#define GPIO_WriteHigh       GPIO_SetBits

extern void LED1_ON(void);
extern void LED1_OFF(void);
extern void LED2_ON(void);
extern void LED2_OFF(void);

#endif /* ____LED_H */
