#ifndef __DHT11_H__
#define __DHT11_H__

#define DHT11_Pin GPIO_Pin_15
#define DHT11_GPIO_Port GPIOB
uint8_t dht11_init (void);
uint8_t dht11_read_data(uint8_t *temp,uint8_t *humi);

#endif
