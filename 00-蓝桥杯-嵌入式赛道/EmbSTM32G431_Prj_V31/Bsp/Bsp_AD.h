#ifndef __BSP_AD_H
#define __BSP_AD_H

#define  ADC1_DMA_BUF_SIZE 10
#define  ADC2_DMA_BUF_SIZE 10

extern unsigned int ADC1_DMABuf[ADC1_DMA_BUF_SIZE];
extern unsigned int ADC2_DMABuf[ADC2_DMA_BUF_SIZE];
extern void Bsp_AD_Init(void);

#endif

