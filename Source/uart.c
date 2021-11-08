#include "stm32f10x.h"
#include "BSP.h"
#include "timer.h"
#include "gpio.h"

void UART_init()
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->BRR |= (int)BOARD_CPU_FREQUENCY / USART1_BAUD_RATE;
	USART1->CR1 |= USART_CR1_UE;
	USART1->CR1 &= ~USART_CR1_M;
	USART1->CR2 &= ~USART_CR2_STOP;

	USART1->CR1 |= USART_CR1_TE;
	USART1->CR1 |= USART_CR1_RE;
	USART1->CR1 |= USART_CR1_RXNEIE;
}

void USART1_IRQHandler()
{
	char value_read = USART1->DR;
	// 0x01~0x63 -> right
	// 0x64~0xFF -> left
	char direction = value_read > 0x64;
	char speed = direction ? ~value_read : value_read;
	Timer_Set_PWM_DutyCycle(TIM3, speed);
	direction ? GPIO_Set(GPIOC, GPIOC_DIRECTION_PLATEAU) : GPIO_Reset(GPIOC, GPIOC_DIRECTION_PLATEAU);
}

void write_message(char message[])
{
	int i;
	USART1->CR1 |= USART_CR1_TE;
	for (i = 0; message[i] != '\0'; i++)
	{
		USART1->DR = message[i];
		while (!(USART1->SR & USART_SR_TC))
		{
		};
	}
	USART1->DR = '\n';
	while (!(USART1->SR & USART_SR_TC))
	{
	};
}
