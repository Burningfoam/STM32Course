#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��2
 ��������ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
    u8 key;
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
	
    while(1)
    {
        key=KEY_Scan(0);            //����ɨ��,0,��֧��������;1,֧��������;
				switch(key)
				{				 
					case  WKUP_PRES:	//����LED0,LED1�������
								printf("The key is %d\r\n",key);
								break;
					case  KEY2_PRES:	//����LED0��ת
								printf("The key is %d\r\n",key);
								break;
					case  KEY1_PRES:	//����LED1��ת	 
								printf("The key is %d\r\n",key);
								break;
					case  KEY0_PRES:	//ͬʱ����LED0,LED1��ת 
								printf("The key is %d\r\n",key);
								break;
					case 5:
						printf("The key 1 and key 2 are pressed together\r\n");
						break;
					case 6:
						printf("The key 1 and key 3 are pressed together\r\n");
						break;
					case 7:
						printf("The key 1 and key 4 are pressed together\r\n");
						break;
					case 8:
						printf("The key 2 and key 3 are pressed together\r\n");
						break;
					case 9:
						printf("The key 2 and key 4 are pressed together\r\n");
						break;
					case 10:
						printf("The key 3 and key 4 are pressed together\r\n");
						break;
			}
        delay_ms(10);
    }
}

