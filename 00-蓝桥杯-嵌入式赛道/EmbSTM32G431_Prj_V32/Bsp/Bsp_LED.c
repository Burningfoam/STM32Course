/*******************************************************************************
* 文件名称：
* 功能描述：1. CT117E_M4实验板上LED的操作
* 配置说明：
* 硬件环境：CT117E_M4
* 注意事项：
* 作者版本：hufq/V1.0
*******************************************************************************/
#include "Bsp_LED.h"
#include "main.h"
#include "gpio.h"

//全局变量，保存LED状态，8位数据对应8个LED（LED_State.0-LED1，以此类推），0-灭；1-亮
//举例：点亮LD1:执行LED_State=LED_State|0x01;然后调用LED_Control(LED_State);LD1~LD7对应的数据是：0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
//举例：熄灭LD1:执行LED_State=LED_State&0xFE;然后调用LED_Control(LED_State);LD1~LD7对应的数据是：0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F
//举例：翻转LD1:执行LED_State=LED_State^0x01;然后调用LED_Control(LED_State);LD1~LD7对应的数据是：0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
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


///****************************************************
//函数名称:
//功    能: 控制LED亮
//入口参数: lednum：0-全亮；1-LED1亮；2-LED2亮，以此类推
//出口参数:
//****************************************************/
//void LED_ON(unsigned char lednum)
//{
//    //因为存在LED和LCD管脚的复用(PC8~PC15)，其中PC8~15是通过D触发器连接LED的
//    //1、需要PD2控制D触发器的触发信号；2、需要全局变量保存LED的当前状态
//    switch(lednum)
//    {
//    case 0:
//        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_RESET);                   //全拉低，全亮，注意这边把PC0~PC7也拉低了
//        LED_State=0;                                                            //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 1:
//        HAL_GPIO_WritePin(LED1_LCD8_GPIO_Port,LED1_LCD8_Pin,GPIO_PIN_RESET);    //LED1亮
//        LED_State=LED_State&0xFE;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 2:
//        HAL_GPIO_WritePin(LED2_LCD9_GPIO_Port,LED2_LCD9_Pin,GPIO_PIN_RESET);    //LED2亮
//        LED_State=LED_State&0xFD;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 3:
//        HAL_GPIO_WritePin(LED3_LCD10_GPIO_Port,LED3_LCD10_Pin,GPIO_PIN_RESET);  //LED3亮
//        LED_State=LED_State&0xFB;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 4:
//        HAL_GPIO_WritePin(LED4_LCD11_GPIO_Port,LED4_LCD11_Pin,GPIO_PIN_RESET);  //LED4亮
//        LED_State=LED_State&0xF7;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 5:
//        HAL_GPIO_WritePin(LED5_LCD12_GPIO_Port,LED5_LCD12_Pin,GPIO_PIN_RESET);  //LED5亮
//        LED_State=LED_State&0xEF;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 6:
//        HAL_GPIO_WritePin(LED6_LCD13_GPIO_Port,LED6_LCD13_Pin,GPIO_PIN_RESET);  //LED6亮
//        LED_State=LED_State&0xDF;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 7:
//        HAL_GPIO_WritePin(LED7_LCD14_GPIO_Port,LED7_LCD14_Pin,GPIO_PIN_RESET);  //LED7亮
//        LED_State=LED_State&0xBF;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 8:
//        HAL_GPIO_WritePin(LED8_LCD15_GPIO_Port,LED8_LCD15_Pin,GPIO_PIN_RESET);  //LED8亮
//        LED_State=LED_State&0x7F;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;    
//    }
////    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
////    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
////    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
////    HAL_GPIO_WritePin(GPIOC,lednum<<8,GPIO_PIN_RESET);
////    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
////    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
//}

///****************************************************
//函数名称:
//功    能: 控制LED灭
//入口参数: lednum：0-全灭；1-LED1灭；2-LED2灭，以此类推
//出口参数:
//****************************************************/
//void LED_OFF(unsigned char lednum)
//{
//    //因为存在LED和LCD管脚的复用(PC8~PC15)，其中PC8~15是通过D触发器连接LED的
//    //1、需要PD2控制D触发器的触发信号；2、需要全局变量保存LED的当前状态
//    switch(lednum)
//    {
//    case 0:
//        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);                     //全拉高，全灭，注意这边把PC0~PC7也拉低了
//        LED_State=0xFF;                                                         //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 1:
//        HAL_GPIO_WritePin(LED1_LCD8_GPIO_Port,LED1_LCD8_Pin,GPIO_PIN_SET);      //LED1灭
//        LED_State=LED_State|0x01;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 2:
//        HAL_GPIO_WritePin(LED2_LCD9_GPIO_Port,LED2_LCD9_Pin,GPIO_PIN_SET);      //LED2灭
//        LED_State=LED_State|0x02;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 3:
//        HAL_GPIO_WritePin(LED3_LCD10_GPIO_Port,LED3_LCD10_Pin,GPIO_PIN_SET);    //LED3灭
//        LED_State=LED_State|0x04;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 4:
//        HAL_GPIO_WritePin(LED4_LCD11_GPIO_Port,LED4_LCD11_Pin,GPIO_PIN_SET);    //LED4灭
//        LED_State=LED_State|0x08;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 5:
//        HAL_GPIO_WritePin(LED5_LCD12_GPIO_Port,LED5_LCD12_Pin,GPIO_PIN_SET);    //LED5灭
//        LED_State=LED_State|0x10;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 6:
//        HAL_GPIO_WritePin(LED6_LCD13_GPIO_Port,LED6_LCD13_Pin,GPIO_PIN_SET);    //LED6灭
//        LED_State=LED_State|0x20;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 7:
//        HAL_GPIO_WritePin(LED7_LCD14_GPIO_Port,LED7_LCD14_Pin,GPIO_PIN_SET);    //LED7灭
//        LED_State=LED_State|0x40;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 8:
//        HAL_GPIO_WritePin(LED8_LCD15_GPIO_Port,LED8_LCD15_Pin,GPIO_PIN_SET);    //LED8灭
//        LED_State=LED_State|0x80;                                               //记录LED状态
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;    
//    }
//}

///****************************************************
//函数名称:
//功    能: 控制LED亮灭切换
//入口参数: lednum：1-LED1切换；2-LED2切换，以此类推
//出口参数:
//****************************************************/
//void LED_Toggle(unsigned char lednum)
//{
//    //因为存在LED和LCD管脚的复用(PC8~PC15)，其中PC8~15是通过D触发器连接LED的
//    //1、需要PD2控制D触发器的触发信号；2、需要全局变量保存LED的当前状态
//    switch(lednum)
//    {
//    case 1:
//        if(LED_State&0x01)  //原来是高电平
//        {
//            LED_State=LED_State&0xFE;
//            HAL_GPIO_WritePin(LED1_LCD8_GPIO_Port,LED1_LCD8_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x01;
//            HAL_GPIO_WritePin(LED1_LCD8_GPIO_Port,LED1_LCD8_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 2:
//        if(LED_State&0x02)  //原来是高电平
//        {
//            LED_State=LED_State&0xFD;
//            HAL_GPIO_WritePin(LED2_LCD9_GPIO_Port,LED2_LCD9_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x02;
//            HAL_GPIO_WritePin(LED2_LCD9_GPIO_Port,LED2_LCD9_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 3:
//        if(LED_State&0x04)  //原来是高电平
//        {
//            LED_State=LED_State&0xFB;
//            HAL_GPIO_WritePin(LED3_LCD10_GPIO_Port,LED3_LCD10_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x04;
//            HAL_GPIO_WritePin(LED3_LCD10_GPIO_Port,LED3_LCD10_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 4:
//        if(LED_State&0x08)  //原来是高电平
//        {
//            LED_State=LED_State&0xF7;
//            HAL_GPIO_WritePin(LED4_LCD11_GPIO_Port,LED4_LCD11_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x08;
//            HAL_GPIO_WritePin(LED4_LCD11_GPIO_Port,LED4_LCD11_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 5:
//        if(LED_State&0x10)  //原来是高电平
//        {
//            LED_State=LED_State&0xEF;
//            HAL_GPIO_WritePin(LED5_LCD12_GPIO_Port,LED5_LCD12_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x10;
//            HAL_GPIO_WritePin(LED5_LCD12_GPIO_Port,LED5_LCD12_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 6:
//        if(LED_State&0x20)  //原来是高电平
//        {
//            LED_State=LED_State&0xDF;
//            HAL_GPIO_WritePin(LED6_LCD13_GPIO_Port,LED6_LCD13_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x20;
//            HAL_GPIO_WritePin(LED6_LCD13_GPIO_Port,LED6_LCD13_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 7:
//        if(LED_State&0x40)  //原来是高电平
//        {
//            LED_State=LED_State&0xBF;
//            HAL_GPIO_WritePin(LED7_LCD14_GPIO_Port,LED7_LCD14_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x40;
//            HAL_GPIO_WritePin(LED7_LCD14_GPIO_Port,LED7_LCD14_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 8:
//        if(LED_State&0x80)  //原来是高电平
//        {
//            LED_State=LED_State&0x7F;
//            HAL_GPIO_WritePin(LED8_LCD15_GPIO_Port,LED8_LCD15_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x80;
//            HAL_GPIO_WritePin(LED8_LCD15_GPIO_Port,LED8_LCD15_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //触发信号
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;    
//    }
//}
