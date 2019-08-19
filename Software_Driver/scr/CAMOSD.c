/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2019-06-21 09:23:54
 * @LastEditors: sueRimn
 * @LastEditTime: 2019-08-19 10:36:34
 */
#include "CAMOSD.h"

struct 	OSDTimeStr HeadOSCTime = { 0,0,NULL,0,0,0,0,0,0,NULL};
uint8_t OSDSysState = OSD_RUN;

uint8_t ReNullOSDID( void )
{
	uint8_t OSDCheckID[MAX_ID];
	uint8_t Count_i;
	struct 	OSDTimeStr *pOSDStrn;

	for( Count_i = 0 ; Count_i < MAX_ID ; Count_i ++ )
	OSDCheckID[ Count_i ] = 0;

	pOSDStrn = HeadOSCTime.Next;
	OSDCheckID[0] = 1;

	while( pOSDStrn != NULL )
	{
		OSDCheckID[ pOSDStrn->ID ] = 1;
		pOSDStrn = pOSDStrn->Next;
	}

	for( Count_i = 0 ; Count_i < MAX_ID ; Count_i ++ )
	{
		if( OSDCheckID[Count_i] == 0 )
		return Count_i;
	}
	return 0;
}

struct 	OSDTimeStr *FindOSDqtr( uint8_t ID )
{
	struct 	OSDTimeStr *pOSDStrn;

	pOSDStrn = HeadOSCTime.Next;
	while( pOSDStrn != NULL )
	{
		if( pOSDStrn->ID == ID )
		return pOSDStrn;

		pOSDStrn = pOSDStrn->Next;
	}
	return NULL;
}

uint8_t OSD_AutoSet( 	uint8_t xPOS , 
						uint8_t yPOS ,
						uint8_t *Str ,
						uint8_t StrSize , 
						uint8_t Time,
						uint8_t *pID )
{
	uint8_t ReState = OSD_FAILED;
	if(( *pID == 0 )||( *pID > MAX_ID ))
	{
		*pID = OSDFastCreat_Malloc(xPOS,yPOS,Str,StrSize,Time);
		return *pID;
	}
	else
	{
		ReState = OSD_SetFromID(xPOS,yPOS,Str,StrSize,Time,*pID);
		if( ReState == OSD_FAILED )
		{		
			*pID = OSDFastCreat_Malloc(xPOS,yPOS,Str,StrSize,Time);
			return *pID;
		}
	}
	return ReState;
}

uint8_t OSDFastCreat_Malloc( uint8_t xPOS , uint8_t yPOS ,uint8_t *Str ,uint8_t StrSize , uint8_t Time )
{
	struct OSDTimeStr *pOSDStr;
	uint8_t	OSD_IDNum;

	OSD_IDNum = ReNullOSDID();
	if( OSD_IDNum == 0 )
	return OSD_FAILED; 

	pOSDStr = m_Malloc( pOSDStr , sizeof( HeadOSCTime ));

	if( pOSDStr != NULL )
	{
		pOSDStr->pStr 	= m_Malloc( pOSDStr->pStr , StrSize );
		pOSDStr->Times 	= Time;
		pOSDStr->Next 	= NULL;
		pOSDStr->State	= OSD_STOP;
		pOSDStr->Xpos 	= xPOS;
		pOSDStr->YPos 	= yPOS;
		pOSDStr->ID		= OSD_IDNum;
		
		if( pOSDStr->pStr != NULL )
		{
			m_Memcopy( pOSDStr->pStr , Str , StrSize );
			SetOSD_Time( pOSDStr );
			return pOSDStr->ID;
		}
		else	
			return OSD_FAILED;
	}
	else
		return OSD_FAILED;
}

uint8_t OSD_SetFromID( uint8_t xPOS , uint8_t yPOS ,uint8_t *Str ,uint8_t StrSize , uint8_t Time , uint8_t ID )
{
	struct 	OSDTimeStr *pOSDStrn;

	OSDSysState = OSD_STOP;
	pOSDStrn = FindOSDqtr( ID );
	if( pOSDStrn != NULL )
	{
		pOSDStrn->Xpos = xPOS;
		pOSDStrn->YPos = yPOS;
		pOSDStrn->Times = Time;
		m_free(pOSDStrn->pStr);
		pOSDStrn->pStr 	= m_Malloc( pOSDStrn->pStr , StrSize );
		if( pOSDStrn->pStr != NULL )
		{
			m_Memcopy( pOSDStrn->pStr , Str , StrSize );
			SetOSD_Time( pOSDStrn );
			OSDSysState = OSD_RUN;
			return OSD_OK;
		}
	}
	OSDSysState = OSD_RUN;
	return OSD_FAILED;
}

uint8_t OSD_DeleteFromID( uint8_t ID )
{
	struct OSDTimeStr *pOSDStrn , *pOSDStrt , *pOSDSava;

	pOSDStrt = &HeadOSCTime;
	pOSDStrn = HeadOSCTime.Next;

	while( pOSDStrn != NULL )
	{
		if( pOSDStrn->ID == ID )
		{
			OSD_Clear( pOSDStrn->Xpos , pOSDStrn->RealYPos , pOSDStrn->Count_Str );
			m_free( pOSDStrn->pStr );
			pOSDStrt->Next = pOSDStrn->Next;
			pOSDSava = pOSDStrn->Next;
			pOSDStrn->Next = NULL;
			m_free(pOSDStrn);
			pOSDStrn = pOSDSava;
			return OSD_OK;
		}
		else
		{
			pOSDStrt = pOSDStrn;
			pOSDStrn = pOSDStrn->Next;
		}
	}
	return OSD_FAILED;
}

void SetOSD_Time( struct OSDTimeStr *pOSDStr )
{
	struct OSDTimeStr *pOSDStrn , *pOSDStrt ;
	uint8_t	LineFull[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t Count_y;

	pOSDStrt = &HeadOSCTime;
	pOSDStrn = HeadOSCTime.Next;

	while( pOSDStrn != NULL )
	{
		LineFull[ pOSDStrn->RealYPos ] = 1;
		pOSDStrt = pOSDStrn;
		pOSDStrn = pOSDStrn->Next;
	}

	pOSDStrt = &HeadOSCTime;
	pOSDStrn = HeadOSCTime.Next;

	while( pOSDStrn != NULL )
	{
		if( pOSDStr == pOSDStrn )break;
		pOSDStrt = pOSDStrn;
		pOSDStrn = pOSDStrn->Next;
	}

	if( pOSDStr == pOSDStrn )
	{
		OSD_Clear( pOSDStrn->Xpos , pOSDStrn->RealYPos , pOSDStrn->Count_Str );
		LineFull[ pOSDStrn->RealYPos ] = 0;
		
		if( LineFull[pOSDStrn->YPos] == 0 ){
			pOSDStrn->RealYPos = pOSDStrn->YPos;
		}
		else{
			for( Count_y = 0 ; Count_y < 12 ; Count_y ++ ){
				if( LineFull[Count_y] == 0 ){
					pOSDStrn->RealYPos = Count_y;
					break;
				}
			}
		}
	}
	else
	{
		if( LineFull[ pOSDStr->YPos ] == 1 )
		{
			for( Count_y = 0 ; Count_y < 12 ; Count_y ++ )
			{
				if( LineFull[Count_y] == 0 )
				{
					pOSDStr->RealYPos = Count_y;
					break;
				}
			}
		}
		else
		{
			pOSDStr->RealYPos = pOSDStr->YPos;
		}
		pOSDStrt->Next = pOSDStr;
	}
	
	pOSDStr->Count_Str = 0;
	while( *( pOSDStr->pStr + pOSDStr->Count_Str ) != '\0' )
	{
		pOSDStr->Count_Str ++;
	}

	//Cam_SendCMD_S( 0x9E22 + pOSDStr->RealYPos , pOSDStr->Color  , WRITE_RAM , 2000 , 10 ,DEF_CAMPAM);
	OSD_DisString( pOSDStr->Xpos , pOSDStr->RealYPos , pOSDStr->pStr );
	pOSDStr->Color	  = 0x07;
	pOSDStr->Count_Ti = pOSDStr->Times;
	pOSDStr->State 	  = OSD_RUN;
}

void OSDInTime1S( void )
{
	struct OSDTimeStr *pOSDStrn;

	pOSDStrn = HeadOSCTime.Next;

	while( pOSDStrn != NULL )
	{
		if(	( pOSDStrn->Count_Ti > 0 )&& \
			( OSDSysState == OSD_RUN )&& \
			( pOSDStrn->State == OSD_RUN )&&\
			( pOSDStrn->Count_Ti != 255))
		pOSDStrn->Count_Ti --;
		pOSDStrn = pOSDStrn->Next;
	}	
}

void OSDInLoop( void )
{
	struct OSDTimeStr *pOSDStrn , *pOSDStrt , *pOSDSava;

	pOSDStrt = &HeadOSCTime;
	pOSDStrn = HeadOSCTime.Next;

	while( pOSDStrn != NULL )
	{
		if( pOSDStrn->Count_Ti == 0 )
		{
			OSD_Clear( pOSDStrn->Xpos , pOSDStrn->RealYPos , pOSDStrn->Count_Str );
			m_free( pOSDStrn->pStr );
			pOSDStrt->Next = pOSDStrn->Next;
			pOSDSava = pOSDStrn->Next;
			pOSDStrn->Next = NULL;
			m_free(pOSDStrn);
			pOSDStrn = pOSDSava;
		}
		else
		{
			pOSDStrt = pOSDStrn;
			pOSDStrn = pOSDStrn->Next;
		}
	}
}
