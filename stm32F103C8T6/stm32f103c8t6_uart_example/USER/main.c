/************************************************************************************
							本例程提供自以下店铺：
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							实验相关外围扩展模块均来自以上店铺
							作者：神秘藏宝室							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "HX711.h"
#include "usart.h"

int main(void)
{	
	u8 i;
	u8 len;	
	u16 times=0;
	
	delay_init();
	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	
	
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n\r\n");
			for(i=0;i<len;i++)
			{
				USART_SendData(USART1, USART_RX_BUF[i]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0; 
		}
        else
		{
			times++;
			if(times%5000==0)
			{
				printf("\r\n我测试的例程\r\n");
				printf("Beetle\r\n\r\n");
			}
			if(times%200==0)printf("请输入数据,以回车键结束\n");  
			delay_ms(10);   
		}
	}
}






