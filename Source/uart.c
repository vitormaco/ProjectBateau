#include "stm32f10x.h"
#include "BSP.h"

char * direction;
char * speed;

void init_UART(char * dir, char * spd){
	direction = dir;
	speed = spd;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->BRR |= (int) BOARD_CPU_FREQUENCY/USART1_BAUD_RATE;
	USART1->CR1 |= USART_CR1_UE;
	USART1->CR1 &= ~USART_CR1_M;
	USART1->CR2 &= ~USART_CR2_STOP;
	
	USART1->CR1 |= USART_CR1_TE;
	USART1->CR1 |= USART_CR1_RE;
	USART1->CR1 |= USART_CR1_RXNEIE;
}

void USART1_IRQHandler () {
	char value_read = USART1->DR;
	*direction = value_read > 0x64;
	*speed = *direction ? ~value_read : value_read;
}

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