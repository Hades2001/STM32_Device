#ifndef __BSP_System_H
#define __BSP_System_H

/**************************************************/

#include "stm32f10x.h"

/****************Type conversion*******************/

typedef unsigned char       bool;
typedef unsigned char       u8;
typedef unsigned short      u16;
#define  True  1
#define  False 0

/*************Include_System_ConfCode**************/

#include "Hardware_System.h"
#include "Software_System.h"


/**************  Extern_System   ******************/ 

extern void Hardware_Init(void);
extern void Software_Init(void);
extern void CPUsware_Init(void);
extern void BSP_ALL_Inits(void);

/**************Include_CPU_ConfCode****************/
#include "stm32f10x.h"


#endif /* __BSP_System_H */
