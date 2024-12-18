#include "Bsp_Button.h"
#include "main.h"
#include "gpio.h"
#include "Bsp_LED.h"

unsigned char Key_scan(void)
{    
    
    if((!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)))
    {

        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET) return B1_PRESS;
        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET) return B2_PRESS;
        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET) return B3_PRESS;
        else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET) return B4_PRESS;
        else return 0;
    } 
    return 0;
}



