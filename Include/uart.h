#include "stm32f10x.h"

void USART1_IRQHandler(void);
void write_message(char message[]);
void init_UART(char *dir, char *spd);
