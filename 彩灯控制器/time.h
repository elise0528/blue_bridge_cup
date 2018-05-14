#ifndef _TIME_H
#define _TIME_H

#include "STC15F2K60S2.h"

void Timer0Init(void)		//1000微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	EA=1;
	ET0=1;
}



void Timer1Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x28;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	
	EA=1;
	ET1=1;
}

void Timer2Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x9A;		//设置定时初值
	T2H = 0xA9;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	
	EA=1;
	IE2|=0X04;
}




#endif