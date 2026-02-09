#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "OLED.h"
#include "BEEP.h"
#include "KEY.h"
#include "Weight.h"




uint8_t TIM2_Flag = 0;//定时器2中断标志

uint8_t keynum; //按键
/*
u32 Pi_weight;
u32 Weight;
u32 weight_err;
*/
float Pi_weight;
float Weight;
float weight_err;
char price[7];
float single_price;
float total_price;
uint8_t  decimal;//小数位

void show_init();
void set_price();//设置单价

void main_Init(void);
void main_loop(void);


int main(void)
{
	main_Init();
	while(1)
	{
		main_loop();
	}
}

void main_Init()
{
	OLED_Init();
	Timer_Init();
	BEEP_Init();
	OLED_Clear();
	HX711_GPIO_Init();
	OLED_ShowChinese(2,2,"正在初始化...");
	Delay_ms(2000);			//对接成功界面停留一定时间
	weight_err = Get_Tare();
	Pi_weight = Get_Tare();	
}

void main_loop()
{
	show_init();
	keynum = KeyScan();
 
	
	Weight = 1.73*Get_Weight(Pi_weight);
	if(Weight < 0.5)Weight = 0;//去除干扰
	total_price = single_price*Weight/1000;
	OLED_ShowNum(1,7,(int)(Pi_weight - weight_err)/1000,1);
	OLED_ShowChar(1,8,'.');
	OLED_ShowNum(1,9,(((Pi_weight - weight_err)/1000)-(int)((Pi_weight - weight_err)/1000))*10000,4);
	OLED_ShowNum(2,7,(int)(Weight)/1000,1);
	OLED_ShowChar(2,8,'.');
	OLED_ShowNum(2,9,(((Weight)/1000)-(int)((Weight)/1000))*10000,4);
	
//	OLED_ShowNum(2,7,Weight,4);
	if(price[0] == '\0')
	{
		OLED_ShowNum(3,7,0,6);
	}else 
		OLED_ShowString(3,7,price);
	OLED_ShowNum(4,7,(int)total_price,5);
	OLED_ShowChar(4,12,'.');
	OLED_ShowNum(4,13,(total_price-(int)total_price)*10,1);
	Delay_ms(100);
}

void show_init()
{
	OLED_ShowChinese(1,1,"皮重：");
	OLED_ShowChinese(1,7,"千克");
	OLED_ShowChinese(2,1,"重量：");
 
}


void set_price()
{
	OLED_Clear();
	OLED_ShowChinese(1,3,"请输入单价");
		uint8_t index = 0;
			uint8_t dot_flag = 0;
			uint8_t end_flag = 0;
		single_price = 0;
	decimal = 0;
			price[0] = '\0';
			while(1)
			{
				if(index == 6)break;
				keynum = KeyScan();
				if(keynum == ok)
				{
					break;
				}else if(keynum >=0 && keynum <= 9 && end_flag == 0)
				{
					price[index] = '0' + keynum;
					if(index < 6)
						price[index+1] = '\0';
					OLED_ShowChar(3,7+index,price[index]);
					if(index > 0 && price[index-1] == '.')
					{
						single_price += keynum*0.1;
						decimal = keynum;
						end_flag = 1;
					}else 
					{
						single_price *= 10;
						single_price += keynum;
					}
					index++;
				} 
				
			}
			OLED_Clear();
}
