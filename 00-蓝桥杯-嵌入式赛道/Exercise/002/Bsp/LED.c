#include "LED.h"
#include "main.h"
#include "gpio.h"

//全局变量，保存LED状态，8位数据对应8个LED（LED_State.0-LED1，以此类推），0-灭；1-亮
//举例：点亮第4个LED:执行LED_State=LED_State|0x08;然后调用LED_Control(LED_State);
//举例：熄灭第4个LED:执行LED_State=LED_State&0xF7;然后调用LED_Control(LED_State);
//举例：翻转第4个LED:执行LED_State=LED_State^0x08;然后调用LED_Control(LED_State);

unsigned char LED_State=0;  
/****************************************************
函数名称:
功    能: 根据ledstate数据中0对应LED灭，1对LED应亮；
          因为LED与LCD复用，故需要定义全局变量来保存当前LED灯状态；
          记得在初始化时需要，全部熄灭LED；
入口参数: ledstate：8位数据对应8个LED（ledstate.0-LED1，以此类推），0-灭；1-亮
出口参数:
****************************************************/
void LED_Control(unsigned char ledstate)
{
    //因为存在LED和LCD管脚的复用(PC8~PC15)，其中PC8~15是通过D触发器连接LED的
    //1、需要PD2控制D触发器的触发信号；2、需要全局变量保存LED的当前状态
    //全部关闭LED，这样保证ledstate中0对应的LED灭    
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);    
    
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
    
    //点亮ledstate8位数据中1对应的LED灯
    HAL_GPIO_WritePin(GPIOC,ledstate<<8,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}
