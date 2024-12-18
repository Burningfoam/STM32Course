/*******************************************************************************
* 文件名称：
* 功能描述：BSP各功能函数的测试任务
* 配置说明：
* 硬件环境：CT117E_M4
* 注意事项：
* 作者版本：hufq/V1.0
*******************************************************************************/
#include "Bsp_Test.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
#include "gpio.h"
#include "Bsp_LED.h"
#include "Bsp_Button.h"
#include "lcd.h"
#include "Bsp_Usart.h"
#include "Bsp_24C02.h"
#include "Bsp_MCP4017.h"
#include "Bsp_AD.h"
#include "Bsp_tim.h"
#include "Bsp_DAC.h"

unsigned char Lcd_Disp_String[25];

/****************************************************
函数名称:
功    能: BSP_LED的测试
入口参数:
出口参数:
****************************************************/
void BSP_LED_TestTask(void)
{
    unsigned char i=0;
    unsigned char ledcode=0x01;
    LED_State=0xff;
    LED_Control(LED_State);
    HAL_Delay(2000);
    LED_State=0x0;
    LED_Control(LED_State);
    HAL_Delay(2000);

    //流水灯
    LED_State=0x01;
    for(i=0; i<8; i++)
    {
        LED_Control(LED_State);
        HAL_Delay(500);
        LED_State=LED_State<<1;
    }
    //单个轮流闪烁
    for(i=0; i<8; i++)
    {
        LED_State=LED_State^ledcode;
        LED_Control(LED_State);
        HAL_Delay(1000);
        LED_State=LED_State^ledcode;
        LED_Control(LED_State);
        HAL_Delay(1000);
        ledcode=ledcode<<1;
    }
}


/****************************************************
函数名称:
功    能: LCD的测试,注意每行超出显示长度时会死机，LCD共10行20列
入口参数:
出口参数:
****************************************************/
void LCD_TestTask(void)
{
    unsigned int systick_freq;
    systick_freq=HAL_GetTickFreq();  //得到滴答时钟的频率
    sprintf((char *)Lcd_Disp_String," systick_freq=%2dKHz",systick_freq);
    LCD_Clear(White);
    LCD_SetBackColor(Blue);
    LCD_SetTextColor(Black);

//    LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
//    LCD_DisplayStringLine(Line1, (uint8_t *)"                    ");
    LCD_DisplayStringLine(Line0, (uint8_t *)" WWW.NJTECH.EDU.CN24");
    LCD_DisplayStringLine(Line1, (uint8_t *)" HAL LIB@80Mhz V32  ");


    LCD_SetBackColor(White);
//    LCD_SetTextColor(Black);
    LCD_DisplayStringLine(Line2, (uint8_t *)Lcd_Disp_String);
    LCD_DisplayStringLine(Line3, (uint8_t *)" ButtonValue=       ");
    LCD_DisplayStringLine(Line4, (uint8_t *)"                    ");
    LCD_DisplayStringLine(Line5, (uint8_t *)"                    ");
    LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
    LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
    LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
    LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
}


/****************************************************
函数名称:
功    能: 按钮处理测试，可以区分按下（第一次检测到），短按，长按，松开等4种情况分别处理
入口参数: 
出口参数:
****************************************************/
void KeyProcess(void)
{    
    static uint32_t KeyScan_uwTick = 0;             //控制KeyProcess的执行速度
    static uint16_t Key_KeepTime=0;                 //记录按键按下持续的时间 
    static uint8_t keyvalue_Last=0;                 //记录上次按键键值
    uint8_t keyvalue_Current;
    if((uwTick -  KeyScan_uwTick)<50)	return;//减速函数    //uwTick为1ms的计数器 ,50ms扫描一次        
	KeyScan_uwTick = uwTick;
    
    keyvalue_Current=Key_scan();           
    switch(keyvalue_Current)
    {
        case B1_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //第一次检测到按下，可认为是键按下的动作，单次按键在此处理
            {
                LED_State=LED_State^0x01;
                LED_Control(LED_State);
            }
            else if(Key_KeepTime%4==0)          //每隔4*50ms=200ms持续动作，连续按键在此处理
            {
            
            }                
            break;
        case B2_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //第一次按下该键
            {
                ;
            }
            break;
        case B3_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //第一次按下该键
            {
                ;
            }
            break;
        case B4_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //第一次按下该键
            {
                ;
            }
            break;
        default:
            if(Key_KeepTime>0)      //有键按下，现在松开了
            {
                switch(keyvalue_Last)
                {
                    case B1_PRESS:                      //该键松开
                        if(Key_KeepTime<10)             // 500ms以内为短键          
                        {
                            LED_State=LED_State^0x02;
                            LED_Control(LED_State);
                        }
                        else                            // 500ms以上为短键 
                        {
                            LED_State=LED_State^0x04;
                            LED_Control(LED_State);
                        }
                        break;
                    case B2_PRESS:                      //该键松开
                        if(Key_KeepTime<20)             // 500ms以内为短键          
                        {
                            ;
                        }
                        else                            // 500ms以上为短键 
                        {
                            ;
                        }
                        break;
                }
                sprintf((char *)Lcd_Disp_String," ButtonValue=%2d",keyvalue_Last);
                LCD_DisplayStringLine(Line3, (uint8_t *)Lcd_Disp_String);
            }
            Key_KeepTime=0;
            break;
    }
    keyvalue_Last=keyvalue_Current;
    
}

/****************************************************
函数名称:
功    能: 串口处理函数
入口参数: 
出口参数:
****************************************************/
void  Uart1_RX_Pro(void)
{
    static unsigned int Usart_uwTick=0;
    /*******使用滴答时钟的控制任务函数执行的间隔时间********/
    if(U1Rx_Len==0)     //没有接收到数据
    {
        Usart_uwTick = uwTick;          //必须要有清时间，保证第一个字节接收到后有一定时间接收剩余数据再进行处理；
        return ;
    }

    if((uwTick-Usart_uwTick)<50)
        return ;
    Usart_uwTick = uwTick;
    
    /*****************把接收到的数据发送回去*****************/
    U1Tx_Len=U1Rx_Len;
    memcpy(Uart1_Tx_Buf,Uart1_Rx_Buf,U1Rx_Len);              //注意需要头文件"string.h"
    Uart1_Send(Uart1_Tx_Buf,U1Tx_Len);

    U1Rx_Len=0;     //一定要清零，要不会重复发送;
}

/****************************************************
函数名称:
功    能: IIC接口的24C02的测试函数
入口参数:
出口参数:
****************************************************/
void  IIC_Test_24C02(void)
{
    unsigned char i;
    for(i=0; i<DATABUF_24C02_LEN; i++)
    {
        WrdataBuf_24C02[i]=i+0x30;
        RddataBuf_24C02[i]=0;
    }

    iic_24c02_write(WrdataBuf_24C02,0,DATABUF_24C02_LEN);
    HAL_Delay(2);   //尽量延时在2ms以上
    iic_24c02_read(RddataBuf_24C02,0,DATABUF_24C02_LEN);

    //通过串口发送
    sprintf((char *)Uart1_Tx_Buf, " IIC_Test_24C02!\r\n");  //注意需要头文件："stdio.h"
    Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
    U1Tx_Len=DATABUF_24C02_LEN;
    memcpy(Uart1_Tx_Buf,RddataBuf_24C02,U1Tx_Len);
    Uart1_Send(Uart1_Tx_Buf,U1Tx_Len);
}

/****************************************************
函数名称:
功    能: IIC接口的24C02的测试函数
入口参数:
出口参数:
****************************************************/
void  IIC_Test_MCP4017(void)
{
    unsigned char temp;
    iic_mcp4017_write(63);
    temp=iic_mcp4017_read();

    //通过串口发送
    sprintf((char *)Uart1_Tx_Buf, " IIC_Test_MCP4017!\r\n");  //注意需要头文件："stdio.h"
    Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));

    Uart1_Tx_Buf[0]=(temp%100)/10+0x30;
    Uart1_Tx_Buf[1]=(temp%10)+0x30;
    U1Tx_Len=2;
    Uart1_Send(Uart1_Tx_Buf,U1Tx_Len);
}

/****************************************************
函数名称:
功    能:
入口参数:
出口参数:
****************************************************/
void  AD_Test(void)
{
    static unsigned int AD_uwTick=0;
    unsigned char temp;
    unsigned int  adresult=0;
    float ad_volt_value;

    if((uwTick-AD_uwTick)<1000)
        return ;    
    AD_uwTick = uwTick;
    
    /**************平均滤波********************/
    for(temp=0; temp<ADC1_DMA_BUF_SIZE; temp++)
        adresult=adresult+ADC1_DMABuf[temp];
    adresult=adresult/ADC1_DMA_BUF_SIZE;
    //转电压值
    ad_volt_value=adresult*3.3/4096;            //
    //显示，建议固定显示数据的位数
    sprintf((char *)Lcd_Disp_String," AD1=%4d,  %.3fV",adresult,ad_volt_value);
    LCD_DisplayStringLine(Line4, (uint8_t *)Lcd_Disp_String);
    
     /**************平均滤波********************/
    adresult=0;
    for(temp=0; temp<ADC2_DMA_BUF_SIZE; temp++)
        adresult=adresult+ADC2_DMABuf[temp];
    adresult=adresult/ADC2_DMA_BUF_SIZE;
    //转电压值
    ad_volt_value=adresult*3.3/4096;            //
    //显示，建议固定显示数据的位数
    sprintf((char *)Lcd_Disp_String," AD2=%4d,  %.3fV",adresult,ad_volt_value);
    LCD_DisplayStringLine(Line5, (uint8_t *)Lcd_Disp_String);
}

/****************************************************
函数名称:
功    能: 外部脉冲测量测试
入口参数:
出口参数:
****************************************************/
void  PulseMeasure_Test(void)
{
    static unsigned int PWM_uwTick;
    if((uwTick-PWM_uwTick)<1000)
        return;
    PWM_uwTick = uwTick;  
    
    //计算TIM2即PA15管脚输入的脉冲的频率和占空比
//      Tim2_Fre=1/(TIM2_RiseEdge_Count*0.000001*1000);   //计算频率(KHz)： 0.000001是指计数单元时间1ms(TIM2的主频是1M)  
    Tim2_Fre=1000/(float)(TIM2_RiseEdge_Count);   //计算频率(KHz)： 0.000001是指计数单元时间1ms(TIM2的主频是1M)                    
    Tim2_Duty=(float)TIM2_FallEdge_Count/TIM2_RiseEdge_Count;
    Tim2_Duty=Tim2_Duty*100;                        //计算占空比
    
    //显示TIM2即PA15管脚输入的脉冲频率和占空比
    LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
    sprintf((char *)Lcd_Disp_String,"TIM2:%.3fKHz,%.1f%%",Tim2_Fre,Tim2_Duty);
    LCD_DisplayStringLine(Line6, (uint8_t *)Lcd_Disp_String);
    
    //计算TIM3即PB4管脚输入的脉冲的频率和占空比  
//      Tim3_Fre=1/(TIM3_RiseEdge_Count*0.000001*1000);   //计算频率(KHz)： 0.000001是指计数单元时间1ms(TIM3的主频是1M)   
    Tim3_Fre=1000/(float)(TIM3_RiseEdge_Count);   //计算频率(KHz)： 0.000001是指计数单元时间1ms(TIM3的主频是1M)  
    Tim3_Duty=(float)TIM3_FallEdge_Count/TIM3_RiseEdge_Count;
    Tim3_Duty=Tim3_Duty*100;                        //计算占空比
    //显示TIM3即PB4管脚输入的脉冲频率和占空比
    LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
    sprintf((char *)Lcd_Disp_String,"TIM3:%.3fKHz,%.1f%%",Tim3_Fre,Tim3_Duty);
    LCD_DisplayStringLine(Line7, (uint8_t *)Lcd_Disp_String);
}

/****************************************************
函数名称:
功    能: 显示DA的输出
入口参数:
出口参数:
****************************************************/
void  DA_Display_Test(void)
{
    sprintf((char *)Lcd_Disp_String," DA1:%.1fV;DA2:Triang",DAC1_CH1_Vol);
    LCD_DisplayStringLine(Line8, (uint8_t *)Lcd_Disp_String);        
}
