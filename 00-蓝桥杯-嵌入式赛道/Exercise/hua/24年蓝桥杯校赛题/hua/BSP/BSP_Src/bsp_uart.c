#include "bsp_uart.h"
#include "main.h"
#include "usart.h"

void uart_send(unsigned char*bufpt,unsigned char buflen)
{
   HAL_UART_Transmit(&huart1,bufpt,buflen,1000);
	 while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);
}





	

