#ifndef _IIC_H
#define _IIC_H

#include <STC15F2K60S2.H>
#include "intrins.h"

//函数声明
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 

void Pcf8591_Init(u8 com);
u8 Pcf8951_Read();
void At24c02_Write(u8 addr,u8 dat);
u8 At24c02_Read(u8 addr);

#endif