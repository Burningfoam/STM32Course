/*******************************************************************************
* �ļ����ƣ�
* ����������1��CT117E_M4ʵ�����24C02�Ĳ���
            2���ڵ���д�뺯���������ʱ2ms�����ٵ��ö�����
* ����˵����
* Ӳ��������CT117E_M4
* ע�����
* ���߰汾��hufq/V1.0
*******************************************************************************/
#include "Bsp_24C02.h"
#include "main.h"
#include "i2c - hal.h"

unsigned char RddataBuf_24C02[DATABUF_24C02_LEN];
unsigned char WrdataBuf_24C02[DATABUF_24C02_LEN];


static void delay2(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

/****************************************************
��������:
��    ��: 24C02��д�������ο���Դ����оƬ����AT24C02.pdf�У���11��ʼ�Ĺ���д��Figure 9 . Page Write����ʱ��
          �ر�ע�⣺�ڵ��ñ�д�뺯��ʱ�����ʱ2ms�����ٵ��ö�����
��ڲ���: pucbuff��д������ָ�룻ucaddr��д���ַ��ucNum��д�����ݳ���
���ڲ���:
****************************************************/
void iic_24c02_write(unsigned char *pucbuff, unsigned char ucaddr, unsigned char ucNum)
{
	I2CStart();                         //Figure 9ʱ���У�START
	I2CSendByte(0xa0);                  //Figure 9ʱ���У�DEVICE ADDRESS ,����0xa0����1010 0000����4λbit7~bit4��оƬ�涨��ַ��bit3~bit1��·��A2~A0��ƽ��
	I2CWaitAck();                       //Figure 9ʱ���У�ACK(�ȴ�ACK)
	
	I2CSendByte(ucaddr);                //Figure 9ʱ���У�WORD ADDRESS      
	I2CWaitAck();                       //Figure 9ʱ���У�ACK(�ȴ�ACK)
	
	while(ucNum--)                      
	{
		I2CSendByte(*pucbuff++);        //Figure 9ʱ���У�DATA
		I2CWaitAck();                   //Figure 9ʱ���У�ACK(�ȴ�ACK)
	}
	I2CStop();                          //Figure 9ʱ���У�STOP
	delay2(DELAY_TIME);                 //��Ҫ��ʱ����ֹ������������
}

/****************************************************
��������:
��    ��: 24C02�Ķ��������ο���Դ����оƬ����AT24C02.pdf�У���11��ʼ�Ĺ��ڶ���Figure 11 .Random Read��
��ڲ���: pucbuff����������ָ�룻ucaddr������ַ��ucNum���������ݳ���
���ڲ���:
****************************************************/
void iic_24c02_read(unsigned char *pucbuff, unsigned char ucaddr, unsigned char ucNum)
{
	I2CStart();                     //Figure 11ʱ���У�START
	I2CSendByte(0xa0);              //Figure 11ʱ���У�DEVICE ADDRESS ��ע����д
	I2CWaitAck();                   //Figure 11ʱ���У�ACK(�ȴ�ACK)
	
	I2CSendByte(ucaddr);            //Figure 11ʱ���У�WORD ADDRESS
	I2CWaitAck();                   //Figure 11ʱ���У�ACK(�ȴ�ACK)
	
	I2CStart();                     //Figure 11ʱ���У�START
	I2CSendByte(0xa1);              //Figure 11ʱ���У�DEVICE ADDRESS ��ע���Ƕ�
	I2CWaitAck();                   //Figure 11ʱ���У�ACK(�ȴ�ACK)
	
	while(ucNum--)
	{
		*pucbuff++ = I2CReceiveByte();  //Figure 11ʱ���У�DATA
		if(ucNum)
			I2CSendAck();               //Figure 11ʱ���У�ACK(ע�������Ƿ���ACK)
		else
			I2CSendNotAck();            //Figure 11ʱ���У�NO ACK(ע�������Ƿ���)
	}
	I2CStop();                          //Figure 9ʱ���У�STOP
//    delay2(DELAY_TIME);                 //��Ҫ��ʱ����ֹ������������
}

