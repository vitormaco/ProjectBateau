#include "stm32f10x.h"

void ADC_init()
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->CR2 |= ADC_CR2_ADON;
}

void ADC_Start(ADC_TypeDef *ADC)
{
	ADC->CR2 |= ADC_CR2_ADON;
}
