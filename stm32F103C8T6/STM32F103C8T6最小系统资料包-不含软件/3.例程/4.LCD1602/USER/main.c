/************************************************************************************
							本例程提供自以下店铺：
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							实验相关外围扩展模块均来自以上店铺
							作者：神秘藏宝室							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "LCD1602.h"
#include "led.h"



int main(void)
{		
	delay_init();
	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Init_LEDpin();
	LCD_init();
	LCD_write_string(1,1,"Welcome to use!");
	
	while(1)
	{
		
		LED1 = 0;
		delay_ms(1000);
		delay_ms(1000);
		LED1 = 1;
				
		delay_ms(1000);
		delay_ms(1000);
	}
}






