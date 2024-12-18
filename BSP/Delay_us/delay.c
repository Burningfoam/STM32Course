#include "delay.h"

// ΢���ӳٺ�����ʹ��TIM7��ʱ��
void delay_us(uint32_t us) 
{
    uint32_t startTick = __HAL_TIM_GET_COUNTER(&htim7);  // ��ȡ��ǰ������ֵ
    uint32_t endTick = startTick + us;                    // �����������ֵ
	
    // ������������������
    if (endTick >= 65536) 
    {
        endTick -= 65536; // ����Ϊ������Ŀ��ֵ
    }
		
    // �ȴ��������ﵽĿ��ֵ
    while (__HAL_TIM_GET_COUNTER(&htim7) != endTick) 
    {
        // ��ѭ����ֱ���������ﵽĿ��ֵ
    }
}
