#ifndef __LED_H
#define __LED_H

#include "System.h"

#define LEDMaxNum   3
#define LED_ON      0
#define LED_OFF     !LED_ON

#define LED_ON_GPIO(g,p)    GPIO_SetBits(g,p)
#define LED_OFF_GPIO(g,p)   GPIO_ResetBits(g,p)

#define LED1        0
#define LED2        1
#define LED3        2


struct  LEDStr{

    uint8_t         LED_State;
    GPIO_TypeDef* 	GPIOx;
	int16_t 		GPIO_Pin;
};

void LEDs_Ctrl( uint16_t Num , uint8_t State );


#endif /* ____LED_H */
