#include "Bsp_Test.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "main.h"
#include "gpio.h"
#include "Bsp_LED.h"
#include "Bsp_Button.h"
#include "lcd.h"
#include "Bsp_Usart.h"


double cprice=3.5;
double vprice=2.0;

unsigned char su=0;

struct Car
{
	char type[4];
	char name[4];
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	
}car[8];


unsigned char Lcd_Disp_String[25];

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
                if(su==0)
								{
									su=1;
									LCD_TestTask();
								}
								else if(su==1)
								{
									su=0;
									LCD_TestTask();
								}
								
            }
            else if(Key_KeepTime%4==0)          //ÿ��4*50ms=200ms�������������������ڴ˴���
            {
            
            }                
            break;
        case B2_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //��һ�ΰ��¸ü�
            {
                if(su==1)
								{
									vprice=vprice+0.5;
									cprice=cprice+0.5;
									sprintf((char *)Lcd_Disp_String," CNBR:%.2f               ",cprice);
									LCD_DisplayStringLine(Line2, (uint8_t *)Lcd_Disp_String);
									sprintf((char *)Lcd_Disp_String," VNBR:%.2f               ",vprice);
									LCD_DisplayStringLine(Line4, (uint8_t *)Lcd_Disp_String);
								}
            }
            break; 
        case B3_PRESS:
            Key_KeepTime++;
            if(Key_KeepTime==1)                 //��һ�ΰ��¸ü�
            {
                if(su==1)
								{
									vprice=vprice-0.5;
									cprice=cprice-0.5;
									sprintf((char *)Lcd_Disp_String," CNBR:%.2f               ",cprice);
									LCD_DisplayStringLine(Line2, (uint8_t *)Lcd_Disp_String);
									sprintf((char *)Lcd_Disp_String," VNBR:%.2f               ",vprice);
									LCD_DisplayStringLine(Line4, (uint8_t *)Lcd_Disp_String);
								}
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
                            //LED_State=LED_State^0x02;
                            //LED_Control(LED_State);
                        }
                        else                            // 500ms����Ϊ�̼� 
                        {
                            //LED_State=LED_State^0x04;
                            //LED_Control(LED_State);
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
                
            }
            Key_KeepTime=0;
            break;
    }
    keyvalue_Last=keyvalue_Current;
    
}

void LCD_TestTask(void)
{
    
    LCD_Clear(White);
		LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
	

//    LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
//    LCD_DisplayStringLine(Line1, (uint8_t *)"                    ");
		if(su==0)
		{
			LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line1, (uint8_t *)"        Data        ");

	//    LCD_SetTextColor(Black);
			LCD_DisplayStringLine(Line2, (uint8_t *)" CNBR:0             ");
			LCD_DisplayStringLine(Line3, (uint8_t *)"                    ");

			LCD_DisplayStringLine(Line4, (uint8_t *)" VNBR:0             ");
			LCD_DisplayStringLine(Line5, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line6, (uint8_t *)" IDLE:8             ");
			LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
		}
		else if(su==1)
		{
			LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line1, (uint8_t *)"        Para        ");
	//    LCD_SetTextColor(Black);
			sprintf((char *)Lcd_Disp_String," CNBR:%.2f               ",cprice);
			LCD_DisplayStringLine(Line2, (uint8_t *)Lcd_Disp_String);
			LCD_DisplayStringLine(Line3, (uint8_t *)"                    ");
			
			sprintf((char *)Lcd_Disp_String," VNBR:%.2f               ",vprice);
			LCD_DisplayStringLine(Line4, (uint8_t *)Lcd_Disp_String);
			LCD_DisplayStringLine(Line5, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
			LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
		}
}

char* substring(unsigned char* sourceString, int startPosition, int substringLength)  
{  
    unsigned char* pointerToSourceString = sourceString;  
    char* resultingSubstring;  
    int i;  

    // Move the pointer to the start position in the source string
    pointerToSourceString = pointerToSourceString + startPosition;  

    // Copy characters from the source string to the substring
    for(i = 0; i < substringLength; i++)  
    {  
        resultingSubstring[i] = *(pointerToSourceString++);  
    }  

    // Add the termination character at the end of the substring
    resultingSubstring[substringLength] = '\0';  

    // Return the resulting substring
    return resultingSubstring; 
} 

void  Uart1_RX_Pro(void)
{
    static unsigned int Usart_uwTick=0;
    /*******ʹ�õδ�ʱ�ӵĿ���������ִ�еļ��ʱ��********/
    if(U1Rx_Len==0)     //û�н��յ�����
    {
        Usart_uwTick = uwTick;          //����Ҫ����ʱ�䣬��֤��һ���ֽڽ��յ�����һ��ʱ�����ʣ�������ٽ��д���
        return ;
    }
		
		unsigned char i=0;
		char* cname;
		//char* ctype;
		unsigned char j=0;
		unsigned char ispark=0;
		int parktime=0;
		double fee=0;
		int year,month,day,hour;
		//,minute,second
		
    if((uwTick-Usart_uwTick)>50)
    {
        Usart_uwTick = uwTick;
				
				//sprintf((char *)Uart1_Tx_Buf, "test1\n");  //ע����Ҫͷ�ļ���"stdio.h"
				Uart1_Send(Uart1_Rx_Buf,U1Rx_Len);
			
				//strcpy(ctype,substring(Uart1_Rx_Buf,0,4));
				strcpy(cname,substring(Uart1_Rx_Buf,5,4));

				for(i=0;i<8;i++)
				{
					if(strcmp(car[i].name,cname)==0)
					{
						
										
						sprintf((char *)Uart1_Tx_Buf, "test4\n");  //ע����Ҫͷ�ļ���"stdio.h"
						Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
			
						
						sprintf((char *)Uart1_Tx_Buf,"%s:%s:%d%d%d%d%d%d\r\n",car[i].type,car[i].name,car[i].year,car[i].month,car[i].day,car[i].hour,car[i].minute,car[i].second);
						Uart1_Send(Uart1_Tx_Buf,strlen((char*)Uart1_Tx_Buf));
						Uart1_Send(Uart1_Rx_Buf,strlen((char*)Uart1_Rx_Buf));
						//strcpy(car[i].type,substring(Uart1_Rx_Buf,0,4));
						//strcpy(car[i].name,substring(Uart1_Rx_Buf,5,4));
						year =  atoi(substring(Uart1_Rx_Buf,10,2));
						month = atoi(substring(Uart1_Rx_Buf,12,2));
						day = atoi(substring(Uart1_Rx_Buf,14,2));
						hour = atoi(substring(Uart1_Rx_Buf,16,2));
						//minute = atoi(substring(Uart1_Rx_Buf,18,2));
						//second = atoi(substring(Uart1_Rx_Buf,20,2));
						
						parktime = (year - car[i].year) * 365 * 24 +
											(month - car[i].month) * 30 * 24 +
											(day - car[i].day) * 24 +
											(hour - car[i].hour);

						if(car[i].type[0]=='C')
						{
							fee=parktime*cprice;
							sprintf((char *)Uart1_Tx_Buf,"%s:%s:%d:%.2f\r\n",car[i].type,car[i].name,parktime,fee);
							Uart1_Send(Uart1_Tx_Buf,strlen((char*)Uart1_Tx_Buf));
						}
						else if(car[i].type[0]=='V')
						{
							fee=parktime*vprice;
							sprintf((char *)Uart1_Tx_Buf,"%s:%s:%d:%.2f\r\n",car[i].type,car[i].name,parktime,fee);
							Uart1_Send(Uart1_Tx_Buf,strlen((char*)Uart1_Tx_Buf));
						}
						ispark=1;
						break;
					}
					ispark=0;
				}
					
								
				sprintf((char *)Uart1_Tx_Buf, "test2\n");  //ע����Ҫͷ�ļ���"stdio.h"
				Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
			
					
				if(ispark==0)
				{
					for(j=0;j<8;j++)
					{
						if(car[j].year==0)
						{
							
											
							sprintf((char *)Uart1_Tx_Buf, "test5\n");  //ע����Ҫͷ�ļ���"stdio.h"
							Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
			
							
							strcpy(car[j].type,substring(Uart1_Rx_Buf,0,4));
							
																		
							sprintf((char *)Uart1_Tx_Buf, "%s\n",car[j].type);  //ע����Ҫͷ�ļ���"stdio.h"
							Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
			
							
							strcpy(car[j].name,substring(Uart1_Rx_Buf,5,4));
							
																		
							sprintf((char *)Uart1_Tx_Buf, "test9\n");  //ע����Ҫͷ�ļ���"stdio.h"
							Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
			
							
							car[j].year =  atoi(substring(Uart1_Rx_Buf,10,2));
							
																		
							sprintf((char *)Uart1_Tx_Buf, "%d\n",car[j].year);  //ע����Ҫͷ�ļ���"stdio.h"
							Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
			
							
							car[j].month = atoi(substring(Uart1_Rx_Buf,12,2));
							car[j].day = atoi(substring(Uart1_Rx_Buf,14,2));
							car[j].hour = atoi(substring(Uart1_Rx_Buf,16,2));
							car[j].minute = atoi(substring(Uart1_Rx_Buf,18,2));
							car[j].second = atoi(substring(Uart1_Rx_Buf,20,2));
							
																		
							sprintf((char *)Uart1_Tx_Buf, "test7\n");  //ע����Ҫͷ�ļ���"stdio.h"
							Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
			
							
							break;
							
						}
					}
					
									
					sprintf((char *)Uart1_Tx_Buf, "test6\n");  //ע����Ҫͷ�ļ���"stdio.h"
					Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
			
					
				}
			
								
				sprintf((char *)Uart1_Tx_Buf, "test3\n");  //ע����Ҫͷ�ļ���"stdio.h"
				Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
			
        memset(Uart1_Rx_Buf, 0, U1Rx_Len);
				U1Rx_Len=0;     //һ��Ҫ���㣬Ҫ�����ظ�����;
    }
}




