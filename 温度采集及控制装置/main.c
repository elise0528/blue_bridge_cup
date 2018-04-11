/*
对于没有准确值的数 判断时最好不要用 ==
做键盘时注意返回值
温度采集为10进制
定时器计数一般大于0xff 应用u16 定时器初始化记的 EA=1 ET0=1
*/

#include <STC15F2K60S2.H>
#include "key.h"
#include "ds18b20.h"
#include "intrins.h"


#define WD_XS_MS (u8) 0
#define WD_SZ_MS (u8) 1


sbit buzz = P0 ^ 6;
sbit relay = P0 ^ 4;

u8 table[12] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xbf};
u8 disbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
u8 discom = 0;

u16 time_count = 0, led_flash_time = 0;		//	记住定时器里的计数大于0xff 要用u16
bit led_stat, key_set_sig;
u8 key, set_count = 0;
u8 maxwendu = 30, minwendu = 20, wendu, maxwendu_set, minwendu_set;			// 读取的温度为10进制

u8 cur_stat, nex_stat, limitset_stat;
u8 key, key_v;


void display()
{
    P2 = ((P2 & 0x1f) | 0xe0);
    P0 = 0xff;
    P2 &= 0x1f;

    P2 = ((P2 & 0x1f) | 0xc0);
    P0 = 0x80 >> discom;
    P2 &= 0x1f;

    P0 = 0xff;
    P2 = ((P2 & 0x1f) | 0xe0);
    P0 = table[disbuf[discom]];
    P2 &= 0x1f;

    P0 = 0x00;

    if(++discom == 8)
        discom = 0;
}

void devic_com(bit com_b,bit com_r)
{
    P0 = 0x00;
    P2 = ((P2 & 0x1f) | 0xa0);
    buzz = com_b;
	relay = com_r;
    P2 &= 0x1f;
}


void led_com(u8 com)
{
    P0 = 0xff;
    P2 = ((P2 & 0x1f) | 0x80);
    P0 = com;
    P2 &= 0x1f;
}

void StcInit()
{
	P2=0xa0;
	P0=0x00;
	P2=0x80;
	P0=0xff;
	P2&=0x1f;
}


void Timer0Init(void)		//1毫秒@11.0592MHz
{
    AUXR |= 0x80;		//定时器时钟1T模式
    TMOD &= 0xF0;		//设置定时器模式
    TL0 = 0xCD;		//设置定时初值
    TH0 = 0xD4;		//设置定时初值
    TF0 = 0;		//清除TF0标志
    TR0 = 1;		//定时器0开始计时

    EA = 1;
    ET0 = 1;
}

void main()
{
	StcInit();
	Timer0Init();
	
	while(1)
	{
		cur_stat=nex_stat;
		switch (cur_stat)
        {
        	case WD_XS_MS:break;
        	case WD_SZ_MS:break;
        	default:break;
        }
		
		key=keyscan();
		switch (key)
        {
        	case 0:break;
        	case 1:break;
        	case 2:break;
			case 3:break;
        	case 4:break;
        	case 5:break;
			case 6:break;
        	case 7:break;
        	case 8:break;
			case 9:break;
        	case 10:break;
        	case 11:break;
        	default:break;
        }
	}
}

void time0() interrupt 1
{
	display();
}
