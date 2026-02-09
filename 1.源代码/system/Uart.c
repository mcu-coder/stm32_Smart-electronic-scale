#include "stm32f10x.h"                  // Device header
#include "string.h"
#include "Delay.h"
#include "Uart.h"
#include "OLED.h"
#include "ESP8266_Wifi.h"
#include "AS608.h"
#include "Uart.h"

/*
** 串口1初始化函数
*/
void UART1_Init( uint32_t USART_BaudRate)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1,ENABLE );//串口时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE );//GPIOA时钟
	
	//串口1
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//发送为推挽复用
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA ,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//接收为浮空输入
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10 ;
	GPIO_Init (GPIOA ,&GPIO_InitStructure);
		
	USART_InitTypeDef USART_InitStructure;//串口初始化
	USART_InitStructure.USART_BaudRate =USART_BaudRate ;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;
	USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx ;//可收可发
	USART_InitStructure.USART_Parity =USART_Parity_No ;
	USART_InitStructure.USART_StopBits =USART_StopBits_1 ;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig (USART1,USART_IT_RXNE,ENABLE );//使能串口接收中断

	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_3);//配置中断
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init (&NVIC_InitStructure);	
	USART_Cmd (USART1,ENABLE );//使能串口

}


/*
** 串口2初始化函数
*/
void UART2_Init( uint32_t USART_BaudRate)
{
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2,ENABLE );//串口时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE );//GPIOA时钟
	
	//串口2
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//发送为推挽复用
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA ,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//接收为浮空输入
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3 ;
	GPIO_Init (GPIOA ,&GPIO_InitStructure);
		
	USART_InitTypeDef USART_InitStructure;//串口初始化
	USART_InitStructure.USART_BaudRate =USART_BaudRate ;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;
	USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx ;//可收可发
	USART_InitStructure.USART_Parity =USART_Parity_No ;
	USART_InitStructure.USART_StopBits =USART_StopBits_1 ;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_ITConfig (USART2,USART_IT_RXNE,ENABLE );//使能串口接收中断

	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_3);//配置中断
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init (&NVIC_InitStructure);	
	USART_Cmd (USART2,ENABLE );//使能串口
}

/*
**串口缓冲区复位
*/
void USART_BUF_Clear(char* UART_BUF,uint16_t* UART_INDEX)
{
	memset(UART_BUF,'\0',*UART_INDEX);
	*UART_INDEX = 0;
}

/*
**串口数据比对，一致返回0，不一致返回1
*/
uint8_t USART_BUF_CMP(const char* UART_BUF,char* str,uint16_t timeout)
{
	uint8_t ret = 0;
	while(timeout > 0)
	{
		if(strstr(UART_BUF,str))
		{
			ret = 0;
			break;
		}
		else
			ret = 1;
		Delay_ms(10);
		timeout--;
	}
	return ret;
}

/*
** 串口发送字符串函数
*/
void UART_SendString(USART_TypeDef* USARTx,char *data)
{
	while(*data != '\0'){
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET );//等待发送寄存器空
		USART_SendData(USARTx, *data);
		data++;
	}
	//末尾增加换行符\r\n
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET );//等待发送寄存器空
	USART_SendData(USARTx, '\r');
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET );//等待发送寄存器空
	USART_SendData(USARTx, '\n');
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET );//等待发送完成
}



/*
**串口1接收中断
*/
void USART1_IRQHandler(void)
{
	char uart_data;
#if wifi_ 
	if(USART_GetITStatus(USART1,USART_IT_RXNE )== SET )
	{	
		if(UART_1_RXBUF_INDEX == UART_1_RXBUF_LEN)
		{
			UART_1_RXBUF_INDEX = 0;
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);  //清除RX标志位	
		uart_data = USART_ReceiveData(USART1); //接收数据
		if(UART_1_RXBUF_INDEX < UART_1_RXBUF_LEN)
		{
			if(UART_1_RXBUF[UART_1_RXBUF_INDEX-1]== ':' && UART_1_RXBUF[UART_1_RXBUF_INDEX-2] == '"'
				&& UART_1_RXBUF[UART_1_RXBUF_INDEX-3] == 'h' && UART_1_RXBUF[UART_1_RXBUF_INDEX-4] == 'c'
				&& UART_1_RXBUF[UART_1_RXBUF_INDEX-5] == 't' && UART_1_RXBUF[UART_1_RXBUF_INDEX-6] == 'i'
				&& UART_1_RXBUF[UART_1_RXBUF_INDEX-7] == 'w' && UART_1_RXBUF[UART_1_RXBUF_INDEX-8] == 's')
			{
				switch_flag = 1;
				if(uart_data == '1')
				{
					MQTT_Rec_Data.led_staus = 1;
				}
				else
				{
					MQTT_Rec_Data.led_staus = 0;
				}
				
			}
			UART_1_RXBUF[UART_1_RXBUF_INDEX++] = uart_data ;
			
		}			
		#if pnt_
		USART_SendData(USART2, uart_data);//调试用
		#endif
	}
#endif
	
}


/*
**串口2接收中断
*/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE )== SET )
	{	
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);  //清除RX标志位
		if(AS608_Rx_Flag == 1)
		{
			FPM10A_RECEICE_BUFFER[FPM10A_RECEICE_BUFFER_INdex++] = USART_ReceiveData(USART2);
		}
	}
	
}



int fputc(int ch, FILE *f) {
    // 等待发送缓冲区空
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    // 发送一个字符
    USART_SendData(USART1, (uint8_t) ch);
    // 等待发送完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    return ch;
}
