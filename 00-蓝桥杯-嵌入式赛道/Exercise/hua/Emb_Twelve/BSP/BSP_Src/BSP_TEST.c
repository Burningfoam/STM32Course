#include "BSP_TEST.h"
#include "main.h"
#include "BSP_key.h"
#include "BSP_led.h"
#include "lcd.h"
#include "tim.h"
#include "stdio.h"
#include "stdlib.h"
#include "usart.h"
unsigned char Lcd_Disp_String[20];
unsigned char Uart1_Tx_Buf[30];
unsigned char Uart1_Rx_Buf[30];
unsigned char Usart1_RxData_Temp=0;
unsigned char U1Rx_Len=0;
unsigned char CNBR=0;
unsigned char VNBR=0;
unsigned char IDLE=8;
unsigned char chewei[8][22];
float CNBR_cost = 3.5;
float VNBR_cost = 2.5;

unsigned char jiemian = 0;



unsigned char compare(unsigned char* A,unsigned char* B);
unsigned char  match_car_id(unsigned char* car_information);
unsigned char find_carport(void);
unsigned int compare_date(unsigned char* car_information,unsigned char carport_id);

void chewei_jiemian()
{
			LCD_Clear(Black);
			LCD_SetBackColor(Black);
			LCD_SetTextColor(White);
			LCD_DisplayStringLine(Line1,(unsigned char *)"       Data");
			sprintf((char *)Lcd_Disp_String, "   CNBR:%d",CNBR);
			LCD_DisplayStringLine(Line3, Lcd_Disp_String);
			sprintf((char *)Lcd_Disp_String, "   VNBR:%d",VNBR);
			LCD_DisplayStringLine(Line5, Lcd_Disp_String);
			sprintf((char *)Lcd_Disp_String, "   IDLE:%d",IDLE);
			LCD_DisplayStringLine(Line7, Lcd_Disp_String);


}

void feilv_jiemian()
{
			LCD_Clear(Black);
			LCD_SetBackColor(Black);
			LCD_SetTextColor(White);
			LCD_DisplayStringLine(Line1,(unsigned char *)"       Para");
			sprintf((char *)Lcd_Disp_String, "   CNBR:%.2f",CNBR_cost);
			LCD_DisplayStringLine(Line3, Lcd_Disp_String);
			sprintf((char *)Lcd_Disp_String, "   VNBR:%.2f",VNBR_cost);
			LCD_DisplayStringLine(Line5, Lcd_Disp_String);

}

void Key_Proc()
{
	  unsigned char key_value=0;
	  static unsigned int key_keeptime=0;
	  static unsigned char last_key_value=0;
	  static unsigned char pwm_flag=0;
    if(uwTick%20)
		   return;
		else
		{
		   key_value = key_scan();
			 switch(key_value)
			 {
				 case 1:
					     key_keeptime++;
				       {
								 if(key_keeptime==1)
								 {  
									  jiemian = !jiemian;
									  if(!jiemian)
											chewei_jiemian();
										else
											feilv_jiemian();
								 }
					       
				       }	
				       break;
				 case 2:
					     key_keeptime++;
				       {
								 if(key_keeptime==1)
								 {
									 if(jiemian)
									 {
											CNBR_cost+=0.5;
											VNBR_cost+=0.5;
											sprintf((char *)Lcd_Disp_String, "   CNBR:%.2f",CNBR_cost);
											LCD_DisplayStringLine(Line3, Lcd_Disp_String);
											sprintf((char *)Lcd_Disp_String, "   VNBR:%.2f",VNBR_cost);
											LCD_DisplayStringLine(Line5, Lcd_Disp_String);
									 }
								 }
					       
				       }	
				       break;			 
				 case 3:
					     key_keeptime++;
				       {
								 if(key_keeptime==1)
								 {
									 if(jiemian)
									 {
											 if(VNBR_cost>0)
											 {
													CNBR_cost-=0.5;
													VNBR_cost-=0.5;
													sprintf((char *)Lcd_Disp_String, "   CNBR:%.2f",CNBR_cost);
													LCD_DisplayStringLine(Line3, Lcd_Disp_String);
													sprintf((char *)Lcd_Disp_String, "   VNBR:%.2f",VNBR_cost);
													LCD_DisplayStringLine(Line5, Lcd_Disp_String);
											 }
								   }
								 }
					       
				       }	
				       break;
				 case 4:
					     key_keeptime++;
				       {
								 if(key_keeptime==1)
								 {
									 pwm_flag = !pwm_flag;
									 if(pwm_flag)
									 {
								      HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);//��pwm����ʱ��3ͨ��2��Ƶ��2k��ռ�ձ�20%
										  led_state = led_state | 0x02;
				              led_control(led_state);//led2����
									 }
									 else
									 {
										  HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
										  led_state = led_state & 0xfd;
				              led_control(led_state);//led2����
									 }
								 }
					       
				       }	
				       break;		
				 default:
               {
							   if((last_key_value>0)&&(key_value==0))
								 {
								    key_keeptime=0;
								 
								 }
							 }break;
			 }
			 last_key_value = key_value;
//		   sprintf((char *)Lcd_Disp_String, "  %d ",key_keeptime);
//			 LCD_DisplayStringLine(Line8, Lcd_Disp_String);
		}


}

void UART_Proc()
{
	 static uint32_t  UART_tick=0;
	 unsigned char i,temp,carport_id,stop_time;
	 float cost;
   if(U1Rx_Len==0)
	 {
		 UART_tick = uwTick ;
		 return;
	 }
	 
	 if((uwTick-UART_tick)>50)
	 {
	   

		 if(U1Rx_Len==22)//���ܵ���Ϣ�߼���ȷ���˴�Ҳ�������ж�������
		 {
			 if(match_car_id(Uart1_Rx_Buf) != 9)//�������д˳�����Ϊ�г�����
			 {
//				  for(i=0;i<30;i++)
//	             Uart1_Tx_Buf[i]=0;
//				  sprintf((char *)Uart1_Tx_Buf, "ok \r\n");  
//          HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
				 
				 
					carport_id = match_car_id(Uart1_Rx_Buf);
				  stop_time = compare_date(Uart1_Rx_Buf,carport_id);//����ͣ��ʱ��
				  if(stop_time!=0)//ʱ���߼���ȷ
					{
						if(chewei[carport_id][0]=='C')//�������
						{
							 cost = stop_time*CNBR_cost;
							 CNBR--;//��λ��Ϣ����
						}
						else if(chewei[carport_id][0]=='V')
						{
							 cost = stop_time*VNBR_cost;
							 VNBR--; 
						}
						IDLE++;
						
						for(i=0;i<30;i++)//���ͼƷ���Ϣ
	             Uart1_Tx_Buf[i]=0;
						for(i=0;i<10;i++)
						   Uart1_Tx_Buf[i] = chewei[carport_id][i];
						Uart1_Tx_Buf[10] = (stop_time%100)/10+48;
						Uart1_Tx_Buf[11] = stop_time%10+48;
						Uart1_Tx_Buf[12] = ':';
						Uart1_Tx_Buf[13] = ((int)cost%100)/10+48;
						Uart1_Tx_Buf[14] = (int)cost%10+48;
						Uart1_Tx_Buf[15] = '.';
						Uart1_Tx_Buf[16] = ((int)(cost*10))%10+48;
						Uart1_Tx_Buf[17] = ((int)(cost*100))%10+48;
					  sprintf((char *)Uart1_Tx_Buf, "%s \r\n",Uart1_Tx_Buf);  
            HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
						
						
						
						
						for(i=0;i<22;i++)//�����λ����
						  chewei[carport_id][i]=0;
						
				  }
					else
					{
						 for(i=0;i<30;i++)
	             Uart1_Tx_Buf[i]=0;
					   sprintf((char *)Uart1_Tx_Buf, "Error \r\n");  
             HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
					}
			 }
			 else//�����޴˳�����Ϊ�г�����
			 {
				  temp = find_carport();//Ѱ�Ҳ����䳵λ
			    if(temp ==9)//��������
					{
						 for(i=0;i<30;i++)
	             Uart1_Tx_Buf[i]=0;
					   sprintf((char *)Uart1_Tx_Buf, "Error \r\n");  
             HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
						 
					}
					else//�п��೵λ
					{
						for(i=0;i<22;i++)
					    chewei[temp][i] = Uart1_Rx_Buf[i];//�����վ�ĳ�����Ϣ
						if(chewei[temp][0]=='C')//�������
						{
							 CNBR++;
						}
						else if(chewei[temp][0]=='V')
						{
							 VNBR++; 
						}
						IDLE--;
						
					}
				 
			 }
			 
			 
			 if(!jiemian)//���½���
			 {
				 for(i=0;i<20;i++)
				    Lcd_Disp_String[i] = 0;
			   sprintf((char *)Lcd_Disp_String, "   CNBR:%d",CNBR);
				 LCD_DisplayStringLine(Line3, Lcd_Disp_String);
				 sprintf((char *)Lcd_Disp_String, "   VNBR:%d",VNBR);
				 LCD_DisplayStringLine(Line5, Lcd_Disp_String);
				 sprintf((char *)Lcd_Disp_String, "   IDLE:%d",IDLE);
				 LCD_DisplayStringLine(Line7, Lcd_Disp_String);
			 }
			 
			 if(IDLE>0)//led1����
			 {
				 led_state = led_state | 0x01;
				 led_control(led_state);
			 }
			 else
			 {
			   led_state =  led_state & 0xfe;
				 led_control(led_state);
			 }
		 }
	   else
		 {
		    for(i=0;i<30;i++)
	             Uart1_Tx_Buf[i]=0;
				sprintf((char *)Uart1_Tx_Buf, "Error \r\n");  
        HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
		 }
	 
		 
		 for(i=0;i<30;i++)
	     Uart1_Rx_Buf[i]=0;
		 
		 U1Rx_Len=0;
	 }


}

unsigned char compare(unsigned char* A,unsigned char* B)//�Ƚ��������������Ƿ���ͬ������Ϊ4��
{
	 unsigned char i;
   for(i=0;i<4;i++)
	    if(A[i]==B[i])
			{}
			else
			  return 0;
   
		return 1;
}

unsigned char  match_car_id(unsigned char* car_information)//�ͳ�����ĳ��Ʊȶԣ�����ͬ�ķ��س�λ��ţ����򷵻�9
{
	unsigned char i,j;
	unsigned char new_car_id[4];
	unsigned char old_car_id[4];
	for(i=0;i<8;i++)
	{
	   for(j=5;j<9;j++)
		 {
		   old_car_id[j-5]=chewei[i][j];
			 new_car_id[j-5]=car_information[j];
		 }
		 if(compare(new_car_id,old_car_id)==1)
		 {
		   return i;
		 }
	}
	return 9;

}


unsigned char find_carport()//Ѱ�ҳ�λ���п����򷵻س�λ��ţ��޿��෵��9
{
  unsigned char i;
	for(i=0;i<8;i++)
  if(chewei[i][0] == 0)
		return i;
  return 9;
}

unsigned int compare_date(unsigned char* information,unsigned char carport_id)//����ͣ��ʱ�䣬���߼�����ȷ������0
{
     unsigned char year=0,month=0,day=0,hour=0,minute=0,second=0;//һ��365�죬һ��30��
	   unsigned char old_year=0,old_month=0,old_day=0,old_hour=0,old_minute=0,old_second=0;
     long  time=0;
	   unsigned char i;
     unsigned char car_information[30];
		 unsigned char cheweiwei[30];
	
	   for(i=10;i<22;i++)
	   {
		   car_information[i] = information[i]-48;
       cheweiwei[i] = chewei[carport_id][i]-48;
		 }
//		 sprintf((char *)Uart1_Tx_Buf, "car_information:%s \r\n",cheweiwei);  
//     HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
//		 sprintf((char *)Uart1_Tx_Buf, "chewei:%s \r\n",chewei[carport_id]);  
//     HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
//		 sprintf((char *)Uart1_Tx_Buf, "cheweiwei:%s \r\n",cheweiwei);  
//     HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
		 year = car_information[10]*10+car_information[11];
     old_year = cheweiwei[10]*10+cheweiwei[11];
     month = car_information[12]*10+car_information[13];
     old_month = cheweiwei[12]*10+cheweiwei[13];
	   month = car_information[12]*10+car_information[13];
     old_month = cheweiwei[12]*10+cheweiwei[13];
     day = car_information[14]*10+car_information[15];
     old_day = cheweiwei[14]*10+cheweiwei[15];
	   hour = car_information[16]*10+car_information[17];
     old_hour = cheweiwei[16]*10+cheweiwei[17];
	   minute = car_information[18]*10+car_information[19];
     old_minute = cheweiwei[18]*10+cheweiwei[19];
	   second = car_information[20]*10+car_information[21];
     old_second = cheweiwei[20]*10+cheweiwei[21];
//	   for(i=0;i<30;i++)
//	       Uart1_Tx_Buf[i]=0;
//	   sprintf((char *)Uart1_Tx_Buf, "new:%d %d %d %d %d %d \r\n",year,month,day,hour,minute,second);//����ʱ����Ϣ
//     HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
//	   sprintf((char *)Uart1_Tx_Buf, "old:%d %d %d %d %d %d \r\n",old_year,old_month,old_day,old_hour,old_minute,old_second);//���ʱ����Ϣ  
//     HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
	   time = (year*8760+month*720+day*24+hour)-(old_year*8760+old_month*720+old_day*24+old_hour);
//	   sprintf((char *)Uart1_Tx_Buf, "%ld,%ld,%ld \r\n",time,(year*8760+month*720+day*24+hour),(old_year*8760+old_month*720+old_day*24+old_hour));  
//     HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
		 if(time==0)
		 {
			  if((minute>=old_minute)&&(second>old_second))
				{
				  time+=1;
//					for(i=0;i<30;i++)
//	             Uart1_Tx_Buf[i]=0;
//					sprintf((char *)Uart1_Tx_Buf, "111 \r\n");  
//           HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
				  return time;
				}
			  else 
				{
//					for(i=0;i<30;i++)
//	             Uart1_Tx_Buf[i]=0;
//					sprintf((char *)Uart1_Tx_Buf, "222 \r\n");  
//          HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
				  return 0;
				}
		 }
		 else if(time<0)
		 {
//			  for(i=0;i<30;i++)
//	             Uart1_Tx_Buf[i]=0;
//			  sprintf((char *)Uart1_Tx_Buf, "333 \r\n");  
//        HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
			  return 0;
		 }
		 else 
		 {
//			 for(i=0;i<30;i++)
//	             Uart1_Tx_Buf[i]=0;
//			 sprintf((char *)Uart1_Tx_Buf, "444 \r\n");  
//       HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
			  return time;
		 }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1)//����Ǵ���1
    {
        Uart1_Rx_Buf[U1Rx_Len]=Usart1_RxData_Temp;
        U1Rx_Len++;
        if(U1Rx_Len>(29)) //ע����Ȼ����UART1RXBUF_LEN��20�����ֽڻ��壬����ֻ���õ�UART1RXBUF_LEN-1��
            U1Rx_Len=0;
        
        //�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������,��Ҫÿ���жϵ���һ��
        HAL_UART_Receive_IT(&huart1, &Usart1_RxData_Temp, 1);    //�������µ��ã���Ϊ�ú����ڽ���һ�κ��ر��ж�    
    }
}
