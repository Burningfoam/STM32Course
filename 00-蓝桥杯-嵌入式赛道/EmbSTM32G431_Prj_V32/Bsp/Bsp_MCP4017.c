/*******************************************************************************
* �ļ����ƣ�
* ����������1��CT117E_M4ʵ�����MCP4017�Ĳ���
            
* ����˵����
* Ӳ��������CT117E_M4
* ע�����
* ���߰汾��hufq/V1.0
*******************************************************************************/
#include "Bsp_MCP4017.h"
#include "main.h"
#include "i2c - hal.h"

//mcp4017write 
/****************************************************
��������:
��    ��: MCP4017д�뺯�����ο���Դ����оƬ����MCP4017.pdf�У�page38��Figure5-12����ʱ��(ע��ֻд��һ���ֽ�)
          1�������õ�MCP4017_104,�������100K�����㹫ʽ(�ο�page42�� EQUATION 6-2)�ǣ�Rwb(��5,6֮��ĵ���)=(val/127)*100K+Rw(оƬ�ڲ�ģ�⿪�صĵ���ֵ��100ŷķ)��
          2��������val=63(mid_scale)��Rwb=49.7K���������·��B�˽ӵأ�W��PB14�ܽź��10K���赽VDD(3.3V),��PB14�ܽŵĵ�ѹӦ���ǣ�2.74V
��ڲ���: val:0~127
���ڲ���:
****************************************************/
void iic_mcp4017_write(unsigned char val)
{
	I2CStart();             //Figure5-12ʱ���У�START   
	I2CSendByte(0x5e);      //Figure5-12ʱ���У�FIXED ADDRESS,ע���7λ�̶������λ��0-write,1-read   
	I2CWaitAck();           //Figure5-12ʱ���У�A(�ȴ�ACK) 	
	I2CSendByte(val);       //Figure5-12ʱ���У�Data(���λno care) 	
	I2CWaitAck();           //Figure5-12ʱ���У�A(�ȴ�ACK)	
	I2CStop();              //Figure5-12ʱ���У�P 
}
//mcp4017read
/****************************************************
��������:
��    ��: MCP4017���������ο���Դ����оƬ����MCP4017.pdf�У�page39��Figure5-13����ʱ��(ע��ֻ��һ���ֽ�)
��ڲ���: 
���ڲ���: 0~127
****************************************************/
unsigned char iic_mcp4017_read(void)
{
	unsigned char val;
	I2CStart();                 //Figure5-13ʱ���У�START  
	I2CSendByte(0x5f);          //Figure5-13ʱ���У�FIXED ADDRESS,ע���7λ�̶������λ��0-write,1-read   
	I2CWaitAck();               //Figure5-13ʱ���У�A(�ȴ�ACK)	
	val = I2CReceiveByte();     //Figure5-13ʱ���У�Data(���λno care) 	
	I2CSendNotAck();            //Figure5-13ʱ���У�A(NO ACK(ע�������Ƿ���))	
	I2CStop();                  //Figure5-13ʱ���У�P 
	return val;                 
}


