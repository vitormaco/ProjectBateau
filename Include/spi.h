#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"

void SPI_init();
char SPI_ReadWrite(char val);
// Fonctions de gestion du Chip Select 
void SPI_CS_On(void) ; 
void SPI_CS_Off(void) ; 
// Fonction de configuration de l'ADXL345
void MyADXL_Config(void);
void MyADXL_SendData(uint8_t reg, uint8_t data);
void Bordage(void);

#endif
