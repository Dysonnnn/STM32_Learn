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
#include "tsensor.h"



int main(void)
{	
	u16 adcx;
	float temp;
 	float temperate;
	delay_init();
	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Init_LEDpin();
	LCD_init();
	LCD_write_string(1,1,"Welcome to use!");
	
	T_Adc_Init();		  		//ADC初始化
	
	uart_init(9600);
	while(1)
	{
		adcx=T_Get_Adc_Average(ADC_CH_TEMP,10);		//ADC的值
		temp=(float)adcx*(3.3/4096);
		temperate=temp;//保存温度传感器的电压值
		temperate=(1.43-temperate)/0.0043+25;		//计算出当前温度值
				
		LCD_write_string(1,2,"Temp = ");
		
		printf("Temp =  %f\r\n", temperate);
		
		LCD_write_data((u8)temperate/100+0x30);
		LCD_write_data((u8)temperate%100/10+0x30);
		LCD_write_data((u8)temperate%10+0x30);
		LCD_write_data('.');
		
		temperate-=(u8)temperate;	//取出小数部分
		temperate *= 100;
		LCD_write_data((u8)temperate/10+0x30);
		LCD_write_data((u8)temperate%10+0x30);
		
		LCD_write_data(' ');
		LCD_write_data(0xdf);
		LCD_write_data('C');
		delay_ms(200);
	}
}






