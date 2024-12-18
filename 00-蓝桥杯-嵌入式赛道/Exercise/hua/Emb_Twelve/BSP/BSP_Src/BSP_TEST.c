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
								      HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);//打开pwm，定时器3通道2，频率2k，占空比20%
										  led_state = led_state | 0x02;
				              led_control(led_state);//led2控制
									 }
									 else
									 {
										  HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
										  led_state = led_state & 0xfd;
				              led_control(led_state);//led2控制
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
	   

		 if(U1Rx_Len==22)//收受的信息逻辑正确（此处也可增加判断条件）
		 {
			 if(match_car_id(Uart1_Rx_Buf) != 9)//车库已有此车，视为有车出库
			 {
//				  for(i=0;i<30;i++)
//	             Uart1_Tx_Buf[i]=0;
//				  sprintf((char *)Uart1_Tx_Buf, "ok \r\n");  
//          HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
				 
				 
					carport_id = match_car_id(Uart1_Rx_Buf);
				  stop_time = compare_date(Uart1_Rx_Buf,carport_id);//计算停车时间
				  if(stop_time!=0)//时间逻辑正确
					{
						if(chewei[carport_id][0]=='C')//计算费用
						{
							 cost = stop_time*CNBR_cost;
							 CNBR--;//车位信息更新
						}
						else if(chewei[carport_id][0]=='V')
						{
							 cost = stop_time*VNBR_cost;
							 VNBR--; 
						}
						IDLE++;
						
						for(i=0;i<30;i++)//发送计费信息
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
						
						
						
						
						for(i=0;i<22;i++)//清除车位数据
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
			 else//车库无此车，视为有车进入
			 {
				  temp = find_carport();//寻找并分配车位
			    if(temp ==9)//车库已满
					{
						 for(i=0;i<30;i++)
	             Uart1_Tx_Buf[i]=0;
					   sprintf((char *)Uart1_Tx_Buf, "Error \r\n");  
             HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
						 
					}
					else//有空余车位
					{
						for(i=0;i<22;i++)
					    chewei[temp][i] = Uart1_Rx_Buf[i];//保存进站的车辆信息
						if(chewei[temp][0]=='C')//计算费用
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
			 
			 
			 if(!jiemian)//更新界面
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
			 
			 if(IDLE>0)//led1控制
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

unsigned char compare(unsigned char* A,unsigned char* B)//比较两个数组内容是否相同（长度为4）
{
	 unsigned char i;
   for(i=0;i<4;i++)
	    if(A[i]==B[i])
			{}
			else
			  return 0;
   
		return 1;
}

unsigned char  match_car_id(unsigned char* car_information)//和车库里的车牌比对，有相同的返回车位编号，否则返回9
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


unsigned char find_carport()//寻找车位，有空余则返回车位编号，无空余返回9
{
  unsigned char i;
	for(i=0;i<8;i++)
  if(chewei[i][0] == 0)
		return i;
  return 9;
}

unsigned int compare_date(unsigned char* information,unsigned char carport_id)//计算停车时间，若逻辑不正确，返回0
{
     unsigned char year=0,month=0,day=0,hour=0,minute=0,second=0;//一年365天，一月30天
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
//	   sprintf((char *)Uart1_Tx_Buf, "new:%d %d %d %d %d %d \r\n",year,month,day,hour,minute,second);//出库时间信息
//     HAL_UART_Transmit(&huart1,Uart1_Tx_Buf,30,1000);
//	   sprintf((char *)Uart1_Tx_Buf, "old:%d %d %d %d %d %d \r\n",old_year,old_month,old_day,old_hour,old_minute,old_second);//入库时间信息  
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
    if(huart->Instance==USART1)//如果是串口1
    {
        Uart1_Rx_Buf[U1Rx_Len]=Usart1_RxData_Temp;
        U1Rx_Len++;
        if(U1Rx_Len>(29)) //注意虽然开了UART1RXBUF_LEN（20）个字节缓冲，但是只能用到UART1RXBUF_LEN-1个
            U1Rx_Len=0;
        
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量,需要每次中断调用一下
        HAL_UART_Receive_IT(&huart1, &Usart1_RxData_Temp, 1);    //必须重新调用，因为该函数在接收一次后会关闭中断    
    }
}
