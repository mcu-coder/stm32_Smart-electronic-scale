#ifndef __ESP8266_WIFI_H__
#define __ESP8266_WIFI_H__
#include "stdlib.h"
#define WIFI_SSID          "KT"
#define WIFI_PASSWD        "88888888"
#define MQTT_CLIENT_ID     "k1bd4Q5IuBr.stm32_esp8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1716280324146|"
#define MQTT_USER_NAME     "stm32_esp8266&k1bd4Q5IuBr"
#define MQTT_PASSWD        "8409f6d05bf844adc5d5e31c3ef596cd0711d721f74d76aa4cc8040daac24781"
#define BROKER_ADDRESS     "iot-06z00jm05tpeogx.mqtt.iothub.aliyuncs.com"
#define SUB_TOPIC          "/k1bd4Q5IuBr/stm32_esp8266/user/wifi"
#define PUB_TOPIC          "/k1bd4Q5IuBr/stm32_esp8266/user/wifi"
#define JSON_FORMAT      "{\\\"params\\\":{\\\"tem\\\":%d\\,\\\"humi\\\":%d\\}\\,\\\"version\\\":\\\"1.0.0\\\"}"
#define REC_Data          " {\"method\":\"thing.service.property.set\",\"id\":\"1438962074\",\"params\":{\"switch\":0},\"version\":\"1.0.0\"}"
struct MQTTDataTypeDef
{
	uint8_t tem_value;
	uint8_t humi_value;
	uint8_t  led_staus;
};

extern struct MQTTDataTypeDef MQTT_Rec_Data;


uint8_t Wifi_Init(void);
uint8_t MQTT_PUB_DATA(uint16_t Tem_value, uint16_t Humi_value);
uint8_t Rec_MQTT_Data(struct MQTTDataTypeDef *pMQTT_Rec_Data);
uint8_t find_json_value(const char *json_str, const char *key, char *value, size_t value_size);

#endif
