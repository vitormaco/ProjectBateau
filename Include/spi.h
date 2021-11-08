#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"

void SPI_init(void);
char receive_message(int address);

#endif
