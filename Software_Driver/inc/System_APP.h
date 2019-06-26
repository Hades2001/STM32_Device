#ifndef __SYSTEM_APP_H
#define __SYSTEM_APP_H

#include "BSP_System.h"

#include "W25QXX.h"
#include "DS18B20.h"

#include "SupCOM.h"
#include "JSON.h"
#include "SupCOMAPP.h"


void SystTemCheck(void);
void SystemAPP_Init(void);
u8 Systemregistration( u8 *String ,u8 *RegString, u8 offset );
void TestFUN(void);
void SysTemDec(void);
void SysTemInf(void);
void SystemDataPrepare(void);



#endif /* __SYSTEM_APP_H */

