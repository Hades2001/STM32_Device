#include "Malloc.h"

__align(32) u8 MemBase[MEM_MAX_SIZE];
u16            MemTable[MEM_TABLE_SIZE];

void m_Memcopy( void *des , void *src , u32 Size )
{
    u8 *qdes = des;
    u8 *qsrc = src;

    while( Size-- )
    *qdes++ = *qsrc++; 
}

void m_MemorySet( void *q , u8 Data , u32 Count )
{
    u8 *qs = q;
    while( Count-- )
    *qs++ = Data;
}

void m_InitMalloc( void )
{
    m_MemorySet( MemTable , 0 , MEM_TABLE_SIZE );
}

u8 m_GetMemPerused()
{
    u32 Used = 0 , Count_i ;

    for( Count_i = 0 ; Count_i < MEM_TABLE_SIZE ; Count_i ++ )
    {
        if( MemTable[Count_i] != 0 )Used++;
    }

    return (Used*100)/MEM_TABLE_SIZE;
}

u32 m_Mem_Malloc( u32 Size )
{
    signed long Offset = 0;
    u32 Count_Null = 0; 
    u32 Count_i;
    u32 BlockNum = 0;

    if( Size == 0 )return 0xffffffff;

    BlockNum = Size/MEM_BLOCK_SIZE;
    if( Size%MEM_BLOCK_SIZE )BlockNum++;

    for( Offset = MEM_TABLE_SIZE - 1  ; Offset >= 0 ; Offset -- )
    {
        if( MemTable[Offset] == 0 )
            Count_Null++;
        else
            Count_Null = 0;

        if( Count_Null == BlockNum )
        {
            for( Count_i = 0 ; Count_i < BlockNum ; Count_i ++ )
            {
                MemTable[ Offset + Count_i ] = BlockNum ;
            }
            return ( Offset * MEM_BLOCK_SIZE );
        }
    }
    return 0xffffffff;
}

u32 m_Mem_Free( u32 Offset )
{
    u32 Count_i;
    u32 Index;
    u32 BlockNum;

    if( Offset < MEM_MAX_SIZE )
    {
        Index = Offset / MEM_BLOCK_SIZE;
        BlockNum = MemTable[ Index ];
        for( Count_i = 0 ; Count_i < BlockNum ; Count_i ++ )
        {
            MemTable[ Index + Count_i ] = 0;
        }
        return 0 ;
    }
    else
        return 2;
}

void m_free( void *ptr )
{
    u32 Offset;
    if( ptr == NULL ) return;
    Offset = (u32)ptr - (u32)MemBase;
    m_Mem_Free( Offset );
}

void *m_Malloc( void *ptr , u32 Size )
{
    u32 Offset;

    Offset = m_Mem_Malloc( Size );
    if( Offset == 0xffffffff )
        return NULL;
    else
        return (void*)((u32)MemBase + Offset);
}
