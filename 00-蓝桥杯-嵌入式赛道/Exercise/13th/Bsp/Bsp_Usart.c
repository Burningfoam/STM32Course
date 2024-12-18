#include "Bsp_Usart.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"

unsigned char Uart1_Rx_Buf[UART1RXBUF_LEN];     //���ջ���
unsigned char U1Rx_Len=0;                       //���ճ���
unsigned char Uart1_Tx_Buf[UART1RXBUF_LEN];     //���ͻ���
unsigned char U1Tx_Len=0;                       //���ͳ���

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1)//����Ǵ���1
    {
        Uart1_Rx_Buf[U1Rx_Len]=Usart1_RxData_Temp;
        U1Rx_Len++;
        if(U1Rx_Len>(UART1RXBUF_LEN-1)) //ע����Ȼ����UART1RXBUF_LEN��20�����ֽڻ��壬����ֻ���õ�UART1RXBUF_LEN-1��
            U1Rx_Len=0;
        
        //�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������,��Ҫÿ���жϵ���һ��
        HAL_UART_Receive_IT(&huart1, &Usart1_RxData_Temp, 1);    //�������µ��ã���Ϊ�ú����ڽ���һ�κ��ر��ж�    
    }
}

void Uart1_Send(unsigned char *bufpt,unsigned char buflen)
{
    HAL_UART_Transmit(&huart1,bufpt,buflen,1000);	//���ͽ��յ�������
    while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
}



