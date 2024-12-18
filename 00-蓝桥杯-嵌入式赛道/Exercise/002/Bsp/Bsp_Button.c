/*******************************************************************************
* �ļ����ƣ�
* ����������1. CT117E_M4ʵ����ϰ����Ĳ���
* ����˵����
* Ӳ��������CT117E_M4
* ע�����
* ���߰汾��hufq/V1.0
*******************************************************************************/
#include "Bsp_Button.h"
#include "main.h"
#include "gpio.h"
#include "LED.h"


/****************************************************
��������:
��    ��: ��ťɨ��,""""����ͨ���δ�ʱ������ʱ����ɨ�躯��"""
          1��ע�����ȼ���
          2��ע��˫�����⣻
          3������ʹ�ò�֧��������ģʽ����Ϊ���������׳��ֶ������⣨��ν������
��ڲ���: mode��0-��֧������(������)��1-֧������
���ڲ���: ����ֵ��0-Ϊ�޼���1~4-��Ӧ�������
****************************************************/
unsigned char Key_scan(void)
{    
    //���ﲻӦ���á�~������Ӧ���á�!��
//    if(key&&(~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||~HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)))
//    if(key&&((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)))
    if((!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)))
    {
//        HAL_Delay(5); 	    //���ö�ʱ��ɨ�裬����ʡȴ����ʱ
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET) return B1_PRESS;
        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET) return B2_PRESS;
        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET) return B3_PRESS;
        else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET) return B4_PRESS;
        else return 0;
    } 
    return 0;
}


/****************************************************
��������:
��    ��: ��ťɨ��,""""����ͨ���δ�ʱ������ʱ����ɨ�躯��"""
          1��ע�����ȼ���
          2��ע��˫�����⣻
          3������ʹ�ò�֧��������ģʽ����Ϊ���������׳��ֶ������⣨��ν������
��ڲ���: mode��0-��֧������(������)��1-֧������
���ڲ���: ����ֵ��0-Ϊ�޼���1~4-��Ӧ�������
****************************************************/
//unsigned char Key_scan(unsigned char mode)
//{
//    static unsigned char key=1;
//    if(mode)key=1;
//    //���ﲻӦ���á�~������Ӧ���á�!��
////    if(key&&(~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||~HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||~HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)))
////    if(key&&((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)))
//    if(key&&(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)||!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)))
//    {
////        HAL_Delay(5); 	    //���ö�ʱ��ɨ�裬����ʡȴ����ʱ
//        key = 0;
//        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET) return B1_PRESS;
//        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET) return B2_PRESS;
//        else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET) return B3_PRESS;
//        else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET) return B4_PRESS;
//        else return 8;
//    } 
//    else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)&&HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)&&HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)&&HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)) 
//        key=1;
//    return 0;
//}




/*************
���������Ϲ�����Ӧ�İ������룬���Բ鿴��ǰ��Щ�������£���Щ�����ɿ��������������ֻ���߼����Ը��ӣ��ʲ����龺��ʱʹ��
**************/
////*����ɨ��ר�ñ���
//unsigned char ucKey_Val, unKey_Down, ucKey_Up, ucKey_Old;

//unsigned char Key_Scan(void)
//{
//	unsigned char unKey_Val = 0;
//	
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
//		unKey_Val = 1;

//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
//		unKey_Val = 2;

//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)
//		unKey_Val = 3;
//	
//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
//		unKey_Val = 4;	
//	
//	return unKey_Val;
//}

//void Key_Proc(void)
//{
//	static uint32_t uwTick_Set_Point = 0;//����Key_Proc��ִ���ٶ�
//    if((uwTick -  uwTick_Set_Point)<100)	return;//���ٺ���
//	uwTick_Set_Point = uwTick;

//	
//	//�ֳɼ������
//	//���1��100ms����ɨ�裬�����õ��Ľ����0����û���£���B4���£��������½��ء�
//	//ucKey_Val = 4(0000 0100)
//	//unKey_Down = 0000 0100 & ( 0000 0000 ^ 0000 0100) = 0000 0100 & 0000 0100 = 0000 0100 (4)
//	//ucKey_Up = 1111 1011 & 0000 0100 = 0000 0000 
//	//ucKey_Old = 4
//	
//	//���2��B4�������½��غ󣬰���һֱ����
//	//ucKey_Val = 4(0000 0100)
//	//unKey_Down = 0000 0100 & ( 0000 0100 ^ 0000 0100) = 0000 0100 & 0000 0000 = 0000 0000 (0)
//	//ucKey_Up = 1111 1011 & 0000 0000 = 0000 0000 
//	//ucKey_Old = 4	
//	
//	//���3��B4����һֱ���������
//	//ucKey_Val = 0(0000 0000)
//	//unKey_Down = 0000 0000 & ( 0000 0100 ^ 0000 0000) = 0000 0000 & 0000 0100 = 0000 0000 (0)
//	//ucKey_Up = 1111 1111 & 0000 0100 = 0000 0100 (4)
//	//ucKey_Old = 0		
//	
//	ucKey_Val = Key_Scan();
//	unKey_Down = ucKey_Val & (ucKey_Old ^ ucKey_Val); 
//	ucKey_Up = ~ucKey_Val & (ucKey_Old ^ ucKey_Val);	
//	ucKey_Old = ucKey_Val;
//	
//	if(unKey_Down == 4)
//	{
//		LED_Disp(0x88);
//	}
//	if(unKey_Down == 3)
//	{
//		LED_Disp(0x00);
//	}	
//}

