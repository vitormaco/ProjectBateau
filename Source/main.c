#include "stm32f10x.h"
#include "BSP.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"

int main(void)
{
	char direction;
	char speed;
	char str[5];

	/******************  GPIO Setup  *******************/

	GPIO_SetupClocks(GPIOA_CLOCK);

	// PA6 - Plateau
	GPIO_Init(GPIOA, GPIOA_TIMER3_PWM_OUTPUT_PIN, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	// PA8 - Direction Plateau
	GPIO_Init(GPIOA, GPIOA_DIRECTION_PLATEAU, OUTPUT_2MHZ_PUSH_PULL);

	// PA9 - Xbee USART TX
	GPIO_Init(GPIOA, GPIOA_USART1_TX, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	// PA10 - Xbee USART RX
	GPIO_Init(GPIOA, GPIOA_USART1_RX, INPUT_PULL_UP_DOWN);

	// PA0 - Battery ADC
	GPIO_Init(GPIOA, GPIOA_ADC1_INPUT0, INPUT_ANALOG);

	/******************  Timers Setup  *******************/

	Timer_SetupClocks(TIMER3_CLOCK);
	Timer_Init_PWM_Mode(TIM3, 20);

	/******************  ADC Setup  *******************/

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->CR2 |= ADC_CR2_ADON;

	/******************  UART Setup  *******************/
	init_UART(&direction, &speed);

	/******************  Interruptions  *******************/

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 10);

	/******************  Start execution  *******************/

	Timer_Start(TIM3);
	ADC1->CR2 |= ADC_CR2_ADON;

	while (1)
	{
		Timer_Set_PWM_DutyCycle(TIM3, speed);
		direction ? GPIO_Set(GPIOA, GPIOA_DIRECTION_PLATEAU) : GPIO_Reset(GPIOA, GPIOA_DIRECTION_PLATEAU);
		sprintf(str, "%d", ADC1->DR * 13 / 12);
		write_message(str);
	};
}
