#include "Control_IO.h"


void Control_IO1_ON(void);
void Control_IO2_ON(void);
void Control_IO3_ON(void);
void Control_IO4_ON(void);

void Control_IO1_OFF(void);
void Control_IO2_OFF(void);
void Control_IO3_OFF(void);
void Control_IO4_OFF(void);

void Control_IO1_ON(void)
{
	GPIO_WriteHigh(Control_IO1);
}

void Control_IO2_ON(void)
{
	GPIO_WriteHigh(Control_IO2);
}

void Control_IO3_ON(void)
{
	GPIO_WriteHigh(Control_IO3);
}

void Control_IO4_ON(void)
{
	GPIO_WriteHigh(Control_IO4);
}



void Control_IO1_OFF(void)
{
	GPIO_WriteLow(Control_IO1);
}

void Control_IO2_OFF(void)
{
	GPIO_WriteLow(Control_IO2);	
}

void Control_IO3_OFF(void)
{
	GPIO_WriteLow(Control_IO3);	
}

void Control_IO4_OFF(void)
{
	GPIO_WriteLow(Control_IO4);	
}
