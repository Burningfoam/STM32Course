#include "Bsp_Test.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "gpio.h"
#include "Bsp_LED.h"
#include "Bsp_Button.h"
#include "lcd.h"
#include "tim.h"
#include "Bsp_Usart.h"

unsigned char su=0;
unsigned char Lcd_Disp_String[25];
int p1=-1;
int p2=-1;
int p3=-1;
unsigned char sp1=1;
unsigned char sp2=2;
unsigned char sp3=3;
unsigned char error_count=0;
unsigned char ld2flag=0;
unsigned char ld2openflag=0;
uint32_t blinkflag;

unsigned int TIM2_PWM_Period;        
unsigned int TIM2CH2_PA1_Duty;      
unsigned int tim3RiseEdge=0;
unsigned int tim3FallEdge=0;
float tim3Duty = 0;

void LCD_TestTask(void)
{
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
		if(su==0)
		{
			LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line1, (uint8_t *)"        PSD         ");
			LCD_DisplayStringLine(Line2, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line3, (uint8_t *)"   B1:@             ");
			LCD_DisplayStringLine(Line4, (uint8_t *)"   B2:@             ");
			LCD_DisplayStringLine(Line5, (uint8_t *)"   B3:@             ");
			LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
		}
		else if(su==1)
		{
			LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line1, (uint8_t *)"        STA         ");
			LCD_DisplayStringLine(Line2, (uint8_t *)"                    ");
			sprintf((char *)Lcd_Disp_String,"   F:%dHz             ",1000000/tim3RiseEdge);
			LCD_DisplayStringLine(Line3, (uint8_t *)Lcd_Disp_String);
			sprintf((char *)Lcd_Disp_String,"   D:%.2f%%           ",tim3Duty*100);
			LCD_DisplayStringLine(Line4, (uint8_t *)Lcd_Disp_String);
			LCD_DisplayStringLine(Line5, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
		}
}

void KeyProcess(void)
{    

		static uint32_t KeyScan_uwTick = 0;             //控制KeyProcess的执行速度
    static uint16_t Key_KeepTime=0;                 //记录按键按下持续的时间 
    static uint8_t keyvalue_Last=0;                 //记录上次按键键值
    uint8_t keyvalue_Current;
    if((uwTick -  KeyScan_uwTick)<50)	return;//减速函数    //uwTick为1ms的计数器 ,50ms扫描一次        
	KeyScan_uwTick = uwTick;
    
    keyvalue_Current=Key_scan();           
    switch(keyvalue_Current)
    {
        case B1_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //第一次检测到按下
            {
                if(su==0)
								{
									if(p1==9)
										p1=0;
									else
										p1++;
									sprintf((char *)Lcd_Disp_String,"   B1:%d           ",p1);
									LCD_DisplayStringLine(Line3, (uint8_t *)Lcd_Disp_String);
									
								}
            }
            break;
        case B2_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //第一次按下该键
            {
                if(su==0)
								{
									if(p2==9)
										p2=0;
									else
										p2++;
									sprintf((char *)Lcd_Disp_String,"   B2:%d           ",p2);
									LCD_DisplayStringLine(Line4, (uint8_t *)Lcd_Disp_String);
								}
            }
            break;
        case B3_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //第一次按下该键
            {
                if(su==0)
								{
									if(p3==9)
										p3=0;
									else
										p3++;
									sprintf((char *)Lcd_Disp_String,"   B3:%d           ",p3);
									LCD_DisplayStringLine(Line5, (uint8_t *)Lcd_Disp_String);
								}
            }
            break;
        case B4_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //第一次按下该键
            {
                if(p1==sp1 && p2==sp2 && p3==sp3)
								{
									su=1;
									error_count=0;
									LED_State=LED_State|0x01;
									LED_Control(LED_State);
									
									TIM2_PWM_Period=500;
									TIM2CH2_PA1_Duty=10;
									__HAL_TIM_SET_AUTORELOAD(&htim2,TIM2_PWM_Period); //修改周期
									__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,TIM2_PWM_Period*TIM2CH2_PA1_Duty/100); //修改占空比
									
									LCD_TestTask();
									
									HAL_Delay(5000);
									LED_State=LED_State&0xFE;
									LED_Control(LED_State);

									TIM2_PWM_Period=1000;
									TIM2CH2_PA1_Duty=50;
									__HAL_TIM_SET_AUTORELOAD(&htim2,TIM2_PWM_Period); //修改周期
									__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,TIM2_PWM_Period*TIM2CH2_PA1_Duty/100); //修改占空比
									
									su=0;
									LCD_TestTask();
									
									p1=-1;
									p2=-1;
									p3=-1;
								}
								else
								{
									LCD_TestTask();
									error_count++;
									p1=-1;
									p2=-1;
									p3=-1;
									if(error_count==3 || error_count>3)
										ld2flag=1;
								}
            }
            break;
        default:
            if(Key_KeepTime>0)      //有键按下，现在松开了
            {
                switch(keyvalue_Last)
                {
                    case B1_PRESS:                      //该键松开
                        if(Key_KeepTime<10)             // 500ms以内为短键          
                        {
                            
                        }
                        else                            // 500ms以上为短键 
                        {
                            
                        }
                        break;
                    case B2_PRESS:                      //该键松开
                        if(Key_KeepTime<20)             // 500ms以内为短键          
                        {
                            ;
                        }
                        else                            // 500ms以上为短键 
                        {
                            ;
                        }
                        break;
                }
            }
            Key_KeepTime=0;
            break;
    }
    keyvalue_Last=keyvalue_Current;
    
}

void  Uart1_RX_Pro(void)
{
		unsigned char np1;
		unsigned char np2;
		unsigned char np3;
	
		unsigned char op1;
		unsigned char op2;
		unsigned char op3;
	
		static unsigned int Usart_uwTick=0;

    if(U1Rx_Len==0)     
    {
        Usart_uwTick = uwTick;          
        return ;
    }

    if((uwTick-Usart_uwTick)>50)
    {
        Usart_uwTick = uwTick;
       
				op1=Uart1_Rx_Buf[0]-48;
				op2=Uart1_Rx_Buf[1]-48;
				op3=Uart1_Rx_Buf[2]-48;
			
				np1=Uart1_Rx_Buf[4]-48;
				np2=Uart1_Rx_Buf[5]-48;
				np3=Uart1_Rx_Buf[6]-48;
			
			if(op1==sp1 && op2==sp2 && op3==sp3)
			{
				sp1=np1;
				sp2=np2;
				sp3=np3;
				
				sprintf((char *)Uart1_Tx_Buf,"NewPassword:%d%d%d\n",sp1,sp2,sp3);
				Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
				
			}
			else
			{
				sprintf((char *)Uart1_Tx_Buf,"ERROR\n");
				Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
				
			}			
				memset(Uart1_Rx_Buf, 0, U1Rx_Len);
        U1Rx_Len=0;     
    }
}

void blink_Proc()
{
	static uint32_t led2_time = 0;

	 if(ld2flag==1)
	 {
			ld2flag = 0;
			ld2openflag = 1;
			led2_time = uwTick ;
	 }
	 if((uwTick - led2_time >=5000)&&(ld2openflag == 1))
	 {
			ld2openflag = 0;
			LED_State=LED_State&0xFD;
			LED_Control(LED_State);
	 }
	 else if((uwTick - led2_time <5000)&&(ld2openflag == 1))
	 {
		 if(((uwTick - led2_time) %100 == 0)&&(blinkflag != uwTick))
		 {
				 blinkflag = uwTick;
				 LED_State=LED_State^0x02;
				 LED_Control(LED_State);
			 
		 }
	 }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
		{
			
			tim3RiseEdge = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
			tim3Duty = (float)tim3FallEdge/tim3RiseEdge;
		}
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
		{
			tim3FallEdge = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);
		}
	}
}



