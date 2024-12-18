#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "BSP_key.h"
#include "lcd.h"
#include "BSP_led.h"
#include "BSP_TEST.h"
#include "stdio.h"

unsigned char Lcd_Disp_String[20];
unsigned char Uart1_Tx_Buf[30];
unsigned char Uart1_Rx_Buf[30];
unsigned char Usart1_RxData_Temp=0;
unsigned char U1Rx_Len=0;

unsigned int TIM2CH2_PA1_Pulse = 1000;
unsigned int TIM2CH2_PA2_Duty = 50;
unsigned int TIM2CH2_PA2_Fre = 1000;
uint32_t PA1_2k_time;
uint32_t shanshuo_flag;
unsigned char FRE_2k_flag = 0;
unsigned char FRE_2k_end_flag = 0;
unsigned char number_1 = '@';
unsigned char number_2 = '@';
unsigned char number_3 = '@';
unsigned char number_1_i = 0;
unsigned char number_2_i = 0;
unsigned char number_3_i = 0;
unsigned char right_number_1 = '1';
unsigned char right_number_2 = '2';
unsigned char right_number_3 = '3';
unsigned char jiemian=0;

unsigned wrong_times = 0;
unsigned led2_flag = 0;
unsigned led2_end_flag = 0;
unsigned char mima[11]={'0','1','2','3','4','5','6','7','8','9','@'};
void jiemian_PSD()
{

	 LCD_DisplayStringLine(Line0,(unsigned char*)"                    ");
	 LCD_DisplayStringLine(Line1,(unsigned char*)"        PSD         ");
	 LCD_DisplayStringLine(Line2,(unsigned char*)"                    ");

	 sprintf((char*)Lcd_Disp_String,"     B1:%c           ",number_1);
	 LCD_DisplayStringLine(Line3,Lcd_Disp_String);

	 sprintf((char*)Lcd_Disp_String,"     B2:%c           ",number_2);
	 LCD_DisplayStringLine(Line4,Lcd_Disp_String); 

	 sprintf((char*)Lcd_Disp_String,"     B3:%c           ",number_3);
	 LCD_DisplayStringLine(Line5,Lcd_Disp_String);

	 LCD_DisplayStringLine(Line6,(unsigned char*)"                    ");
	 LCD_DisplayStringLine(Line7,(unsigned char*)"                    ");
	 LCD_DisplayStringLine(Line8,(unsigned char*)"                    ");
	 LCD_DisplayStringLine(Line9,(unsigned char*)"                    ");
}

void jiemian_STA()
{

	 LCD_DisplayStringLine(Line0,(unsigned char*)"                    ");
	 LCD_DisplayStringLine(Line1,(unsigned char*)"        STA         ");
	 LCD_DisplayStringLine(Line2,(unsigned char*)"                    ");
	 sprintf((char*)Lcd_Disp_String,"     F:%dHz           ",TIM2CH2_PA2_Fre);
	 LCD_DisplayStringLine(Line3,Lcd_Disp_String);
	 sprintf((char*)Lcd_Disp_String,"     D:%d%%          ",TIM2CH2_PA2_Duty);
	 LCD_DisplayStringLine(Line4,Lcd_Disp_String); 
	 LCD_DisplayStringLine(Line5,(unsigned char*)"                    ");
	 LCD_DisplayStringLine(Line6,(unsigned char*)"                    ");
	 LCD_DisplayStringLine(Line7,(unsigned char*)"                    ");
	 LCD_DisplayStringLine(Line8,(unsigned char*)"                    ");
	 LCD_DisplayStringLine(Line9,(unsigned char*)"                    ");
}

void Key_Proc()
{
	  unsigned char key_value=0;
	  static unsigned int key_keeptime=0;
	  static unsigned char last_key_value=0;
	  static unsigned char pwm_flag=0;
	  
    if(uwTick%20)
		   return;
		else
		{
		   key_value = key_scan();
			 switch(key_value)
			 {
				 case 1:
					     key_keeptime++;
				       {
								 if(key_keeptime==1)
								 {  
									 if(jiemian==0)
									 {
										 if(number_1=='@')
										 {
											 number_1_i = 0;
										 	 number_1 = mima[number_1_i];
										 }
										 else 
									     number_1 = mima[(++number_1_i)%10];
									    
									 }
 
								 }
					       
				       }	
				       break;
				 case 2:
					     key_keeptime++;
				       {
								 if(key_keeptime==1)
								 {
                   if(jiemian==0)
									 {
										 if(number_2=='@')
										 {
											 number_2_i = 0;
											 number_2 = mima[number_2_i];
										 }
										 else 
									     number_2 = mima[(++number_2_i)%10];
									    
									 }
								 }
					       
				       }	
				       break;			 
				 case 3:
					     key_keeptime++;
				       {
								 if(key_keeptime==1)
								 {
									 if(jiemian==0)
									 {
										 if(number_3=='@')
										 {
											 number_3_i = 0;
										   number_3 = mima[number_3_i];
										 }
										 else 
									     number_3 = mima[(++number_3_i)%10];
									    
									 }

								 }
					       
				       }	
				       break;
				 case 4:
					     key_keeptime++;
				       {
								 if(key_keeptime==1)
								 {
										if(jiemian==0)
										{
											if((number_1==right_number_1)&&(number_2==right_number_2)&&(number_3==right_number_3))
											{
												 jiemian = 1;
												 TIM2CH2_PA1_Pulse = 500;
                         TIM2CH2_PA2_Duty = 10;
                         TIM2CH2_PA2_Fre = 2000;
												 FRE_2k_flag = 1;
											}
											else
											{
											  number_1 = '@';
                        number_2 = '@';
                        number_3 = '@';
												
												wrong_times++;
												if(wrong_times >= 3)
													led2_flag = 1;
											
											}
											
										}

								 }
					       
				       }	
				       break;		
				 default:
               {
							   if((last_key_value>0)&&(key_value==0))
								 {
								    key_keeptime=0;
									 if(jiemian==0)//刷新界面
								     jiemian_PSD();
									 else
										 jiemian_STA();
								 }
							 }break;
			 }
			 last_key_value = key_value;
//		   sprintf((char *)Lcd_Disp_String, "  %d ",key_keeptime);
//			 LCD_DisplayStringLine(Line8, Lcd_Disp_String);
		}


}

void PWM_Proc()
{
    if(FRE_2k_flag == 1)
		{
			PA1_2k_time = uwTick;
			FRE_2k_flag = 0;
			FRE_2k_end_flag = 1;
			
			led_state = led_state | 0x01;//led1打开
	    led_control(led_state);
		}
		if((uwTick-PA1_2k_time>=5000)&&(FRE_2k_end_flag==1))
		{
			 FRE_2k_end_flag = 0;
		   TIM2CH2_PA1_Pulse = 1000;
       TIM2CH2_PA2_Duty = 50;
       TIM2CH2_PA2_Fre = 1000;
			
			 number_1 = '@';
       number_2 = '@';
       number_3 = '@';
			 jiemian = 0;
			 jiemian_PSD();
			
			 led_state = led_state & 0xFE; //led1关闭
	     led_control(led_state);
		}

}

void Uart_Proc()
{

	 static uint32_t  UART_tick=0;
	 unsigned char i;
   if(U1Rx_Len==0)
	 {
		 UART_tick = uwTick ;
		 return;
	 }
	 
	 if((uwTick-UART_tick)>50)
	 {
	   if(U1Rx_Len==7)
		 {
		    if((Uart1_Rx_Buf[0]==right_number_1)&&(Uart1_Rx_Buf[1]==right_number_2)&&(Uart1_Rx_Buf[2]==right_number_3)&&(Uart1_Rx_Buf[3]=='-'))
				{
				   right_number_1 = Uart1_Rx_Buf[4];
				   right_number_2 = Uart1_Rx_Buf[5];
					 right_number_3 = Uart1_Rx_Buf[6];
					 for(i=0;i<30;i++)
	               Uart1_Tx_Buf[i]=0;
//					 sprintf((char *)Uart1_Tx_Buf, " xiu gai cheng gong \r\n"); 
//           HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
				}

		 }


	 
		 
		 for(i=0;i<30;i++)
	     Uart1_Rx_Buf[i]=0;
		 
		 U1Rx_Len=0;
	 }


}

void Led_Proc()
{
	static uint32_t led2_time = 0;

	 if(led2_flag)
	 {
			led2_flag = 0;
			led2_end_flag = 1;
			led2_time = uwTick ;
	 }
	 if((uwTick - led2_time >=5000)&&(led2_end_flag == 1))
	 {
			led2_end_flag = 0;
			led_state = led_state & 0xfd;//关闭led2
			led_control(led_state);
	 }
	 else if((uwTick - led2_time <5000)&&(led2_end_flag == 1))
	 {
		 if((uwTick%100 == 0)&&(shanshuo_flag != uwTick))
		 {
				 shanshuo_flag = uwTick;
				 led_state = led_state ^ 0x02;
				 led_control(led_state);
			 
		 }
	 }


}





void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1)//如果是串口1
    {
        Uart1_Rx_Buf[U1Rx_Len]=Usart1_RxData_Temp;
        U1Rx_Len++;
        if(U1Rx_Len>(29)) //注意虽然开了UART1RXBUF_LEN（20）个字节缓冲，但是只能用到UART1RXBUF_LEN-1个
            U1Rx_Len=0;
        
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量,需要每次中断调用一下
        HAL_UART_Receive_IT(&huart1, &Usart1_RxData_Temp, 1);    //必须重新调用，因为该函数在接收一次后会关闭中断    
    }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    unsigned int  OC_Count=0;
    OC_Count= __HAL_TIM_GET_COUNTER(&htim2);      //取得当前定时器计数值
    if(htim->Instance == TIM2)
	  {
		    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
//			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1)+250);//2khz，每次加相同的值，占空比则为50%
            //在PA11输出周期10ms(频率100Hz)，占空比10%的脉冲
            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA , GPIO_PIN_1))   //TIM2_CH3:PA2  频率1k，占空比30%
            {
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,OC_Count+(TIM2CH2_PA1_Pulse-TIM2CH2_PA1_Pulse*TIM2CH2_PA2_Duty/100));
            }
            else
            {
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,OC_Count+TIM2CH2_PA1_Pulse*TIM2CH2_PA2_Duty/100);
            }
        }

	  }

}