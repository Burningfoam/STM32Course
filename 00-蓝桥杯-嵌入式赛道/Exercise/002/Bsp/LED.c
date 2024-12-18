#include "LED.h"
#include "main.h"
#include "gpio.h"

//ȫ�ֱ���������LED״̬��8λ���ݶ�Ӧ8��LED��LED_State.0-LED1���Դ����ƣ���0-��1-��
//������������4��LED:ִ��LED_State=LED_State|0x08;Ȼ�����LED_Control(LED_State);
//������Ϩ���4��LED:ִ��LED_State=LED_State&0xF7;Ȼ�����LED_Control(LED_State);
//��������ת��4��LED:ִ��LED_State=LED_State^0x08;Ȼ�����LED_Control(LED_State);

unsigned char LED_State=0;  
/****************************************************
��������:
��    ��: ����ledstate������0��ӦLED��1��LEDӦ����
          ��ΪLED��LCD���ã�����Ҫ����ȫ�ֱ��������浱ǰLED��״̬��
          �ǵ��ڳ�ʼ��ʱ��Ҫ��ȫ��Ϩ��LED��
��ڲ���: ledstate��8λ���ݶ�Ӧ8��LED��ledstate.0-LED1���Դ����ƣ���0-��1-��
���ڲ���:
****************************************************/
void LED_Control(unsigned char ledstate)
{
    //��Ϊ����LED��LCD�ܽŵĸ���(PC8~PC15)������PC8~15��ͨ��D����������LED��
    //1����ҪPD2����D�������Ĵ����źţ�2����Ҫȫ�ֱ�������LED�ĵ�ǰ״̬
    //ȫ���ر�LED��������֤ledstate��0��Ӧ��LED��    
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);    
    
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
    
    //����ledstate8λ������1��Ӧ��LED��
    HAL_GPIO_WritePin(GPIOC,ledstate<<8,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}
