#include "stm32f10x.h"
#include "BSP.h"
#include "gpio.h"
#include "timer.h"

char direction;
char speed;

char str[5];

void write_message(char message[]) {
	int i;
	USART1->CR1 |= USART_CR1_TE;
	for (i=0; message[i]!='\0';i++) {
		USART1->DR = message[i];
		while (!(USART1->SR & USART_SR_TC));
	}
	USART1->DR = '\n';
	while (!(USART1->SR & USART_SR_TC));
}

int main(void)
{
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

  // UART ----------

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->BRR |= (int) 72000000/USART1_BAUD_RATE;
	USART1->CR1 |= USART_CR1_UE;
	USART1->CR1 &= ~USART_CR1_M;
	USART1->CR2 &= ~USART_CR2_STOP;
	
	USART1->CR1 |= USART_CR1_TE;
	USART1->CR1 |= USART_CR1_RE;
	USART1->CR1 |= USART_CR1_RXNEIE;

	// Interruptions

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 10);

	/******************  Start execution  *******************/

	Timer_Start(TIM3);
	ADC1->CR2 |= ADC_CR2_ADON;

	while (1)
	{
		Timer_Set_PWM_DutyCycle(TIM3, speed);
		direction ? GPIO_Set(GPIOA, GPIOA_DIRECTION_PLATEAU) : GPIO_Reset(GPIOA, GPIOA_DIRECTION_PLATEAU);
		sprintf(str, "%d", ADC1->DR*13/12);
		write_message(str);
		//ADC1->DR;
	};
}


void USART1_IRQHandler () {
	char value_read = USART1->DR;
	direction = value_read > 0x64;
	speed = direction ? ~value_read : value_read;
}
