/************************************************************************************
							�������ṩ�����µ��̣�
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							ʵ�������Χ��չģ����������ϵ���
							���ߣ����زر���							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "LCD1602.h"
#include "led.h"



int main(void)
{		
	delay_init();
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
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






