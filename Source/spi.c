#include "stm32f10x.h"
#include "BSP.h"
#include "timer.h"
#include "gpio.h"

void SPI_init()
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 |= SPI_CR1_MSTR;
}

char SPI_read_write_message(int address)
{
	SPI1->CR1 |= SPI_CR1_SPE;
	
	while(!(SPI1->SR & SPI_SR_TXE)) {}
	SPI1->DR = address;
	
	while(!(SPI1->SR & SPI_SR_RXNE)) {}

	return SPI1->DR;
}
