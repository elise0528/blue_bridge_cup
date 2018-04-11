#ifndef __DS1302_H
#define __DS1302_H

#include <STC15F2K60S2.H>
#include <intrins.h>

void Write_Ds1302_Byte(unsigned  char temp);
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );
void Ds1302Init();

#endif
