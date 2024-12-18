#ifndef __BSP_TIM_H
#define __BSP_TIM_H

typedef struct{
	unsigned short int First_Rising_T1,First_Falling_T2;
	unsigned char State_flag;       //0-初始状态，准备捕获第一个上升沿；1-准备捕获第一个下升沿；2-准备捕获第二个上升沿
}CaptureTim_Str;

extern CaptureTim_Str Tim3_CapData;


extern unsigned short int TIM3_RiseEdge_Count;       //tim3捕获计数值:上升沿~上升沿，即周期
extern unsigned short int TIM3_FallEdge_Count;       //tim3捕获计数值:上升沿~下升沿，即高电平时间
extern unsigned int TIM2_RiseEdge_Count;       //tim2捕获计数值:上升沿~上升沿，即周期
extern unsigned int TIM2_FallEdge_Count;       //tim2捕获计数值:上升沿~下升沿，即高电平时间

extern float   Tim3_Duty;                  //tim3测量的PWM的占空比
extern float   Tim2_Duty;                  //tim2测量的PWM的占空比
extern float   Tim3_Fre;                   //tim3测量的PWM的频率
extern float   Tim2_Fre;                   //tim2测量的PWM的频率

extern void  CaptureTim3_Start(void);

#endif
