#include "CAMCOM.h"

void Send_HEX_S(unsigned char HEXData,USART_TypeDef* USARTx)
{
		unsigned char Data_H,Data_L;
		Data_L = HEXData&0x0f;
		Data_H = HEXData>>4;

		if(Data_H<10)	USART_SendBit_S(Data_H + 0x30,USARTx);
		else			USART_SendBit_S(Data_H - 10 + 0x41,USARTx);
		
		if(Data_L<10)	USART_SendBit_S(Data_L + 0x30,USARTx);
		else			USART_SendBit_S(Data_L - 10 + 0x41,USARTx);	
}

void SendCMD_S( u16 Addr ,u8 Data ,u8 SW ,USART_TypeDef* USARTx)
{
	u8	CheckData = 0;

	USART_SendBit_S(0x02,USARTx);

	if( SW == WRITE_RAM )
	USART_SendString_S(CMD_WRITE_RAM,USARTx);
	else
	USART_SendString_S(CMD_WRITE_EPM,USARTx);

	Send_HEX_S(( Addr>>8) & 0x00ff ,USARTx);
	Send_HEX_S(  Addr & 0x00ff,USARTx);
	USART_SendBit_S(':',USARTx);
	Send_HEX_S(0x01,USARTx);
	USART_SendBit_S(':',USARTx);
	Send_HEX_S(Data,USARTx);

	CheckData = Data^0xAA;

	USART_SendBit_S(':',USARTx);
	Send_HEX_S(CheckData,USARTx);
	USART_SendBit_S(0x03,USARTx);
}

void SendCMDDatas_S( u16 Addr , u8 *Buff ,u8 Length ,u8 SW ,USART_TypeDef* USARTx)
{
	u8 Count_i ;
	u8	CheckData = 0xAA;

	USART_SendBit_S(0x02,USARTx);

	if( SW == WRITE_RAM )
	USART_SendString_S(CMD_WRITE_RAM,USARTx);
	else
	USART_SendString_S(CMD_WRITE_EPM,USARTx);

	Send_HEX_S(( Addr>>8) & 0x00ff ,USARTx);
	Send_HEX_S(  Addr & 0x00ff,USARTx);
	USART_SendBit_S(':',USARTx);
	Send_HEX_S(Length,USARTx);
	USART_SendBit_S(':',USARTx);
	for( Count_i = 0 ; Count_i < Length ; Count_i++ )
	{
		CheckData = CheckData^(*( Buff + Count_i ));
		Send_HEX_S(*( Buff + Count_i ),USARTx);
	}

	USART_SendBit_S(':',USARTx);
	Send_HEX_S(CheckData,USARTx);
	USART_SendBit_S(0x03,USARTx);

}

void SendCMDSameData_S( u16 Addr , u8 Data ,u8 Length ,u8 SW ,USART_TypeDef* USARTx)
{
	u8 Count_i ;
	u8	CheckData = 0xAA;

	USART_SendBit_S(0x02,USARTx);

	if( SW == WRITE_RAM )
	USART_SendString_S(CMD_WRITE_RAM,USARTx);
	else
	USART_SendString_S(CMD_WRITE_EPM,USARTx);

	Send_HEX_S(( Addr>>8) & 0x00ff ,USARTx);
	Send_HEX_S(  Addr & 0x00ff,USARTx);
	USART_SendBit_S(':',USARTx);
	Send_HEX_S(Length,USARTx);
	USART_SendBit_S(':',USARTx);
	for( Count_i = 0 ; Count_i < Length ; Count_i++ )
	{
		CheckData = CheckData ^ Data ;
		Send_HEX_S( Data , USARTx );
	}
	USART_SendBit_S(':',USARTx);
	Send_HEX_S(CheckData,USARTx);
	USART_SendBit_S(0x03,USARTx);
}


void SendRead_S( u16 Addr ,u8 Num ,u8 SW ,USART_TypeDef* USARTx)
{
	USART_SendBit_S(0x02,USARTx);
	if( SW == READ_RAM )
	USART_SendString_S(CMD_READ_RAM,USARTx);
	else
	USART_SendString_S(CMD_READ_EPM,USARTx);

	Send_HEX_S(( Addr>>8) & 0x00ff , USARTx);
	Send_HEX_S(  Addr & 0x00ff , USARTx);
	USART_SendBit_S(':',USARTx);
	Send_HEX_S(Num,USARTx);
	USART_SendBit_S(0x03,USARTx);
}

s8	Cam_SendCMD_S( u16 Addr , u8 Data , u8 Target , u16 SetTime , u8 RetriesCount , USART_TypeDef* USARTx , struct CamUartFsm *USART_S )
{
	u16 Count_i = 0 ,Count_r = 0;

	while( Count_r < RetriesCount ){

		USART_S->DataFlag = 0;
		Count_i		     = 0;
		SendCMD_S( Addr , Data , Target ,USARTx );
		
		while( Count_i < SetTime )
		{
			if( USART_S->DataFlag == 1 )
			{
				break;
			}
			DelayMs(1);
			Count_i ++ ;
		}
		
		if( Count_i >= SetTime ){
			Count_r ++;
			Count_i = 0;
			#ifdef DEBUG
				USART2_SendString( ">Send CMD Time Out \r\n" );
			#endif
		}
		else{
			USART_S->DataFlag = 0;
			if( USART_S->Length <= 3 )
			{
				if(( USART_S->DataBuff[0] == 'O' )&&( USART_S->DataBuff[1] == 'K' )){
					#ifdef DEBUG
					USART2_SendString( ">Send CMD Succesful ,Time:" );
					Printf_Num(Count_i);
					USART2_SendString("\r\n"); 
					#endif
					return SUCCESS;
				}
				else if(( USART_S->DataBuff[0] == 'N' )&&( USART_S->DataBuff[1] == 'G' )){
					#ifdef DEBUG
					USART2_SendString( ">Send CMD Ng ,Time :" );
					Printf_Num(Count_i);
					USART2_SendString("\r\n"); 
					#endif
					return FAILED;
				}
				else{

					return FAILED;
				}
			}
			else{
				#ifdef DEBUG
				USART2_SendString( ">Send CMD Error \r\n" );
				#endif
				return ERROR;
			}
		}
	}
	return COUNTOUT;
}

s8	Cam_SendCMDBuff_S( u16 Addr , u8 *Datauff , u8 Length ,u8 Target , u16 SetTime , u8 RetriesCount ,USART_TypeDef* USARTx , struct CamUartFsm *USART_S )
{
	u16 Count_i = 0 ,Count_r = 0;
	while( Count_r < RetriesCount ){

		USART_S->DataFlag = 0;
		Count_i		     = 0;
		SendCMDDatas_S( Addr , Datauff , Length , Target ,USARTx );
		
		while( Count_i < SetTime )
		{
			if( USART_S->DataFlag == 1 )
			{
				break;
			}
			DelayMs(1);
			Count_i ++ ;
		}
		
		if( Count_i >= SetTime ){
			Count_r ++;
			Count_i = 0;
			#ifdef DEBUG
				USART2_SendString( ">Send CMD Time Out \r\n" );
			#endif
		}
		else{
			USART_S->DataFlag = 0;
			if( USART_S->Length <= 3 )
			{
				if(( USART_S->DataBuff[0] == 'O' )&&( USART_S->DataBuff[1] == 'K' )){
					#ifdef DEBUG
					USART2_SendString( ">Send CMD Succesful ,Time:" );
					Printf_Num(Count_i);
					USART2_SendString("\r\n"); 
					#endif
					return SUCCESS;
				}
				else if(( USART_S->DataBuff[0] == 'N' )&&( USART_S->DataBuff[1] == 'G' )){
					#ifdef DEBUG
					USART2_SendString( ">Send CMD Ng ,Time :" );
					Printf_Num(Count_i);
					USART2_SendString("\r\n"); 
					#endif
					return FAILED;
				}
				else{

					return FAILED;
				}
			}
			else
			{
				#ifdef DEBUG
				USART2_SendString( ">Send CMD Error \r\n" );
				#endif
				return ERROR;
			}
		}
	}
	return COUNTOUT;
}

s8	Cam_SendCMDSame_S( u16 Addr , u8 Data , u8 Length ,u8 Target , u16 SetTime , u8 RetriesCount ,USART_TypeDef* USARTx , struct CamUartFsm *USART_S )
{
	u16 Count_i = 0 ,Count_r = 0;
	while( Count_r < RetriesCount ){

		USART_S->DataFlag = 0;
		Count_i		     = 0;
		SendCMDSameData_S( Addr , Data , Length , Target ,USARTx );
		
		while( Count_i < SetTime )
		{
			if( USART_S->DataFlag == 1 )
			{
				break;
			}
			DelayMs(1);
			Count_i ++ ;
		}
		
		if( Count_i >= SetTime ){
			Count_r ++;
			Count_i = 0;
			#ifdef DEBUG
				USART2_SendString( ">Send CMD Time Out \r\n" );
			#endif
		}
		else{
			USART_S->DataFlag = 0;
			if( USART_S->Length <= 3 )
			{
				if(( USART_S->DataBuff[0] == 'O' )&&( USART_S->DataBuff[1] == 'K' )){
					#ifdef DEBUG
					USART2_SendString( ">Send CMD Succesful ,Time:" );
					Printf_Num(Count_i);
					USART2_SendString("\r\n"); 
					#endif
					return SUCCESS;
				}
				else if(( USART_S->DataBuff[0] == 'N' )&&( USART_S->DataBuff[1] == 'G' )){
					#ifdef DEBUG
					USART2_SendString( ">Send CMD Ng ,Time :" );
					Printf_Num(Count_i);
					USART2_SendString("\r\n"); 
					#endif
					return FAILED;
				}
				else{

					return FAILED;
				}
			}
			else
			{
				#ifdef DEBUG
				USART2_SendString( ">Send CMD Error \r\n" );
				#endif
				return ERROR;
			}
		}
	}
	return COUNTOUT;
}

u8 ChangeHex8Bit( u8 *Databuff )
{
	u8 Data = 0;

	if(( *(Databuff + 0) >= '0' )&&( *(Databuff + 0) <= '9' ))
	{
		Data |= *(Databuff + 0) - '0';
	}
	else if(( *(Databuff + 0) >= 'A' )&&( *(Databuff + 0) <= 'F' ))
	{
		Data |= *(Databuff + 0) - 'A' + 0x0a;
	}
	else if(( *(Databuff + 0) >= 'a' )&&( *(Databuff + 0) <= 'f' ))
	{
		Data |= *(Databuff + 0) - 'a' + 0x0a;
	}

	Data <<= 4;

	if(( *(Databuff + 1) >= '0' )&&( *(Databuff + 1) <= '9' ))
	{
		Data |= *(Databuff + 1) - '0';
	}
	else if(( *(Databuff + 1) >= 'A' )&&( *(Databuff + 1) <= 'F' ))
	{
		Data |= *(Databuff + 1) - 'A' + 0x0a;
	}
	else if(( *(Databuff + 1) >= 'a' )&&( *(Databuff + 1) <= 'f' ))
	{
		Data |= *(Databuff + 1) - 'a' + 0x0a;
	}
    return Data;
}

s8 ReadData_S( u16 Addr , struct CAMData *qData , u8 Length ,u8 Target , u16 SetTime , u8 RetriesCount ,USART_TypeDef* USARTx , struct CamUartFsm *USART_S )
{
	u16 Count_i = 0 ,Count_r = 0 , Sig_Length ;
	u8 Count_Data = 0;
	while( Count_r < RetriesCount ){

		USART_S->DataFlag = 0;
		Count_i		     = 0;
		SendRead_S( Addr , Length , Target , USARTx );
		
		while( Count_i < SetTime )
		{
			if( USART_S->DataFlag == 1 )
			{
				break;
			}
			DelayMs(1);
			Count_i ++ ;
		}
		
		if( Count_i >= SetTime ){
			Count_r ++;
			Count_i = 0;
			#ifdef DEBUG
				USART1_SendString( ">Send CMD Time Out \r\n" );
			#endif
		}
		else{
			USART_S->DataFlag = 0;
			if( USART_S->Length <= 3 )
			{
				#ifdef DEBUG
				USART1_SendString( ">Send CMD Error \r\n" );
				#endif
				return ERROR;
			}
			else
			{
				qData->Addr = 0;
				qData->Addr =  ChangeHex8Bit( &USART_S->DataBuff[0] );
				qData->Addr <<= 8;
				qData->Addr |= ChangeHex8Bit( &USART_S->DataBuff[2] );

				if( USART_S->DataBuff[4] != ':' )
				{
					return ERROR;
				}

				qData->Length = ChangeHex8Bit( &USART_S->DataBuff[5] );
				Sig_Length = qData->Length;

				if( USART_S->DataBuff[7] != ':' )
				{
					return ERROR;
				}
				while( Sig_Length != 0 )
				{
					qData->Databuff[Count_Data] = ChangeHex8Bit( &USART_S->DataBuff[8 + (Count_Data * 2)] );
					Count_Data ++;
					Sig_Length--;
				}
				return SUCCESS;
			}
		}
	}
	return COUNTOUT;
}


void OSD_DisString( u8 XPos, u8 YPos , u8 *Buff )
{
    u8 StringLength = 0;

    while( *( Buff + StringLength ) != 0 )
    StringLength ++;

    Cam_SendCMDBuff_S( DISP_LINE0 + ( (u16)YPos * 56 ) + XPos , Buff , StringLength , WRITE_RAM , 2000 , 10 ,DEF_CAMPAM);

}
void OSD_DisChar( u8 XPos, u8 YPos , u8 Char )
{
    Cam_SendCMD_S( DISP_LINE0 + ( (u16)YPos * 56 ) + XPos , Char , WRITE_RAM , 2000 , 10 , DEF_CAMPAM);
}

void OSD_Clear( u8 XPos, u8 YPos , u8 Length )
{
	Cam_SendCMDSame_S( DISP_LINE0 + ( (u16)YPos * 56 ) + XPos , ' ' , Length , WRITE_RAM , 2000 , 10 , DEF_CAMPAM);
}

void OSD_ClearAll(void)
{
	u8 YPos = 0;
	for( YPos = 0 ; YPos < 12 ; YPos ++ )
	{
		OSD_Clear( 0 , YPos , 56 );
	}
}

void OSD_DisStringColorMode( u8 XPos, u8 YPos , u8 *Buff ,u8 ColorMode)
{
    u8 StringLength = 0;
    u8 ColorBuff[64];

    while( *( Buff + StringLength ) != 0 )
    {
        ColorBuff[StringLength] = ColorMode;
        StringLength ++;
    }

    Cam_SendCMDBuff_S( CTRL_LINE0 + ( YPos * 56 ) + XPos , ColorBuff , StringLength , WRITE_RAM , 2000 , 10 ,DEF_CAMPAM);
    Cam_SendCMDBuff_S( DISP_LINE0 + ( YPos * 56 ) + XPos , Buff      , StringLength , WRITE_RAM , 2000 , 10 ,DEF_CAMPAM);

}
void OSD_DisCharColorMode( u8 XPos, u8 YPos , u8 Char ,u8 ColorMode )
{
    Cam_SendCMD_S( CTRL_LINE0 + ( YPos * 56 ) + XPos , ColorMode  , WRITE_RAM , 2000 , 10 ,DEF_CAMPAM);
    Cam_SendCMD_S( DISP_LINE0 + ( YPos * 56 ) + XPos , Char       , WRITE_RAM , 2000 , 10 ,DEF_CAMPAM);
}

