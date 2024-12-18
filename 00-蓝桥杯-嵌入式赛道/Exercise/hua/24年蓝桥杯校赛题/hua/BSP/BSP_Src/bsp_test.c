#include "main.h"
#include "adc.h"
#include "gpio.h"
#include "bsp_test.h"
#include "bsp_adc.h"
#include "bsp_led.h"
#include "bsp_time.h"
#include "bsp_key.h"
#include "bsp_uart.h"
#include "i2c_hal.h"
#include "lcd.h"
#include "stdio.h"
#include "usart.h"
#include "tim.h"
#include "rtc.h"

RTC_TimeTypeDef rtc_time;
RTC_TimeTypeDef rtc_time2;
RTC_DateTypeDef rtc_date;

unsigned char lcd_string[21];
unsigned char uart_string[40];
unsigned char hour=10;
unsigned char minute=10;
unsigned char second=10;
unsigned int out_Fre=1000;
unsigned int in_Fre=0;
unsigned char out_Duty=50;
unsigned char in_Duty=0;
unsigned char State = 0;
unsigned char gaoliang = 0;
void LCD_DisplayStringLine_gaoliang(u8 Line, u8 *ptr, u8 position,u8 size)
{
    u32 i = 0;
    u16 refcolumn = 319;//319;

    while ((*ptr != 0) && (i < 20))	 //	20
    {
			  if((i>=position)&&(i<position+size))
				{
           LCD_SetBackColor(Magenta);
					 LCD_DisplayChar(Line, refcolumn, *ptr);
					 LCD_SetBackColor(Blue);
				}
				else
				{
					LCD_DisplayChar(Line, refcolumn, *ptr);
				}
        refcolumn -= 16;
        ptr++;
        i++;
    }
}

void lcd_disp()
{
	static uint32_t lcd_uwtick;
	if(uwTick-lcd_uwtick<50)
		return;
	lcd_uwtick = uwTick;
	

	in_Fre = in_frequent;
	in_Duty = in_dutyy;
	if(State!=2)
	{
			sprintf((char*)lcd_string," RTC:%02d:%02d:%02d     ",hour,minute,second);
			LCD_DisplayStringLine(Line0,lcd_string);
			
			sprintf((char*)lcd_string," Out freq:%04dHz     ",out_Fre);
			LCD_DisplayStringLine(Line1,lcd_string);
			
			sprintf((char*)lcd_string," Out duty:%02d%%     ",out_Duty);
			LCD_DisplayStringLine(Line2,lcd_string);
	}
	sprintf((char*)lcd_string," In freq:%dHz     ",in_Fre);
  LCD_DisplayStringLine(Line3,lcd_string);
	
	sprintf((char*)lcd_string," In duty:%02d%%     ",in_Duty);
  LCD_DisplayStringLine(Line4,lcd_string);
	

  LCD_DisplayStringLine(Line5,(u8*)"                    ");
	LCD_DisplayStringLine(Line6,(u8*)"                    ");
	
	if(State==0)
    sprintf((char*)lcd_string,"     State:run      ");
	else if(State==1)
		sprintf((char*)lcd_string,"     State:pause    ");
	else if(State==2)
		sprintf((char*)lcd_string,"     State:set      ");
	LCD_DisplayStringLine(Line7,lcd_string);
	
	LCD_DisplayStringLine(Line8,(u8*)"                    ");
	LCD_DisplayStringLine(Line9,(u8*)"                    ");
//	sprintf((char*)lcd_string," %d   %d  %d    ",in_rise_time,in_fall_time,in_frequent);
//  LCD_DisplayStringLine(Line9,lcd_string);
	
	if(State==2)//在设置状态下才高亮
	{
			switch(gaoliang)
			{
				case 0:sprintf((char*)lcd_string," RTC:%02d:%02d:%02d     ",hour,minute,second);
			         LCD_DisplayStringLine_gaoliang(Line0,lcd_string,5,2);
							 sprintf((char*)lcd_string," Out freq:%04dHz     ",out_Fre);
			         LCD_DisplayStringLine(Line1,lcd_string);
							 sprintf((char*)lcd_string," Out duty:%02d%%     ",out_Duty);
							 LCD_DisplayStringLine(Line2,lcd_string);
							 break;	
				case 1:sprintf((char*)lcd_string," RTC:%02d:%02d:%02d     ",hour,minute,second);
			         LCD_DisplayStringLine_gaoliang(Line0,lcd_string,8,2);
				       sprintf((char*)lcd_string," Out freq:%04dHz     ",out_Fre);
			         LCD_DisplayStringLine(Line1,lcd_string);
							 sprintf((char*)lcd_string," Out duty:%02d%%     ",out_Duty);
							 LCD_DisplayStringLine(Line2,lcd_string);
							 break;
				case 2:sprintf((char*)lcd_string," RTC:%02d:%02d:%02d     ",hour,minute,second);
			         LCD_DisplayStringLine_gaoliang(Line0,lcd_string,11,2);
				       sprintf((char*)lcd_string," Out freq:%04dHz     ",out_Fre);
			         LCD_DisplayStringLine(Line1,lcd_string);
							 sprintf((char*)lcd_string," Out duty:%02d%%     ",out_Duty);
							 LCD_DisplayStringLine(Line2,lcd_string);
							 break;
				case 3:sprintf((char*)lcd_string," RTC:%02d:%02d:%02d     ",hour,minute,second);
			         LCD_DisplayStringLine(Line0,lcd_string);
					     sprintf((char*)lcd_string," Out freq:%04dHz     ",out_Fre);
			         LCD_DisplayStringLine_gaoliang(Line1,lcd_string,10,4);
							 sprintf((char*)lcd_string," Out duty:%02d%%     ",out_Duty);
							 LCD_DisplayStringLine(Line2,lcd_string);
							 break;
				case 4:sprintf((char*)lcd_string," RTC:%02d:%02d:%02d     ",hour,minute,second);
			         LCD_DisplayStringLine(Line0,lcd_string);
			         sprintf((char*)lcd_string," Out freq:%04dHz     ",out_Fre);
			         LCD_DisplayStringLine(Line1,lcd_string);
					     sprintf((char*)lcd_string," Out duty:%02d%%     ",out_Duty);
			         LCD_DisplayStringLine_gaoliang(Line2,lcd_string,10,2);
							 break;
				default: ;break;
			}
  }
}



void store_value()
{
   eerom_white(0x00,hour);
   HAL_Delay(10);
   eerom_white(0x01,minute);
   HAL_Delay(10);
	 eerom_white(0x02,second);
   HAL_Delay(10);
}



void key_proc()
{
   static uint32_t key_uwtick;
   static uint16_t key_keeptime=0;
   static uint16_t last_key_value;
	 unsigned char key_value; 
	 

	 if(uwTick-key_uwtick<50)
		 return;
	 key_uwtick = uwTick;
	
	 key_value = key_scan();
	 switch(key_value)
	 {
	   case 1:
			      key_keeptime++;
		        break;
	   case 2:
			      key_keeptime++;
		        break;
		 case 3:
			      key_keeptime++;
		        if(key_keeptime>10)
						{
							 switch(gaoliang)
							 {
										case 0:hour=(hour+1)%24;
													 break;	
										case 1:minute=(minute+1)%60;
													 break;
										case 2:second=(second+1)%60;
													 break;
										case 3:out_Fre += 1000;
								           if(out_Fre==5000)
														 out_Fre = 1000;
													 break;
										case 4:out_Duty += 10;
								           if(out_Duty==90)
														 out_Duty = 20;
													 break;
										default: ;break;
							 }
						
						}
		        break;
		 case 4:
			      key_keeptime++;
		        break;
	   default:
			      switch(last_key_value)
						{ 
							case 1:if(key_keeptime>10)
							       {
											 if(State == 1)
											 {
											  rtc_time.Hours = eerom_read(0x00);
											  rtc_time.Minutes = eerom_read(0x01);
											  rtc_time.Seconds  = eerom_read(0x02);
												__HAL_RCC_RTC_ENABLE();
                        HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
												__HAL_RCC_RTC_DISABLE();
											 }
										 }
										 else
										 {
                        if(State==1)
												{
													__HAL_RCC_RTC_ENABLE();
													State = 0;
												}
												else if(State==0)
												{
													__HAL_RCC_RTC_DISABLE();
												  State = 1;
												}
										 }
								     
							       break;
						  case 2:if(key_keeptime>10)
							       {
											 if(State==1)
												 State = 2;
											 else if(State==2)
											 {
												  State = 1;

											 }
													 
										 }
										 else
										 {
										    if(State==2)
												{
												  gaoliang = (gaoliang+1)%5;//切换高亮
												
												}
										 }
								
							       break;
						  case 3:if(key_keeptime>10)
							       {
										    
										 }
										 else
										 {
										   if(State==2)
											 {
											   switch(gaoliang)
												 {
															case 0:hour=(hour+1)%24;
																		 break;	
															case 1:minute=(minute+1)%60;
																		 break;
															case 2:second=(second+1)%60;
																		 break;
															case 3:out_Fre += 1000;
								                     if(out_Fre==5000)
														            out_Fre = 1000;
													           break;
										          case 4:out_Duty += 10;
								                     if(out_Duty==90)
														            out_Duty = 20;
													           break;
															default: ;break;
												 }
												 rtc_time.Hours = hour;
		                     rtc_time.Minutes = minute;
		                     rtc_time.Seconds = second;
												 __HAL_RCC_RTC_ENABLE();
												  HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
												 __HAL_RCC_RTC_DISABLE();
											 }
										 }
								
							       break;
							case 4:if(key_keeptime>10)
							       {
										     if(State==1)
												 {
												   store_value();
												 }
										 }
										 else
										 {
										 
										 }
								
							       break;
							default:
								     key_keeptime = 0;
							       break;
						
						}
	 
	 }
	 last_key_value = key_value;


}
	


void uart_proc()
{
  static uint32_t uart_uwtick;
	static uint32_t rx_uart_uwtick;
	static float adc_value;
	unsigned char i=0;
	unsigned char mingling[] = "Volt analysis";
	if(uwTick-uart_uwtick<2000)
	{

	}
	else
  {
	 uart_uwtick = uwTick;
	 adc_value = adc_read();
		if(State == 0)
		{
	    sprintf((char*)uart_string,"Running Volt=%.2fV,Time=%2d.%2d.%2d \r\n",adc_value,hour,minute,second);
	    HAL_UART_Transmit(&huart1,uart_string,40,1000);
		}
  }
	
	if(rx_count==0)
	{
	   rx_uart_uwtick = uwTick;
	}
	if(uwTick - rx_uart_uwtick >100)
	{

	  rx_count = 0;
		if(State==1)
		{
				for(i=0;i<13;i++)
				{
					if(mingling[i]!=usart_rx_string[i])
					{
						break;
					}
					
				}
				if(i==13)
				{
					 sprintf((char*)uart_string,"Running Volt=%.2fV,Time=%2d.%2d.%2d \r\n",adc_value,hour,minute,second);
					 HAL_UART_Transmit(&huart1,uart_string,40,1000);
				}
	  }
	}
}


void led_proc()
{
	static u8 liushui_flag = 0;
	static uint32_t led_uwtick;
	if(uwTick - led_uwtick < 50)
	  return;
	
	led_uwtick = uwTick;
	
	if(State==0)
	{
		liushui_flag = ((unsigned int)uwTick/500)%8;
		led_state = 0x01<<liushui_flag;
    led_control(led_state);
	}
	else if(State==1)
	{
	  led_state = 0x00;
    led_control(led_state);
	}
	else if(State==2)
	{
	  led_state = 0xff;
    led_control(led_state);
	}
}
	


void RTC_Process(void)
{
	static uint32_t rtc_uwtick;
	if(uwTick - rtc_uwtick < 100)
	  return;
	rtc_uwtick = uwTick;
	
	
	HAL_RTC_GetTime(&hrtc,&rtc_time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&rtc_date,RTC_FORMAT_BIN);//必须同时读取时间和日期，否则rtc不会计时
	if(State!=2)
	{
		hour=rtc_time.Hours ;
		minute=rtc_time.Minutes ;
		second=rtc_time.Seconds ;
		rtc_time2.Hours = rtc_time.Hours+2;
		rtc_time2.Minutes = rtc_time.Minutes;
		rtc_time2.Seconds = rtc_time.Seconds;
	}
}



