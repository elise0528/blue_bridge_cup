/*
.h文件编写时 __XX_H 重名 导致找不到函数原型！
*/

#include <STC15F2K60S2.H>
#include "key.h"
#include "iic.h"

#define CS_MS (u8) 0
#define TZ_MS (u8) 1

sbit relay = P0 ^ 4;

u8 tab[21] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10, 0xff};
u8 disbuf[8] = {20, 20, 20, 20, 20, 20, 20, 20};
u8 discom;

u8 key, guang, count_100ms;
u8 cur_stat, nex_stat;
bit key_s7, key_s6, led_stat, relay_stat, chushui_stat;
u16 chushui, jiage;


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

    P0 = 0xff;

    if(++discom > 8) {
        discom = 0;
    }
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

void Timer1Init(void)		//1毫秒@11.0592MHz
{
    AUXR |= 0x40;		//定时器时钟1T模式
    TMOD &= 0x0F;		//设置定时器模式
    TL1 = 0xCD;		//设置定时初值
    TH1 = 0xD4;		//设置定时初值
    TF1 = 0;		//清除TF1标志
    TR1 = 1;		//定时器1开始计时
    EA = 1;
    ET1 = 1;
}


void stc_init()
{
    P2 = 0xa0;
    P0 = 0x00;
    P2 = 0x80;
    P0 = 0xff;
}

void led_com(u8 com)
{

    P0 = 0xff;
    P2 = ((P2 & 0x1f) | 0x80);
    P0 = com;
    P2 &= 0x1f;

    P0 = 0x00;
}

void relay_com(bit relay_c)
{
    P2 = ((P2 & 0x1f) | 0xa0);
    relay = relay_c;
    P2 &= 0x1f;
}

void cs_func()
{
    disbuf[7] = 20;
    disbuf[6] = 10;
    disbuf[5] = 5;
    disbuf[4] = 0;
    if(chushui_stat && chushui <= 9999) {
        disbuf[3] = chushui / 1000;
        disbuf[2] = (chushui % 1000 / 100) + 10;
        disbuf[1] = chushui % 1000 % 100 / 10;
        disbuf[0] = chushui % 10;
        relay_stat = 1;
    } else {
        disbuf[3] = 0;
        disbuf[2] = 10;
        disbuf[1] = 0;
        disbuf[0] = 0;
        relay_stat = 0;
    }
}

void tz_func()
{
    jiage = chushui  * 0.5 + 0.5;

    disbuf[7] = 20;
    disbuf[6] = 10;
    disbuf[5] = 5;
    disbuf[4] = 0;
    disbuf[3] = jiage / 1000;
    disbuf[2] = (jiage % 1000 / 100) + 10;
    disbuf[1] = jiage % 1000 % 100 / 10;
    disbuf[0] = jiage % 10;
}


void main()
{
    stc_init();
    pcf8951_init(0x01);
    Timer0Init();
    Timer1Init();

    while(1) {

        guang = pcf8951_read();

        cur_stat = nex_stat;
        switch (cur_stat) {
        case CS_MS:						//出水模式
            cs_func();
            if(key_s7) {
                nex_stat = CS_MS;
                chushui_stat = 1;
                key_s7 = 0;
            }
            if(key_s6) {
                nex_stat = TZ_MS;
                chushui_stat = 0;
                relay_stat = 0;
                key_s6 = 0;
            }
            break;
        case TZ_MS:						//停止模式
            tz_func();
            if(key_s7) {
                nex_stat = CS_MS;
                chushui_stat = 1;
                count_100ms = 0;
                chushui = 0;
                key_s7 = 0;
            }
			if(key_s6){
				key_s6 = 0;
			}
            break;
        default:
            break;
        }

        key = keyscan();
        switch (key) {
        case 7:
            key_s7 = ~key_s7;
            break;
        case 6:
            key_s6 = ~key_s7;
            break;
        default:
            break;
        }
    }
}

void time0() interrupt 1
{
    display();
    if(chushui_stat) {
        count_100ms++;
        if(count_100ms >= 50) {
            count_100ms = 0;
            chushui++;
        }
    }
}

void time1() interrupt 3
{

    if(guang < 64) {
        led_com(0xfe);
    } else {
        led_com(0xff);
    }

    if(relay_stat) {
        relay_com(1);
    } else {
        relay_com(0);
    }
}
