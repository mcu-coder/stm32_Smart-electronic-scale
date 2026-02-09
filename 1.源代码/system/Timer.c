#include "stm32f10x.h"                  // Device header
#include "Timer.h"

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//72M
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 5000-1;//计数0->5000 //周期为2S
	TIM_TimeBaseInitStructure.TIM_Prescaler = 28800-1;//72MHZ->2.5kHZ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	
	TIM_Cmd(TIM2,ENABLE );//启迪定时器

}

void TIM2_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM2 ,TIM_IT_Update)==SET)
   {
		TIM2_Flag = 1;
		TIM_ClearITPendingBit (TIM2 ,TIM_IT_Update);//清除中断标志位
   }
}


