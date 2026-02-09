#include "stm32f10x.h"                  // Device header
#include "Flash.h"
#include "OLED.h"
//#include "AS608.h"

uint16_t NEW_ParArry[5] = {0};

void FLASH_Writedata(u32 addr ,uint16_t data)
{
	
		int i = 0;
	
		FLASH_Status FLASHstatus = FLASH_COMPLETE;
		FLASH_Unlock(); //解锁flash
		
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);  //清除标志位
		FLASHstatus=FLASH_ErasePage(addr);   //擦除指定地址页
		
		
		FLASH_ProgramHalfWord(addr, data);//从指定的地址写入数据
		FLASH_Lock();//上锁 
}


uint16_t FLASH_Readdata(uint32_t addr)
{
	uint16_t data;
	FLASH_Unlock();//解锁flash
	

	data = *(u16 *)(addr);    //读取flash地址中的值
	
	FLASH_Lock();//上锁
	return data;
}


void FLASH_Writeadd(u32 addr ,uint16_t *flashdata1,uint8_t len)
{
	
		int i = 0;
	
		FLASH_Status FLASHstatus = FLASH_COMPLETE;
		FLASH_Unlock(); //解锁flash
		
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);  //清除标志位
		FLASHstatus=FLASH_ErasePage(addr);   //擦除指定地址页
		
		for(i=0; i<len;i++)
		{
			FLASH_ProgramHalfWord(addr+i*2, flashdata1[i]);//从指定的地址写入数据
		}
		FLASH_Lock();//上锁 
}

void FLASH_WriteByte(u32 addr ,uint16_t data)
{
	
		
	
		//FLASH_Status FLASHstatus = FLASH_COMPLETE;
		FLASH_Unlock(); //解锁flash

		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);  //清除标志位
		FLASH_ErasePage(address);
		//FLASHstatus=FLASH_ErasePage(addr);   //擦除指定地址页
		
		
		FLASH_ProgramHalfWord(addr, data);//从指定的地址写入数据
		FLASH_Lock();//上锁 
}

void Read_Flash(uint32_t addr,uint8_t len)
{
	
	u8 i=0;
	
	FLASH_Unlock();//解锁flash
	
	for(i=0;i<len;i++)
	{
		NEW_ParArry[i] = *(u16 *)(addr + i*2);    //读取flash地址中的值
	}
	
	FLASH_Lock();//上锁
	
}

