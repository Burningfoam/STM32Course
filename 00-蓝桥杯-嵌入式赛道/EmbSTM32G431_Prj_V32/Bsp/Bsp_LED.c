/*******************************************************************************
* �ļ����ƣ�
* ����������1. CT117E_M4ʵ�����LED�Ĳ���
* ����˵����
* Ӳ��������CT117E_M4
* ע�����
* ���߰汾��hufq/V1.0
*******************************************************************************/
#include "Bsp_LED.h"
#include "main.h"
#include "gpio.h"

//ȫ�ֱ���������LED״̬��8λ���ݶ�Ӧ8��LED��LED_State.0-LED1���Դ����ƣ���0-��1-��
//����������LD1:ִ��LED_State=LED_State|0x01;Ȼ�����LED_Control(LED_State);LD1~LD7��Ӧ�������ǣ�0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
//������Ϩ��LD1:ִ��LED_State=LED_State&0xFE;Ȼ�����LED_Control(LED_State);LD1~LD7��Ӧ�������ǣ�0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F
//��������תLD1:ִ��LED_State=LED_State^0x01;Ȼ�����LED_Control(LED_State);LD1~LD7��Ӧ�������ǣ�0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
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


///****************************************************
//��������:
//��    ��: ����LED��
//��ڲ���: lednum��0-ȫ����1-LED1����2-LED2�����Դ�����
//���ڲ���:
//****************************************************/
//void LED_ON(unsigned char lednum)
//{
//    //��Ϊ����LED��LCD�ܽŵĸ���(PC8~PC15)������PC8~15��ͨ��D����������LED��
//    //1����ҪPD2����D�������Ĵ����źţ�2����Ҫȫ�ֱ�������LED�ĵ�ǰ״̬
//    switch(lednum)
//    {
//    case 0:
//        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_RESET);                   //ȫ���ͣ�ȫ����ע����߰�PC0~PC7Ҳ������
//        LED_State=0;                                                            //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 1:
//        HAL_GPIO_WritePin(LED1_LCD8_GPIO_Port,LED1_LCD8_Pin,GPIO_PIN_RESET);    //LED1��
//        LED_State=LED_State&0xFE;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 2:
//        HAL_GPIO_WritePin(LED2_LCD9_GPIO_Port,LED2_LCD9_Pin,GPIO_PIN_RESET);    //LED2��
//        LED_State=LED_State&0xFD;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 3:
//        HAL_GPIO_WritePin(LED3_LCD10_GPIO_Port,LED3_LCD10_Pin,GPIO_PIN_RESET);  //LED3��
//        LED_State=LED_State&0xFB;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 4:
//        HAL_GPIO_WritePin(LED4_LCD11_GPIO_Port,LED4_LCD11_Pin,GPIO_PIN_RESET);  //LED4��
//        LED_State=LED_State&0xF7;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 5:
//        HAL_GPIO_WritePin(LED5_LCD12_GPIO_Port,LED5_LCD12_Pin,GPIO_PIN_RESET);  //LED5��
//        LED_State=LED_State&0xEF;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 6:
//        HAL_GPIO_WritePin(LED6_LCD13_GPIO_Port,LED6_LCD13_Pin,GPIO_PIN_RESET);  //LED6��
//        LED_State=LED_State&0xDF;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 7:
//        HAL_GPIO_WritePin(LED7_LCD14_GPIO_Port,LED7_LCD14_Pin,GPIO_PIN_RESET);  //LED7��
//        LED_State=LED_State&0xBF;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 8:
//        HAL_GPIO_WritePin(LED8_LCD15_GPIO_Port,LED8_LCD15_Pin,GPIO_PIN_RESET);  //LED8��
//        LED_State=LED_State&0x7F;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;    
//    }
////    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
////    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
////    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
////    HAL_GPIO_WritePin(GPIOC,lednum<<8,GPIO_PIN_RESET);
////    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
////    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
//}

///****************************************************
//��������:
//��    ��: ����LED��
//��ڲ���: lednum��0-ȫ��1-LED1��2-LED2���Դ�����
//���ڲ���:
//****************************************************/
//void LED_OFF(unsigned char lednum)
//{
//    //��Ϊ����LED��LCD�ܽŵĸ���(PC8~PC15)������PC8~15��ͨ��D����������LED��
//    //1����ҪPD2����D�������Ĵ����źţ�2����Ҫȫ�ֱ�������LED�ĵ�ǰ״̬
//    switch(lednum)
//    {
//    case 0:
//        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);                     //ȫ���ߣ�ȫ��ע����߰�PC0~PC7Ҳ������
//        LED_State=0xFF;                                                         //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 1:
//        HAL_GPIO_WritePin(LED1_LCD8_GPIO_Port,LED1_LCD8_Pin,GPIO_PIN_SET);      //LED1��
//        LED_State=LED_State|0x01;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 2:
//        HAL_GPIO_WritePin(LED2_LCD9_GPIO_Port,LED2_LCD9_Pin,GPIO_PIN_SET);      //LED2��
//        LED_State=LED_State|0x02;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 3:
//        HAL_GPIO_WritePin(LED3_LCD10_GPIO_Port,LED3_LCD10_Pin,GPIO_PIN_SET);    //LED3��
//        LED_State=LED_State|0x04;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 4:
//        HAL_GPIO_WritePin(LED4_LCD11_GPIO_Port,LED4_LCD11_Pin,GPIO_PIN_SET);    //LED4��
//        LED_State=LED_State|0x08;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 5:
//        HAL_GPIO_WritePin(LED5_LCD12_GPIO_Port,LED5_LCD12_Pin,GPIO_PIN_SET);    //LED5��
//        LED_State=LED_State|0x10;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 6:
//        HAL_GPIO_WritePin(LED6_LCD13_GPIO_Port,LED6_LCD13_Pin,GPIO_PIN_SET);    //LED6��
//        LED_State=LED_State|0x20;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 7:
//        HAL_GPIO_WritePin(LED7_LCD14_GPIO_Port,LED7_LCD14_Pin,GPIO_PIN_SET);    //LED7��
//        LED_State=LED_State|0x40;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 8:
//        HAL_GPIO_WritePin(LED8_LCD15_GPIO_Port,LED8_LCD15_Pin,GPIO_PIN_SET);    //LED8��
//        LED_State=LED_State|0x80;                                               //��¼LED״̬
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;    
//    }
//}

///****************************************************
//��������:
//��    ��: ����LED�����л�
//��ڲ���: lednum��1-LED1�л���2-LED2�л����Դ�����
//���ڲ���:
//****************************************************/
//void LED_Toggle(unsigned char lednum)
//{
//    //��Ϊ����LED��LCD�ܽŵĸ���(PC8~PC15)������PC8~15��ͨ��D����������LED��
//    //1����ҪPD2����D�������Ĵ����źţ�2����Ҫȫ�ֱ�������LED�ĵ�ǰ״̬
//    switch(lednum)
//    {
//    case 1:
//        if(LED_State&0x01)  //ԭ���Ǹߵ�ƽ
//        {
//            LED_State=LED_State&0xFE;
//            HAL_GPIO_WritePin(LED1_LCD8_GPIO_Port,LED1_LCD8_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x01;
//            HAL_GPIO_WritePin(LED1_LCD8_GPIO_Port,LED1_LCD8_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 2:
//        if(LED_State&0x02)  //ԭ���Ǹߵ�ƽ
//        {
//            LED_State=LED_State&0xFD;
//            HAL_GPIO_WritePin(LED2_LCD9_GPIO_Port,LED2_LCD9_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x02;
//            HAL_GPIO_WritePin(LED2_LCD9_GPIO_Port,LED2_LCD9_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 3:
//        if(LED_State&0x04)  //ԭ���Ǹߵ�ƽ
//        {
//            LED_State=LED_State&0xFB;
//            HAL_GPIO_WritePin(LED3_LCD10_GPIO_Port,LED3_LCD10_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x04;
//            HAL_GPIO_WritePin(LED3_LCD10_GPIO_Port,LED3_LCD10_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 4:
//        if(LED_State&0x08)  //ԭ���Ǹߵ�ƽ
//        {
//            LED_State=LED_State&0xF7;
//            HAL_GPIO_WritePin(LED4_LCD11_GPIO_Port,LED4_LCD11_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x08;
//            HAL_GPIO_WritePin(LED4_LCD11_GPIO_Port,LED4_LCD11_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 5:
//        if(LED_State&0x10)  //ԭ���Ǹߵ�ƽ
//        {
//            LED_State=LED_State&0xEF;
//            HAL_GPIO_WritePin(LED5_LCD12_GPIO_Port,LED5_LCD12_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x10;
//            HAL_GPIO_WritePin(LED5_LCD12_GPIO_Port,LED5_LCD12_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 6:
//        if(LED_State&0x20)  //ԭ���Ǹߵ�ƽ
//        {
//            LED_State=LED_State&0xDF;
//            HAL_GPIO_WritePin(LED6_LCD13_GPIO_Port,LED6_LCD13_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x20;
//            HAL_GPIO_WritePin(LED6_LCD13_GPIO_Port,LED6_LCD13_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 7:
//        if(LED_State&0x40)  //ԭ���Ǹߵ�ƽ
//        {
//            LED_State=LED_State&0xBF;
//            HAL_GPIO_WritePin(LED7_LCD14_GPIO_Port,LED7_LCD14_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x40;
//            HAL_GPIO_WritePin(LED7_LCD14_GPIO_Port,LED7_LCD14_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;
//    case 8:
//        if(LED_State&0x80)  //ԭ���Ǹߵ�ƽ
//        {
//            LED_State=LED_State&0x7F;
//            HAL_GPIO_WritePin(LED8_LCD15_GPIO_Port,LED8_LCD15_Pin,GPIO_PIN_RESET);
//        }
//        else
//        {
//            LED_State=LED_State|0x80;
//            HAL_GPIO_WritePin(LED8_LCD15_GPIO_Port,LED8_LCD15_Pin,GPIO_PIN_SET);
//        }
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_SET);            //�����ź�
//        HAL_GPIO_WritePin(LED_CS_GPIO_Port,LED_CS_Pin,GPIO_PIN_RESET);
//        break;    
//    }
//}
