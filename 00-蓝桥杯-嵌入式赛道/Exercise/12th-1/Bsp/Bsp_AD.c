/*******************************************************************************
* �ļ����ƣ�
* ����������1��CT117E_M4ʵ�����STM32����AD�Ĳ�����ʹ��DMA������
            2��cubeMX�е�����˵�����£�
                (1)�ܽ�mode����ѡsingle-ended����ѡdifferential
                (2)Parameters settings�У�continuous Conversion Mode,DMA continous requests, enable regular conversionsѡENABLE
                (3)DMA settings��data width ѡword
* ����˵����
* Ӳ��������CT117E_M4
* ע�����
* ���߰汾��hufq/V1.0
*******************************************************************************/
#include "Bsp_AD.h"
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "adc.h"

unsigned int ADC1_DMABuf[ADC1_DMA_BUF_SIZE];
unsigned int ADC2_DMABuf[ADC2_DMA_BUF_SIZE];


/****************************************************
��������: 
��    ��: ��Ҫ��AD����������DMA����������ע��DMA�Ĵ��仺�����ͳ��ȣ���Bsp_Test.c�е�AD_Test()�����У���ʾ���ȡ��ADֵ��
��ڲ���:
���ڲ���:
****************************************************/
void Bsp_AD_Init(void)
{
    memset(ADC1_DMABuf,0,ADC1_DMA_BUF_SIZE);
    HAL_ADC_Start_DMA(&hadc1,  (uint32_t*)&ADC1_DMABuf, ADC1_DMA_BUF_SIZE);	//ʹ��AD_DMA�Ĵ��䣬�ú����ڿ⺯��stm32g4xx_hal_adc.c��
    
    memset(ADC2_DMABuf,0,ADC2_DMA_BUF_SIZE);
    HAL_ADC_Start_DMA(&hadc2,  (uint32_t*)&ADC2_DMABuf, ADC2_DMA_BUF_SIZE);	//ʹ��AD_DMA�Ĵ���    
}


