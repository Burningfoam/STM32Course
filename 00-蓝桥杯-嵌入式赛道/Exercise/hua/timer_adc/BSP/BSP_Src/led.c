#include "main.h"
#include "led.h"
#include "gpio.h"
unsigned char led_state = 0;

void Led_Control(unsigned char ledstate)
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	
	  HAL_GPIO_WritePin(GPIOC,ledstate<<8,GPIO_PIN_RESET);
	
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}