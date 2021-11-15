#include "timer.h"
#include "stm32f10x.h"
#include "BSP.h"
#include "gpio.h"

void Timer_SetupClocks(TIMER_Clock_Type clock)
{
	RCC->APB1ENR |= clock;
}

void Timer_Init_PWM_Mode(TIM_TypeDef *Timer, int dutyCycleInPercent)
{
	Timer->ARR = TIMER_PWM_PERIOD_IN_CLOCKS - 1;
	Timer->CCR1 = (dutyCycleInPercent * TIMER_PWM_PERIOD_IN_CLOCKS / 100) - 1;
	Timer->CCR2 = (dutyCycleInPercent * TIMER_PWM_PERIOD_IN_CLOCKS / 100) - 1;
	Timer->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE; // PWM Mode 1
	Timer->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE; // PWM Mode 1
	Timer->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
	Timer->CR1 |= TIM_CR1_ARPE;
	Timer->EGR |= TIM_EGR_UG;
	Timer->DIER |= TIM_DIER_UIE;
}

void Timer_Init_Encoder_Mode(TIM_TypeDef *Timer){
	Timer->ARR = 360*4;
	Timer->SMCR |= TIM_SMCR_SMS & 0x3;
	Timer->CCMR1 |= TIM_CCMR1_CC1S & 0x1;
	Timer->CCMR1 |= TIM_CCMR1_CC2S & 0x1;
	Timer->CR1 |= TIM_CR1_CEN;
}

void Timer_Set_PWM_DutyCycle(TIM_TypeDef *Timer, int dutyCycleInPercent)
{
	int dutyCycleInClocks = (dutyCycleInPercent * TIMER_PWM_PERIOD_IN_CLOCKS / 100) - 1;
	Timer->CCR1 = dutyCycleInClocks > 0 ? dutyCycleInClocks : 0;
	Timer->CCR2 = dutyCycleInClocks > 0 ? dutyCycleInClocks : 0;
}

void Timer_Start(TIM_TypeDef *Timer)
{
	Timer->CR1 |= TIM_CR1_CEN;
}

void Timer_Stop(TIM_TypeDef *Timer)
{
	Timer->CR1 &= ~TIM_CR1_CEN;
}
