#include "BSP_adc.h"
#include "adc.h"



uint16_t getADC(void)
{
	uint16_t adc = 0;
	
	HAL_ADC_Start(&hadc2);
	adc = HAL_ADC_GetValue(&hadc2);
	
	return adc;
}

