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
extern void pcf8591_Init(u8 com);
extern u8 pcf8591_read();
extern void at24c02_write(u8 addr,u8 dat);

#endif