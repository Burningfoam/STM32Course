#ifndef __BSP_TIM_H
#define __BSP_TIM_H

typedef struct{
	unsigned short int First_Rising_T1,First_Falling_T2;
	unsigned char State_flag;       //0-��ʼ״̬��׼�������һ�������أ�1-׼�������һ�������أ�2-׼������ڶ���������
}CaptureTim_Str;

extern CaptureTim_Str Tim3_CapData;


extern unsigned short int TIM3_RiseEdge_Count;       //tim3�������ֵ:������~�����أ�������
extern unsigned short int TIM3_FallEdge_Count;       //tim3�������ֵ:������~�����أ����ߵ�ƽʱ��
extern unsigned int TIM2_RiseEdge_Count;       //tim2�������ֵ:������~�����أ�������
extern unsigned int TIM2_FallEdge_Count;       //tim2�������ֵ:������~�����أ����ߵ�ƽʱ��

extern float   Tim3_Duty;                  //tim3������PWM��ռ�ձ�
extern float   Tim2_Duty;                  //tim2������PWM��ռ�ձ�
extern float   Tim3_Fre;                   //tim3������PWM��Ƶ��
extern float   Tim2_Fre;                   //tim2������PWM��Ƶ��

extern void  CaptureTim3_Start(void);

#endif
