#ifndef __BSP_USART_H
#define __BSP_USART_H

#define UART1RXBUF_LEN  20
#define UART1TXBUF_LEN  20


extern unsigned char Uart1_Rx_Buf[UART1RXBUF_LEN];     //接收缓冲
extern unsigned char U1Rx_Len;                       //接收长度
extern unsigned char Uart1_Tx_Buf[UART1RXBUF_LEN];     //发送缓冲
extern unsigned char U1Tx_Len;                       //发送长度

extern void Uart1_Send(unsigned char *bufpt,unsigned char buflen);

#endif
