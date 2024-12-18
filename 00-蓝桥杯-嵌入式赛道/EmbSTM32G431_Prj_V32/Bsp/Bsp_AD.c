/*******************************************************************************
* 文件名称：
* 功能描述：1、CT117E_M4实验板上STM32内置AD的操作，使用DMA操作，
            2、cubeMX中的设置说明如下；
                (1)管脚mode都是选single-ended，不选differential
                (2)Parameters settings中：continuous Conversion Mode,DMA continous requests, enable regular conversions选ENABLE
                (3)DMA settings中data width 选word
* 配置说明：
* 硬件环境：CT117E_M4
* 注意事项：
* 作者版本：hufq/V1.0
*******************************************************************************/
#include "Bsp_AD.h"
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "adc.h"

unsigned int ADC1_DMABuf[ADC1_DMA_BUF_SIZE];
unsigned int ADC2_DMABuf[ADC2_DMA_BUF_SIZE];


/****************************************************
函数名称: 
功    能: 主要是AD启动，包括DMA传输启动，注意DMA的传输缓冲区和长度，在Bsp_Test.c中的AD_Test()函数中，演示如何取得AD值；
入口参数:
出口参数:
****************************************************/
void Bsp_AD_Init(void)
{
    memset(ADC1_DMABuf,0,ADC1_DMA_BUF_SIZE);
    HAL_ADC_Start_DMA(&hadc1,  (uint32_t*)&ADC1_DMABuf, ADC1_DMA_BUF_SIZE);	//使能AD_DMA的传输，该函数在库函数stm32g4xx_hal_adc.c中
    
    memset(ADC2_DMABuf,0,ADC2_DMA_BUF_SIZE);
    HAL_ADC_Start_DMA(&hadc2,  (uint32_t*)&ADC2_DMABuf, ADC2_DMA_BUF_SIZE);	//使能AD_DMA的传输    
}


