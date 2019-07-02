#include "LED.h"

void LED1_ON(void)
{
	GPIO_WriteHigh(LED1);
}
void LED1_OFF(void)
{
	GPIO_WriteLow(LED1);
}

void LED2_ON(void)
{
	GPIO_WriteHigh(LED2);
}
void LED2_OFF(void)
{
	GPIO_WriteLow(LED2);
}
void LED3_ON(void)
{
	GPIO_WriteHigh(LED3);
}
void LED3_OFF(void)
{
	GPIO_WriteLow(LED3);
}

