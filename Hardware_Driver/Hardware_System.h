#ifndef __Hardware_System_H
#define __Hardware_System_H

/**************************************************/

/************Hardware_Include**********************/
#include "BSP_System.h"
#include "IIC.h"
#include "delay.h"
#include "LED.h"
#include "USART.h"
#include "AT24C0x.h"
#include "NY3P065.H"
#include "Control_IO.h"

extern void HardWare_ALL_Init(void);
extern unsigned char SystemID[12];

#endif /* __Hardware_System_H */
