#ifndef TIMER_H
#define TIMER_H

#include "stm32f10x.h"

typedef enum TIMER_Clock
{
	TIMER1_CLOCK = RCC_APB2ENR_TIM1EN,
	TIMER2_CLOCK = 0x01 << 0,
	TIMER3_CLOCK = 0x01 << 1,
	TIMER4_CLOCK = 0x01 << 2,
} TIMER_Clock_Type;

void Timer_SetupClocks(TIMER_Clock_Type clock);
void Timer_Init(TIM_TypeDef *Timer, int TriggerTimeInMs);
void Timer_Init_PWM_Mode(TIM_TypeDef *Timer, int dutyCycleInPercent);
void Timer_Init_Encoder_Mode(TIM_TypeDef *Timer);
void Timer_ActiveIT ( TIM_TypeDef  * Timer , char Prio, void (*IT_function) (void));
void Timer_Set_PWM_DutyCycle(TIM_TypeDef *Timer, int dutyCycleInPercent);
void Timer_Set_PWM_Servo(TIM_TypeDef *Timer, int percentage);
void Timer_Start(TIM_TypeDef *Timer);
void Timer_Stop(TIM_TypeDef *Timer);

#endif
