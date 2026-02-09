#ifndef __WEIGHT_H
#define __WEIGHT_H
#include "stm32f10x.h"                  // Device header


#define HX711_DATA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))
#define  HX711_SCK(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)(x))




// USART GPIO 引脚宏定义
#define  HX711_GPIO_CLK            RCC_APB2Periph_GPIOB
#define  HX711_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd
	
#define  HX711_DATA_GPIO_PORT      GPIOB
#define  HX711_DATA_GPIO_PIN       GPIO_Pin_10	
#define  HX711_SCK_GPIO_PORT       GPIOB   
#define  HX711_SCK_GPIO_PIN        GPIO_Pin_11



void HX711_GPIO_Init(void);
uint32_t Read_HX711(void);
float Get_Tare(void);
float Get_Weight(float pi_weight);

#endif