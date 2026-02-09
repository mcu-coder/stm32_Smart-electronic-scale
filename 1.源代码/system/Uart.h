#ifndef UART_H
#define UART_H
#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdint.h"

#define wifi_ 1 //1表示使用，0表示不使用 wifi使用串口1
#define pnt_  0 //调试用

#define UART_1_RXBUF_LEN 2048
#define UART_2_RXBUF_LEN 2048

extern char  UART_1_RXBUF[UART_1_RXBUF_LEN];
extern char  UART_2_RXBUF[UART_2_RXBUF_LEN];
extern uint16_t UART_1_RXBUF_INDEX ;
extern uint16_t UART_2_RXBUF_INDEX ;

extern uint8_t switch_flag;

void UART1_Init( uint32_t USART_BaudRate);
void UART2_Init( uint32_t USART_BaudRate);
void USART_BUF_Clear(char* UART_BUF,uint16_t* UART_INDEX);
uint8_t USART_BUF_CMP(const char* UART_BUF,char* str,uint16_t timeout);
void UART_SendString(USART_TypeDef* USARTx,char *data);

#endif
