#include "LED.h"

void LED1_ON(void);
void LED1_OFF(void);

void LED1_ON(void)
{
	GPIO_WriteLow(LED1);
}
void LED1_OFF(void)
{
	GPIO_WriteHigh(LED1);
}

void LED2_ON(void)
{
	GPIO_WriteLow(LED2);
}
void LED2_OFF(void)
{
	GPIO_WriteHigh(LED2);
}
