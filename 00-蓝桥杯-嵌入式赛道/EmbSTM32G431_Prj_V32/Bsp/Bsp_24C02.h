#ifndef __BSP_24C02_H
#define __BSP_24C02_H

#define DATABUF_24C02_LEN  10

extern unsigned char RddataBuf_24C02[DATABUF_24C02_LEN];
extern unsigned char WrdataBuf_24C02[DATABUF_24C02_LEN];

extern void iic_24c02_write(unsigned char *pucbuff, unsigned char ucaddr, unsigned char ucNum);
extern void iic_24c02_read(unsigned char *pucbuff, unsigned char ucaddr, unsigned char ucNum);
#endif
