#include "stm32f10x.h"
#include "BSP.h"
#include "timer.h"
#include "gpio.h"

uint8_t reg ; 
uint8_t data ; 

void SPI_init()
{
	// Clock enable
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	// On se met en mode master pour la r�c�ption et l'envoi de donn�es
	SPI1->CR1 |= SPI_CR1_MSTR;
	
	// En mode transfert de donn�es CPOL et CPHA
	SPI1->CR1 |= SPI_CR1_CPOL;
	SPI1->CR1 |= SPI_CR1_CPHA;
	
	SPI1->CR1 |= SPI_CR1_SSM;
	SPI1->CR1 |= SPI_CR1_SSI;
	
	SPI1->CR1 |= SPI_CR1_SPE;
	
	// Selection du baud rate inf�rieur � 5MHz
	SPI1->CR1 |= SPI_CR1_BR_2;
	
	GPIO_Set(GPIOA, GPIOA_SPI_NSS); 
	
}

char SPI_ReadWrite(char data)
{
	int a ; // Return value
	
	// Write //
	while ((SPI1->SR&SPI_SR_TXE)== 0) {
		// Tant que que le registre TXE est plein alors on attend avant 
	}
	SPI1->DR = data ; 
	
	// Read //
	while ((SPI1->SR&SPI_SR_RXNE)== 0) {
		// Tant qu'il n'y a rien � lire, on attend
	}
	a=SPI1->DR ; 
	
	return a ; 
}

// Fonctions de gestion du Chip Select 
void MySPI_CS_On() {
	GPIO_Reset(GPIOA, 4) ; 
}

void MySPI_CS_Off() {
	GPIO_Set(GPIOA, 4) ; 
}

void MyADXL_SendData(uint8_t reg, uint8_t data) {
	
	// Mise � 1 du Chip Select 
	MySPI_CS_On() ; 
	
	// Envoi du register  
	SPI_ReadWrite(reg) ;
	
	// Envoi des donn�es 
	SPI_ReadWrite(data) ;
	
	// Mise � 0 du Chip Select 
	MySPI_CS_Off() ; 
}


	
void MyADXL_Config() {
	
	// Construction de la trame � envoyer 
	// 1er bit � 0 car mode Write 
	// 2eme bit � 0 car bit MB � 0 pour lire une seule donn�e 
	// Adresse du POWER_CTL 
	reg = 0x2d ; 
	
	// Data � envoyer 
	data = 0x0 ; 
	
	// Envoi des donn�es 
	MyADXL_SendData(reg, data) ; 
	
	
	
	// Modification du format des donn�es 
	
	// Construction de la trame � envoyer 
	// 1er bit � 0 car mode Write 
	// 2eme bit � 0 car bit MB � 0 pour lire une seule donn�e 
	// Adresse du DATA_FORMAT pour recevoir les donn�es format�es
	reg = 0x31 ; 
	
	// Data � envoyer 
	data = 0x1 ; 
	
	// Envoi des donn�es 
	MyADXL_SendData(reg, data) ; 
}

void Bordage() {
	char p;
	char val;
	// Message � envoyer
	uint8_t address = 0;
	
	// Initialisation du SPI et de l'ADXL
	SPI_init(); 
	MyADXL_Config();
	
	// mettre le bit de read � 1
	address |= 1 << 7;
	// mettre l'option multibit � 1
	address |= 1 << 6;
	// mettre dans le reste l'adresse ou ce trouve l'axe Z
	address |= 0x36;
	
	GPIO_Reset(GPIOA, GPIOA_SPI_NSS);
	while(!(SPI1->SR & SPI_SR_TXE)) {}
		
	SPI1->DR = address;
		
	while(!(SPI1->SR & SPI_SR_RXNE)) {}
	p = SPI1->DR;
	while(!(SPI1->SR & SPI_SR_TXE)) {}
	SPI1->DR = val; // ecrire n'importe quoi pour attendre la r�ponse
	while(!(SPI1->SR & SPI_SR_RXNE)) {}
	val = SPI1->DR;
	while(SPI1->SR & SPI_SR_BSY) {}
	
	GPIO_Set(GPIOA, GPIOA_SPI_NSS); 
}
