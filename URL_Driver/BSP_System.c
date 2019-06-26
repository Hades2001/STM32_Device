#include "BSP_System.h"

typedef void (* FunPtr_TypA)(void);
//typedef void (* FunPtr_TypB)(void);
//typedef void (* FunPtr_TypB)(void);
//typedef void (* FunPtr_TypB)(void);

void Hardware_Init(void);
void Software_Init(void);
void CPUsware_Init(void);
void BSP_ALL_Inits(void);


void CPUsware_Init(void)
{
/********************/
	;
}


void Hardware_Init(void)
{
/********************/
	;
	HardWare_ALL_Init();
}

void Software_Init(void)
{
/********************/
	;
	SoftWare_ALL_Init();
}

void BSP_ALL_Inits(void)
{
/*******************/
	CPUsware_Init();
	Software_Init();
	Hardware_Init();
}
