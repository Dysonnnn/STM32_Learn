# stm32

## 部分总结

- 使用cubeMX配置的时候，注意初始化函数的顺序，如果有DMA初始化的，要放到uart初始化之前。
- 定时器的使用需要先调用函数开启定时器中断；
- 


## 串口
###  空闲中断+DMA接收数据


[[STM32L4] STM32 HAL库 串口中断接收不定长问题](https://bbs.21ic.com/icview-2512754-1-1.html?ordertype=1&_dsign=03b3abaa)
```
以下是我学习DMA接收不定长数据的学习笔记，没有完整的程序，但是根据笔记中的核心代码，编写相应的程序没什么难度。

使用串口DMA方式接收不定长数据学习笔记
编程思路：
1、通过检测串口空闲中断来检测一帧不定长数据接收完毕
2、当空闲中断发生后，停止DMA接收数据，检测DMA剩余接收字节和缓冲区固定长度计算已接收的字符数
3、数据处理后，恢复相关中断标志、变量等
4、继续打开串口DMA接收，为下一帧数据的接收做准备（如果DMA设置为循环方式的话，开启命令可以省略，但是计数器需要处理）


具体代码如下：

首先定义一个串口收发的缓冲区和相应的标志变量（如果只接收的话，TX可以不定义）

//用户全局变量定义
#ifndef Buffer_Size
        #define        Buffer_Size                255
#endif
uint8_t USART_TxBuffer[Buffer_Size];                //定义32字节串口发送缓冲区
uint8_t USART_RxBuffer[Buffer_Size];                //定义32字节串口接收缓冲区
uint8_t USART_Rx_OK = 0;                                                                //定义串口接收完成标志
uint8_t USART_Buffer_index;                                                        //缓冲区索引



在主程序初始化后开启空闲中断和DMA接收

__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);    //使能空闲中断
  HAL_UART_Receive_DMA(&huart1,USART_RxBuffer,Buffer_Size);        //开启串口DMA接收，开始需运行一次，后面会自动打开



在是stm32f0xx_it.c里
先将中断中用到的全局变量外部引用声明一下，免得编译时出错

#ifndef Buffer_Size
        #define        Buffer_Size                255
#endif

extern uint8_t USART_TxBuffer[Buffer_Size];                //定义32字节串口发送缓冲区
extern uint8_t USART_RxBuffer[Buffer_Size];                //定义32字节串口接收缓冲区
extern uint8_t USART_Rx_OK;                                                                //定义串口接收完成标志
extern uint8_t USART_Buffer_index;                                                        //缓冲区索引


在串口中断处理函数里加入判断是否是空闲中断的代码，如果发生了串口空闲中断，则进行进一步处理，并将串口完成全局标志变量置位
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  uint32_t temp;
  
        if(huart1.Instance == USART1)                //判断是否是串口1
        {
                if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET)        //判断是否是串口空闲中断
                {
                        __HAL_UART_CLEAR_IDLEFLAG(&huart1);                        //先清除串口空闲中断标志位
                        HAL_UART_DMAStop(&huart1);                                //停止DMA接收
                        temp = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);        //取DMA 未接收 的数据个数
                        USART_Buffer_index = Buffer_Size - temp;        //通过串口接收缓冲区长度计算实际接收的数据个数
                        USART_Rx_OK = 1;                                //将串口不定长接收标志位置位
               
                }
       
        }
       
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
       
  /* USER CODE END USART1_IRQn 1 */
}


检测串口完成标志，如置位则进一步处理，这段程序可以放在主循环里，也可以放在定时器中断或者系统滴答中断里，如果放在中断里面执行，需要确保代码运行时间不要超过中断的总时长，以免影响中断功能。在中断里尽量少使用printf（）等运行时间长的函数。可以使用DMA发送函数来解决运行时长的问题。

if(USART_Rx_OK == 1)                //串口传输结束标志
        {
                printf("接收字符数量：%d\r\n",USART_Buffer_index);   //此次接收到了多少数据
                for(i=0;i<USART_Buffer_index;i++)
                {
                        printf("%c",USART_RxBuffer);                //用户数据处理
                }
                for(i=0;i<Buffer_Size;i++)                        //缓冲区清零，实际有准确的接收字符个数后，可以不清零
                {
                        USART_RxBuffer = 0;
                }
                USART_Buffer_index = 0;
                USART_Rx_OK = 0;
        }
HAL_UART_Receive_DMA(&huart1,USART_RxBuffer,Buffer_Size);        //打开串口DMA接收，为接收下一帧数据做准备

```
