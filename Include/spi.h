#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"

void SPI_Enable(void);
void SPI_Disable(void);
void CS_Enable(void);
void CS_Disable(void);
void SPI_Config(void);
void SPI_Transmission(uint8_t *data, int size);
void SPI_Receive(uint8_t *data, int size);
void adxl345_write(uint8_t address, uint8_t value);
void adxl345_read(uint8_t address, uint8_t *RxData);
void adxl345_init(void);
void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);

#endif
