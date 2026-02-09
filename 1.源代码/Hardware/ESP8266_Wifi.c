#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "OLED.h"
#include "Delay.h"
#include "Uart.h"
//#include "core_json.h"
#include "ESP8266_Wifi.h" 


extern uint16_t UART_1_RXBUF_INDEX ;
extern uint16_t UART_2_RXBUF_INDEX ;

/*
** MQTT配置
*/
uint8_t MQTT_Config(void)
{
	uint8_t count = 5;
	while(count>0)
	{
		Delay_ms(100);
		USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
		UART_SendString(USART1,(char *)"AT+MQTTUSERCFG=0,1,\""MQTT_CLIENT_ID"\",\""MQTT_USER_NAME"\",\""MQTT_PASSWD"\",0,0,\"\"");
		Delay_ms(100);
		if((USART_BUF_CMP(UART_1_RXBUF,"OK",200)==0))
		{
			break;
		}
		if((USART_BUF_CMP(UART_1_RXBUF,"ERROR",100)==0))
		{
			UART_SendString(USART1,"AT+MQTTCLEAN=0");//断开连接
			if((USART_BUF_CMP(UART_1_RXBUF,"OK",200)==0))
			{
				Delay_ms(100);
				USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
				UART_SendString(USART1,(char *)"AT+MQTTUSERCFG=0,1,\""MQTT_CLIENT_ID"\",\""MQTT_USER_NAME"\",\""MQTT_PASSWD"\",0,0,\"\"");
				Delay_ms(100);
				if((USART_BUF_CMP(UART_1_RXBUF,"OK",200)==0))
				{
					break;
				}
			}
		}
		if(count == 5)
		{
			OLED_Clear();
			OLED_ShowString(1,1,"MQTT USECFG Timeout");
			return 1;
		}
	}
	count = 5;
	while(count>0)
	{
		USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
		UART_SendString(USART1,(char *)"AT+MQTTCONN=0,\""BROKER_ADDRESS"\",1883,0");
		Delay_ms(100);
		if((USART_BUF_CMP(UART_1_RXBUF,"OK",200)==0))
		{
			break;
		}
		if(count == 5)
		{
			OLED_Clear();
			OLED_ShowString(1,1,"MQTT CONNECT Timeout");
			return 1;
		}
	}
	count = 5;
	while(count>0)
	{
		USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
		UART_SendString(USART1,(char *)"AT+MQTTSUB=0,\""SUB_TOPIC"\",0");
		Delay_ms(100);
		if((USART_BUF_CMP(UART_1_RXBUF,"OK",200)==0))
		{
			USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
			break;
		}
		if(count == 5)
		{
			USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
			OLED_Clear();
			OLED_ShowString(1,1,"MQTT SUB Timeout");
			return 1;
		}
	}
	return 0;
}


/*
**配置网络
*/
uint8_t ESP8266_Config_Network(void)
{
	uint8_t ret = 0;
	USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
	UART_SendString(USART1,(char *)"AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASSWD"\""); 
	Delay_ms(100);
	if(USART_BUF_CMP(UART_1_RXBUF,"OK",200))
	{
		ret = 1;
	}
	return ret;
}



uint8_t Wifi_Init(void)
{
	UART1_Init(115200);
	OLED_Clear();
	OLED_ShowString(1,1,"WIFI Start Init");
	USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
	UART_SendString(USART1,"AT");
	Delay_ms(100);
	if(USART_BUF_CMP(UART_1_RXBUF,"OK",100))
	{
		OLED_Clear();
		OLED_ShowString(1,1,"AT Timeout");
		return 1;
	}
	Delay_ms(100);
	USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
	UART_SendString(USART1,"AT+CWMODE=1");//设置为station模式，需连接wife
	Delay_ms(100);
	if(USART_BUF_CMP(UART_1_RXBUF,"OK",100))
	{
		OLED_Clear();
		OLED_ShowString(1,1,"CWMODE Timeout");
		return 1;
	}
	Delay_ms(100);
	USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
	UART_SendString(USART1,"AT+CWAUTOCONN=0");//不要自动连接
	Delay_ms(100);
	if(USART_BUF_CMP(UART_1_RXBUF,"OK",100))
	{
		OLED_Clear();
		OLED_ShowString(1,1,"CWAUTOCONN Timeout");
		return 1;
	}
	USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
//	UART_SendString(USART1,"AT+RST");//复位
	Delay_ms(100);
	if(ESP8266_Config_Network())
	{
		OLED_Clear();
		OLED_ShowString(1,1,"Network cfg Timeout");
		return 1;
	}
	if(MQTT_Config())
	{
		return 1;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"Init SUCCESS");
	return 0;
}

uint8_t MQTT_PUB_DATA(uint16_t Tem_value, uint16_t Humi_value)
{
	char msg_buf[256];
	uint8_t count = 5;
  sprintf((char *)msg_buf,"AT+MQTTPUB=0,\""PUB_TOPIC"\",\""JSON_FORMAT"\",0,0",Tem_value,Humi_value);
	while(count>0)
	{
		USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
		UART_SendString(USART1,msg_buf);
		if((USART_BUF_CMP(UART_1_RXBUF,"OK",200)==0))
		{
			USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
			break;
		}
		if(count == 5)
		{
			//OLED_Clear();
			USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
			//OLED_ShowString(1,1,"MQTT PUB Timeout");
			return 1;
		}
	}
	return 0;
}

/*
**查找 JSON 字符串中指定键的值
**key是匹配关键字
*/
uint8_t find_json_value(const char *json_str, const char *key, char *value, size_t value_size) {
    // 在 json_str 中查找 key 的位置
	char *key_pos,*end_pos;
    key_pos = strstr(json_str, key);
    if (key_pos == NULL) {
        return 1; // 未找到键
    }
    
    // 移动指针到键后面的字符
    key_pos += strlen(key);
    // 找到冒号
    while (*key_pos != ':') {
        key_pos++;
    }
	key_pos++;
	
	end_pos = key_pos;
    while (*end_pos != ',' && *end_pos != '}' && *end_pos != '"' && *end_pos != '\0' && *end_pos != '\\') {
        end_pos++;
    }
    // 将找到的值复制到提供的缓冲区
    size_t len = end_pos - key_pos;
    if (len >= value_size) {
        return 1; // 值太大，无法存储在缓冲区中
    }
    strncpy(value, key_pos, len);
    value[len] = '\0';
    return 0; // 成功
}



/*
**接收MQTT服务器数据
**返回数据0表示接收成功
*/
uint8_t Rec_MQTT_Data(struct MQTTDataTypeDef *pMQTT_Rec_Data)
{
	int Msg_len = 0;
	char Msg_body[256] = {'\0'};
	uint8_t ret = 0;
	char value[100] = {'\0'};
	if(UART_1_RXBUF_INDEX>100)
	{
		if(strstr(UART_1_RXBUF,"+MQTTSUBRECV:"))
		{
			Delay_ms(100);
			sscanf(UART_1_RXBUF,"+MQTTSUBRECV:0,\""SUB_TOPIC"\",%d,%s",&Msg_len,Msg_body);
			/*UART_SendString(USART2,Msg_len);
			UART_SendString(USART2,"\r\nMsg\r\n");
			UART_SendString(USART2,Msg_body);
			UART_SendString(USART2,"\r\nMsg\r\n");*/
			USART_BUF_Clear(UART_1_RXBUF,&UART_1_RXBUF_INDEX);
			if(1)
			{
				if(!find_json_value(Msg_body,"switch",value,100))
				{
					OLED_ShowString(3,1,value);
					pMQTT_Rec_Data->led_staus = atoi(value);
				}
				else if(!find_json_value(Msg_body,"tem",value,100))
				{
					//OLED_ShowString(4,1,value);
					pMQTT_Rec_Data->tem_value = atoi(value);
				}
				else
				{
					ret = 1;
				}
			}
			else
			{
				ret = 1;
			}
		}
		else
		{
			ret = 1;
		}
	}
	else
	{
		ret = 1;
	}
	return ret;
}
