#ifndef __ESP_AT_H
#define __ESP_AT_H


#include "USART.h"
#include "delay.h"
#include "SupCOM.h"

extern 	void 	ESP_Init(void);

extern 	void 	AT_Test(void);
extern 	void 	AT_FUM( u8 Data );
extern 	u8 		AT_LinkTCP_Sever(void);
extern 	u8 		AT_LinkTCP_Sever_IPStr( u8 LinkType , u8 *IPString , u32 PortNum );
extern 	u8 		AT_CloseSeverLink(void);
extern 	u8 		AT_WaitTCPData( u32 TimeOUTMs );
extern 	void 	AT_Query_connection(void);
extern 	void 	AT_DisTPCData(void);

extern 	u8 		AT_SendCMD( u8 *String );
extern 	u8 		AT_Send_ReCall( u8 *String , void (*AT_OK_FUN)() , void (*AT_ERROR_FUN)() , void(*AT_TIME_OUT_FUN)());
extern 	void 	SendBuffData(void);
extern 	void 	AT_GetSATAMAC(void);
extern 	void 	AT_TCP_SetReDataMode(void);
extern 	u8 		AT_GetTCPReFlag(void);
extern 	u8 		AT_SendTCPData(u16 DataNum ,u8 *DataBuff);
extern 	void 	AT_CopyTCPBuff( u8 *Databuff , u16 DataNum );
extern 	void 	AT_GETNetSta(void);

extern 	void 	AT_APName( void );
extern 	u8 		ESP_Touch(void);

extern 	u8 		AT_StringtoIP( u8 *IPStringBuff , u8 *IPBuff );
extern 	void 	ESP_ReadIPPort(void);
extern 	void 	ESP_WriteServerData( void );

extern 	u8 		AT_OSPFTCPStart( void );
extern 	void 	AT_Send_OSPFTCPData( u16 DataNum , u8 *DataBuff );

extern 	u8 		AT_EndOSPFTCP( void );



extern u8 SATMAC[6];

#define ServerIP		"120.79.92.192"
#define ServerPort		9001


#define AT_ATARE_A		0
#define AT_ATARE_T		1
#define AT_ATARE_CMD	2
#define AT_ATARE_DATA	3
#define AT_ATARE_Flag	4

#define AT_TCP_Send		0xA0
#define AT_TCP_SendRe	0xA1
#define AT_TCP_ReData	0xA2
#define AT_TCP_Re_I		0xA3
#define AT_TCP_Re_D		0xA4
#define AT_TCP_Re_P		0xA5
#define AT_TCP_Re_F		0xA6
#define AT_TCP_ReNum	0xA7
#define AT_TCP_Data		0xA8
#define AT_TCP_END		0xA9

#define AT_TCP_TYPE		0x01	
#define AT_UDP_TYPE		0x02

#define AT_TCP_SendReady	0xAA

#define AT_ESP_T_0		0xE0


#define AT_NULL			0
#define AT_OK			1
#define AT_ERROR		2
#define	AT_TOut			3

#define ESP_NULL		0
#define ESP_OK			1
#define	ESP_ERROR		2
#define	ESP_TIMEOUT		3



#define AT_TCP_NULL		0
#define AT_TCP_OK		1
#define AT_TCP_ERROR	2

#define AT_TCP_Receive	4
#define AT_TCP_NEmpty	5
#define AT_TCP_GetReady	6

#define AT_TimeOUT		20
	
#define ESP_AP_LINK			1
#define ESP_AP_UNUNITED		2

#define TCP_Null			0
#define TCP_SupCOM_Transfer	1
#define TCP_Sever_Transfer	2

#define	TCP_LINK_NULL		0
#define TCP_LINK_OK			1
#define TCP_LINK_TIMEOUT	2
#define TCP_LINK_ERROR		3


struct AT_SETStr
{
	u8 	AT_Start;
	u8 	AT_CMDString[64];

	u8 	AT_DataNum;
	u8 	AT_ReDataBuff[16][64];

	u8 	AT_ReFlag;
	u16 TimeOUT_Count ;
};

struct AT_TCPStr
{
	u8 	AT_TCPReBuff[4096];
	 
	u8 	AT_TCPNumBuff[8];
	u16 AT_TCPReDataNum;

	u8 	AT_TCPFlag;
	u8 	AT_TCP_ReFlag;

	u8 	TCP_State;
	u8	TCP_LinkState;
};

struct ESP_Wifi
{
	u8 	ESP_State;
	u8 	ESP_AP_State;
	u8 	ESP_TouchState;
	
	u8	ESP_FUM_Count;

	u8 	ESP_APName[64];

	u8 	ESP_MAC[6];

	u8	ESP_IPString[17];
	u8	ESP_IP[4];

	u8	ESP_Net_State;

	u8 	ESP_ReDataBuff[128];

	
};

struct ServerSrt
{
	u8  IPString;
	u8	IP[4];

	u32 Port;

	u8 State;
};

extern struct 	AT_SETStr 	AT_Str;
extern struct	AT_TCPStr	AT_TCP;
extern struct 	ESP_Wifi	ESP_Srt;
extern struct  	ServerSrt	Server;


//extern struct	AT_TCPStr	AT_TCP;

#endif /* ____ESP_AT_H */

