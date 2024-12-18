#ifndef __BSP_TIM_H
#define __BSP_TIM_H

extern unsigned int TIM3_RiseEdge_Count;       //tim3�������ֵ:������~�����أ�������
extern unsigned int TIM3_FallEdge_Count;       //tim3�������ֵ:������~�����أ����ߵ�ƽʱ��
extern unsigned int TIM2_RiseEdge_Count;       //tim2�������ֵ:������~�����أ�������
extern unsigned int TIM2_FallEdge_Count;       //tim2�������ֵ:������~�����أ����ߵ�ƽʱ��
extern float   Tim3_Duty;                  //tim3������PWM��ռ�ձ�
extern float   Tim2_Duty;                  //tim2������PWM��ռ�ձ�
extern float   Tim3_Fre;                   //tim3������PWM��Ƶ��
extern float   Tim2_Fre;                   //tim2������PWM��Ƶ��
extern unsigned int TIM15_PWM_Period;          //TIM15��PWM��������ڣ�1~65535
extern unsigned int TIM15CH1_PA2_Duty;         //TIM15_CH1��PWM�����ռ�ձȣ�1~99 



#endif
