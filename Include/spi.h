#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"

void SPI_init(void);
int SPI_read_write_message(int address);

#endif
