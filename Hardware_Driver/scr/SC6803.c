#include "SC6803.h"


void Delay_6803( uint32_t Count )
{
    while( Count != 0  )
    Count--;
}

void SC6803_SendBuff( uint16_t *DataBuff , uint16_t Length )
{
    uint16_t Count_i , CountSendBit , SendData;

    GPIO_ResetBits( SC6803_SDA );

    for ( Count_i = 0; Count_i < 32; Count_i++)
    {
        GPIO_SetBits( SC6803_CLK );
		Delay_6803(5);
		GPIO_ResetBits( SC6803_CLK );
        Delay_6803(5);
    }

    for ( Count_i = 0; Count_i < Length; Count_i++ )
    {
        SendData = *( DataBuff + Count_i ) | 0x8000 ;
        //SendData = 0x8000;
        for ( CountSendBit = 0; CountSendBit < 16; CountSendBit++)
        {
            if( SendData & 0x8000 )
            GPIO_SetBits( SC6803_SDA );
            else
            GPIO_ResetBits( SC6803_SDA );

            GPIO_SetBits( SC6803_CLK );
            Delay_6803(5);
            GPIO_ResetBits( SC6803_CLK );
            Delay_6803(5);

            SendData <<= 1;
        }
    }

    GPIO_ResetBits( SC6803_SDA );

    for ( Count_i = 0; Count_i < Length; Count_i++)
    {
        GPIO_SetBits( SC6803_CLK );
		Delay_6803(5);
		GPIO_ResetBits( SC6803_CLK );
        Delay_6803(5);
    }
}
