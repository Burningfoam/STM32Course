#include "Bsp_Test.h"
#include "main.h"
#include "gpio.h"
#include "stdio.h"
#include "string.h"
#include "Bsp_LED.h"
#include "lcd.h"
#include "rtc.h"
#include "Bsp_tim.h"
#include "Bsp_key.h"
#include "tim.h"
#include "i2c_hal.h"
#include "Bsp_adc.h"
#include "usart.h"

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;

unsigned char lcd_string[20];
unsigned char uart_string[40];
unsigned char state=1;
unsigned char hour;
unsigned char minute;
unsigned char second;
unsigned char flag_hi=1;

void led_proc()
{
	static unsigned char liushui_flag = 0;
	static uint32_t led_uwtick;
	if(uwTick - led_uwtick < 50)
	  return;
	
	led_uwtick = uwTick;
	
	liushui_flag = ((unsigned int)uwTick/500)%8;
	led_state = 0x01<<liushui_flag;
	led_control(led_state);
	

}

void LCD_DisplayStringLine_highlight(u8 Line, u8 *ptr,u8 position,u8 size)
{
    u32 i = 0;
    u16 refcolumn = 319;//319;

    while ((*ptr != 0) && (i < 20))	 //	20
    {
        if((i>=position)&&(i<position+size))
				{
					LCD_SetBackColor(Magenta);
					LCD_DisplayChar(Line, refcolumn, *ptr);
					LCD_SetBackColor(Black);
				}
				else
					LCD_DisplayChar(Line, refcolumn, *ptr);
        
				refcolumn -= 16;
        ptr++;
        i++;
    }
}

void store()
{
   eeprom_write(0x00,rtc_time.Hours);
   HAL_Delay(10);
   eeprom_write(0x01,rtc_time.Minutes);
   HAL_Delay(10);
	 eeprom_write(0x02,rtc_time.Seconds);
   HAL_Delay(10);
}

void lcd_proc()
{
	static uint32_t lcd_uwtick;
	if(uwTick-lcd_uwtick<60)
		return;
	lcd_uwtick = uwTick;
	

	if(state==1)
	{
		sprintf((char*)lcd_string,"RTC:%02d:%02d:%02d     ",rtc_time.Hours,rtc_time.Minutes ,rtc_time.Seconds);
		LCD_DisplayStringLine(Line0,lcd_string);
		
		sprintf((char*)lcd_string,"Out freq:%04d  ",out_freq);
		LCD_DisplayStringLine(Line1,lcd_string);
		
		sprintf((char*)lcd_string,"Out duty:%02d  ",out_duty);
		LCD_DisplayStringLine(Line2,lcd_string);
		
		sprintf((char*)lcd_string,"IN freq:%04d  ",in_freq);
		LCD_DisplayStringLine(Line3,lcd_string);
		
		sprintf((char*)lcd_string,"IN duty:%02d  ",in_duty);
		LCD_DisplayStringLine(Line4,lcd_string);

		sprintf((char*)lcd_string,"     State:run   ");
		LCD_DisplayStringLine(Line5,lcd_string);
	}
	else if(state==2)
	{
		sprintf((char*)lcd_string,"RTC:%02d:%02d:%02d      ",rtc_time.Hours,rtc_time.Minutes ,rtc_time.Seconds);
		LCD_DisplayStringLine(Line0,lcd_string);
		
		sprintf((char*)lcd_string,"Out freq:%04d  ",out_freq);
		LCD_DisplayStringLine(Line1,lcd_string);
		
		sprintf((char*)lcd_string,"Out duty:%02d  ",out_duty);
		LCD_DisplayStringLine(Line2,lcd_string);
		
		sprintf((char*)lcd_string,"IN freq:%04d  ",in_freq);
		LCD_DisplayStringLine(Line3,lcd_string);
		
		sprintf((char*)lcd_string,"IN duty:%02d  ",in_duty);
		LCD_DisplayStringLine(Line4,lcd_string);

		sprintf((char*)lcd_string,"     State:pause   ");
		LCD_DisplayStringLine(Line5,lcd_string);
	}
	else if(state==3)
	{
		sprintf((char*)lcd_string,"IN freq:%04d  ",in_freq);
		LCD_DisplayStringLine(Line3,lcd_string);
		
		sprintf((char*)lcd_string,"IN duty:%02d  ",in_duty);
		LCD_DisplayStringLine(Line4,lcd_string);

		sprintf((char*)lcd_string,"     State:set   ");
		LCD_DisplayStringLine(Line5,lcd_string);
		
		switch(flag_hi)
		{
			case 1:
				sprintf((char*)lcd_string,"RTC:%02d:%02d:%02d  ",hour,minute,second);
				LCD_DisplayStringLine_highlight(Line0,lcd_string,4,2);
				
				sprintf((char*)lcd_string,"Out freq:%04d  ",out_freq);
				LCD_DisplayStringLine(Line1,lcd_string);
				
				sprintf((char*)lcd_string,"Out duty:%02d  ",out_duty);
				LCD_DisplayStringLine(Line2,lcd_string);
				break;
			case 2:
				sprintf((char*)lcd_string,"RTC:%02d:%02d:%02d  ",hour,minute,second);
				LCD_DisplayStringLine_highlight(Line0,lcd_string,7,2);
				
				sprintf((char*)lcd_string,"Out freq:%04d  ",out_freq);
				LCD_DisplayStringLine(Line1,lcd_string);
				
				sprintf((char*)lcd_string,"Out duty:%02d  ",out_duty);
				LCD_DisplayStringLine(Line2,lcd_string);
				break;
			case 3:
				sprintf((char*)lcd_string,"RTC:%02d:%02d:%02d  ",hour,minute,second);
				LCD_DisplayStringLine_highlight(Line0,lcd_string,10,2);
				
				sprintf((char*)lcd_string,"Out freq:%04d  ",out_freq);
				LCD_DisplayStringLine(Line1,lcd_string);
				
				sprintf((char*)lcd_string,"Out duty:%02d  ",out_duty);
				LCD_DisplayStringLine(Line2,lcd_string);
				break;
			case 4:
				sprintf((char*)lcd_string,"RTC:%02d:%02d:%02d  ",hour,minute,second);
				LCD_DisplayStringLine(Line0,lcd_string);
				
				sprintf((char*)lcd_string,"Out freq:%04d  ",out_freq);
				LCD_DisplayStringLine_highlight(Line1,lcd_string,9,4);
				
				sprintf((char*)lcd_string,"Out duty:%02d  ",out_duty);
				LCD_DisplayStringLine(Line2,lcd_string);
				break;
			case 5:
				sprintf((char*)lcd_string,"RTC:%02d:%02d:%02d  ",hour,minute,second);
				LCD_DisplayStringLine(Line0,lcd_string);
				
				sprintf((char*)lcd_string,"Out freq:%04d  ",out_freq);
				LCD_DisplayStringLine(Line1,lcd_string);
				
				sprintf((char*)lcd_string,"Out duty:%02d  ",out_duty);
				LCD_DisplayStringLine_highlight(Line2,lcd_string,9,2);
				break;
			
			
		}
		
		
	}
	
}

void key_proc()
{
	static uint32_t keyscan_uwtick;
	static uint16_t kktime;
	static uint8_t keyvalue_last;
	unsigned char keyvalue=0;
	
	if((uwTick-keyscan_uwtick)<50) return;
	keyscan_uwtick=uwTick;
	
	keyvalue=key_scan();
	
	switch(keyvalue)
	{
		case 1:
			kktime++;
			break;
		case 2:
			kktime++;
			if(kktime==1)
			{
				if(state==3)
				{
					flag_hi++;
					if(flag_hi==6)
						flag_hi=1;
				}	
			}
			break;
		case 3:
			kktime++;
			if(kktime==1|| kktime%10==0)
			{
				if(flag_hi==1)
				{
					rtc_time.Hours++;
					if(rtc_time.Hours==24)
						rtc_time.Hours=0;
				}
				else if(flag_hi==2)
				{
					rtc_time.Minutes++;
					if(rtc_time.Minutes==60)
						rtc_time.Minutes=0;
				}
				else if(flag_hi==3)
				{
					rtc_time.Seconds++;
					if(rtc_time.Seconds==60)
						rtc_time.Seconds=0;
				}
				else if(flag_hi==4)
				{
					out_freq=out_freq+1000;
					if(out_freq==6000)
						out_freq=1000;
				}
				else if(flag_hi==5)
				{
					out_duty=out_duty+10;
					if(out_duty==90)
						out_duty=20;
				}
				__HAL_RCC_RTC_ENABLE();
			  HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
				__HAL_RCC_RTC_DISABLE();
			}
			break;
		case 4:
			kktime++;
			break;
		default:
			if(kktime>0)
			{
				switch(keyvalue_last)
				{
					case 1:
						if(kktime>10)
						{
							if(state==2)
							{
								rtc_time.Hours=eeprom_read(0x00);
								rtc_time.Minutes=eeprom_read(0x01);
								rtc_time.Seconds=eeprom_read(0x02);
								__HAL_RCC_RTC_ENABLE();
								HAL_RTC_SetTime(&hrtc,&rtc_time,RTC_FORMAT_BIN);
								__HAL_RCC_RTC_DISABLE();
							}
						}
						else if(kktime<10)
						{
							if(state==1)
							{
								__HAL_RCC_RTC_DISABLE();
								state=2;
							}
							else if(state==2)
							{
								__HAL_RCC_RTC_ENABLE();
								state=1;
							}		
						}
						break;
					case 2:
						if(kktime>10)
						{
							if(state==2)
							{
								state=3;
								flag_hi=1;

							}
							else if(state==3)
							{
								state=2;
							}
							
						}
						break;
					case 4:
						if(kktime>10)
						{
							if(state==2)
								store();
						}
						break;
				}
			}
			kktime=0;
			break;
	}
	keyvalue_last=keyvalue;
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
		if(state == 1)
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
		if(state==2)
		{
				for(i=0;i<13;i++)
				{
					if(mingling[i]!=uart_rx_string[i])
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

void rtc_proc()
{
	static uint32_t rtc_uwtick;
	if(uwTick-rtc_uwtick<100)
		return;
	rtc_uwtick=uwTick;
	
	HAL_RTC_GetTime(&hrtc,&rtc_time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&rtc_date,RTC_FORMAT_BIN);//必须同时读取时间和日期，否则rtc不会计时
	
	hour=rtc_time.Hours;
	minute=rtc_time.Minutes;
	second=rtc_time.Seconds;
	
	
}





