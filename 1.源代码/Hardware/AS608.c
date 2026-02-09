#include "stm32f10x.h"                  // Device header

#include "AS608.h"
#include "key.h"
#include "Uart.h"
#include "OLED.h"
#include "Delay.h"
#include "BEEP.h"

#define UART_AS608 USART2


unsigned char FPM10A_RECEICE_BUFFER[32];
uint8_t FPM10A_RECEICE_BUFFER_INdex = 0;
uint8_t AS608_Rx_Flag = 0;
extern unsigned int finger_id;

//FINGERPRINT通信协议定义

unsigned char FPM10A_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};//口令验证
unsigned char FPM10A_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //协议包头
unsigned char FPM10A_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    //获得指纹图像
unsigned char FPM10A_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 }; //获得模版总数
unsigned char FPM10A_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; //搜索指纹搜索范围0 - 999,使用BUFFER1中的特征码搜索
unsigned char FPM10A_Search_0_9[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x13,0x00,0x21}; //搜索0-9号指纹
unsigned char FPM10A_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; //将图像放入到BUFFER1
unsigned char FPM10A_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; //将图像放入到BUFFER2
unsigned char FPM10A_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09}; //将BUFFER1跟BUFFER2合成特征模版
unsigned char FPM10A_Delete_All_Model[6]={0x01,0x00,0x03,0x0d,0x00,0x11};//删除指纹模块里所有的模版
volatile unsigned char  FPM10A_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};//将BUFFER1中的特征码存放到指定的位置




//AS608串口发送字节
void Uart_Send_Byte(unsigned char data)
{
	while(USART_GetFlagStatus(UART_AS608,USART_FLAG_TXE)==RESET );//等待发送寄存器空
	USART_SendData(UART_AS608, data);
}

//AS608串口接收字节
unsigned char Uart_Receive_Byte()
{
	USART_ReceiveData(UART_AS608);
}

 //发送包头
void FPM10A_Cmd_Send_Pack_Head(void)
{
	int i;	
	for(i=0;i<6;i++) //包头
   {
     Uart_Send_Byte(FPM10A_Pack_Head[i]);   
    }		
}
//发送指令
void FPM10A_Cmd_Check(void)
{
	int i=0;
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<10;i++)
	 {		
		Uart_Send_Byte(FPM10A_Get_Device[i]);
	  }
}
//接收反馈数据缓冲
void FPM10A_Receive_Data(unsigned char ucLength)
{
  unsigned char i;

  for (i=0;i<ucLength;i++)
     FPM10A_RECEICE_BUFFER[i] = Uart_Receive_Byte();

}

//FINGERPRINT_获得指纹图像命令
void FPM10A_Cmd_Get_Img(void)
{
    unsigned char i;
    FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
    for(i=0;i<6;i++) //发送命令 0x1d
	{
       Uart_Send_Byte(FPM10A_Get_Img[i]);
	}
}
//讲图像转换成特征码存放在Buffer1中
void FINGERPRINT_Cmd_Img_To_Buffer1(void)
{
 	unsigned char i;
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头      
   	for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
     {
      Uart_Send_Byte(FPM10A_Img_To_Buffer1[i]);
   	 }
}
//将图像转换成特征码存放在Buffer2中
void FINGERPRINT_Cmd_Img_To_Buffer2(void)
{
     unsigned char i;
     for(i=0;i<6;i++)    //发送包头
	 {
    	Uart_Send_Byte(FPM10A_Pack_Head[i]);   
   	 }
     for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
      {
      	Uart_Send_Byte(FPM10A_Img_To_Buffer2[i]);
   	  }
}
//搜索全部用户999枚
void FPM10A_Cmd_Search_Finger(void)
{
       unsigned char i;	   	    
			 FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
       for(i=0;i<11;i++)
           {
    	      Uart_Send_Byte(FPM10A_Search[i]);   
   		   }
}

void FPM10A_Cmd_Reg_Model(void)
{
       unsigned char i;	   
	    
			 FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头

       for(i=0;i<6;i++)
           {
    	      Uart_Send_Byte(FPM10A_Reg_Model[i]);   
   		   }


}
//删除指纹模块里的所有指纹模版
void FINGERPRINT_Cmd_Delete_All_Model(void)
{
     unsigned char i;    
    for(i=0;i<6;i++) //包头
      Uart_Send_Byte(FPM10A_Pack_Head[i]);   
    for(i=0;i<6;i++) //命令合并指纹模版
	   {
      Uart_Send_Byte(FPM10A_Delete_All_Model[i]);   
		 }	
}
//保存指纹
void FPM10A_Cmd_Save_Finger( unsigned int storeID )
{
       unsigned long temp = 0;
		   unsigned char i;
       FPM10A_Save_Finger[5] =(storeID&0xFF00)>>8;
       FPM10A_Save_Finger[6] = (storeID&0x00FF);
		   for(i=0;i<7;i++)   //计算校验和
		   	   temp = temp + FPM10A_Save_Finger[i]; 
		   FPM10A_Save_Finger[7]=(temp & 0x00FF00) >> 8; //存放校验数据
		   FPM10A_Save_Finger[8]= temp & 0x0000FF;		   
       FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头	
       for(i=0;i<9;i++)  
      		Uart_Send_Byte(FPM10A_Save_Finger[i]);      //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
}




void AS608_Add_Fingerprint()
{
	unsigned char id_show[]={0,0,0};
	uint8_t i = 0;
	OLED_Clear(); //清屏  
	OLED_ShowString(1,1,"Add  finger");
	Delay_ms(1500);

	 //按确认键开始录入指纹信息 		 			
			OLED_ShowString(1,1,"Please  finger");
			do{
				FPM10A_RECEICE_BUFFER_INdex = 0;
				AS608_Rx_Flag = 1;
				FPM10A_Cmd_Get_Img(); //获得指纹图像
				Delay_ms(500);
			  }while(FPM10A_RECEICE_BUFFER[9]!=0);
				AS608_Rx_Flag = 0;
				//判断接收到的确认码,等于0指纹获取成功
					Delay_ms(100);
					FINGERPRINT_Cmd_Img_To_Buffer1();
					OLED_ShowString(1,1,"Successful entry");
					BEEP_Time(1);
					Delay_ms(1000);
					OLED_ShowString(1,1,"Please  finger");
					do{
						FPM10A_RECEICE_BUFFER_INdex = 0;
						AS608_Rx_Flag = 1;
						FPM10A_Cmd_Get_Img(); //获得指纹图像
						Delay_ms(500);
					  }while(FPM10A_RECEICE_BUFFER[9]!=0);
						AS608_Rx_Flag = 0;
						Delay_ms(200);
						OLED_ShowString(1,1,"Successful entry");

						FINGERPRINT_Cmd_Img_To_Buffer2();
						FPM10A_Cmd_Reg_Model();//转换成特征码 
						do{
							FPM10A_RECEICE_BUFFER_INdex = 0;
							AS608_Rx_Flag = 1;
							FPM10A_Cmd_Save_Finger(finger_id);
							Delay_ms(500);
							OLED_ShowString(1,1,"Storing         ");
							if(i++ > 10)
							{
								OLED_ShowString(1,1,"Storing  Fail");
								Delay_ms(2000);
								break;
							}
							}while(FPM10A_RECEICE_BUFFER[9]!=0);
						AS608_Rx_Flag = 0;
						BEEP_Time(1);
						Delay_ms(1000);
						finger_id=finger_id+1;
						OLED_Clear();

}

//搜索指纹
void AS608_Find_Fingerprint()
{
	unsigned int find_fingerid = 0;
	unsigned char id_show[]={0,0,0};
	OLED_Clear();
		OLED_ShowString(1,1,"Please  finger");
		do{
				FPM10A_RECEICE_BUFFER_INdex = 0;
				AS608_Rx_Flag = 1;
				FPM10A_Cmd_Get_Img(); //获得指纹图像
				Delay_ms(300);
			}while(FPM10A_RECEICE_BUFFER[9]!=0);
			AS608_Rx_Flag = 0;		
		//判断接收到的确认码,等于0指纹获取成功		
			Delay_ms(100);
			FINGERPRINT_Cmd_Img_To_Buffer1();		
				FPM10A_RECEICE_BUFFER_INdex = 0;
				AS608_Rx_Flag = 1;
				FPM10A_Cmd_Search_Finger();
				Delay_ms(500);
				AS608_Rx_Flag = 0;
			if(FPM10A_RECEICE_BUFFER[9] == 0) //搜索到  
			{
				OLED_ShowString(1,1,"                ");
				OLED_ShowString(1,1,"Search success");
				BEEP_Time(1);					
				//拼接指纹ID数
				find_fingerid = FPM10A_RECEICE_BUFFER[10]*256 + FPM10A_RECEICE_BUFFER[11];										
				Delay_ms(2000);				
			   }
				else //没有找到
				{
					OLED_ShowString(1,1,"                ");
					OLED_ShowString(1,1," Search  failed ");
					OLED_ShowString(2,1,"                ");
				 	BEEP_Time(3);
				}
				OLED_Clear();
}

//删除所有存贮的指纹库
void AS608_Delete_All_Fingerprint()
{
		unsigned char i=0;
		OLED_Clear();
		FINGERPRINT_Cmd_Delete_All_Model();
		OLED_Clear();
		OLED_ShowString(1,1,"All finger empty");
		finger_id = 0;	
		BEEP_Time(3);
		OLED_Clear();
}


void Device_Check(void)
{
		unsigned char i=0;
		FPM10A_RECEICE_BUFFER[9]=1;				           //串口数组第九位可判断是否通信正常
		OLED_Clear();
		OLED_ShowString(1,1,"Loading");	           //设备加载中界面							   	
		FPM10A_RECEICE_BUFFER_INdex = 0;
		do
		{
			AS608_Rx_Flag = 1;
			FPM10A_Cmd_Check();									//单片机向指纹模块发送校对命令
			Delay_ms(10);
		}while(FPM10A_RECEICE_BUFFER[9] != 0);
		AS608_Rx_Flag = 0;
		FPM10A_Cmd_Get_Img(); //获得指纹图像
//		FPM10A_RECEICE_BUFFER[9]=1;
//		OLED_ShowString(1,1,"AS608 Init  success");		//符合成功条件则显示对接成功
}





