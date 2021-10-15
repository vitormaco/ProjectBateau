#include "stm32f10x.h"
#include "BSP.h"
#include "gpio.h"
#include "timer.h"

int main(void)
{
	/******************  GPIO Setup  *******************/

	GPIO_SetupClocks(GPIOA_CLOCK);

	GPIO_Init(GPIOA, GPIOA_TIMER3_PWM_OUTPUT_PIN, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);
	GPIO_Init(GPIOA, GPIOA_ADC1_INPUT0, INPUT_ANALOG);

	/******************  Timers Setup  *******************/

	Timer_SetupClocks(TIMER3_CLOCK);
	Timer_Init_PWM_Mode(TIM3, 20);

	/******************  ADC Setup  *******************/

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->CR2 |= ADC_CR2_ADON;

	/******************  Start execution  *******************/

	Timer_Start(TIM3);
	ADC1->CR2 |= ADC_CR2_ADON;

	while (1)
	{
		Timer_Set_PWM_DutyCycle(TIM3, (ADC1->DR*100/0xfff)/25*25);
	};
}
