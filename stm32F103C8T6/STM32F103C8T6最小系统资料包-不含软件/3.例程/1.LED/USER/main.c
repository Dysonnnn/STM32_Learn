/************************************************************************************
							�������ṩ�����µ��̣�
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							ʵ�������Χ��չģ����������ϵ���
							���ߣ����زر���							
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






