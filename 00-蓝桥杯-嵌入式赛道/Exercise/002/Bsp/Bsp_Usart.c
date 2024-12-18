/*******************************************************************************
* 文件名称：
* 功能描述：1、CT117E_M4实验板上串口的操作
            2、函数HAL_UART_Receive_IT(&huart1, &Usart1_RxData_Temp, 1)在串口初始化和中断函数中必须要调用；
* 配置说明：
* 硬件环境：CT117E_M4
* 注意事项：
* 作者版本：hufq/V1.0
*******************************************************************************/
#include "Bsp_Usart.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"

unsigned char Uart1_Rx_Buf[UART1RXBUF_LEN];     //接收缓冲
unsigned char U1Rx_Len=0;                       //接收长度
unsigned char Uart1_Tx_Buf[UART1RXBUF_LEN];     //发送缓冲
unsigned char U1Tx_Len=0;                       //发送长度


/****************************************************
函数名称:
功    能: 串口接收中断的回调函数，函数名字在stm32g4xx_hal_uart.c中定义为weak函数；
入口参数: 无
出口参数:
****************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1)//如果是串口1
    {
        Uart1_Rx_Buf[U1Rx_Len]=Usart1_RxData_Temp;
        U1Rx_Len++;
        if(U1Rx_Len>(UART1RXBUF_LEN-1)) //注意虽然开了UART1RXBUF_LEN（20）个字节缓冲，但是只能用到UART1RXBUF_LEN-1个
            U1Rx_Len=0;
        
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量,需要每次中断调用一下
        HAL_UART_Receive_IT(&huart1, &Usart1_RxData_Temp, 1);    //必须重新调用，因为该函数在接收一次后会关闭中断    
    }
}

/****************************************************
函数名称:
功    能: 串口发送函数
入口参数: bufpt：发送数组；buflen：发送数据长度
出口参数:
****************************************************/
void Uart1_Send(unsigned char *bufpt,unsigned char buflen)
{
    HAL_UART_Transmit(&huart1,bufpt,buflen,1000);	//发送接收到的数据
    while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);		//等待发送结束
}
