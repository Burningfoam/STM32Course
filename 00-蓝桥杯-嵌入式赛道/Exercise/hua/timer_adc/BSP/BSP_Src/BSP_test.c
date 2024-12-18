#include "BSP_test.h"
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "BSP_adc.h"
#include "string.h"
#include "stdio.h"
unsigned int TIM2CH3_PA2_Pulse = 2000;
unsigned int TIM2CH3_PA2_Duty = 30;
unsigned int TIM2CH3_PA2_Fre = 2000;
unsigned char Lcd_Disp_String[20];

unsigned int tim3RiseEdge=0;
unsigned int tim3FallEdge=0;
float tim3Duty = 0;
void Key_Proc()
{
    static uint32_t KeyScan_uwTick = 0;             //����KeyProcess��ִ���ٶ�
    static uint16_t Key_KeepTime=0;                 //��¼�������³�����ʱ�� 
    static uint8_t keyvalue_Last=0;                 //��¼�ϴΰ�����ֵ
    uint8_t keyvalue_Current;
	
    if((uwTick -  KeyScan_uwTick)<50)	return;//���ٺ���    //uwTickΪ1ms�ļ����� ,50msɨ��һ��        
	  KeyScan_uwTick = uwTick;
    
    keyvalue_Current=key_scan();           
    switch(keyvalue_Current)
    {
        case 1:
            Key_KeepTime++;

						if(Key_KeepTime==1)                 //��һ�ΰ��¸ü�
						{
               TIM2CH3_PA2_Fre =((TIM2CH3_PA2_Fre-2000)+500)%2500+2000;
							 TIM2CH3_PA2_Pulse = 1000000/TIM2CH3_PA2_Fre;
						}

								
            break;
        case 2:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //��һ�ΰ��¸ü�
            {
    
            }
						
            break;
        case 3:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //��һ�ΰ��¸ü�
            {
               
            }
            break;
        case 4:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //��һ�ΰ��¸ü�
            {

            }
            break;
        default:
            if(Key_KeepTime>0)      //�м����£������ɿ���
            {
                switch(keyvalue_Last)
                {
                    case 1:                      //�ü��ɿ�
										    {

												}
                        break;
                    case 2:                      //�ü��ɿ�
										    {
												  
												}
                        break;
                }


            }

            Key_KeepTime=0;
            break;
    }
    keyvalue_Last=keyvalue_Current;

}




void LCD_Proc()
{
    static uint32_t LCD_uwTick = 0; 
    if((uwTick -  LCD_uwTick)<300)	return;//���ٺ���    //uwTickΪ1ms�ļ����� ,50msɨ��һ��        
	  LCD_uwTick = uwTick;
    
	  sprintf((char *)Lcd_Disp_String,"  PWM Fre:%d  ",TIM2CH3_PA2_Fre);
	  LCD_DisplayStringLine(Line2,(uint8_t *)Lcd_Disp_String);
	
	  sprintf((char *)Lcd_Disp_String,"  PWM Dute:%d%%  ",TIM2CH3_PA2_Duty);
	  LCD_DisplayStringLine(Line3,(uint8_t *)Lcd_Disp_String);
	 
	  sprintf((char *)Lcd_Disp_String,"  PB4 Fre:%d  ",1000000/tim3RiseEdge);
	  LCD_DisplayStringLine(Line5,(uint8_t *)Lcd_Disp_String);
	
	  sprintf((char *)Lcd_Disp_String,"  PB4 Dute:%.2f%%  ",tim3Duty*100);
	  LCD_DisplayStringLine(Line6,(uint8_t *)Lcd_Disp_String);
	
}


void ADC_Proc(void)
{
	  float adc_value = 0;
    if(uwTick%200 != 0)
         return;
		else
	  {
		  adc_value = getADC()*3.3/4096;
			if(adc_value < 1)
				TIM2CH3_PA2_Duty = 10;
			else if((adc_value >= 1)&&(adc_value <= 2))
			   TIM2CH3_PA2_Duty = 75*adc_value-65;
			else if(adc_value > 2)
				 TIM2CH3_PA2_Duty = 85;
		}
		  
}







void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    unsigned int  OC_Count=0;
    OC_Count= __HAL_TIM_GET_COUNTER(&htim2);      //ȡ�õ�ǰ��ʱ������ֵ
    if(htim->Instance == TIM2)
	  {
		    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
//			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1)+250);//2khz��ÿ�μ���ͬ��ֵ��ռ�ձ���Ϊ50%
            //��PA11�������10ms(Ƶ��100Hz)��ռ�ձ�10%������
            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA , GPIO_PIN_2))   //TIM2_CH3:PA2  Ƶ��1k��ռ�ձ�30%
            {
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,OC_Count+(TIM2CH3_PA2_Pulse-TIM2CH3_PA2_Pulse*TIM2CH3_PA2_Duty/100));
            }
            else
            {
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,OC_Count+TIM2CH3_PA2_Pulse*TIM2CH3_PA2_Duty/100);
            }
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
