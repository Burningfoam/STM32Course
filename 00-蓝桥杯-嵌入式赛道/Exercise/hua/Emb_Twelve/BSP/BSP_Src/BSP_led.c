#include "BSP_led.h"
#include "main.h"
unsigned char led_state = 0;

void led_control(unsigned char led)
{
	   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_SET);
     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	
		 HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}
