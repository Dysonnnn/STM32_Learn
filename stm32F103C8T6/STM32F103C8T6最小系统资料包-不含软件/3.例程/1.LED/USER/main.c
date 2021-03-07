/************************************************************************************
							本例程提供自以下店铺：
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							实验相关外围扩展模块均来自以上店铺
							作者：神秘藏宝室							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "LED.h"

int main(void)
{	
	Init_LEDpin();
 
	delay_init();
	while(1)
	{
		LED1 = 1;
		delay_ms(500);

		LED1 = 0;
		delay_ms(500);
	}
}






