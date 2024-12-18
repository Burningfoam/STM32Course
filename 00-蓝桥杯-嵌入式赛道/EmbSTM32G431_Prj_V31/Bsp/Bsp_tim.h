#ifndef __BSP_TIM_H
#define __BSP_TIM_H

extern unsigned int TIM3_RiseEdge_Count;       //tim3捕获计数值:上升沿~上升沿，即周期
extern unsigned int TIM3_FallEdge_Count;       //tim3捕获计数值:上升沿~下升沿，即高电平时间
extern unsigned int TIM2_RiseEdge_Count;       //tim2捕获计数值:上升沿~上升沿，即周期
extern unsigned int TIM2_FallEdge_Count;       //tim2捕获计数值:上升沿~下升沿，即高电平时间
extern float   Tim3_Duty;                  //tim3测量的PWM的占空比
extern float   Tim2_Duty;                  //tim2测量的PWM的占空比
extern float   Tim3_Fre;                   //tim3测量的PWM的频率
extern float   Tim2_Fre;                   //tim2测量的PWM的频率
#endif
