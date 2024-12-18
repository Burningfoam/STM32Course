/*******************************************************************************
* 文件名称：
* 功能描述：1、CT117E_M4实验板上STM32内置DA的操作，DAC_CH1_PA4，DAC_CH2_PA5
            2、DAC_CH1_PA4输出指定电压，cubeMX中的设置说明如下；
                (1)OUT1 MODE中选择“connected to external pin only”，然后确认IO口为PA4即可；
                (2)需要main初始化后人工调用启动DAC函数；
                (3)有网帖说不推荐output buffer使能，因为缓冲器打开时最低的DAC输出电压大概0.2V； 
                
            3、DAC_CH2_PA5自动输出三角波，cubeMX中的设置说明如下；
                (1)trigger用定时TIM6触发，波形选择三角波。
                (2)定时器TIM6设定好分频和周期，无需自动重装载，可以选择不开中断，trigger event 选update Event
                (3)TIM6频率太低，三角波可能不显示，频率太高则影响LCD的显示(在LCD屏左侧出现像素点，类乱码)，原因待定；
* 配置说明：
* 硬件环境：CT117E_M4
* 注意事项：
* 作者版本：hufq/V1.0
*******************************************************************************/
#include "Bsp_DAC.h"
#include "dac.h"

float DAC1_CH1_Vol=2.5;     //DAC1_CH1的输出电压值

/****************************************************
函数名称:
功    能: 设置DCA1_CH1_PA4输出电压
入口参数: 无
出口参数:
****************************************************/
void DAC1_CH1_Set_Vol(float vol)
{
	unsigned int temp;	
	temp = (4096*vol/3.3f);	
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);
    
}

