#include "System_APP.h"
#include "stddef.h"


u8 JSONVALDataBuff[64];

extern struct SupCOMRxData RxDataStr;


void SystTemCheck(void)
{
//	PrintSYSINFStr("SysTem Check now");

//	PrintSYSINFStr("Start checking external Flash.");

//	PrintSYSINFStr("Start checking Wifi Module.");

//	PrintSYSINFStr("Start checking NetWork.");

//	PrintSYSINFStr("Start checking NTP.");

//	PrintSYSINFStr("Start checking RTC.");
	
}
void SystemAPP_Init(void)
{
	u8 DataBuff[64] , count_i = 0;
	
	SupCOM_ChanreColor(SupCOM_Color_YB);
	
	SupCOMPrintString_Level( "->Step 1 \r\n" , 0);
	SupCOMPrintString_Level( "--Erase Flash Chip , This step will last 1-2 minutes.\r\n" , 0);
	
	SupCOMPrintString_Level( "--Erase \r\n" , 0);

	W25QXX_Erase_Chip_NACK();

	while((W25QXX_ReadSR()&0x01)==0x01)
	{
		DelayS(1);
		SupCOMPrintChar_Level( '.', 0 );
		count_i ++;
		if( count_i >= 180 )
		{
			SupCOM_ChanreColor(SupCOM_Color_RB);
			SupCOMPrintString_Level( "--Erase Flash Chip TimeOut!\r\n" , 0);
			SupCOMPrintString_Level( "Please check hardware!!!\r\n" , 0);
			SupCOM_ChanreColor(SupCOM_Color_YB);
			break;
		}
	}

	SupCOMPrintString_Level( "\r\n" , 0);

	if( count_i < 180 )
	{

		SupCOMPrintString_Level( "--Erase Flash Chip Successful.\r\n" , 0);

		SupCOMPrintString_Level( "->Step 2 \r\n" , 0);
		SupCOMPrintString_Level( "--Write Inf to Flash , This step will last 10-30 Sec.\r\n" , 0);


		SupCOMPrintString_Level( "---1.System Inf. \r\n" , 0);

		DataBuff[0] = 0x1A;

		DataBuff[1] = 'H';
		DataBuff[2] = 'a';
		DataBuff[3] = 'd';
		DataBuff[4] = 'e';
		DataBuff[5] = 's';

		DataBuff[6] = 0x00;

		DataBuff[7] = 0x00;

		W25QXX_Write_Page( DataBuff , 0x000000 , 64 );
		

		SupCOMPrintString_Level( "---2.SupCOM Inf. \r\n" , 0);
		W25QXX_Erase_Sector( 0x002 );

		SupCOM_PClass.ColorMode = ColoMode_ON;
		SupCOM_PClass.Level = PD_Level_2;
		SupCOM_PClass.BackColor = C_Black;
		SupCOM_PClass.ForeColor = C_White;

		DataBuff[0] = SupCOM_PClass.Level;
		DataBuff[1] = SupCOM_PClass.ColorMode;
		DataBuff[2] = SupCOM_PClass.ForeColor;
		DataBuff[3] = SupCOM_PClass.BackColor;

		W25QXX_Write_Page( DataBuff , 0x002001 , 4 );
		
		SupCOM_ChanreColor(SupCOM_Color_YB);

		SupCOMPrintString_Level( "---3.Wifi Inf. \r\n" , 0);

		Server.State = 1;

		DataBuff[4] = Server.State  ;

		W25QXX_Erase_Sector(0x003);

		W25QXX_Write_Page( Server.IP , 0x003000 , 4 );
		W25QXX_Write_Page( DataBuff , 0x003004 , 5 );
		
		SupCOMPrintString_Level( "--Write Inf Successful.\r\n" , 0);
	}

	SupCOMPrintString_Level( "->Step 3 \r\n" , 0);
	SupCOMPrintString_Level( "--Write Workspace to Flash , This step will last 1-5 minutes.\r\n" , 0);

	DelayS(1);

	SupCOMPrintString_Level( "--Write Workspace Successful.\r\n" , 0);


	SupCOMPrintString_Level( "->Step 4 \r\n" , 0);
	SupCOMPrintString_Level( "--ESP Touch Setting.\r\n" , 0);

	ESP_Srt.ESP_TouchState = ESP_NULL;
	
	ESP_Touch();

	if( ESP_Srt.ESP_TouchState == ESP_OK )
	{
		SupCOMPrintString_Level( "--ESP Touch Setting Successful.\r\n" , 0);
		ESP_Init();
	}
	else if( ESP_Srt.ESP_TouchState == ESP_TIMEOUT )
	{	
		SupCOMPrintString_Level( "--ESP Touch Setting Time out.\r\n" , 0);
	}

	SupCOM_ChanreColor(SupCOM_Color_RB);

	SupCOMPrintString_Level( "Restart device Now... Don't turn off the power.\r\n" , 0);
	
	__set_FAULTMASK(1); 
	NVIC_SystemReset();
	
	SupCOM_ChanreColor(SupCOM_Color_WB);

}

u8 Systemregistration( u8 *String ,u8 *RegString, u8 offset )
{
	u8 Count_i = 0 , Res = 0;
	u8 DataBuff[16];
	
	for( Count_i = 0 ; Count_i < 16 ; Count_i++ )
	DataBuff[ Count_i ] = (*( RegString + Count_i )) ^ (*( String + offset + Count_i));

	for( Count_i = 0 ; Count_i < 12 ; Count_i++ )
	{
		if( SystemID[ Count_i ] != DataBuff[ Count_i ] )
		{
			Res = 0;
			break;
		}
		else
		Res = 1; 
	}

	if( Res == 1)
	{	
		SysRegTime.S_Year = ((u16)((DataBuff[12]>>4)*1000)) + \
							((u16)((DataBuff[12]&0x0f)*100))+ \
							((u16)((DataBuff[13]>>4)*10)) + \
							((u16)((DataBuff[13]&0x0f)));

		SysRegTime.S_Month 	= ((DataBuff[14]>>4)*10) + DataBuff[14]&0x0f;		
		SysRegTime.S_Days 	= ((DataBuff[15]>>4)*10) + DataBuff[15]&0x0f;

		SupCOM_ChanreColor(SupCOM_Color_GB);

		SupCOMPrintString_Level( "Registration Code Time : " , 0);
		
		SupCOM_ChanreColor(SupCOM_Color_YB);

		SupCOM_Printf_Num(SysRegTime.S_Year,0); SupCOMPrintChar_Level('-',0);
		SupCOM_Printf_Num(SysRegTime.S_Month,0);SupCOMPrintChar_Level('-',0);
		SupCOM_Printf_Num(SysRegTime.S_Days,0);
		SupCOMPrintString_Level("\r\n",0);
	}
	
	return Res;
}

void TestFUN(void)
{
	u8 Count_i = 0 ;
	u8 DataBuffBuff[16] = "Hades2001TimeKey";
	u8 DataBuff[16];
	u8 TimeBuff[4]={0x20,0x18,0x06,0x06};
	
	for( Count_i = 0 ; Count_i < 12 ; Count_i++ )
	{
		DataBuff[Count_i] = SystemID[Count_i]^DataBuffBuff[Count_i];
		SupCOM_Printf_HEX(SystemID[Count_i]^DataBuffBuff[Count_i] , 0 );

	}
	
	for( Count_i = 0 ; Count_i < 4 ; Count_i++ )
	{
		DataBuff[ 12 + Count_i ] = TimeBuff[Count_i]^DataBuffBuff[12+Count_i];
		SupCOM_Printf_HEX(TimeBuff[Count_i]^DataBuffBuff[12+Count_i] , 0 );

	}

	SupCOMPrintString_Level("\r\n",0);

	Systemregistration(DataBuff,"Hades2001TimeKey",0);

}

void SysTemDec(void)
{
	u8 SysTemBuff[64];
//	u8 RegDataBuff[16];
	u8 Count_i = 0;
//	u8 RegRes = 0;

	W25QXX_Read( SysTemBuff , 0x000000 , 64 );

	if((  SysTemBuff[0] != 0x1A )|| \
		( SysTemBuff[1] != 'H' )|| \
		( SysTemBuff[2] != 'a' )|| \
		( SysTemBuff[3] != 'd' )|| \
		( SysTemBuff[4] != 'e' )|| \
		( SysTemBuff[5] != 's' ))
	{
		
		SupCOM_PClass.Level = 5 ;
		
		SupCOMPrintString_Level( "\r\n" , 0);
		
		SupCOM_ChanreColor(SupCOM_Color_RB);
		SupCOMPrintString_Level( "SysTem First User !! \r\n" , 0);

		SupCOMPrintString_Level( "Init SysTem Now Plase Wait \r\n" , 0);
		
		SupCOM_ChanreColor(SupCOM_Color_YB);

		
		SystemAPP_Init();
	}
		

	SupCOMPrintString_Level( "\r\n" , 0);
	
	SupCOM_ChanreColor(SupCOM_Color_GB);

	SupCOMPrintString_Level( "System ID : " , 0);
	SupCOM_ChanreColor(SupCOM_Color_YB);

	for( Count_i = 0 ; Count_i < 12 ; Count_i++ )
	SupCOM_Printf_HEX( SystemID[Count_i] , 0 );
	
	SupCOMPrintString_Level( "\r\n" , 0);

//	if( SysTemBuff[7] == 0x1B )
//	{
//		
//		
//		for( Count_i = 0 ; Count_i < 16 ; Count_i++ )
//		RegDataBuff[ Count_i ] = SysTemBuff[ 8 + Count_i ];
//		
//		RegRes = Systemregistration(RegDataBuff,"Hades2001TimeKey",0);

//		if( RegRes != 1)
//		{
//			SysTemBuff[7] = 0x00;
//		}

//	}
//	else if( SysTemBuff[7] != 0x1B )
//	{
//		
//		SupCOM_ChanreColor(SupCOM_Color_RB);
//		
//		SupCOMPrintString_Level( "Software not registered \r\n" , 0);
//				
//		SupCOM_ChanreColor(SupCOM_Color_GB);
//		
//		SupCOMPrintString_Level( "Please enter the registration code \r\n" , 0);
//		
//		SupCOM_ChanreColor(SupCOM_Color_YB);

//		while(1)
//		{
//			if( RxDataStr.RxFlag == 1)
//			{
//				RxDataStr.RxFlag = 0;
//				SupCOMStringToHex(RxDataStr.RxString,RegDataBuff,16);
//	
//				SupCOMPrintString_Level( "\r\n" , 0);

//				RegRes = Systemregistration(RegDataBuff,"Hades2001TimeKey",0);

//				if( RegRes == 1)
//				{
//					SupCOMPrintString_Level( "Registered successfully. \r\n" , 0);
//					break;
//				}
//				else
//				{
//					SupCOMPrintString_Level( "Registered Failed. Please Enter Again.\r\n" , 0);
//				}
//			}
//		}

//		SysTemBuff[7] = 0x1B;
//		
//		for( Count_i = 0 ; Count_i < 16 ; Count_i++ )
//		SysTemBuff[ 8 + Count_i ] = RegDataBuff[ Count_i ];

//		SupCOMPrintString_Level( "Write registration code Now  \r\n" , 0);

//		W25QXX_Erase_Sector( 0x000 );
//		W25QXX_Write_Page( SysTemBuff , 0x000000 , 64 );

//		SupCOMPrintString_Level( "Write registration code Successful!  \r\n" , 0);
//	}

	SupCOM_ChanreColor(SupCOM_Color_WB);

}

void SysTemInf(void)
{	
	SupCOMPrintString_Level( "\r\n" , 0);

	SupCOM_ChanreColor(SupCOM_Color_GB);
	SupCOMPrintString_Level( "Target server IP : " , 0);

	SupCOM_ChanreColor(SupCOM_Color_YB);
	
	if( Server.State == 1)
	SupCOMPrintString_Level( ServerIP , 0 );
	else
	{
		SupCOM_Printf_Num(Server.IP[0],0);SupCOMPrintChar_Level( '.' ,0);
		SupCOM_Printf_Num(Server.IP[1],0);SupCOMPrintChar_Level( '.' ,0);
		SupCOM_Printf_Num(Server.IP[2],0);SupCOMPrintChar_Level( '.' ,0);
		SupCOM_Printf_Num(Server.IP[3],0);
	}
	
	SupCOM_ChanreColor(SupCOM_Color_GB);
	SupCOMPrintString_Level( "---->Port : " , 0);

	SupCOM_ChanreColor(SupCOM_Color_YB);
	
	if( Server.State == 1)
	SupCOM_Printf_Num( ServerPort , 0 );
	else
	SupCOM_Printf_Num( Server.Port , 0 );	

	SupCOMPrintString_Level( "\r\n" , 0);
	
	
	SupCOM_ChanreColor(SupCOM_Color_GB);
	SupCOMPrintString_Level( "Wifi MAC : " , 0);

	SupCOM_ChanreColor(SupCOM_Color_YB);
	SupCOM_Printf_HEX(SATMAC[0],0);SupCOMPrintChar_Level( '-' ,0);	
	SupCOM_Printf_HEX(SATMAC[1],0);SupCOMPrintChar_Level( '-' ,0);	
	SupCOM_Printf_HEX(SATMAC[2],0);SupCOMPrintChar_Level( '-' ,0);	
	SupCOM_Printf_HEX(SATMAC[3],0);SupCOMPrintChar_Level( '-' ,0);	
	SupCOM_Printf_HEX(SATMAC[4],0);SupCOMPrintChar_Level( '-' ,0);	
	SupCOM_Printf_HEX(SATMAC[5],0);
	SupCOMPrintString_Level( "\r\n" , 0);

	
	SupCOM_ChanreColor(SupCOM_Color_GB);
	SupCOMPrintString_Level( "Wifi State : " , 0);

	SupCOM_ChanreColor(SupCOM_Color_YB);

	switch(ESP_Srt.ESP_AP_State)
	{
			case 2:
			SupCOMPrintString_Level(" Connected AP,Get IP \r\n",0);
			break;
			case 3:
			SupCOMPrintString_Level(" Established TCP or UDP \r\n",0);
			break;
			case 4:
			SupCOMPrintString_Level(" Disconnect \r\n",0);
			break;
			case 5:	
			SupCOM_ChanreColor(SupCOM_Color_RB);
			SupCOMPrintString_Level(" ununited AP \r\n",0);
			break;
	}
	SupCOM_ChanreColor(SupCOM_Color_YB);


	if( ESP_Srt.ESP_AP_State != 5 )
	{
//		AT_Send_ReCall("AT+CWJAP_DEF?",AT_APName,NULL,NULL);
		SupCOM_ChanreColor(SupCOM_Color_GB);
		SupCOMPrintString_Level( "AP Name : ", 0 );
		SupCOM_ChanreColor(SupCOM_Color_YB);
		
		SupCOMPrintString_Level( ESP_Srt.ESP_APName , 0 );
		SupCOMPrintString_Level( " \r\n" , 0 );
		
		
		SupCOM_ChanreColor(SupCOM_Color_GB);
		SupCOMPrintString_Level( "Wifi LAN IP : " , 0);

		SupCOM_ChanreColor(SupCOM_Color_YB);
		SupCOM_Printf_Num(ESP_Srt.ESP_IP[0],0);SupCOMPrintChar_Level( '.' ,0);
		SupCOM_Printf_Num(ESP_Srt.ESP_IP[1],0);SupCOMPrintChar_Level( '.' ,0);
		SupCOM_Printf_Num(ESP_Srt.ESP_IP[2],0);SupCOMPrintChar_Level( '.' ,0);
		SupCOM_Printf_Num(ESP_Srt.ESP_IP[3],0);
	}
	else
	{
		SupCOM_ChanreColor(SupCOM_Color_RB);
		SupCOMPrintString_Level( "ESP Get AP Error . off-line mode \r\n", 0 );
	}


	SupCOMPrintString_Level( "\r\n" , 0);

	SupCOM_ChanreColor(SupCOM_Color_WB);

}

void SystemDataPrepare(void)
{
//	PrintSYSINFStr("Preparing the Database Environment");



}


