/*******************************************************************************
* 文件名称：
* 作者版本：hufq/V1.0
* 功能描述：1. CT117E_M4实验板上内置定时器的使用:基本定时器TIM6和TIM2、TIM3的捕获定时器使用
* 配置说明：1. TIM6的基本模式使用           
             (1)功能：实现100ms的定时；在DA中同时作为DA2自动输出三角波的触发条件；
             (2)需要在main函数初始化人工启动定时器；
             (3) 在cubeMX的TIM6设置中：
                prescaler(分频)为8000，即在主频80M的前提下，定时时钟频率为10K(计时单元为0.1ms)
                Counter Period(计数周期)为1000，即计时中断周期为100ms
                
            2. TIM2、TIM3的捕获模式使用            
             (1)功能：测量PA15管脚(TIM2_CH1)，PB4管脚(TIM3_CH1)上输入脉冲的频率和占空比；
             (2)cubeMX的设置参考：https://blog.csdn.net/jx5631833/article/details/126563264 
             (3)需要注意的是：
                TIM2,3的分频参数是80，即主频80M的前提下，定时器时钟为1M（计时单元为1us）；
                TIM2,3的计数周期是65535（能设置的最大值），则测量周期最大是65.535ms，最小频率为15Hz，而理论最大频率为1M；
                GPIO改为PA15管脚(TIM2_CH1)，PB4管脚(TIM3_CH1)，这个参考《STM32G431RB数据手册.pdf》
                需要在main函数初始化人工启动定时器，包括3个函数；
                
            3. TIM4的比较模式使用（输出PWM波时不推荐，除非PWM模式被占用） 
             (1)功能：在PA11(TIM4CH1)输出周期10ms(频率100Hz)，占空比10%的脉冲，在PA12(TIM4CH2)输出周期5ms(频率200Hz)，占空比40%的脉冲
             (2)cubeMX的设置参考(包括比较模式和PWM模式的原理和区别)：https://blog.csdn.net/weixin_47042449/article/details/122619370 
             (3)需要注意和修改的是：
                TIM4CH1_PA11_Pulse=10000，比较输出的CCR值即周期，TIM4的主频分频后是1M，故周期是10ms(频率100Hz)，如果需要改变周期，即直接修改该变量即可；
                TIM4CH1_PA11_Duty=10，即比较输出的占空比：1~99 ，如果需要改变占空比，即直接修改该变量即可；
                在cubeMX中需要允许TIM4中断，可以不设置每个通道的Pulse值，不需使能自动重载
                需要在main函数初始化人工启动定时器，包括2个函数；
                
            4. TIM15的PWM模式使用            
             (1)功能：在PA2(TIM15CH1)输出周期1ms，占空比20%的PWM,PA3(TIM15CH2)输出周期1ms，占空比70%的PWM
             (2)cubeMX的设置参考(包括比较模式和PWM模式的原理和区别)：https://blog.csdn.net/weixin_47042449/article/details/122619370 
             (3)需要注意和修改的是：
                在cubeMX中，设置自动装载，不要开启中断，确认IO口为PA2,PA3
                TIM15的prescaler(分频)为80(即定时器时钟1M),ARR设为1000，即周期为1ms,频率1K;
                TIM15CH1_PA2的Pulse设为200，即占空比为20%；
                TIM15CH2_PA3的Pulse设为700，即占空比为70%；
                需要在main函数初始化人工启动定时器，2个函数；
                如果需要修改周期和占空比，则调用以下两个函数：
                __HAL_TIM_SET_AUTORELOAD(&htim15,TIM15_PWM_Period); //修改周期
                __HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,TIM15_PWM_Period*TIM15CH1_PA2_Duty/100); //修改占空比
                其中变量：TIM15_PWM_Period为周期：1~65535；
                          TIM15CH1_PA2_Duty为WM输出的占空比：1~99 

* 硬件环境：CT117E_M4
* 注意事项：
* 作者版本：hufq/V2.0
            1、修改了V1.0版本上捕获测量频率稍高的信号导致程序卡的问题；原因是在计算频率和占空比同时在下降沿中断，计算耗时长导致的，可以把频率放在上升沿中断时计算
               或者在本版本中采用整体移到中断外的显示频率和占空比时再计算；
            2、修改TIM3捕获测量脉冲信号时，用了两个通道（CH1(上升沿)和CH2（下降沿）），改用一个通过（在一个通道被占用无法使用时）进行测量，测量原理如下：
               （1）先初始化设置上升沿中断，
               （2）在进入上升沿捕获中断函数后，设置下降沿中断， 读取捕获值，该值为周期时间；
               （2）在进入下降沿捕获中断后，设置上升沿中断，读取捕获值，该值为高电平持续时间；
               （3）再次进入上升沿中断后，清标志位，重新开始 ；
               （4）trigger source选择为TI1FP1;slave mode设置为Reset Mode后，检测到边沿后定时器计数值会复位清零
*******************************************************************************/
#include "Bsp_tim.h"
#include "main.h"
#include "tim.h"
#include "Bsp_LED.h"

CaptureTim_Str Tim3_CapData;
unsigned short int TIM3_RiseEdge_Count; //tim3捕获计数值:上升沿~上升沿，即周期
unsigned short int TIM3_FallEdge_Count; //tim3捕获计数值:上升沿~下升沿，即高电平时间
unsigned  int TIM2_RiseEdge_Count;      //tim2捕获计数值:上升沿~上升沿，即周期
unsigned  int TIM2_FallEdge_Count;      //tim2捕获计数值:上升沿~下升沿，即高电平时间
float   Tim3_Duty=0.0;                  //tim3测量的PWM的占空比
float   Tim2_Duty=0.0;                  //tim2测量的PWM的占空比
float   Tim3_Fre=0.0;                   //tim3测量的PWM的频率
float   Tim2_Fre=0.0;                   //tim2测量的PWM的频率

unsigned int TIM4CH1_PA11_Pulse=10000;  //TIM4_CH1的比较输出的CCR值，其实就是周期，TIM4的主频分频后是1M，故周期是10ms(频率100Hz)
unsigned int TIM4CH2_PA12_Pulse=5000;   //TIM4_CH2的比较输出的CCR值，其实就是周期，TIM4的主频分频后是1M，故周期是5ms(频率200Hz)
unsigned int TIM4CH1_PA11_Duty=10;      //TIM4_CH1的比较输出的占空比：1~99 
unsigned int TIM4CH2_PA12_Duty=40;      //TIM4_CH2的比较输出的占空比：1~99 

unsigned int TIM15_PWM_Period;          //TIM15的PWM输出的周期：1~65535
unsigned int TIM15CH1_PA2_Duty;         //TIM15_CH1的PWM输出的占空比：1~99 
unsigned int TIM15CH2_PA3_Duty;         //TIM15_CH2的PWM输出的占空比：1~99 

/****************************************************
函数名称:
功    能: 基本定时中断的回调函数，函数名字在stm32g4xx_hal_tim.c中定义为weak函数；
          1、定时中断，都是使用本回调函数，故最好加switch区分不同的定时器中断；
          2、本回调函数中无需对中断标志清零（系统中断响应函数已经清过）
入口参数: 无
出口参数:
****************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance ==htim6.Instance)
    {
//        LED_State=~LED_State;
        LED_State=LED_State^0x80;
        LED_Control(LED_State);
    } 
}


/****************************************************
函数名称:
功    能: 单通道捕获定时器TIM3的启动函数 
入口参数: 无
出口参数:
****************************************************/
void  CaptureTim3_Start(void)
{
    Tim3_CapData.State_flag=0;    
	__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING); //设置上沿触发
//    __HAL_TIM_SET_COUNTER(&htim3,0);                    //相当于	TIM3->CNT = 0; 即定时器计数值清零
    TIM3->CNT = 0;
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);          //启动      
}


/****************************************************
函数名称:
功    能: 捕获定时中断的回调函数，函数名字在stm32g4xx_hal_tim.c中定义为weak函数；
          1、定时中断，都是使用本回调函数，故最好加switch区分不同的定时器中断；
          2、本回调函数中无需对中断标志清零（系统中断响应函数已经清过）
入口参数: 无
出口参数:
****************************************************/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance ==TIM2)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            TIM2_RiseEdge_Count = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1)+1;  
            //频率计算最初是和占空比一起计算的，这样由于用时太长，导致测量稍高频率信号时会导致进中断频繁从而影响主程序运行，故挪到此处；
////            Tim2_Fre=1/(TIM2_RiseEdge_Count*0.000001*1000);   //计算频率(KHz)： 0.000001是指计数单元时间1ms(TIM2的主频是1M)  
//            Tim2_Fre=1000/(float)(TIM2_RiseEdge_Count);   //计算频率(KHz)： 0.000001是指计数单元时间1ms(TIM2的主频是1M)             
        }       
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            TIM2_FallEdge_Count = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2)+1;            
//            Tim2_Duty=(float)TIM2_FallEdge_Count/TIM2_RiseEdge_Count;
//            Tim2_Duty=Tim2_Duty*100;                        //计算占空比
        }
    }
    
//    if(htim->Instance ==htim3.Instance)
    if(htim->Instance ==TIM3)
    {
//        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
//        {
//            TIM3_RiseEdge_Count = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1; 
//            //频率计算最初是和占空比一起计算的，这样由于用时太长，导致测量稍高频率信号时会导致进中断频繁从而影响主程序运行，故挪到此处；  
//////            Tim3_Fre=1/(TIM3_RiseEdge_Count*0.000001*1000);   //计算频率(KHz)： 0.000001是指计数单元时间1ms(TIM3的主频是1M)   
////            Tim3_Fre=1000/(float)(TIM3_RiseEdge_Count);   //计算频率(KHz)： 0.000001是指计数单元时间1ms(TIM3的主频是1M)           
//        }        
//        else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
//        {
//            TIM3_FallEdge_Count = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2)+1;            
////            Tim3_Duty=(float)TIM3_FallEdge_Count/TIM3_RiseEdge_Count;
////            Tim3_Duty=Tim3_Duty*100;                        //计算占空比
//        }
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(Tim3_CapData.State_flag==0)
            {
                Tim3_CapData.State_flag=1;                    
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
//                Tim3_CapData.First_Rising_T1 = TIM3->CCR1; 
                Tim3_CapData.First_Rising_T1 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1;  //低电平持续时间
                TIM3_RiseEdge_Count=Tim3_CapData.First_Rising_T1+TIM3_FallEdge_Count;               //高低电平时间之和，即周期
            }
            else
            {
                Tim3_CapData.State_flag=0;  
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
//                Tim3_CapData.First_Falling_T2 = TIM3->CCR1;     
                Tim3_CapData.First_Falling_T2 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1; 
                TIM3_FallEdge_Count= Tim3_CapData.First_Falling_T2;                 //高电平持续时间  
            }
        }
//        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
//        {                        
//            switch(Tim3_CapData.State_flag)
//            {
//                case 0:             //上沿中断
//                    Tim3_CapData.State_flag++;                    
//                    __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
//                    Tim3_CapData.First_Rising_T1 = TIM3->CCR1; 
////                    Tim3_CapData.First_Rising_T1 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1; 
//                     TIM3_RiseEdge_Count=Tim3_CapData.First_Rising_T1+TIM3_FallEdge_Count;  //两个上沿间隔时间，即周期
//                    break;
//                case 1:             //下沿中断
//                    Tim3_CapData.State_flag=0;  
//                    __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
//                    Tim3_CapData.First_Falling_T2 = TIM3->CCR1;     
////                    Tim3_CapData.First_Falling_T2 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1; 
//                    TIM3_FallEdge_Count= Tim3_CapData.First_Falling_T2;    //高电平持续时间  
//                    break;
////                case 2 :             //上沿中断
//                default:                //再次，上沿中断，重新开始
//                    Tim3_CapData.State_flag=0; 
////                    __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
////                    Tim3_CapData.Second_Rising_T3 = TIM3->CCR1; 
//                    break;
////                default:             //上沿中断,计算值
////                     Tim3_CapData.State_flag=0;    
//////                    __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING); //设置上沿触发
//////                    __HAL_TIM_SET_COUNTER(&htim3,0);                    //相当于	TIM3->CNT = 0; 即定时器计数值清零
//////                    TIM3->CNT = 0;  
//////                    TIM3_RiseEdge_Count= Tim3_CapData.Second_Rising_T3-Tim3_CapData.First_Rising_T1;  //两个上沿间隔时间，即周期                 
//////                    TIM3_FallEdge_Count= Tim3_CapData.First_Falling_T2-Tim3_CapData.First_Rising_T1;    //高电平持续时间    
////                    break;                
//            }
//        }      
    }
}

/****************************************************
函数名称:
功    能: 比较定时中断的回调函数，函数名字在stm32g4xx_hal_tim.c中定义为weak函数；
          1、定时中断，都是使用本回调函数，故最好加switch区分不同的定时器中断；
          2、本回调函数中无需对中断标志清零（系统中断响应函数已经清过）
入口参数: 无
出口参数:
****************************************************/
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	unsigned int  OC_Count=0;
    OC_Count= __HAL_TIM_GET_COUNTER(htim);      //取得当前定时器计数值
    if(htim->Instance == TIM4)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
//			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1)+250);//2khz，每次加相同的值，占空比则为50%
            //在PA11输出周期10ms(频率100Hz)，占空比10%的脉冲
            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA , GPIO_PIN_11))   //TIM4_CH1:PA11
            {
                __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1,OC_Count+(TIM4CH1_PA11_Pulse-TIM4CH1_PA11_Pulse*TIM4CH1_PA11_Duty/100));
            }
            else
            {
                __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1,OC_Count+TIM4CH1_PA11_Pulse*TIM4CH1_PA11_Duty/100);
            }
        }
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {			
//            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2)+500);//1khz，每次加相同的值，占空比则为50%
            //在PA12输出周期5ms(频率200Hz)，占空比40%的脉冲
            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA , GPIO_PIN_12))   //TIM4_CH2:PA12
            {
                __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,OC_Count+(TIM4CH2_PA12_Pulse-TIM4CH2_PA12_Pulse*TIM4CH2_PA12_Duty/100));
            }
            else
            {
                __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,OC_Count+TIM4CH2_PA12_Pulse*TIM4CH2_PA12_Duty/100);
            }
        }
	}
}

