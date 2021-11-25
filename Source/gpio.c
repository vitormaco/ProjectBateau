#include "gpio.h"
#include "stm32f10x.h"

void GPIO_SetupClocks(GPIO_Clock_Type clock)
{
	RCC->APB2ENR |= clock;
}

void GPIO_Init(GPIO_TypeDef *GPIO, char Pin, GPIO_Mode_Type Config)
{
	int isCRH = Pin > 0x7;
	int shift4bits = (Pin % 8) * 4;
	if (isCRH)
	{
		GPIO->CRH &= ~(0xF << shift4bits);
		GPIO->CRH |= Config << shift4bits;
	}
	else
	{
		GPIO->CRL &= ~(0xF << shift4bits);
		GPIO->CRL |= Config << shift4bits;
	}
}

void GPIO_Set(GPIO_TypeDef *GPIO, char GPIO_Pin)
{
	GPIO->BSRR |= 0x1 << GPIO_Pin;
}

void GPIO_Reset(GPIO_TypeDef *GPIO, char GPIO_Pin)
{
	GPIO->BSRR |= 0x1 << (GPIO_Pin + 16);
}

void GPIO_Reset_Timer(GPIO_TypeDef *GPIO, char GPIO_Pin) {}

void GPIO_Toggle(GPIO_TypeDef *GPIO, char GPIO_Pin)
{
	GPIO->ODR ^= 0x1 << GPIO_Pin;
}

int GPIO_Read(GPIO_TypeDef *GPIO, char GPIO_Pin)
{
	// depends on polarity
	if (GPIO->IDR & (0x1 << GPIO_Pin))
	{
		return 0;
	}
	return 1;
}
