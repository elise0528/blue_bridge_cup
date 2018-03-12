#ifndef __CONFIG_H
#define __CONFIG_H

#include "delay.h"

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long int

sbit buzz  = P0^6; 	//蜂鸣器接口
sbit relay = P0^4;	//继电器接口


void led_com(u8 com)
{
	P0 = 0xff;	//消除P0之前的状态对LED的影响！
	P2 = (P2&0x1f|0x80);
	P0 = com;
	P2 &= 0x1f;
}

void buzz_com(bit buzz_state)
{
	P2 = (P2&0x1f|0xa0);
	buzz = buzz_state;
	P2 &= 0x1f;
}

void relay_com(bit relay_state)	//继电器控制
{
	P2 = (P2&0x1f|0xa0);
	relay = relay_state;
	P2 &= 0x1f;
}


#endif 