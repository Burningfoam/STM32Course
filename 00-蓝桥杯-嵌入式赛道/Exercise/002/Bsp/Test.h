#ifndef __Test_H
#define __Test_H

extern unsigned char su;
extern unsigned char year;
extern unsigned char month,day;


extern void BSP_LED_TestTask(void);
extern void KeyProcess(void);
extern void KeyProcess2(void);
extern void LCD_TestTask(void);
extern void  Uart1_RX_Pro(void);
extern void PWM_Display(void);

void pwmchange(void);

#endif
