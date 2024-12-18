#include "Bsp_Usart.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"

unsigned char Uart1_Rx_Buf[UART1RXBUF_LEN];     //接收缓冲
unsigned char U1Rx_Len=0;                       //接收长度
unsigned char Uart1_Tx_Buf[UART1RXBUF_LEN];     //发送缓冲
unsigned char U1Tx_Len=0;                       //发送长度

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

void Uart1_Send(unsigned char *bufpt,unsigned char buflen)
{
    HAL_UART_Transmit(&huart1,bufpt,buflen,1000);	//发送接收到的数据
    while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);		//等待发送结束
}



