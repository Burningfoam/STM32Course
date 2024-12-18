#ifndef __BSP_USART_H
#define __BSP_USART_H

#define UART1RXBUF_LEN  20
#define UART1TXBUF_LEN  20


extern unsigned char Uart1_Rx_Buf[UART1RXBUF_LEN];     //���ջ���
extern unsigned char U1Rx_Len;                       //���ճ���
extern unsigned char Uart1_Tx_Buf[UART1RXBUF_LEN];     //���ͻ���
extern unsigned char U1Tx_Len;                       //���ͳ���

extern void Uart1_Send(unsigned char *bufpt,unsigned char buflen);

#endif
