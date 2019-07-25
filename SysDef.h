#ifndef __SYSDEF_H__
#define __SYSDEF_H__

#include "SysConfig.h"

typedef unsigned char       bool;
typedef unsigned char       u8;
typedef unsigned short      u16;

#define TRUE    1
#define FALSE   !TRUE


#define SECTION(x)                      __attribute__((section(x)))
#define SYS_UNUSED                      __attribute__((unused))
#define SYS_USED                        __attribute__((used))
#define ALIGN(n)                        __attribute__((aligned(n)))

typedef int (*init_fn_t)(void);

#define INIT_EXPORT(fn, level)  SYS_USED const init_fn_t __sys_init_##fn SECTION(".sysi_fn."level) = fn

#define INIT_BOARD_EXPORT(fn)           INIT_EXPORT(fn, "1")
#define INIT_CPU_EXPORT(fn)             INIT_EXPORT(fn, "2")
#define INIT_DEVICE_EXPORT(fn)          INIT_EXPORT(fn, "3")
#define INIT_COMPONENT_EXPORT(fn)       INIT_EXPORT(fn, "4")
#define INIT_FS_EXPORT(fn)              INIT_EXPORT(fn, "5")
#define INIT_APP_EXPORT(fn)             INIT_EXPORT(fn, "6")


#define ENABLE_MALLOC
#define ENABLE_RTOS

#endif  //__SYSDEF_H__
