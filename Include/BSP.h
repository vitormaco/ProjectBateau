#ifndef BSP_H
#define BSP_H

// Pins
#define GPIOA_BOARD_LED_PIN 5
#define GPIOC_BOARD_USER_BUTTON_PIN 13
#define GPIOC_PC10_PIN 10
#define GPIOA_TIMER2_PWM_OUTPUT_PIN 0
#define GPIOA_TIMER3_PWM_OUTPUT_PIN 6
#define GPIOA_TIMER2_PWM_OUTPUT_REMAP_PIN 15
#define GPIOA_ADC1_INPUT0 0
#define GPIOA_USART1_TX 9
#define GPIOA_USART1_RX 10
#define GPIOA_DIRECTION_PLATEAU 8

// Signals
#define TIMER_PWM_PERIOD_IN_CLOCKS 720 // 72Mhz / 720 = 100Khz
#define BOARD_CPU_FREQUENCY 72000000

#define USART1_BAUD_RATE 9600

#endif
