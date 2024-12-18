#ifndef __BSP_TEST_H
#define __BSP_TEST_H

void lcd_disp();
void key_proc();
void uart_proc();
void store_value();
void led_proc();
void RTC_Process(void);
extern unsigned int out_Fre;
extern unsigned char out_Duty;

#endif
