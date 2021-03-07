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
	Init_HX711pin();
	delay_init();
	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	
	Get_Maopi();				//称毛皮重量
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//重新获取毛皮重量
	
	while(1)
	{
		Get_Weight();
		if(Flag_Error == 0)
		{
			printf("净重量 = %d g\r\n",Weight_Shiwu); //打印 
			delay_ms(1000);
		}
		else
		{
			printf("负重或者超重\r\n");
			delay_ms(1000);
		}
	}
}






