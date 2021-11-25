#ifndef GPIO_H
#define GPIO_H

#include "stm32f10x.h"

typedef enum GPIO_Mode
{
	INPUT_ANALOG = 0x0,
	INPUT_FLOATING = 0x4,
	INPUT_PULL_UP_DOWN = 0x8,
	OUTPUT_2MHZ_PUSH_PULL = 0x2,
	OUTPUT_2MHZ_OPEN_DRAIN = 0x6,
	OUTPUT_2MHZ_ALTERNATE_PUSH_PULL = 0xA,
	OUTPUT_2MHZ_ALTERNATE_OPEN_DRAIN = 0xE
} GPIO_Mode_Type;

typedef enum GPIO_Clock
{
	GPIOA_CLOCK = 0x01 << 2,
	GPIOB_CLOCK = 0x01 << 3,
	GPIOC_CLOCK = 0x01 << 4,
	GPIOD_CLOCK = 0x01 << 5
} GPIO_Clock_Type;

void GPIO_SetupClocks(GPIO_Clock_Type clock);
void GPIO_Init(GPIO_TypeDef *GPIO, char Pin, GPIO_Mode_Type Config);
int GPIO_Read(GPIO_TypeDef *GPIO, char GPIO_Pin);
void GPIO_Set(GPIO_TypeDef *GPIO, char GPIO_Pin);
void GPIO_Reset(GPIO_TypeDef *GPIO, char GPIO_Pin);
void GPIO_Reset_Timer(GPIO_TypeDef *GPIO, char GPIO_Pin);
void GPIO_Toggle(GPIO_TypeDef *GPIO, char GPIO_Pin);

#endif
