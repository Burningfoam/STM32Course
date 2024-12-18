#include "main.h"
#include "adc.h"
#include "Bsp_tim.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

unsigned int in_rise;
unsigned int in_fall;
unsigned char in_duty;
unsigned int in_freq;

unsigned int out_freq=1000;
unsigned char out_duty=50;
unsigned int out_period;

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	unsigned int oc_count;
	
	oc_count=__HAL_TIM_GET_COUNTER(htim);
	
	out_period=1000000.0/out_freq;
	
	if(htim->Instance==TIM2)
	{
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1))
			{
				__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,oc_count+(out_period-out_period*out_duty/100));
				
			}
			else
			{
				__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,oc_count+(out_period*out_duty/100));

			}

		}
	}

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
		{
			in_rise=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2)+1;
			
		}
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
		{
			in_fall=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			in_duty=in_fall*100.0/in_rise;
			in_freq=1000000.0/in_rise;
		}
	}
}





