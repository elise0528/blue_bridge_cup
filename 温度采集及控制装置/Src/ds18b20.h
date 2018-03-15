#ifndef __DS18B20_H
#define __DS18B20_H


#include <STC15F2K60S2.H>

//单总线延时函数
void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
bit Ds18b20_Init(void);
unsigned char Temper_Read();

#endif
