#ifndef __EXTERNFLASHAPP_H
#define __EXTERNFLASHAPP_H

#include "stm32f10x.h"
#include "W25QXX.h"

#define Extern_FlashSize			

#define ExFlashSectorsSize			4096
#define ExFlashSectorsBuffSize		4096




struct FLASH_TypeDef
{
	u8 *FlashName;

	u8 	Type;
	
	u32	FlashSize;
	u32 FlashID;
	
};

struct ExFlash_ACtion
{
	u8 		State;

	u32 	WriteADDR;
	u32 	ReadADDR;

	u8 		SectorBuff[ExFlashSectorsBuffSize];

	u8 		*WriteBuffPtr;
	u8 		*ReadBuffPtr;

	u32 	WriteDataNum;
	u32 	ReadDataNum;
	
	u8  	*DataPrt;
	u8		DataSize;
};

extern struct FLASH_TypeDef *ExFlash;




#endif /* __EXTERNFLASHAPP_H */

