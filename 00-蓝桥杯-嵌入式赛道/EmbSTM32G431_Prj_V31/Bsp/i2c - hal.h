#ifndef __I2C_H
#define __I2C_H

#include "main.h"


#define DELAY_TIME	20

//extern void delay1(unsigned int n);

extern void I2CStart(void);
extern void I2CStop(void);
extern unsigned char I2CWaitAck(void);
extern void I2CSendAck(void);
extern void I2CSendNotAck(void);
extern void I2CSendByte(unsigned char cSendByte);
extern unsigned char I2CReceiveByte(void);
extern void I2CInit(void);

#endif
