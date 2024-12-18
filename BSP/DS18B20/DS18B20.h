#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f4xx_hal.h"

// ��ʼ��DS18B20
uint8_t DS18B20_Init(void);

// ��ȡ�¶�ֵ
float DS18B20_Get_Temp(void);

// �����¶�ת��
void DS18B20_Start(void);

#endif // __DS18B20_H

