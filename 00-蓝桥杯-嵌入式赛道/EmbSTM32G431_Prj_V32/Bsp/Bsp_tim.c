/*******************************************************************************
* �ļ����ƣ�
* ���߰汾��hufq/V1.0
* ����������1. CT117E_M4ʵ��������ö�ʱ����ʹ��:������ʱ��TIM6��TIM2��TIM3�Ĳ���ʱ��ʹ��
* ����˵����1. TIM6�Ļ���ģʽʹ��           
             (1)���ܣ�ʵ��100ms�Ķ�ʱ����DA��ͬʱ��ΪDA2�Զ�������ǲ��Ĵ���������
             (2)��Ҫ��main������ʼ���˹�������ʱ����
             (3) ��cubeMX��TIM6�����У�
                prescaler(��Ƶ)Ϊ8000��������Ƶ80M��ǰ���£���ʱʱ��Ƶ��Ϊ10K(��ʱ��ԪΪ0.1ms)
                Counter Period(��������)Ϊ1000������ʱ�ж�����Ϊ100ms
                
            2. TIM2��TIM3�Ĳ���ģʽʹ��            
             (1)���ܣ�����PA15�ܽ�(TIM2_CH1)��PB4�ܽ�(TIM3_CH1)�����������Ƶ�ʺ�ռ�ձȣ�
             (2)cubeMX�����òο���https://blog.csdn.net/jx5631833/article/details/126563264 
             (3)��Ҫע����ǣ�
                TIM2,3�ķ�Ƶ������80������Ƶ80M��ǰ���£���ʱ��ʱ��Ϊ1M����ʱ��ԪΪ1us����
                TIM2,3�ļ���������65535�������õ����ֵ������������������65.535ms����СƵ��Ϊ15Hz�����������Ƶ��Ϊ1M��
                GPIO��ΪPA15�ܽ�(TIM2_CH1)��PB4�ܽ�(TIM3_CH1)������ο���STM32G431RB�����ֲ�.pdf��
                ��Ҫ��main������ʼ���˹�������ʱ��������3��������
                
            3. TIM4�ıȽ�ģʽʹ�ã����PWM��ʱ���Ƽ�������PWMģʽ��ռ�ã� 
             (1)���ܣ���PA11(TIM4CH1)�������10ms(Ƶ��100Hz)��ռ�ձ�10%�����壬��PA12(TIM4CH2)�������5ms(Ƶ��200Hz)��ռ�ձ�40%������
             (2)cubeMX�����òο�(�����Ƚ�ģʽ��PWMģʽ��ԭ�������)��https://blog.csdn.net/weixin_47042449/article/details/122619370 
             (3)��Ҫע����޸ĵ��ǣ�
                TIM4CH1_PA11_Pulse=10000���Ƚ������CCRֵ�����ڣ�TIM4����Ƶ��Ƶ����1M����������10ms(Ƶ��100Hz)�������Ҫ�ı����ڣ���ֱ���޸ĸñ������ɣ�
                TIM4CH1_PA11_Duty=10�����Ƚ������ռ�ձȣ�1~99 �������Ҫ�ı�ռ�ձȣ���ֱ���޸ĸñ������ɣ�
                ��cubeMX����Ҫ����TIM4�жϣ����Բ�����ÿ��ͨ����Pulseֵ������ʹ���Զ�����
                ��Ҫ��main������ʼ���˹�������ʱ��������2��������
                
            4. TIM15��PWMģʽʹ��            
             (1)���ܣ���PA2(TIM15CH1)�������1ms��ռ�ձ�20%��PWM,PA3(TIM15CH2)�������1ms��ռ�ձ�70%��PWM
             (2)cubeMX�����òο�(�����Ƚ�ģʽ��PWMģʽ��ԭ�������)��https://blog.csdn.net/weixin_47042449/article/details/122619370 
             (3)��Ҫע����޸ĵ��ǣ�
                ��cubeMX�У������Զ�װ�أ���Ҫ�����жϣ�ȷ��IO��ΪPA2,PA3
                TIM15��prescaler(��Ƶ)Ϊ80(����ʱ��ʱ��1M),ARR��Ϊ1000��������Ϊ1ms,Ƶ��1K;
                TIM15CH1_PA2��Pulse��Ϊ200����ռ�ձ�Ϊ20%��
                TIM15CH2_PA3��Pulse��Ϊ700����ռ�ձ�Ϊ70%��
                ��Ҫ��main������ʼ���˹�������ʱ����2��������
                �����Ҫ�޸����ں�ռ�ձȣ��������������������
                __HAL_TIM_SET_AUTORELOAD(&htim15,TIM15_PWM_Period); //�޸�����
                __HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,TIM15_PWM_Period*TIM15CH1_PA2_Duty/100); //�޸�ռ�ձ�
                ���б�����TIM15_PWM_PeriodΪ���ڣ�1~65535��
                          TIM15CH1_PA2_DutyΪWM�����ռ�ձȣ�1~99 

* Ӳ��������CT117E_M4
* ע�����
* ���߰汾��hufq/V2.0
            1���޸���V1.0�汾�ϲ������Ƶ���Ըߵ��źŵ��³��򿨵����⣻ԭ�����ڼ���Ƶ�ʺ�ռ�ձ�ͬʱ���½����жϣ������ʱ�����µģ����԰�Ƶ�ʷ����������ж�ʱ����
               �����ڱ��汾�в��������Ƶ��ж������ʾƵ�ʺ�ռ�ձ�ʱ�ټ��㣻
            2���޸�TIM3������������ź�ʱ����������ͨ����CH1(������)��CH2���½��أ���������һ��ͨ������һ��ͨ����ռ���޷�ʹ��ʱ�����в���������ԭ�����£�
               ��1���ȳ�ʼ�������������жϣ�
               ��2���ڽ��������ز����жϺ����������½����жϣ� ��ȡ����ֵ����ֵΪ����ʱ�䣻
               ��2���ڽ����½��ز����жϺ������������жϣ���ȡ����ֵ����ֵΪ�ߵ�ƽ����ʱ�䣻
               ��3���ٴν����������жϺ����־λ�����¿�ʼ ��
               ��4��trigger sourceѡ��ΪTI1FP1;slave mode����ΪReset Mode�󣬼�⵽���غ�ʱ������ֵ�Ḵλ����
*******************************************************************************/
#include "Bsp_tim.h"
#include "main.h"
#include "tim.h"
#include "Bsp_LED.h"

CaptureTim_Str Tim3_CapData;
unsigned short int TIM3_RiseEdge_Count; //tim3�������ֵ:������~�����أ�������
unsigned short int TIM3_FallEdge_Count; //tim3�������ֵ:������~�����أ����ߵ�ƽʱ��
unsigned  int TIM2_RiseEdge_Count;      //tim2�������ֵ:������~�����أ�������
unsigned  int TIM2_FallEdge_Count;      //tim2�������ֵ:������~�����أ����ߵ�ƽʱ��
float   Tim3_Duty=0.0;                  //tim3������PWM��ռ�ձ�
float   Tim2_Duty=0.0;                  //tim2������PWM��ռ�ձ�
float   Tim3_Fre=0.0;                   //tim3������PWM��Ƶ��
float   Tim2_Fre=0.0;                   //tim2������PWM��Ƶ��

unsigned int TIM4CH1_PA11_Pulse=10000;  //TIM4_CH1�ıȽ������CCRֵ����ʵ�������ڣ�TIM4����Ƶ��Ƶ����1M����������10ms(Ƶ��100Hz)
unsigned int TIM4CH2_PA12_Pulse=5000;   //TIM4_CH2�ıȽ������CCRֵ����ʵ�������ڣ�TIM4����Ƶ��Ƶ����1M����������5ms(Ƶ��200Hz)
unsigned int TIM4CH1_PA11_Duty=10;      //TIM4_CH1�ıȽ������ռ�ձȣ�1~99 
unsigned int TIM4CH2_PA12_Duty=40;      //TIM4_CH2�ıȽ������ռ�ձȣ�1~99 

unsigned int TIM15_PWM_Period;          //TIM15��PWM��������ڣ�1~65535
unsigned int TIM15CH1_PA2_Duty;         //TIM15_CH1��PWM�����ռ�ձȣ�1~99 
unsigned int TIM15CH2_PA3_Duty;         //TIM15_CH2��PWM�����ռ�ձȣ�1~99 

/****************************************************
��������:
��    ��: ������ʱ�жϵĻص�����������������stm32g4xx_hal_tim.c�ж���Ϊweak������
          1����ʱ�жϣ�����ʹ�ñ��ص�����������ü�switch���ֲ�ͬ�Ķ�ʱ���жϣ�
          2�����ص�������������жϱ�־���㣨ϵͳ�ж���Ӧ�����Ѿ������
��ڲ���: ��
���ڲ���:
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
��������:
��    ��: ��ͨ������ʱ��TIM3���������� 
��ڲ���: ��
���ڲ���:
****************************************************/
void  CaptureTim3_Start(void)
{
    Tim3_CapData.State_flag=0;    
	__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING); //�������ش���
//    __HAL_TIM_SET_COUNTER(&htim3,0);                    //�൱��	TIM3->CNT = 0; ����ʱ������ֵ����
    TIM3->CNT = 0;
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);          //����      
}


/****************************************************
��������:
��    ��: ����ʱ�жϵĻص�����������������stm32g4xx_hal_tim.c�ж���Ϊweak������
          1����ʱ�жϣ�����ʹ�ñ��ص�����������ü�switch���ֲ�ͬ�Ķ�ʱ���жϣ�
          2�����ص�������������жϱ�־���㣨ϵͳ�ж���Ӧ�����Ѿ������
��ڲ���: ��
���ڲ���:
****************************************************/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance ==TIM2)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            TIM2_RiseEdge_Count = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1)+1;  
            //Ƶ�ʼ�������Ǻ�ռ�ձ�һ�����ģ�����������ʱ̫�������²����Ը�Ƶ���ź�ʱ�ᵼ�½��ж�Ƶ���Ӷ�Ӱ�����������У���Ų���˴���
////            Tim2_Fre=1/(TIM2_RiseEdge_Count*0.000001*1000);   //����Ƶ��(KHz)�� 0.000001��ָ������Ԫʱ��1ms(TIM2����Ƶ��1M)  
//            Tim2_Fre=1000/(float)(TIM2_RiseEdge_Count);   //����Ƶ��(KHz)�� 0.000001��ָ������Ԫʱ��1ms(TIM2����Ƶ��1M)             
        }       
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            TIM2_FallEdge_Count = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2)+1;            
//            Tim2_Duty=(float)TIM2_FallEdge_Count/TIM2_RiseEdge_Count;
//            Tim2_Duty=Tim2_Duty*100;                        //����ռ�ձ�
        }
    }
    
//    if(htim->Instance ==htim3.Instance)
    if(htim->Instance ==TIM3)
    {
//        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
//        {
//            TIM3_RiseEdge_Count = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1; 
//            //Ƶ�ʼ�������Ǻ�ռ�ձ�һ�����ģ�����������ʱ̫�������²����Ը�Ƶ���ź�ʱ�ᵼ�½��ж�Ƶ���Ӷ�Ӱ�����������У���Ų���˴���  
//////            Tim3_Fre=1/(TIM3_RiseEdge_Count*0.000001*1000);   //����Ƶ��(KHz)�� 0.000001��ָ������Ԫʱ��1ms(TIM3����Ƶ��1M)   
////            Tim3_Fre=1000/(float)(TIM3_RiseEdge_Count);   //����Ƶ��(KHz)�� 0.000001��ָ������Ԫʱ��1ms(TIM3����Ƶ��1M)           
//        }        
//        else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
//        {
//            TIM3_FallEdge_Count = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2)+1;            
////            Tim3_Duty=(float)TIM3_FallEdge_Count/TIM3_RiseEdge_Count;
////            Tim3_Duty=Tim3_Duty*100;                        //����ռ�ձ�
//        }
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(Tim3_CapData.State_flag==0)
            {
                Tim3_CapData.State_flag=1;                    
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
//                Tim3_CapData.First_Rising_T1 = TIM3->CCR1; 
                Tim3_CapData.First_Rising_T1 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1;  //�͵�ƽ����ʱ��
                TIM3_RiseEdge_Count=Tim3_CapData.First_Rising_T1+TIM3_FallEdge_Count;               //�ߵ͵�ƽʱ��֮�ͣ�������
            }
            else
            {
                Tim3_CapData.State_flag=0;  
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
//                Tim3_CapData.First_Falling_T2 = TIM3->CCR1;     
                Tim3_CapData.First_Falling_T2 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1; 
                TIM3_FallEdge_Count= Tim3_CapData.First_Falling_T2;                 //�ߵ�ƽ����ʱ��  
            }
        }
//        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
//        {                        
//            switch(Tim3_CapData.State_flag)
//            {
//                case 0:             //�����ж�
//                    Tim3_CapData.State_flag++;                    
//                    __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
//                    Tim3_CapData.First_Rising_T1 = TIM3->CCR1; 
////                    Tim3_CapData.First_Rising_T1 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1; 
//                     TIM3_RiseEdge_Count=Tim3_CapData.First_Rising_T1+TIM3_FallEdge_Count;  //�������ؼ��ʱ�䣬������
//                    break;
//                case 1:             //�����ж�
//                    Tim3_CapData.State_flag=0;  
//                    __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
//                    Tim3_CapData.First_Falling_T2 = TIM3->CCR1;     
////                    Tim3_CapData.First_Falling_T2 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1; 
//                    TIM3_FallEdge_Count= Tim3_CapData.First_Falling_T2;    //�ߵ�ƽ����ʱ��  
//                    break;
////                case 2 :             //�����ж�
//                default:                //�ٴΣ������жϣ����¿�ʼ
//                    Tim3_CapData.State_flag=0; 
////                    __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
////                    Tim3_CapData.Second_Rising_T3 = TIM3->CCR1; 
//                    break;
////                default:             //�����ж�,����ֵ
////                     Tim3_CapData.State_flag=0;    
//////                    __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING); //�������ش���
//////                    __HAL_TIM_SET_COUNTER(&htim3,0);                    //�൱��	TIM3->CNT = 0; ����ʱ������ֵ����
//////                    TIM3->CNT = 0;  
//////                    TIM3_RiseEdge_Count= Tim3_CapData.Second_Rising_T3-Tim3_CapData.First_Rising_T1;  //�������ؼ��ʱ�䣬������                 
//////                    TIM3_FallEdge_Count= Tim3_CapData.First_Falling_T2-Tim3_CapData.First_Rising_T1;    //�ߵ�ƽ����ʱ��    
////                    break;                
//            }
//        }      
    }
}

/****************************************************
��������:
��    ��: �Ƚ϶�ʱ�жϵĻص�����������������stm32g4xx_hal_tim.c�ж���Ϊweak������
          1����ʱ�жϣ�����ʹ�ñ��ص�����������ü�switch���ֲ�ͬ�Ķ�ʱ���жϣ�
          2�����ص�������������жϱ�־���㣨ϵͳ�ж���Ӧ�����Ѿ������
��ڲ���: ��
���ڲ���:
****************************************************/
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	unsigned int  OC_Count=0;
    OC_Count= __HAL_TIM_GET_COUNTER(htim);      //ȡ�õ�ǰ��ʱ������ֵ
    if(htim->Instance == TIM4)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
//			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1)+250);//2khz��ÿ�μ���ͬ��ֵ��ռ�ձ���Ϊ50%
            //��PA11�������10ms(Ƶ��100Hz)��ռ�ձ�10%������
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
//            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2)+500);//1khz��ÿ�μ���ͬ��ֵ��ռ�ձ���Ϊ50%
            //��PA12�������5ms(Ƶ��200Hz)��ռ�ձ�40%������
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

