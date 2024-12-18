#include "bsp_adc.h"
#include "main.h"
#include "adc.h"

float adc_read()
{
	unsigned int temp;
	HAL_ADC_Start_IT(&hadc2);
  temp = HAL_ADC_GetValue(&hadc2);
	return temp*3.3/4096;
}