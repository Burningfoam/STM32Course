#include "Test.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
#include "gpio.h"
#include "LED.h"
#include "Bsp_Button.h"
#include "lcd.h"
#include "Bsp_Usart.h"
#include "Bsp_tim.h"
#include "tim.h"

unsigned char Lcd_Disp_String[25];
unsigned char su=0;
unsigned char year=24;
unsigned char month=1;
unsigned char day=1;

//unsigned int TIM15CH1_PA2_Pulse = 200;
//unsigned int TIM15CH1_PA2_Duty = 20;
//unsigned int TIM15CH1_PA2_Fre = 1000;
//unsigned int TIM15_PWM_Period=1000;

unsigned int tim3RiseEdge=0;
unsigned int tim3FallEdge=0;
float tim3Duty = 0;

void BSP_LED_TestTask(void)
{
    unsigned char i=0;
    unsigned char ledcode=0x01;
    LED_State=0xff;
    LED_Control(LED_State);
    HAL_Delay(2000);
    LED_State=0x0;
    LED_Control(LED_State);
    HAL_Delay(2000);

    //流水灯
    LED_State=0x01;
    for(i=0; i<8; i++)
    {
        LED_Control(LED_State);
        HAL_Delay(500);
        LED_State=LED_State<<1;
    }
    //单个轮流闪烁
    for(i=0; i<8; i++)
    {
        LED_State=LED_State^ledcode;
        LED_Control(LED_State);
        HAL_Delay(1000);
        LED_State=LED_State^ledcode;
        LED_Control(LED_State);
        HAL_Delay(1000);
        ledcode=ledcode<<1;
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
	
    static unsigned char flag=0;
	
    keyvalue_Current=Key_scan();           
    switch(keyvalue_Current)
    {
        case B1_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1 && flag==0)                 //第一次按下该键
            {
                LED_State=LED_State|0x01;
                LED_Control(LED_State);
            }
						else if(Key_KeepTime>20 && flag==0)
						{
							if(Key_KeepTime%6==0)
								LED_State=LED_State^0x01;
								LED_Control(LED_State);
						}
            break;
        case B2_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //第一次按下该键
            {
							if(flag==0)
              {  
								LED_State=LED_State|0x02;
                LED_Control(LED_State);
								flag=1;
							}
							else
							{
								LED_State=LED_State&0xFC;
                LED_Control(LED_State);
								flag=0;
								
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
                            LED_State=LED_State&0xFE;
														LED_Control(LED_State);
                        }
                        else                            // 500ms以上为短键 
                        {
                            ;
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
                sprintf((char *)Lcd_Disp_String," ButtonValue=%2d",keyvalue_Last);
                LCD_DisplayStringLine(Line3, (uint8_t *)Lcd_Disp_String);
								
            }
            Key_KeepTime=0;
            break;
    }
    keyvalue_Last=keyvalue_Current;
    
}

void KeyProcess2(void)
{    
    static uint32_t KeyScan_uwTick = 0;             //控制KeyProcess的执行速度
    static uint16_t Key_KeepTime=0;                 //记录按键按下持续的时间 
    static uint8_t keyvalue_Last=0;                 //记录上次按键键值
    uint8_t keyvalue_Current;
    if((uwTick -  KeyScan_uwTick)<50)	return;//减速函数    //uwTick为1ms的计数器 ,50ms扫描一次        
		KeyScan_uwTick = uwTick;
		
		static unsigned char target=0;
		
	
    keyvalue_Current=Key_scan();           
    switch(keyvalue_Current)
    {
        case B1_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1 && su==0)                 //第一次按下该键
            {
                LED_State=LED_State|0x02;
                LED_Control(LED_State);
								su=1;
								LCD_TestTask();
            }
						else if(Key_KeepTime==1 && su==1)
						{
								LED_State=LED_State|0x01;
                LED_Control(LED_State);
								su=0;
								LCD_TestTask();
						}
            break;
        case B2_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1 && su==1)                 //第一次按下该键
            {
								switch(target)
								{
										case 0:
										{
										
											if(year==99)
												year=0;
											else
												year++;
											//target++;
											break;
										}
										case 1:
										{
										
											if(month==12)
												month=1;
											else
												month++;
											//target++;
											break;
										}
										case 2:
										{
										
											if(day==31)
												day=1;
											else
												day++;
											//day++;
											break;
										}
										default:
											break;
									}
									sprintf((char *)Lcd_Disp_String,"      %02d %02d %02d     ",year,month,day);
									LCD_DisplayStringLine(Line3, (uint8_t *)Lcd_Disp_String);
            }
            break;
        case B3_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1 && su==1)                 //第一次按下该键
            {
								switch(target)
								{
										case 0:
										{
										
											if(year==0)
												year=99;
											else
												year--;
											break;
										}
										case 1:
										{
										
											if(month==1)
												month=12;
											else
												month--;
											break;
										}
										case 2:
										{
										
											if(day==1)
												day=31;
											else
												day--;
											break;
										}
										default:
											break;
									}
									sprintf((char *)Lcd_Disp_String,"      %02d %02d %02d     ",year,month,day);
									LCD_DisplayStringLine(Line3, (uint8_t *)Lcd_Disp_String);
            }
            break;
				case B4_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1 && su==1)                 //第一次按下该键
            {
                target++;
								if(target==3)
										target =0;
							
            }
            break;
				default:
            if(Key_KeepTime>0)      //有键按下，现在松开了
            {
                switch(keyvalue_Last)
                {
                    case B1_PRESS:                      //该键松开
                        if(Key_KeepTime<10 && su==1)             // 500ms以内为短键          
                        {
														LED_State=LED_State&0xFE;
														LED_Control(LED_State);
												}
												else if(Key_KeepTime<10 && su==0)
												{	
														LED_State=LED_State&0xFD;
														LED_Control(LED_State);
												}
                        else                            // 500ms以上为短键 
                        {
                            ;
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


void LCD_TestTask(void)
{
    
    sprintf((char *)Lcd_Disp_String,"      %02d %02d %02d      ",year,month,day);
    LCD_Clear(White);

		
		if(su==0)
		{
			LCD_SetBackColor(Black);
			LCD_SetTextColor(White);

//    LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
//    LCD_DisplayStringLine(Line1, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line1, (uint8_t *)"       NJTECH       ");


			
//    LCD_SetTextColor(Black);
			LCD_DisplayStringLine(Line2, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line3, (uint8_t *)"     Hello World    ");
			LCD_DisplayStringLine(Line4, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line5, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
		}
		else if(su==1)
		{
			LCD_SetBackColor(Black);
			LCD_SetTextColor(White);

//    LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
//    LCD_DisplayStringLine(Line1, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line1, (uint8_t *)"       Date         ");


//    LCD_SetTextColor(Black);
			LCD_DisplayStringLine(Line2, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line3, (uint8_t *)Lcd_Disp_String);
			LCD_DisplayStringLine(Line4, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line5, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
		}
		
}

void  Uart1_RX_Pro(void)
{
    static unsigned int Usart_uwTick=0;
		unsigned char counta=0;
		unsigned char countb=0;
		unsigned char i=0;

    /*******使用滴答时钟的控制任务函数执行的间隔时间********/
    if(U1Rx_Len==0)     //没有接收到数据
    {
        Usart_uwTick = uwTick;          //必须要有清时间，保证第一个字节接收到后有一定时间接收剩余数据再进行处理；
        return ;
    }

    if((uwTick-Usart_uwTick)>50)
    {
        Usart_uwTick = uwTick;

				if(U1Rx_Len <12)
				{
						LCD_Clear(Black);
						sprintf((char *)Lcd_Disp_String,"   %s",Uart1_Rx_Buf);
						LCD_DisplayStringLine(Line1, (uint8_t *)Lcd_Disp_String);
						for(i=0;i<U1Rx_Len;i++)
						{
								if(Uart1_Rx_Buf[i]=='A')
										counta ++;
								else if(Uart1_Rx_Buf[i]=='B')
										countb ++;
							
						}
						sprintf((char *)Lcd_Disp_String,"   %d ",counta );
						LCD_DisplayStringLine(Line3, (uint8_t *)Lcd_Disp_String);
						sprintf((char *)Lcd_Disp_String,"   %d ",countb);
						LCD_DisplayStringLine(Line5, (uint8_t *)Lcd_Disp_String);
						
						sprintf((char *)Uart1_Tx_Buf,"OK:A+B=%d\n",counta+countb );
						Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));

				}
				
				else if(U1Rx_Len >12 || U1Rx_Len ==12)
				{
						LCD_Clear(Black);
						LCD_DisplayStringLine(Line1, (uint8_t *)" ERROR ");
						sprintf((char *)Uart1_Tx_Buf,"ERROR\n");
						Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));

					
				}
				for(i=0;i<U1Rx_Len;i++)
				{
							
							Uart1_Rx_Buf[i]=' ';
		
				}
        U1Rx_Len=0;     //一定要清零，要不会重复发送;
    }
}


//void Key_Proc()
//{
//    static uint32_t KeyScan_uwTick = 0;             //控制KeyProcess的执行速度
//    static uint16_t Key_KeepTime=0;                 //记录按键按下持续的时间 
//    static uint8_t keyvalue_Last=0;                 //记录上次按键键值
//    uint8_t keyvalue_Current;
//	
//    if((uwTick -  KeyScan_uwTick)<50)	return;//减速函数    //uwTick为1ms的计数器 ,50ms扫描一次        
//	  KeyScan_uwTick = uwTick;
//    
//    keyvalue_Current=key_scan();           
//    switch(keyvalue_Current)
//    {
//        case 1:
//            Key_KeepTime++;

//						if(Key_KeepTime==1)                 //第一次按下该键
//						{
//               TIM15CH1_PA2_Fre =100000/79+500;//1000000/arr
//							 TIM15CH1_PA2_Pulse = 1000000/TIM15CH1_PA2_Fre;
//						}

//								
//            break;
//        case 2:
//            Key_KeepTime++;
//            if(Key_KeepTime==1)                 //第一次按下该键
//            {
//    
//            }
//						
//            break;
//        case 3:
//            Key_KeepTime++;
//            if(Key_KeepTime==1)                 //第一次按下该键
//            {
//               
//            }
//            break;
//        case 4:
//            Key_KeepTime++;
//            if(Key_KeepTime==1)                 //第一次按下该键
//            {

//            }
//            break;
//        default:
//            if(Key_KeepTime>0)      //有键按下，现在松开了
//            {
//                switch(keyvalue_Last)
//                {
//                    case 1:                      //该键松开
//										    {

//												}
//                        break;
//                    case 2:                      //该键松开
//										    {
//												  
//												}
//                        break;
//                }


//            }

//            Key_KeepTime=0;
//            break;
//    }
//    keyvalue_Last=keyvalue_Current;

//}





//void PWM_Display(void)
//{
//    
//    static uint32_t LCD_uwTick = 0; 
//    if((uwTick -  LCD_uwTick)<300)	return;//减速函数    //uwTick为1ms的计数器 ,50ms扫描一次        
//	  LCD_uwTick = uwTick;
//    
//	  sprintf((char *)Lcd_Disp_String,"  PWM Fre:%d  ",TIM15CH1_PA2_Fre);
//	  LCD_DisplayStringLine(Line2,(uint8_t *)Lcd_Disp_String);
//	
//	  sprintf((char *)Lcd_Disp_String,"  PWM Dute:%d%%  ",TIM15CH1_PA2_Duty);
//	  LCD_DisplayStringLine(Line3,(uint8_t *)Lcd_Disp_String);
//	 
//	  sprintf((char *)Lcd_Disp_String,"  PB4 Fre:%d  ",1000000/tim3RiseEdge);
//	  LCD_DisplayStringLine(Line5,(uint8_t *)Lcd_Disp_String);
//	
//	  sprintf((char *)Lcd_Disp_String,"  PB4 Dute:%.2f%%  ",tim3Duty*100);
//	  LCD_DisplayStringLine(Line6,(uint8_t *)Lcd_Disp_String);

//}

//void ADC_Proc(void)
//{
//	  float adc_value = 0;
//    if(uwTick%200 != 0)
//         return;
//		else
//	  {
//		  adc_value = getADC()*3.3/4096;
//			if(adc_value < 1)
//				TIM15CH1_PA2_Duty = 10;
//			else if((adc_value >= 1)&&(adc_value <= 2))
//			   TIM15CH1_PA2_Duty = 75*adc_value-65;
//			else if(adc_value > 2)
//				 TIM15CH1_PA2_Duty = 85;
//			
//			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,TIM15_PWM_Period*TIM15CH1_PA2_Duty/100); //修改占空比
//		}
//		  
//}
void pwmchange(void)
{
		__HAL_TIM_SET_AUTORELOAD(&htim15,500); //修改周期
    __HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,500*60/100); //修改占空比
}
	