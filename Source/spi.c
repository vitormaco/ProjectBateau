#include "stm32f10x.h"
#include "BSP.h"
#include "timer.h"
#include "gpio.h"

#define DATA_FORMAT 0x31
#define POWER_CTL 0x2d

void SPI_Enable(void)
{
	SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI_Disable(void)
{
	SPI1->CR1 &= ~SPI_CR1_SPE;
}

void CS_Enable(void)
{
	GPIOA->BSRR |= 1<<20;
}

void CS_Disable(void)
{
	GPIOA->BSRR |= 1<<4;	
}

void SPI_Config(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 |= SPI_CR1_CPOL;
	SPI1->CR1 |= SPI_CR1_CPHA;
	SPI1->CR1 |= SPI_CR1_MSTR;
	SPI1->CR1 |= SPI_CR1_BR_0;
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	SPI1->CR1 |= SPI_CR1_SSM;
	SPI1->CR1 |= SPI_CR1_SSI;
	SPI1->CR1 &= ~SPI_CR1_RXONLY;
	SPI1->CR1 &= ~SPI_CR1_DFF;
	SPI1->CR2 = 0;
}

void SPI_Transmission(uint8_t *data, int size)
{
	uint8_t clear;
	int i=0;
	while (i<size)
	{
		while(!(SPI1->SR&SPI_SR_TXE));
		SPI1->DR = data[i];
		i++;
	}
	while(!(SPI1->SR & SPI_SR_TXE));
	while(SPI1->SR & SPI_SR_BSY);
	clear = SPI1->DR;
	clear = SPI1->SR;
}

void SPI_Receive(uint8_t *data, int size)
{
	while (size)
	{
		while(SPI1->SR & SPI_SR_BSY);
		SPI1->DR = 0;
		while(!(SPI1->SR & SPI_SR_RXNE));
		*data++ = SPI1->DR;
		size--;
	}
}

void adxl345_write(uint8_t address, uint8_t value)
{
	uint8_t data[2];
	data[0] = address|0x40;
	data[1] = value;
	CS_Enable();
	SPI_Transmission(data, 2);
	CS_Disable();
}

void adxl345_read(uint8_t address, uint8_t *RxData)
{
	address |= 0x80;
	address |= 0x40;
	CS_Enable();
	SPI_Transmission(&address, 1);
	SPI_Receive(RxData, 6);
	CS_Disable();
}

void adxl345_init(void)
{
	adxl345_write(DATA_FORMAT, 0x01);
	adxl345_write(POWER_CTL, 0x00);
	adxl345_write(POWER_CTL, 0x08);	
}

void Delay_us(uint16_t us)
{
	int i=0;
	while(i<us)
	{
		if(TIM3->SR & TIM_SR_UIF) 
		{
			TIM3->SR = TIM3->SR & ~TIM_SR_UIF;
			i++;
		}
	}
}

void Delay_ms(uint16_t ms)
{
	int i;
	for (i=0; i<ms; i++)
	{
		Delay_us(1000);
	}
}