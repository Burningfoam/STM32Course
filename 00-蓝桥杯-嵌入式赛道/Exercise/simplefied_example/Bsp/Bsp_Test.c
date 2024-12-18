#include "main.h"
#include "Bsp_Test.h"
#include "Bsp_uart.h"
#include "stdio.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "lcd.h"
#include "Bsp_tim.h"
unsigned char tx_string[50];
unsigned char mingling[10];
unsigned char lcd_string[20];

void lcd_proc()
{
	static uint32_t lcd_uwtick;
	if(uwTick-lcd_uwtick<60)
		return;
	lcd_uwtick = uwTick;
	
		
		sprintf((char*)lcd_string,"IN freq:%04d  ",in_freq);
		LCD_DisplayStringLine(Line3,lcd_string);
		
		sprintf((char*)lcd_string,"IN duty:%02d  ",in_duty);
		LCD_DisplayStringLine(Line4,lcd_string);

		sprintf((char*)lcd_string,"     State:run   ");
		LCD_DisplayStringLine(Line5,lcd_string);
	
}


void uart_proc()
{
	static uint32_t tx_uwtick;
	static uint32_t rx_uwtick;
	unsigned char i;
	
	
	if(uwTick- tx_uwtick<2000)
	{

	}
	else
	{
		tx_uwtick=uwTick;
		
		sprintf((char*)tx_string," dam:456abc \r\n");
		HAL_UART_Transmit(&huart1,tx_string,40,1000);
		
	}
	
	if(uwTick-rx_uwtick<100)
	{
		
	}
	else
	{
		if(rx_count==0)
			return;
		
		for(i=0;i<4;i++)
		{
			mingling[i]=rx_string[i];

		}

		if(mingling[0]=='1')
		{
			sprintf((char*)tx_string," dam:yes  abc \r\n");
			HAL_UART_Transmit(&huart1,tx_string,40,1000);
		}
		else if(mingling[0]=='4')
		{
			sprintf((char*)tx_string," dam:noo  abc \r\n");
			HAL_UART_Transmit(&huart1,tx_string,40,1000);
		}

		rx_count=0;
		
	}
	
}



