#ifndef __SC6803_H
#define __SC6803_H

#include "System.h"

#define SC6803_CLK      GPIOB , GPIO_Pin_8
#define SC6803_SDA      GPIOB , GPIO_Pin_5


void SC6803_SendBuff( uint16_t *DataBuff , uint16_t Length );


#endif
