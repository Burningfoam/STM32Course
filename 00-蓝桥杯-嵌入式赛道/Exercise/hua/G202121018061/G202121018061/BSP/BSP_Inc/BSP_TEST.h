#ifndef __BSP_TEST_H
#define __BSP_TEST_H


extern unsigned char Uart1_Tx_Buf[30];
extern unsigned char Usart1_RxData_Temp;

void jiemian_PSD(void);
void jiemian_STA(void);
void Key_Proc(void);
void PWM_Proc(void);
void Uart_Proc(void);
void Led_Proc(void);
#endif