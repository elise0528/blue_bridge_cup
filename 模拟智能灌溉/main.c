#include <STC15F2K60S2.H>
#include "key.h"
#include "ds1302.h"
#include "iic.h"

#define ZD_MS (u8) 0
#define SD_MS (u8) 1
#define SZ_MS (u8) 2

sbit buzz = P0 ^ 6;
sbit relay = P0 ^ 4;

u8 tab[12] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xbf};
u8 disbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
u8 discom;

u8 cur_stat, nex_stat, devices_chage, key;
bit key_s7, key_s6, key_s5, key_s4, shidu_h,shidu_l;
bit buzz_stat, relay_stat, led_stat, at24c02_stat, buzz_set;
u8 hour, second;
u8 shidu, shidu_set = 40;

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

    if(++discom > 8)
    {
        discom = 0;
    }
}

void stcinit()
{
    P0 = 0x00;
    P2 = 0xa0;
    P0 = 0x00;
    P2 = 0x80;
    P0 = 0xff;
    P2 = 0x1f;
}


void led_com(u8 com)
{
    P0 = 0xff;
    P2 = ((P2 & 0x1f) | 0x80);
    P0 = com;
    P2 &= 0x1f;

}

void devices_com(bit com_b, bit com_r)
{
    P0 = 0x00;
    P2 = ((P2 & 0x1f) | 0xa0);
    buzz = com_b;
    relay = com_r;
    P2 &= 0x1f;
}



void Timer0Init(void)       //2����@11.0592MHz
{
    AUXR |= 0x80;       //��ʱ��ʱ��1Tģʽ
    TMOD &= 0xF0;       //���ö�ʱ��ģʽ
    TL0 = 0x9A;     //���ö�ʱ��ֵ
    TH0 = 0xA9;     //���ö�ʱ��ֵ
    TF0 = 0;        //���TF0��־
    TR0 = 1;        //��ʱ��0��ʼ��ʱ
    EA = 1;
    ET0 = 1;
}


void zd_func()
{
    second = Ds1302_Single_Byte_Read(0x83);
    hour = Ds1302_Single_Byte_Read(0x85);
    shidu = pcf8591_read() * 100 / 255;

    disbuf[7] = hour / 16;
    disbuf[6] = hour % 16;
    disbuf[5] = 11;
    disbuf[4] = second / 16;
    disbuf[3] = second % 16;
    disbuf[2] = 10;
    disbuf[1] = shidu / 10;
    disbuf[0] = shidu % 10;

    led_com(0xfe);

    if(at24c02_stat)
    {
        at24c02_write(0x00, shidu_set);
        at24c02_stat = 0;
    }


    if(shidu < shidu_set && !shidu_l)
    {
        devices_chage = 1;
        shidu_l = 1;
        shidu_h=0;
        relay_stat = 1;
    }
    if (shidu >= shidu_set && !shidu_h)
    {
        devices_chage = 1;
        shidu_l = 0;
        shidu_h=1;
        relay_stat = 0;
    }

    if (devices_chage)
    {
        devices_com(0, relay_stat);
        devices_chage = 0;
    }



}

void sd_func()
{
    second = Ds1302_Single_Byte_Read(0x83);
    hour = Ds1302_Single_Byte_Read(0x85);
    shidu = pcf8591_read() * 100 / 255;

    disbuf[7] = hour / 16;
    disbuf[6] = hour % 16;
    disbuf[5] = 11;
    disbuf[4] = second / 16;
    disbuf[3] = second % 16;
    disbuf[2] = 10;
    disbuf[1] = shidu / 10;
    disbuf[0] = shidu % 10;

    led_com(0xfd);


    if(shidu < shidu_set && !shidu_l)
    {
        devices_chage = 1;
        shidu_l = 1;
        shidu_h=0;
        buzz_stat = 1;
    }
    if (shidu >= shidu_set && !shidu_h)
    {
        devices_chage = 1;
        shidu_l = 0;
        shidu_h=1
        buzz_stat = 0;
    }

    if (devices_chage)
    {
        if (buzz_set)
        {
            devices_com(buzz_stat, relay_stat);
        }
        else
        {
            devices_com(0, relay_stat);
        }
        devices_chage = 0;
    }

}

void sz_func()
{
    disbuf[7] = 11;
    disbuf[6] = 11;
    disbuf[5] = 10;
    disbuf[4] = 10;
    disbuf[3] = 10;
    disbuf[2] = 10;
    disbuf[1] = shidu_set / 10;
    disbuf[0] = shidu_set % 10;
}


void main()
{
    stcinit();
    Ds1302_Init();
    pcf8591_Init(0x03);
    Timer0Init();

    while(1)
    {

        cur_stat = nex_stat;
        switch (cur_stat)
        {
        case ZD_MS:
            zd_func();
            if(key_s7)
            {
                nex_stat = SD_MS;
                shidu_l = 0;
                shidu_l = 0;
                key_s7 = 0;
            }
            if (key_s6)
            {
                nex_stat = SZ_MS;
                key_s6 = 0;
            }
            if (key_s5)
            {
                key_s5 = 0;
            }
            if (key_s4)
            {
                key_s4 = 0;
            }
            break;
        case SD_MS:
            sd_func();
            if (key_s7)
            {
                nex_stat = ZD_MS;
                shidu_l = 0;
                shidu_h=0
                
                key_s7 = 0;
            }
            if (key_s6)
            {
                nex_stat = SD_MS;
                devices_chage = 1;
                buzz_stat = ~buzz_stat;
                key_s6 = 0;
            }
            if (key_s5)
            {
                nex_stat = SD_MS;
                devices_chage = 1;
                relay_stat = 1;
                key_s5 = 0;
            }
            if (key_s4)
            {
                nex_stat = SD_MS;
                devices_chage = 1;
                relay_stat = 0;
                key_s4 = 0;
            }
            break;
        case SZ_MS:
            sz_func();
            if (key_s7)
            {
                key_s7 = 0;
            }
            if (key_s6)
            {
                nex_stat = ZD_MS;
                at24c02_stat = 1;
                key_s6 = 0;
            }
            if (key_s5)
            {
                nex_stat = SZ_MS;
                ++shidu_set;
                key_s5 = 0;
            }
            if (key_s4)
            {
                nex_stat = SZ_MS;
                shidu_set--;
                key_s4 = 0;
            }
            break;
        default:
            break;
        }

        key = keyscan();
        switch (key)
        {
        case 7:
            key_s7 = ~key_s7;
            break;
        case 6:
            key_s6 = ~key_s6;
            break;
        case 5:
            key_s5 = ~key_s5;
            break;
        case 4:
            key_s4 = ~key_s4;
        default:
            break;
        }

    }

}


void time0() interrupt 1
{
    display();
}

