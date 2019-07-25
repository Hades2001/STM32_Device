#ifndef __MALLOC_H
#define __MALLOC_H

#include "System.h"

#define MEM_BLOCK_SIZE      32
#define MEM_MAX_SIZE        5*1024
#define MEM_TABLE_SIZE      MEM_MAX_SIZE/MEM_BLOCK_SIZE

void m_Memcopy( void *des , void *src , u32 Size );
int m_InitMalloc( void );
void m_free( void *ptr );
u8 m_GetMemPerused(void);
void *m_Malloc( void *ptr , u32 Size );

#define myfree(p)           m_free(p) 
#define mymalloc(s)         m_Malloc(NULL,s) 
//#define MALLOC(s)           (type *) m_Malloc ( sizeof())


#endif
