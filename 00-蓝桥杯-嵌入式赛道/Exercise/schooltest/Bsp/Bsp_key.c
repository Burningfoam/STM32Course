#include "Bsp_key.h"
#include "main.h"
#include "gpio.h"


unsigned char key_scan(void)
{
	if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)|!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)|!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)|!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0) return 1;
		else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0) return 2;
		else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==0) return 3;
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0) return 4;
		else return 0;
		
		
	}
	return 0;

}

