/************************************************************************************
							本例程提供自以下店铺：
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							实验相关外围扩展模块均来自以上店铺
							作者：神秘藏宝室							
*************************************************************************************/
#include "LCD1602.h"


GPIO_InitTypeDef GPIO_InitStructure;
/* If processor works on high frequency delay has to be increased, it can be 
   increased by factor 2^N by this constant                                   */
#define DELAY_2N     0

//==================================================
void LCD_init(void)
{
    /*********************液晶使用的I/O口初始化**************************/ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    CLR_RW();			//读写位直接低电平，只写不读

    /*********************液晶初始化**************************/        
    delay (15000);
	

   
    CLR_RS();
    LCD_Write_half_byte(0x3);                 
    delay (15000);
    LCD_Write_half_byte(0x3);
    delay (15000);
    LCD_Write_half_byte(0x3);
    LCD_Write_half_byte(0x2);
   
    
    LCD_write_cmd(0x28);          // 4bit显示模式,2行,5x7字体
    delay (20000);  
    LCD_write_cmd(0x08);         // 显示关闭 
    delay (20000); 
    LCD_write_cmd(0x01);         // 显示清屏 
    delay (20000); 
    LCD_write_cmd(0x06);         // 显示光标移动设置 
    delay (20000);
    LCD_write_cmd(0x0C);         //显示开,光标关，
    //LCD_write_cmd(0x0F);         // 显示开，光标开，光标闪烁
    delay (20000);
	LCD_write_cmd(0x01);         //清屏
}
/*--------------------------------------------------
函数说明：写命令到液晶


---------------------------------------------------*/
void LCD_write_cmd(unsigned char cmd)
{
    CLR_RS();
    LCD_Write_half_byte(cmd >> 4);
    LCD_Write_half_byte(cmd);
    delay (10000);
}
/*--------------------------------------------------
函数说明：写数据到液晶


---------------------------------------------------*/
void LCD_write_data(unsigned char w_data)
{
    SET_RS();
    LCD_Write_half_byte(w_data >> 4);
    LCD_Write_half_byte(w_data);
    delay (10000);
}
/*--------------------------------------------------
函数说明：写4bit到液晶
--------------------------------------------------*/
void LCD_Write_half_byte(unsigned char half_byte)
{  
//    u16 temp_io = 0x0000;
//    temp_io = GPIO_ReadOutputData(GPIOE);   //读端口E输出口的数据
//    temp_io &= 0xfff0;                      //屏蔽低四位
//    temp_io |= (u16)(half_byte&0x0f);       //得到新数据
//    GPIO_Write(GPIOE,temp_io);              //写入新数据
	if (half_byte&0x01)
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
	else
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);

	if (half_byte&0x02)
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
	else
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);

	if (half_byte&0x04)
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
	else
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);

	if (half_byte&0x08)
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
	else
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);

    SET_EN();
    delay(2000);
    CLR_EN(); 
    delay(2000);
}

/*----------------------------------------------------
LCD_set_xy        : 设置LCD显示的起始位置
输入参数：x、y    : 显示字符串的位置，X:1-16，Y:1-2                
-----------------------------------------------------*/
void LCD_set_xy( unsigned char x, unsigned char y )
{
    unsigned char address;
    if (y==1) 
    {
        address=0x80-1+x;
    }
    else 
    {
        address=0xc0-1+x;
    }
    LCD_write_cmd(address);
}
/*---------------------------------------------------
LCD_write_string  : 英文字符串显示函数
输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置                
---------------------------------------------------*/
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s)
{
    LCD_set_xy( X, Y );   
    while (*s) 
    {
        LCD_write_data(*s);
        s++;
    }
}
//=======================================================
/*
void Move(unsigned int step,unsigned int dirction,unsigned int time)
{
          unsigned int i;
         for(i=0;i<step-1;i++)
         {
                   LCD_write_byte(1,dirction);      //字符移动方向                                    
         Delay_nms(time);                //控制移动时间
         }
}
*/
//=========================================================
/*
void Flash_lcd(unsigned int delay_t, unsigned int times)
{
           unsigned int j;
         for(j=0;j<times;j++)
         {
                  LCD_write_byte(1,0x08);
                Delay_nms(delay_t);
                LCD_write_byte(1,0x0c);
                Delay_nms(delay_t);
         }
}
*/
//========================================================
void delay(vu32 cnt)
{
  cnt <<= DELAY_2N;

  while (cnt--);
}
//========================================================

