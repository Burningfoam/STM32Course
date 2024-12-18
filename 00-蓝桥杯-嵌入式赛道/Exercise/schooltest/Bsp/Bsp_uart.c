#include "Bsp_uart.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance == USART1)
	 {
	    uart_rx_string[rx_count++] = uart_rx_temp;
	    if(rx_count == 49)
				rx_count = 0;
	    HAL_UART_Receive_IT(&huart1,&uart_rx_temp,1);

	 }
	
	
	
	
}





