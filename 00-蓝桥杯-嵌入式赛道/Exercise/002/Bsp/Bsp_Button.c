/*******************************************************************************
* 文件名称：
* 功能描述：1. CT117E_M4实验板上按键的操作
* 配置说明：
* 硬件环境：CT117E_M4
* 注意事项：
* 作者版本：hufq/V1.0
*******************************************************************************/
#include "Bsp_Button.h"
#include "main.h"
#include "gpio.h"
#include "LED.h"


/****************************************************
函数名称:
功    能: 按钮扫描,""""建议通过滴答时钟来定时调用扫描函数"""
          1、注意优先级；
          2、注意双键问题；
          3、尽量使用不支持连续按模式，因为连续按容易出现抖动问题（如何解决）；
入口参数: mode：0-不支持连按(即按下)；1-支持连按
出口参数: 按键值：0-为无键；1~4-对应按键编号
****************************************************/
unsigned char Key_scan(void)
{    
    //这里不应该用“~”，而应该用“!”
//    if(key&&(~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||~HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)))
//    if(key&&((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)))
    if((!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)))
    {
//        HAL_Delay(5); 	    //利用定时器扫描，可以省却该延时
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET) return B1_PRESS;
        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET) return B2_PRESS;
        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET) return B3_PRESS;
        else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET) return B4_PRESS;
        else return 0;
    } 
    return 0;
}


/****************************************************
函数名称:
功    能: 按钮扫描,""""建议通过滴答时钟来定时调用扫描函数"""
          1、注意优先级；
          2、注意双键问题；
          3、尽量使用不支持连续按模式，因为连续按容易出现抖动问题（如何解决）；
入口参数: mode：0-不支持连按(即按下)；1-支持连按
出口参数: 按键值：0-为无键；1~4-对应按键编号
****************************************************/
//unsigned char Key_scan(unsigned char mode)
//{
//    static unsigned char key=1;
//    if(mode)key=1;
//    //这里不应该用“~”，而应该用“!”
////    if(key&&(~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||~HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)))
////    if(key&&((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)))
//    if(key&&(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)))
//    {
////        HAL_Delay(5); 	    //利用定时器扫描，可以省却该延时
//        key = 0;
//        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET) return B1_PRESS;
//        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET) return B2_PRESS;
//        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET) return B3_PRESS;
//        else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET) return B4_PRESS;
//        else return 8;
//    } 
//    else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)&&HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)&&HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)&&HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)) 
//        key=1;
//    return 0;
//}




/*************
以下是蚂蚁工场对应的按键代码，可以查看当前哪些按键按下，哪些按键松开，不过代码虽简单只是逻辑稍显复杂，故不建议竞赛时使用
**************/
////*按键扫描专用变量
//unsigned char ucKey_Val, unKey_Down, ucKey_Up, ucKey_Old;

//unsigned char Key_Scan(void)
//{
//	unsigned char unKey_Val = 0;
//	
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
//		unKey_Val = 1;

//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
//		unKey_Val = 2;

//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)
//		unKey_Val = 3;
//	
//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
//		unKey_Val = 4;	
//	
//	return unKey_Val;
//}

//void Key_Proc(void)
//{
//	static uint32_t uwTick_Set_Point = 0;//控制Key_Proc的执行速度
//    if((uwTick -  uwTick_Set_Point)<100)	return;//减速函数
//	uwTick_Set_Point = uwTick;

//	
//	//分成几种情况
//	//情况1：100ms两次扫描，按键得到的结果从0（都没按下）到B4按下，产生了下降沿。
//	//ucKey_Val = 4(0000 0100)
//	//unKey_Down = 0000 0100 & ( 0000 0000 ^ 0000 0100) = 0000 0100 & 0000 0100 = 0000 0100 (4)
//	//ucKey_Up = 1111 1011 & 0000 0100 = 0000 0000 
//	//ucKey_Old = 4
//	
//	//情况2：B4产生了下降沿后，按键一直按着
//	//ucKey_Val = 4(0000 0100)
//	//unKey_Down = 0000 0100 & ( 0000 0100 ^ 0000 0100) = 0000 0100 & 0000 0000 = 0000 0000 (0)
//	//ucKey_Up = 1111 1011 & 0000 0000 = 0000 0000 
//	//ucKey_Old = 4	
//	
//	//情况3：B4按键一直按着随后弹起
//	//ucKey_Val = 0(0000 0000)
//	//unKey_Down = 0000 0000 & ( 0000 0100 ^ 0000 0000) = 0000 0000 & 0000 0100 = 0000 0000 (0)
//	//ucKey_Up = 1111 1111 & 0000 0100 = 0000 0100 (4)
//	//ucKey_Old = 0		
//	
//	ucKey_Val = Key_Scan();
//	unKey_Down = ucKey_Val & (ucKey_Old ^ ucKey_Val); 
//	ucKey_Up = ~ucKey_Val & (ucKey_Old ^ ucKey_Val);	
//	ucKey_Old = ucKey_Val;
//	
//	if(unKey_Down == 4)
//	{
//		LED_Disp(0x88);
//	}
//	if(unKey_Down == 3)
//	{
//		LED_Disp(0x00);
//	}	
//}

