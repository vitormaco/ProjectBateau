#include "stm32f10x.h"
#include "BSP.h"
#include "timer.h"
#include "gpio.h"

void SPI_init()
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	SPI1->CR1 |= SPI_CR1_MSTR;
}

char RW_message(int address)
{
	int txe;
	int rxne;
	
	SPI1->CR1 |= SPI_CR1_SPE;
	
	txe = (SPI1->SR &= SPI_SR_TXE);
	while(!(txe)) {
	}
	SPI1->DR |= address;
	
	rxne = (SPI1->SR &= SPI_SR_RXNE); 
	while(!rxne) {
	}
	return SPI1->DR;
}


