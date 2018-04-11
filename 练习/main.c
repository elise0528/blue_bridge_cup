#include <STC15F2K60S2.H>
#include "iic.h"


u8 tab[12] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xbf};
u8 disbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
u8 discom = 0;

u8 second, minute, hour, temp, shidu, dat, guang;

void display()
{
    P2 = ((P2 & 0x1f) | 0xe0);
    P0 = 0xff;
    P2 &= 0x1f;

    P2 = ((P2 & 0x1f) | 0xc0);
    P0 = 0x80 >> discom;
    P2 &= 0x1f;

    P2 = ((P2 & 0x1f) | 0xe0);
    P0 = tab[disbuf[discom]];
    P2 &= 0x1f;

    P0 = 0x00;

    if (++discom > 8) {
        discom = 0;
    }
}


void StcInit()
{
    P2 = 0xa0;
    P0 = 0x00;
    P2 = 0x80;
    P0 = 0xff;
    P2 &= 0x1f;
}

void Timer0Init(void)		//2毫秒@11.0592MHz
{
    AUXR |= 0x80;		//定时器时钟1T模式
    TMOD &= 0xF0;		//设置定时器模式
    TL0 = 0x9A;		//设置定时初值
    TH0 = 0xA9;		//设置定时初值
    TF0 = 0;		//清除TF0标志
    TR0 = 1;		//定时器0开始计时
    EA = 1;
    ET0 = 1;
}





void main()
{
    StcInit();
    Timer0Init();
	Pcf8591_Init(0x01);
    while(1) {
		
		guang=Pcf8951_Read()*100/256;


		disbuf[7]=guang/100;
		disbuf[6]=guang%100/10;
 		disbuf[5]=guang%10;
        disbuf[4] = 10;
        disbuf[3] = 10;
        disbuf[2] = 10;
        disbuf[1] = dat / 10;
        disbuf[0] = dat % 10;

    }
}

void time0() interrupt 1
{
    display();
}