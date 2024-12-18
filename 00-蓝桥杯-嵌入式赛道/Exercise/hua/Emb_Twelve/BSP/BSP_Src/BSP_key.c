#include "BSP_key.h"
#include "main.h"

unsigned char key_scan(void)
{
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==0)
         return 1;
		
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)==0)
         return 2;
		
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)==0)
         return 3;
		
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==0)
         return 4;
		
		else 
			   return 0;
}
