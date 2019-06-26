#ifndef __FLASH_API_H
#define __FLASH_API_H		

#include "USART.h"
#include "W25QXX.h"
#include "malloc.h"  
#include "ff.h"  
#include "exfuns.h"
#include "OLED_GUI.h"
#include "delay.h"
#include "WS2812.h"
#include "SupCOM.h"

extern u8 Hex_Flash(void);
extern void TestMenu(void);
#endif

