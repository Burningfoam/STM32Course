#include "bsp_time.h"
#include "bsp_test.h"
#include "main.h"
#include "tim.h"

unsigned int out_paulse;
unsigned char out_dutyy;
unsigned int in_rise_time;
unsigned int in_fall_time;
unsigned int in_frequent;
unsigned char in_dutyy;
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	unsigned int OC_Count=0;
    OC_Count = __HAL_TIM_GET_COUNTER(htim);
	
	out_paulse = 1000000/out_Fre;
	out_dutyy = out_Duty;
	if(htim->Instance == TIM2)
	{
	   if(htim->Channel  == HAL_TIM_ACTIVE_CHANNEL_2 )
		 {
		     if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1))
				 {
				   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,OC_Count+(out_paulse-out_paulse*out_dutyy/100));
				 
				 }
				 else
         {
				   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,OC_Count+out_paulse*out_dutyy/100);
				 }
		 }
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM3)
	{
	   if(htim->Channel  == HAL_TIM_ACTIVE_CHANNEL_2 )
		 {
        in_rise_time = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2 )+1;
		 }
		 if(htim->Channel  == HAL_TIM_ACTIVE_CHANNEL_1)
		 {
		    in_fall_time = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		    in_dutyy = in_fall_time*100.0/in_rise_time;
			  in_frequent = 1000000.0/in_rise_time;
		 }
		 	
	    
	}
}