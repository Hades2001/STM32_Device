#ifndef __CAMCOM_H
#define __CAMCOM_H

#include "USART.h"
#include "delay.h"
#include "SupCOM.h"

#define	WRITE_RAM		1
#define	WRITE_EPM		2

#define	READ_RAM		3
#define	READ_EPM		4

#define	CMD_WRITE_RAM	"%WM:"
#define	CMD_WRITE_EPM	"%WE:"

#define	CMD_READ_RAM	"%RM:"
#define	CMD_READ_EPM	"%RE:"

#define SUCCESS		0
#define FAILED		-1
#define TIEMOUT		-2
#define ERROR		-3
#define COUNTOUT	-4

#define DISP_LINE0   0x98E2
#define CTRL_LINE0   0x9B82

#define COLO_LINE0   0xA3CE

#define	DEF_CAMPAM	 USART2,&s_CamUART
#define	DEF_IPPAM	 USART3,&s_IPUAR

#define CAM_KEY_SEL		GPIOE , GPIO_Pin_10
#define CAM_KEY_MENU	GPIOB , GPIO_Pin_0
#define CAM_KEY_UP		GPIOE , GPIO_Pin_9
#define CAM_KEY_DOWN	GPIOB , GPIO_Pin_1
#define CAM_KEY_LEFT	GPIOE , GPIO_Pin_8
#define CAM_KEY_RIGHT	GPIOE , GPIO_Pin_7

struct CAMData
{	
	u16 Addr;
	u8 	Length;
	u8 	Data;
	u8 	Databuff[64];
};

struct CamStateStr
{
	uint8_t Scene;
	uint8_t AWC;
	uint8_t GAIN;
	uint8_t Rec;
	uint8_t USB;

	uint8_t Mode;

	unsigned __int64	FullCapacity;
	unsigned __int64	RemainCalacity;

};

void SendCMD( u16 Addr ,u8 Data ,u8 SW );
void SendCMDDatas( u16 Addr , u8 *Buff ,u8 Length ,u8 SW );
void SendRead( u16 Addr ,u8 Num ,u8 SW );
s8	Cam_SendCMD( u16 Addr , u8 Data , u8 Target , u16 SetTime , u8 RetriesCount );
s8	Cam_SendCMDBuff( u16 Addr , u8 *Datauff , u8 Length ,u8 Target , u16 SetTime , u8 RetriesCount );
s8 ReadData( u16 Addr , struct CAMData *qData , u8 Length ,u8 Target , u16 SetTime , u8 RetriesCount );
void OSD_ClearAll(void);
void OSD_DisString( u8 XPos, u8 YPos , u8 *Buff );
void OSD_DisChar( u8 XPos, u8 YPos , u8 Char );
void OSD_DisStringColorMode( u8 XPos, u8 YPos , u8 *Buff ,u8 ColorMode);
void OSD_DisCharColorMode( u8 XPos, u8 YPos , u8 Char ,u8 ColorMode );
void OSD_Clear( u8 XPos, u8 YPos , u8 Length );
void SendCMD_S( u16 Addr ,u8 Data ,u8 SW ,USART_TypeDef* USARTx);
void SendRead_S( u16 Addr ,u8 Num ,u8 SW ,USART_TypeDef* USARTx);
void SendCMDDatas_S( u16 Addr , u8 *Buff ,u8 Length ,u8 SW ,USART_TypeDef* USARTx);
s8	Cam_SendCMD_S( u16 Addr , u8 Data , u8 Target , u16 SetTime , u8 RetriesCount , USART_TypeDef* USARTx , struct CamUartFsm *USART_S );
s8	Cam_SendCMDBuff_S( u16 Addr , u8 *Datauff , u8 Length ,u8 Target , u16 SetTime , u8 RetriesCount ,USART_TypeDef* USARTx , struct CamUartFsm *USART_S );
s8 ReadData_S( u16 Addr , struct CAMData *qData , u8 Length ,u8 Target , u16 SetTime , u8 RetriesCount ,USART_TypeDef* USARTx , struct CamUartFsm *USART_S );




#endif /* ____CAMCOM_H */
