#include "stm32f10x.h"
#include "BSP.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "spi.h"
#include <stdio.h>
#include <math.h>

#define DATAX0 0x32

char str[5];
int degree, battery_voltage, sail_pwm;
uint8_t RxData[6] = {0,0,0,0,0,0};
int x,y,z;
int result;
double z_acc;


void Callback(void) {
	sprintf(str, "gir %d, voil: %d, roulis: %d, bat: %d \n", degree, sail_pwm, result, battery_voltage);
	write_message(str);
}

int main(void)
{
	/******************  GPIO Setup  *******************/

	GPIO_SetupClocks(GPIOA_CLOCK);
	GPIO_SetupClocks(GPIOB_CLOCK);
	GPIO_SetupClocks(GPIOC_CLOCK);

	// PC6 - Plateau
	GPIO_Init(GPIOC, GPIOC_TIMER3_PWM_OUTPUT_PIN, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	// PC4 - Direction Plateau
	GPIO_Init(GPIOC, GPIOC_DIRECTION_PLATEAU, OUTPUT_2MHZ_PUSH_PULL);
	
	// PA1 - PWM Voilier
	GPIO_Init(GPIOA, GPIOA_TIMER2_PWM_OUTPUT_PIN2, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	// PA9 - Xbee USART TX
	GPIO_Init(GPIOA, GPIOA_USART1_TX, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	// PA10 - Xbee USART RX
	GPIO_Init(GPIOA, GPIOA_USART1_RX, INPUT_PULL_UP_DOWN);

	// PA0 - Battery ADC
	GPIO_Init(GPIOA, GPIOA_ADC1_INPUT0, INPUT_ANALOG);

	// PA5 - Rotary Encoder IDX
	// GPIO_Init(GPIOC, GPIOC_ROTARY_ENCODER_IDX, INPUT_PULL_UP_DOWN);

	// PB6 - Timer Channel 1 - Rotary Encoder PHA
	GPIO_Init(GPIOB, GPIOB_ROTARY_ENCODER_PHA, INPUT_PULL_UP_DOWN);

	// PB7 - Timer Channel 2 - Rotary Encoder PHB
	GPIO_Init(GPIOB, GPIOB_ROTARY_ENCODER_PHB, INPUT_PULL_UP_DOWN);

	// PA4 - SPI NSS
	GPIO_Init(GPIOA, GPIOA_SPI_NSS, OUTPUT_2MHZ_PUSH_PULL);

	// PA5 - SPI SCK
	GPIO_Init(GPIOA, GPIOA_SPI_SCK, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	// PA6 - SPI MISO
	GPIO_Init(GPIOA, GPIOA_SPI_MISO, INPUT_FLOATING);

	// PA7 - SPI MOSI
	GPIO_Init(GPIOA, GPIOA_SPI_MOSI, OUTPUT_2MHZ_ALTERNATE_PUSH_PULL);

	/******************  Timers Setup  *******************/

	Timer_SetupClocks(TIMER1_CLOCK);
	Timer_SetupClocks(TIMER2_CLOCK);
	Timer_SetupClocks(TIMER3_CLOCK);
	Timer_SetupClocks(TIMER4_CLOCK);
	Timer_Init_PWM_Mode(TIM3, 0);
	Timer_Init_PWM_Mode(TIM2, 0);
	Timer_Init_Encoder_Mode(TIM4);

	/******************  ADC Setup  *******************/

	ADC_init();

	/******************  UART Setup  *******************/

	UART_init();
	
	/******************  AFIO - GPIO Remap Setup  *******************/

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_FULLREMAP;


	/******************  SPI Setup  *******************/

	SPI_Config();
	SPI_Enable();
	adxl345_init();
	
	/******************  Interruptions  *******************/

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 10);

	/******************  Start execution  *******************/

	Timer_Start(TIM1);
	Timer_Start(TIM2);
	Timer_Start(TIM3);
	ADC_Start(ADC1);
	Timer_ActiveIT(TIM1 , 4, &Callback );

	while (1)
	{
		// adc reads 12/13 of the battery value (voltage divider)
		battery_voltage = ADC1->DR * 13 / 12;
		// angle is measure with 1/4 of a degree precision
		degree = TIM4->CNT/4;
		// normalise angles to 0 < degree < 180
		degree = degree < 180 ? degree : 360 - degree;
		// control boat sails
		sail_pwm = 100 - (degree > 45 ? (100 * (degree - 45) / 135) : 0);
		Timer_Set_PWM_Servo(TIM2, sail_pwm);
		// SPI
		adxl345_read(DATAX0, RxData);
		x = ((RxData[1]<<8)|RxData[0]);
		y = ((RxData[3]<<8)|RxData[2]);
		z = ((RxData[5]<<8)|RxData[4]);
		z_acc = (double) 0.0078*z;
		result = acos(z_acc)*180/(atan(1)*4);
	};
}
