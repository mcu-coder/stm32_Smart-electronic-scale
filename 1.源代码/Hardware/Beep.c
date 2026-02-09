#include "stm32f10x.h"                  // Device header
#include "Beep.h"
#include "Delay.h"

void BEEP_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = BEEP_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_GPIO_Port, &GPIO_InitStruct);
	Delay_ms(100);
	GPIO_ResetBits(BEEP_GPIO_Port,BEEP_Pin);//高点平驱动，初始状态不驱动
}

void BEEP_Set(uint8_t staus)
{
	if(staus == 0)
	{
		GPIO_ResetBits(BEEP_GPIO_Port,BEEP_Pin);
	}
	else
	{
		GPIO_SetBits(BEEP_GPIO_Port,BEEP_Pin);
	}
}

void BEEP_Time(unsigned char times)
{
	unsigned char i=0;
	for(i=0;i<times;i++)
	{
		 GPIO_SetBits(BEEP_GPIO_Port,BEEP_Pin);
		 Delay_ms(200);
		 GPIO_ResetBits(BEEP_GPIO_Port,BEEP_Pin);
		 Delay_ms(200);
	}
}
