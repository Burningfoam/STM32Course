#ifndef __BSP_TEST_H
#define __BSP_TEST_H

extern void BSP_LED_TestTask(void);
extern void LCD_TestTask(void);
extern void KeyProcess(void);
extern void  Uart1_RX_Pro(void);
extern void  IIC_Test_24C02(void);
extern void  IIC_Test_MCP4017(void);
extern void  AD_Test(void);
extern void  PulseMeasure_Test(void);
extern void  DA_Display_Test(void);
#endif
