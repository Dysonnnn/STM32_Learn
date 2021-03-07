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
#include "tsensor.h"



int main(void)
{	
	u16 adcx;
	float temp;
 	float temperate;
	delay_init();
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Init_LEDpin();
	LCD_init();
	LCD_write_string(1,1,"Welcome to use!");
	
	T_Adc_Init();		  		//ADC��ʼ��
	
	uart_init(9600);
	while(1)
	{
		adcx=T_Get_Adc_Average(ADC_CH_TEMP,10);		//ADC��ֵ
		temp=(float)adcx*(3.3/4096);
		temperate=temp;//�����¶ȴ������ĵ�ѹֵ
		temperate=(1.43-temperate)/0.0043+25;		//�������ǰ�¶�ֵ
				
		LCD_write_string(1,2,"Temp = ");
		
		printf("Temp =  %f\r\n", temperate);
		
		LCD_write_data((u8)temperate/100+0x30);
		LCD_write_data((u8)temperate%100/10+0x30);
		LCD_write_data((u8)temperate%10+0x30);
		LCD_write_data('.');
		
		temperate-=(u8)temperate;	//ȡ��С������
		temperate *= 100;
		LCD_write_data((u8)temperate/10+0x30);
		LCD_write_data((u8)temperate%10+0x30);
		
		LCD_write_data(' ');
		LCD_write_data(0xdf);
		LCD_write_data('C');
		delay_ms(200);
	}
}






