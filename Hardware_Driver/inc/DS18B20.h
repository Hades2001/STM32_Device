#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f10x.h"
#include "delay.h"


#define DS18B20_Data   GPIOA,GPIO_Pin_8





extern 	void DS18B20_StartTemp(void);
extern 	u16 DS18B20_ReadTemp(void);






#endif /* __DS18B20_H */
