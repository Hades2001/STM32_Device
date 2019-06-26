#include "CamApp.h"

struct CAMData 		s_ReadData;
struct CamStateStr 	CamState;

uint8_t SceneID = 0,GAINID = 0,DiskID = 0,AWC_ID = 0;

uint8_t Str_UserA[32];

s8 ReadAndSetCamInit()
{
	s8 ReCam;

	CamState.Mode 	= 0x00;
	CamState.Rec  	= 0x00;
	CamState.USB	= 0x00;

	Cam_SendCMD_S(0x9336,0x00,WRITE_RAM,2000,10,DEF_CAMPAM);
	Cam_SendCMD_S(0x24	,0x00,WRITE_EPM,2000,10,DEF_CAMPAM);
	Cam_SendCMD_S(0x1FF	,0x00,WRITE_EPM,2000,10,DEF_CAMPAM);
	OSD_ClearAll();
	
	ReCam = ReadData_S( 0x90B7 , &s_ReadData , 1 , READ_RAM , 5000 , 20 ,DEF_CAMPAM);

	if( ReCam == SUCCESS )
	{
		CamState.Scene = s_ReadData.Databuff[0];
		DisSceneLED( CamState.Scene );
	}
	else
	{
		return ReCam;
	}

	Cam_SendCMD_S(0x9314,0x06,WRITE_RAM,2000,10,DEF_CAMPAM);

	ReCam = ReadData_S( 0x907F , &s_ReadData , 1 , READ_RAM , 5000 , 20 ,DEF_CAMPAM);
	if( ReCam == SUCCESS )
	{
		CamState.GAIN = s_ReadData.Databuff[0];
	}
	else
	{
		return ReCam;
	}

	return ReCam;
}

void DisSceneLED( uint8_t SecneNum )
{
	SysKLStr.LEDState  &= ~( LED_Scene1 | LED_Scene2 | LED_Scene3 );

	switch( SecneNum )
	{
		case 0x01:	SysKLStr.LEDState |= LED_Scene1; 	OSD_AutoPrint(0,0,"SCENE 1",5,&SceneID);	break;
		case 0x02:	SysKLStr.LEDState |= LED_Scene2;	OSD_AutoPrint(0,0,"SCENE 2",5,&SceneID);	break;
		case 0x03:	SysKLStr.LEDState |= LED_Scene3;	OSD_AutoPrint(0,0,"SCENE 3",5,&SceneID);	break;
	}
}

void ChangeGAIN()
{
	CamState.GAIN++;
	if( CamState.GAIN > 3 )
	{
		CamState.GAIN = 1;
	}
	
	switch( CamState.GAIN )
	{
		case 0x01:	OSD_AutoPrint(0,0,"GAIN OFF",5,&GAINID);	break;
		case 0x02:	OSD_AutoPrint(0,0,"GAIN HIGH",5,&GAINID);	break;
		case 0x03:	OSD_AutoPrint(0,0,"GAIN LOW",5,&GAINID);	break;
	}

	Cam_SendCMD_S(0x907F,CamState.GAIN,WRITE_RAM,2000,10,DEF_CAMPAM);
}

void SetAWC(void)  
{
	uint8_t TimeCount = 0;

	OSD_AutoPrint(0,0,"AWC Process",10,&AWC_ID);

	Cam_SendCMD_S(0x90B1,0x01,WRITE_RAM,2000,10,DEF_CAMPAM);

	do{
		ReadData_S( 0x9416 + (( CamState.Scene - 1 ) * 0x05 ) , &s_ReadData , 1 , READ_RAM , 5000 , 20 ,DEF_CAMPAM);
		TimeCount ++;
	}while(( s_ReadData.Databuff[0] == 0x02 )&&( TimeCount < 200 ));

	if( s_ReadData.Databuff[0] == 0x01 )
		OSD_AutoPrint(0,0,"AWC OK",5,&AWC_ID);
	else if( TimeCount >= 200 )
		OSD_AutoPrint(0,0,"AWC TimeOUT",5,&AWC_ID);
	else if( s_ReadData.Databuff[0] == 0x00 )
		OSD_AutoPrint(0,0,"AWC NG",5,&AWC_ID);
}

s8 ReadIPCamacity()
{
	s8 ReIP;
	u8 Count;

	ReIP = ReadData_S( 0x8070 , &s_ReadData , 16 , READ_RAM , 5000 , 20 ,DEF_IPPAM);
	if( ReIP == SUCCESS )
	{
		#ifdef	DEBUG
		for( Count = 0 ; Count < 16 ; Count ++ )
		{
			printf( "%02X",s_ReadData.Databuff[Count]);
		}
		printf("\r\n");
		#endif

		CamState.FullCapacity = 0;
		for( Count = 0 ; Count < 8 ; Count ++ )
		{
			CamState.FullCapacity <<= 8;
			CamState.FullCapacity |= s_ReadData.Databuff[ 7 - Count ];
		}
		CamState.RemainCalacity = 0;
		for( Count = 0 ; Count < 8 ; Count ++ )
		{
			CamState.RemainCalacity <<= 8;
			CamState.RemainCalacity |= s_ReadData.Databuff[ 15 - Count ];
		}
		return ReIP;
	}
	else
	{
		return ReIP;
	}
}

void ReadIPStateLED()
{

	if( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11) == 1 )
	{
		SysKLStr.LEDState  &= ~LED_USB ;
		CamState.USB		= 0x00;
	}
	else
	{
		SysKLStr.LEDState  |= LED_USB ;
		CamState.USB		= 0x01;
	}

	if( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12) == 1 )
	{
		SysKLStr.LEDState  &= ~LED_REC ;
		CamState.Rec		= 0x00;
	}
	else
	{
		SysKLStr.LEDState  |= LED_REC ;
		CamState.Rec		= 0x01;
	}

	if( SysKLStr.IPLEDMountStateSava != SysKLStr.IPLEDMountState )
	{
		switch( SysKLStr.IPLEDMountState )
		{
			case MOUNTING:
				OSD_AutoPrint(0,0,"Disk is mounting",5,&DiskID);
			break;
			case MOUNTED:
				if( ReadIPCamacity() == SUCCESS )
				{
					sprintf((char * )Str_UserA, "Mounted %d.%01dGB/%d.%01dGB \r\n"	\
													,(u32)(CamState.RemainCalacity/1024/1024)\
													,(u32)(CamState.RemainCalacity/1024%1024/100)\
													,(u32)(CamState.FullCapacity/1024/1024)\
													,(u32)(CamState.FullCapacity/1024%1024/100));
					#ifdef	DEBUG
					printf( "Mounted %d.%01dGB/%d.%01dGB \r\n"	\
													,(u32)(CamState.RemainCalacity/1024/1024)\
													,(u32)(CamState.RemainCalacity/1024%1024/100)\
													,(u32)(CamState.FullCapacity/1024/1024)\
													,(u32)(CamState.FullCapacity/1024%1024/100));
					#endif
					OSD_AutoPrint(0,0,Str_UserA,20,&DiskID);
				}
				else
				{
					OSD_AutoPrint(0,0,"Read Error",5,&DiskID);
				}
			break;
			case UMOUNT:
				OSD_AutoPrint(0,0,"Disk is umount",5,&DiskID);
			break;
		}
		SysKLStr.IPLEDMountStateSava = SysKLStr.IPLEDMountState;
	}
}

