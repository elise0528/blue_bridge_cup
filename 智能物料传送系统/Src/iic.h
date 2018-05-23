#ifndef _IIC_H
#define _IIC_H

#include <STC15F2K60S2.H>

#include "intrins.h"

#define somenop Delay5us()
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

//函数声明
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
u8 e2prom_read(u8 addr);
void e2prom_write(u8 addr,u8 dat);
void adcinit();
u8 adc_read();

#endif