#ifndef __DELAY_H
#define __DELAY_H 			   

#include "System.h"

extern int DelayInit(void);
extern void DelayMs(unsigned int nms);
extern void DelayUs(unsigned long nus);
extern void DelayS(unsigned int ns);

#endif
