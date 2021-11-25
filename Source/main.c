#include "stm32f10x.h"
#include "BSP.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "spi.h"
#include <stdio.h>

int main(void)
{
	char str[5];
	int degree;

	/******************  GPIO Setup  *******************/

	GPIO_SetupClocks(GPIOA_CLOCK);
	GPIO_SetupClocks(GPIOB_CLOCK);
	GPIO_SetupClocks(GPIOC_CLOCK);

	// PB4 - Plateau
	GPIO_Init(GPIOB, GPIOB_TIMER3_PWM_OUTPUT_PIN, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	// PC4 - Direction Plateau
	GPIO_Init(GPIOC, GPIOC_DIRECTION_PLATEAU, OUTPUT_2MHZ_PUSH_PULL);
	
	// PA1 - PWM Voilier
	GPIO_Init(GPIOA, GPIOA_TIMER2_PWM_OUTPUT_PIN2, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

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

	// PA4 - SPI NSS
	GPIO_Init(GPIOA, GPIOA_SPI_NSS, OUTPUT_2MHZ_PUSH_PULL);

	// PA5 - SPI SCK
	GPIO_Init(GPIOA, GPIOA_SPI_SCK, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	// PA6 - SPI MISO
	GPIO_Init(GPIOA, GPIOA_SPI_MISO, INPUT_FLOATING);

	// PA7 - SPI MOSI
	GPIO_Init(GPIOA, GPIOA_SPI_MOSI, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	/******************  Timers Setup  *******************/

	Timer_SetupClocks(TIMER2_CLOCK);
	Timer_SetupClocks(TIMER3_CLOCK);
	Timer_SetupClocks(TIMER4_CLOCK);
	Timer_Init_PWM_Mode(TIM3, 0);
	Timer_Init_PWM_Mode(TIM2, 0);
	Timer_Init_Encoder_Mode(TIM4);

	/******************  ADC Setup  *******************/

	ADC_init();

	/******************  UART Setup  *******************/

	UART_init();
	
	/******************  AFIO - GPIO Remap Setup  *******************/

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_PARTIALREMAP;


	/******************  SPI Setup  *******************/

	SPI_init();
	
	/******************  Interruptions  *******************/

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 10);

	/******************  Start execution  *******************/

	Timer_Start(TIM3);
	Timer_Start(TIM2);
	ADC_Start(ADC1);

	while (1)
	{
		// battery voltage
		// sprintf(str, "%d", ADC1->DR * 13 / 12);
		// girouette 
		degree = TIM4->CNT/4;
		degree = degree < 180 ? degree : 360 - degree;
		if ( degree < 45 ) {
			Timer_Set_PWM_Servo(TIM2, 100);
		} else {
			Timer_Set_PWM_Servo(TIM2, 100 - 100 * (degree - 45) / 135);
		}
		sprintf(str, "%d", TIM4->CNT/4);
		write_message(str);
		SPI_read_write_message(0x36, 0x00);
	};
}
