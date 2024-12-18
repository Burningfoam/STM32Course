/*******************************************************************************
* �ļ����ƣ�
* ����������1��CT117E_M4ʵ����ϴ��ڵĲ���
            2������HAL_UART_Receive_IT(&huart1, &Usart1_RxData_Temp, 1)�ڴ��ڳ�ʼ�����жϺ����б���Ҫ���ã�
* ����˵����
* Ӳ��������CT117E_M4
* ע�����
* ���߰汾��hufq/V1.0
*******************************************************************************/
#include "Bsp_Usart.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"

unsigned char Uart1_Rx_Buf[UART1RXBUF_LEN];     //���ջ���
unsigned char U1Rx_Len=0;                       //���ճ���
unsigned char Uart1_Tx_Buf[UART1RXBUF_LEN];     //���ͻ���
unsigned char U1Tx_Len=0;                       //���ͳ���


/****************************************************
��������:
��    ��: ���ڽ����жϵĻص�����������������stm32g4xx_hal_uart.c�ж���Ϊweak������
��ڲ���: ��
���ڲ���:
****************************************************/
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

/****************************************************
��������:
��    ��: ���ڷ��ͺ���
��ڲ���: bufpt���������飻buflen���������ݳ���
���ڲ���:
****************************************************/
void Uart1_Send(unsigned char *bufpt,unsigned char buflen)
{
    HAL_UART_Transmit(&huart1,bufpt,buflen,1000);	//���ͽ��յ�������
    while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
}
