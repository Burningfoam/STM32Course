/*******************************************************************************
* �ļ����ƣ�
* ����������BSP�����ܺ����Ĳ�������
* ����˵����
* Ӳ��������CT117E_M4
* ע�����
* ���߰汾��hufq/V1.0
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
��������:
��    ��: BSP_LED�Ĳ���
��ڲ���:
���ڲ���:
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

    //��ˮ��
    LED_State=0x01;
    for(i=0; i<8; i++)
    {
        LED_Control(LED_State);
        HAL_Delay(500);
        LED_State=LED_State<<1;
    }
    //����������˸
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
��������:
��    ��: LCD�Ĳ���,ע��ÿ�г�����ʾ����ʱ��������LCD��10��20��
��ڲ���:
���ڲ���:
****************************************************/
void LCD_TestTask(void)
{
    unsigned int systick_freq;
    systick_freq=HAL_GetTickFreq();  //�õ��δ�ʱ�ӵ�Ƶ��
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
��������:
��    ��: ��ť������ԣ��������ְ��£���һ�μ�⵽�����̰����������ɿ���4������ֱ���
��ڲ���: 
���ڲ���:
****************************************************/
void KeyProcess(void)
{    
    static uint32_t KeyScan_uwTick = 0;             //����KeyProcess��ִ���ٶ�
    static uint16_t Key_KeepTime=0;                 //��¼�������³�����ʱ�� 
    static uint8_t keyvalue_Last=0;                 //��¼�ϴΰ�����ֵ
    uint8_t keyvalue_Current;
    if((uwTick -  KeyScan_uwTick)<50)	return;//���ٺ���    //uwTickΪ1ms�ļ����� ,50msɨ��һ��        
	KeyScan_uwTick = uwTick;
    
    keyvalue_Current=Key_scan();           
    switch(keyvalue_Current)
    {
        case B1_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //��һ�μ�⵽���£�����Ϊ�Ǽ����µĶ��������ΰ����ڴ˴���
            {
                LED_State=LED_State^0x01;
                LED_Control(LED_State);
            }
            else if(Key_KeepTime%4==0)          //ÿ��4*50ms=200ms�������������������ڴ˴���
            {
            
            }                
            break;
        case B2_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //��һ�ΰ��¸ü�
            {
                ;
            }
            break;
        case B3_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //��һ�ΰ��¸ü�
            {
                ;
            }
            break;
        case B4_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //��һ�ΰ��¸ü�
            {
                ;
            }
            break;
        default:
            if(Key_KeepTime>0)      //�м����£������ɿ���
            {
                switch(keyvalue_Last)
                {
                    case B1_PRESS:                      //�ü��ɿ�
                        if(Key_KeepTime<10)             // 500ms����Ϊ�̼�          
                        {
                            LED_State=LED_State^0x02;
                            LED_Control(LED_State);
                        }
                        else                            // 500ms����Ϊ�̼� 
                        {
                            LED_State=LED_State^0x04;
                            LED_Control(LED_State);
                        }
                        break;
                    case B2_PRESS:                      //�ü��ɿ�
                        if(Key_KeepTime<20)             // 500ms����Ϊ�̼�          
                        {
                            ;
                        }
                        else                            // 500ms����Ϊ�̼� 
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
��������:
��    ��: ���ڴ�����
��ڲ���: 
���ڲ���:
****************************************************/
void  Uart1_RX_Pro(void)
{
    static unsigned int Usart_uwTick=0;
    /*******ʹ�õδ�ʱ�ӵĿ���������ִ�еļ��ʱ��********/
    if(U1Rx_Len==0)     //û�н��յ�����
    {
        Usart_uwTick = uwTick;          //����Ҫ����ʱ�䣬��֤��һ���ֽڽ��յ�����һ��ʱ�����ʣ�������ٽ��д���
        return ;
    }

    if((uwTick-Usart_uwTick)<50)
        return ;
    Usart_uwTick = uwTick;
    
    /*****************�ѽ��յ������ݷ��ͻ�ȥ*****************/
    U1Tx_Len=U1Rx_Len;
    memcpy(Uart1_Tx_Buf,Uart1_Rx_Buf,U1Rx_Len);              //ע����Ҫͷ�ļ�"string.h"
    Uart1_Send(Uart1_Tx_Buf,U1Tx_Len);

    U1Rx_Len=0;     //һ��Ҫ���㣬Ҫ�����ظ�����;
}

/****************************************************
��������:
��    ��: IIC�ӿڵ�24C02�Ĳ��Ժ���
��ڲ���:
���ڲ���:
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
    HAL_Delay(2);   //������ʱ��2ms����
    iic_24c02_read(RddataBuf_24C02,0,DATABUF_24C02_LEN);

    //ͨ�����ڷ���
    sprintf((char *)Uart1_Tx_Buf, " IIC_Test_24C02!\r\n");  //ע����Ҫͷ�ļ���"stdio.h"
    Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
    U1Tx_Len=DATABUF_24C02_LEN;
    memcpy(Uart1_Tx_Buf,RddataBuf_24C02,U1Tx_Len);
    Uart1_Send(Uart1_Tx_Buf,U1Tx_Len);
}

/****************************************************
��������:
��    ��: IIC�ӿڵ�24C02�Ĳ��Ժ���
��ڲ���:
���ڲ���:
****************************************************/
void  IIC_Test_MCP4017(void)
{
    unsigned char temp;
    iic_mcp4017_write(63);
    temp=iic_mcp4017_read();

    //ͨ�����ڷ���
    sprintf((char *)Uart1_Tx_Buf, " IIC_Test_MCP4017!\r\n");  //ע����Ҫͷ�ļ���"stdio.h"
    Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));

    Uart1_Tx_Buf[0]=(temp%100)/10+0x30;
    Uart1_Tx_Buf[1]=(temp%10)+0x30;
    U1Tx_Len=2;
    Uart1_Send(Uart1_Tx_Buf,U1Tx_Len);
}

/****************************************************
��������:
��    ��:
��ڲ���:
���ڲ���:
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
    
    /**************ƽ���˲�********************/
    for(temp=0; temp<ADC1_DMA_BUF_SIZE; temp++)
        adresult=adresult+ADC1_DMABuf[temp];
    adresult=adresult/ADC1_DMA_BUF_SIZE;
    //ת��ѹֵ
    ad_volt_value=adresult*3.3/4096;            //
    //��ʾ������̶���ʾ���ݵ�λ��
    sprintf((char *)Lcd_Disp_String," AD1=%4d,  %.3fV",adresult,ad_volt_value);
    LCD_DisplayStringLine(Line4, (uint8_t *)Lcd_Disp_String);
    
     /**************ƽ���˲�********************/
    adresult=0;
    for(temp=0; temp<ADC2_DMA_BUF_SIZE; temp++)
        adresult=adresult+ADC2_DMABuf[temp];
    adresult=adresult/ADC2_DMA_BUF_SIZE;
    //ת��ѹֵ
    ad_volt_value=adresult*3.3/4096;            //
    //��ʾ������̶���ʾ���ݵ�λ��
    sprintf((char *)Lcd_Disp_String," AD2=%4d,  %.3fV",adresult,ad_volt_value);
    LCD_DisplayStringLine(Line5, (uint8_t *)Lcd_Disp_String);
}

/****************************************************
��������:
��    ��: �ⲿ�����������
��ڲ���:
���ڲ���:
****************************************************/
void  PulseMeasure_Test(void)
{
    static unsigned int PWM_uwTick;
    if((uwTick-PWM_uwTick)<1000)
        return;
    PWM_uwTick = uwTick;  
    
    //����TIM2��PA15�ܽ�����������Ƶ�ʺ�ռ�ձ�
//      Tim2_Fre=1/(TIM2_RiseEdge_Count*0.000001*1000);   //����Ƶ��(KHz)�� 0.000001��ָ������Ԫʱ��1ms(TIM2����Ƶ��1M)  
    Tim2_Fre=1000/(float)(TIM2_RiseEdge_Count);   //����Ƶ��(KHz)�� 0.000001��ָ������Ԫʱ��1ms(TIM2����Ƶ��1M)                    
    Tim2_Duty=(float)TIM2_FallEdge_Count/TIM2_RiseEdge_Count;
    Tim2_Duty=Tim2_Duty*100;                        //����ռ�ձ�
    
    //��ʾTIM2��PA15�ܽ����������Ƶ�ʺ�ռ�ձ�
    LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
    sprintf((char *)Lcd_Disp_String,"TIM2:%.3fKHz,%.1f%%",Tim2_Fre,Tim2_Duty);
    LCD_DisplayStringLine(Line6, (uint8_t *)Lcd_Disp_String);
    
    //����TIM3��PB4�ܽ�����������Ƶ�ʺ�ռ�ձ�  
//      Tim3_Fre=1/(TIM3_RiseEdge_Count*0.000001*1000);   //����Ƶ��(KHz)�� 0.000001��ָ������Ԫʱ��1ms(TIM3����Ƶ��1M)   
    Tim3_Fre=1000/(float)(TIM3_RiseEdge_Count);   //����Ƶ��(KHz)�� 0.000001��ָ������Ԫʱ��1ms(TIM3����Ƶ��1M)  
    Tim3_Duty=(float)TIM3_FallEdge_Count/TIM3_RiseEdge_Count;
    Tim3_Duty=Tim3_Duty*100;                        //����ռ�ձ�
    //��ʾTIM3��PB4�ܽ����������Ƶ�ʺ�ռ�ձ�
    LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
    sprintf((char *)Lcd_Disp_String,"TIM3:%.3fKHz,%.1f%%",Tim3_Fre,Tim3_Duty);
    LCD_DisplayStringLine(Line7, (uint8_t *)Lcd_Disp_String);
}

/****************************************************
��������:
��    ��: ��ʾDA�����
��ڲ���:
���ڲ���:
****************************************************/
void  DA_Display_Test(void)
{
    sprintf((char *)Lcd_Disp_String," DA1:%.1fV;DA2:Triang",DAC1_CH1_Vol);
    LCD_DisplayStringLine(Line8, (uint8_t *)Lcd_Disp_String);        
}
