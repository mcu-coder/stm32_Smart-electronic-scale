#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"




void KEY1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //开启AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//使能JTAGDisable，即禁用JTAG接口
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOA、B
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入  
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;  
   GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入  
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_3|GPIO_Pin_15;
   GPIO_Init(GPIOB,&GPIO_InitStructure);  	
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;        //推挽输出
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
   GPIO_Init(GPIOB,&GPIO_InitStructure);       
   GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //行输出低电平，扫描列
}
void KEY2_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;        //推挽输出
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;  
   GPIO_Init(GPIOA,&GPIO_InitStructure);
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_3|GPIO_Pin_15;
   GPIO_Init(GPIOB,&GPIO_InitStructure);
   GPIO_SetBits(GPIOA,GPIO_Pin_15);  //扫描行
   GPIO_SetBits(GPIOB,GPIO_Pin_14|GPIO_Pin_3|GPIO_Pin_15);
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;   //下拉输入
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
   GPIO_Init(GPIOB,&GPIO_InitStructure);	 
}

uint8_t PBin(uint8_t pin)
{
	uint16_t gpio_pin = (1<<pin);
	return GPIO_ReadInputDataBit(GPIOB,gpio_pin);
}
uint8_t PAin(uint8_t pin)
{
	uint16_t gpio_pin = (1<<pin);
	return GPIO_ReadInputDataBit(GPIOA,gpio_pin);
}

uint8_t KeyScan(void)
{ 
	 		
   return 1;
}

