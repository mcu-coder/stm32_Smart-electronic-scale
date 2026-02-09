#ifndef __BEEP_H
#define __BEEP_H

#define BEEP_Pin GPIO_Pin_13
#define BEEP_GPIO_Port GPIOB

void BEEP_Init(void);
void BEEP_Set(uint8_t staus);
void BEEP_Time(unsigned char times);

#endif
