#include "stm32f10x.h"
#include "BSP.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include <stdio.h>

int main(void)
{
	char str[5];

	/******************  GPIO Setup  *******************/

	GPIO_SetupClocks(GPIOA_CLOCK);
	GPIO_SetupClocks(GPIOB_CLOCK);
	GPIO_SetupClocks(GPIOC_CLOCK);

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

	// PA5 - Rotary Encoder IDX
	// GPIO_Init(GPIOC, GPIOC_ROTARY_ENCODER_IDX, INPUT_PULL_UP_DOWN);

	// PB6 - Timer Channel 1 - Rotary Encoder PHA
	GPIO_Init(GPIOB, GPIOB_ROTARY_ENCODER_PHA, INPUT_PULL_UP_DOWN);

	// PB7 - Timer Channel 2 - Rotary Encoder PHB
	GPIO_Init(GPIOB, GPIOB_ROTARY_ENCODER_PHB, INPUT_PULL_UP_DOWN);

	/******************  Timers Setup  *******************/

	Timer_SetupClocks(TIMER3_CLOCK);
	Timer_SetupClocks(TIMER4_CLOCK);
	Timer_Init_PWM_Mode(TIM3, 0);
	Timer_Init_Encoder_Mode(TIM4);

	/******************  ADC Setup  *******************/

	ADC_init();

	/******************  UART Setup  *******************/

	UART_init();

	/******************  Interruptions  *******************/

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 10);

	/******************  Start execution  *******************/

	Timer_Start(TIM3);
	ADC_Start(ADC1);

	while (1)
	{
		// battery voltage
		// sprintf(str, "%d", ADC1->DR * 13 / 12);
		// girouette angle
		sprintf(str, "%d", TIM4->CNT/4);
		write_message(str);
	};
}
