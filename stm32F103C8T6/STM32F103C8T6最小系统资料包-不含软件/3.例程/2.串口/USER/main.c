/************************************************************************************
							�������ṩ�����µ��̣�
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							ʵ�������Χ��չģ����������ϵ���
							���ߣ����زر���							
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
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	
	
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
			for(i=0;i<len;i++)
			{
				USART_SendData(USART1, USART_RX_BUF[i]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0; 
		}
        else
		{
			times++;
			if(times%5000==0)
			{
				printf("\r\n�Ҳ��Ե�����\r\n");
				printf("Beetle\r\n\r\n");
			}
			if(times%200==0)printf("����������,�Իس�������\n");  
			delay_ms(10);   
		}
	}
}






