#ifndef __FLASH_H_
#define __FLASH_H_

#define address          ((uint32_t)0x0801f000) 

void FLASH_Writeadd(uint32_t addr ,uint16_t *flashdata,uint8_t len);
void Read_Flash(uint32_t addr,uint8_t len);
void FLASH_WriteByte(u32 addr ,uint16_t data);
uint16_t FLASH_Readdata(uint32_t addr);
void FLASH_Writedata(u32 addr ,uint16_t data);

#endif
