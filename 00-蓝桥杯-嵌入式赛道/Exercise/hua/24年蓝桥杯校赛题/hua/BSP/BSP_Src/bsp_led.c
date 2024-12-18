#include "bsp_led.h"
#include "main.h"
unsigned char led_state = 0x00;
void led_control(unsigned char led_value)
{
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	
	 HAL_GPIO_WritePin(GPIOC,led_value<<8,GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	
}
