/*******************************************************************************
* 文件名称：
* 功能描述：1、CT117E_M4实验板上24C02的操作
            2、在调用写入函数后最好延时2ms以上再调用读函数
* 配置说明：
* 硬件环境：CT117E_M4
* 注意事项：
* 作者版本：hufq/V1.0
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
函数名称:
功    能: 24C02的写操作：参考资源包中芯片资料AT24C02.pdf中，第11开始的关于写（Figure 9 . Page Write）的时序
          特别注意：在调用本写入函数时最好延时2ms以上再调用读函数
入口参数: pucbuff：写入数组指针；ucaddr：写入地址；ucNum：写入数据长度
出口参数:
****************************************************/
void iic_24c02_write(unsigned char *pucbuff, unsigned char ucaddr, unsigned char ucNum)
{
	I2CStart();                         //Figure 9时序中：START
	I2CSendByte(0xa0);                  //Figure 9时序中：DEVICE ADDRESS ,其中0xa0，是1010 0000（高4位bit7~bit4是芯片规定地址，bit3~bit1电路中A2~A0电平）
	I2CWaitAck();                       //Figure 9时序中：ACK(等待ACK)
	
	I2CSendByte(ucaddr);                //Figure 9时序中：WORD ADDRESS      
	I2CWaitAck();                       //Figure 9时序中：ACK(等待ACK)
	
	while(ucNum--)                      
	{
		I2CSendByte(*pucbuff++);        //Figure 9时序中：DATA
		I2CWaitAck();                   //Figure 9时序中：ACK(等待ACK)
	}
	I2CStop();                          //Figure 9时序中：STOP
	delay2(DELAY_TIME);                 //需要延时，防止连续操作出错；
}

/****************************************************
函数名称:
功    能: 24C02的读操作：参考资源包中芯片资料AT24C02.pdf中，第11开始的关于读（Figure 11 .Random Read）
入口参数: pucbuff：读入数组指针；ucaddr：读地址；ucNum：读入数据长度
出口参数:
****************************************************/
void iic_24c02_read(unsigned char *pucbuff, unsigned char ucaddr, unsigned char ucNum)
{
	I2CStart();                     //Figure 11时序中：START
	I2CSendByte(0xa0);              //Figure 11时序中：DEVICE ADDRESS ，注意是写
	I2CWaitAck();                   //Figure 11时序中：ACK(等待ACK)
	
	I2CSendByte(ucaddr);            //Figure 11时序中：WORD ADDRESS
	I2CWaitAck();                   //Figure 11时序中：ACK(等待ACK)
	
	I2CStart();                     //Figure 11时序中：START
	I2CSendByte(0xa1);              //Figure 11时序中：DEVICE ADDRESS ，注意是读
	I2CWaitAck();                   //Figure 11时序中：ACK(等待ACK)
	
	while(ucNum--)
	{
		*pucbuff++ = I2CReceiveByte();  //Figure 11时序中：DATA
		if(ucNum)
			I2CSendAck();               //Figure 11时序中：ACK(注意这里是发送ACK)
		else
			I2CSendNotAck();            //Figure 11时序中：NO ACK(注意这里是发送)
	}
	I2CStop();                          //Figure 9时序中：STOP
//    delay2(DELAY_TIME);                 //需要延时，防止连续操作出错；
}

