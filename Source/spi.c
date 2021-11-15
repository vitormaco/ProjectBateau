#include "stm32f10x.h"
#include "BSP.h"
#include "timer.h"
#include "gpio.h"

void SPI_init()
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 |= SPI_CR1_MSTR;
	
	SPI1->CR1 |= 0x011 << 3;
	
	SPI1->CR1 |= SPI_CR1_CPOL;
	SPI1->CR1 |= SPI_CR1_CPHA;
	
	SPI1->CR1 |= SPI_CR1_SSM;
	SPI1->CR1 |= SPI_CR1_SSI;
	
	SPI1->CR1 |= SPI_CR1_SPE;
	
	SPI1->CR1 |= SPI_CR1_BR_1;
	
	GPIO_Set(GPIOA, GPIOA_SPI_NSS); 
	
}

int SPI_read_write_message(int address, int val)
{
	int poub;
	
	GPIO_Reset(GPIOA, GPIOA_SPI_NSS);
	while(!(SPI1->SR & SPI_SR_TXE)) {}
		
	SPI1->DR = address;
		
	while(!(SPI1->SR & SPI_SR_RXNE)) {}
	poub = SPI1->DR;
	while(!(SPI1->SR & SPI_SR_TXE)) {}
	SPI1->DR = val; // ecrire n'importe quoi pour attendre la réponse
	while(!(SPI1->SR & SPI_SR_RXNE)) {}
	val = SPI1->DR;
	while(SPI1->SR & SPI_SR_BSY) {}
	
	GPIO_Set(GPIOA, GPIOA_SPI_NSS); 
	return val;
}

