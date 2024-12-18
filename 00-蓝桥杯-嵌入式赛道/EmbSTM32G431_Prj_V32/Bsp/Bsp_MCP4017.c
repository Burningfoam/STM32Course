/*******************************************************************************
* 文件名称：
* 功能描述：1、CT117E_M4实验板上MCP4017的操作
            
* 配置说明：
* 硬件环境：CT117E_M4
* 注意事项：
* 作者版本：hufq/V1.0
*******************************************************************************/
#include "Bsp_MCP4017.h"
#include "main.h"
#include "i2c - hal.h"

//mcp4017write 
/****************************************************
函数名称:
功    能: MCP4017写入函数，参考资源包中芯片资料MCP4017.pdf中，page38的Figure5-12）的时序(注意只写入一个字节)
          1、板上用的MCP4017_104,即最大是100K，计算公式(参考page42的 EQUATION 6-2)是：Rwb(即5,6之间的电阻)=(val/127)*100K+Rw(芯片内部模拟开关的典型值：100欧姆)；
          2、举例：val=63(mid_scale)，Rwb=49.7K，开发板电路中B端接地，W接PB14管脚后接10K电阻到VDD(3.3V),故PB14管脚的电压应该是：2.74V
入口参数: val:0~127
出口参数:
****************************************************/
void iic_mcp4017_write(unsigned char val)
{
	I2CStart();             //Figure5-12时序中：START   
	I2CSendByte(0x5e);      //Figure5-12时序中：FIXED ADDRESS,注意高7位固定，最低位：0-write,1-read   
	I2CWaitAck();           //Figure5-12时序中：A(等待ACK) 	
	I2CSendByte(val);       //Figure5-12时序中：Data(最高位no care) 	
	I2CWaitAck();           //Figure5-12时序中：A(等待ACK)	
	I2CStop();              //Figure5-12时序中：P 
}
//mcp4017read
/****************************************************
函数名称:
功    能: MCP4017读函数，参考资源包中芯片资料MCP4017.pdf中，page39的Figure5-13）的时序(注意只读一个字节)
入口参数: 
出口参数: 0~127
****************************************************/
unsigned char iic_mcp4017_read(void)
{
	unsigned char val;
	I2CStart();                 //Figure5-13时序中：START  
	I2CSendByte(0x5f);          //Figure5-13时序中：FIXED ADDRESS,注意高7位固定，最低位：0-write,1-read   
	I2CWaitAck();               //Figure5-13时序中：A(等待ACK)	
	val = I2CReceiveByte();     //Figure5-13时序中：Data(最高位no care) 	
	I2CSendNotAck();            //Figure5-13时序中：A(NO ACK(注意这里是发送))	
	I2CStop();                  //Figure5-13时序中：P 
	return val;                 
}


