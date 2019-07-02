#ifndef __LED_H
#define __LED_H

#include "System.h"

#define LED1  GPIOA, GPIO_Pin_11
#define LED2  GPIOA, GPIO_Pin_12
#define LED3  GPIOA, GPIO_Pin_15

#define GPIO_WriteLow        GPIO_ResetBits
#define GPIO_WriteHigh       GPIO_SetBits

void LED1_ON(void);
void LED1_OFF(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED3_ON(void);
void LED3_OFF(void);

#endif /* ____LED_H */
