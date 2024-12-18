#include "main.h"
#include "adc.h"
#include "Bsp_uart.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		rx_string[rx_count++]=rx_temp;
		if(rx_count==50)
			rx_count=0;
		HAL_UART_Receive_IT(&huart1,&rx_temp,1);
		
		
		
	}
	
	
	
	
}



