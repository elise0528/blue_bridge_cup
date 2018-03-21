#include "key.h"
#include "onewire.h"

#define SMF_MS  (u8)0
#define ZRF_MS (u8)1
#define CF_MS  (u8)2
#define WD_MS (u8)3

#define PWM P34

u8 tab[13] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xbf, 0xc6};
u8 disbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
u8 discom;

u16 time_count;

u8 key, key_s7, key_s6, key_s5, key_s4;
u8 stat, stat_t, stat_pwm;
u16 time = 50;

bit pwm_flag;
bit stat_tz, stat_chage;
u8 pwm_th, pwm_tl;




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
    P2 = 0xa0;
    P0 = 0x00;
    P2 = 0x80;
    P0 = 0xff;
    P2 = 0x1f;
    P0 = 0xff;
}

void led_com(u8 com)
{
    P0 = 0xff;
    P2 = ((P2 & 0x1f) | 0x80);
    P0 = com;
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

void Timer1Init(void)		//200微秒@11.0592MHz
{
    AUXR |= 0x40;		//定时器时钟1T模式
    TMOD &= 0x0F;		//设置定时器模式
    TMOD |= 0x10;		//设置定时器模式
    TL1 = 0xFF;		//设置定时初值
    TH1 = 0xFF;		//设置定时初值
    TF1 = 0;		//清除TF1标志
    TR1 = 1;		//定时器1开始计时

    EA = 1;
    ET1 = 1;
}



void fs_func()
{
    switch (stat)
    {
    case SMF_MS:
        if(!stat_tz)led_com(0xfe);
        disbuf[6] = 1;
        break;
    case ZRF_MS:
        if(!stat_tz)led_com(0xfd);
        disbuf[6] = 2;
        break;
    case CF_MS:
        if(!stat_tz)led_com(0xfb);
        disbuf[6] = 3;
        break;
    default:
        break;
    }
    disbuf[7] = 11;
    disbuf[5] = 11;
    disbuf[4] = 10;
    disbuf[3] = time / 1000;
    disbuf[2] = time % 1000 / 100;
    disbuf[1] = time % 1000 % 100 / 10;
    disbuf[0] = time % 10;
}




void wd_func()
{
    u8 temp;
    temp = Read_Temper();
    disbuf[7] = 11;
    disbuf[6] = 4;
    disbuf[5] = 11;
    disbuf[4] = 10;
    disbuf[3] = 10;
    disbuf[2] = temp / 10;
    disbuf[1] = temp % 10;
    disbuf[0] = 12;

    if(stat_chage)
    {
        led_com(0xff);
        stat_chage = 0;
    }

}




void main()
{
    stcinit();
    Timer0Init();
    Timer1Init();

    while(1)
    {

        switch (stat)
        {
        case SMF_MS:
            fs_func();
            break;
        case ZRF_MS:
            fs_func();
            break;
        case CF_MS:
            fs_func();
            break;
        case WD_MS:
            wd_func();
            break;
        default:
            break;
        }


        key = keyscan();

        if(stat == WD_MS)
        {
            if(key != 7)
                key = 99;
        }
        switch (key)
        {
        case 7:
            key_s7++;

            if(key_s7 > 1)key_s7 = 0;
            switch (key_s7)
            {
            case 1:
                stat_t = stat;
                stat_chage = 1;
                stat = WD_MS;
                break;
            case 0:
                stat = stat_t;
                break;
            default:
                break;
            }
            break;
        case 6:
            ET1 = 0;
            time = 0;
            stat_tz = 1;
            led_com(0xff);

            break;
        case 5:
            key_s5++;
            if(key_s5 > 2)key_s5 = 0;
            switch (key_s5)
            {
            case 0:
                time = time + 0;
                break;
            case 1:
                time = time + 60;
                break;
            case 2:
                time = time + 60;
                break;
            default:
                break;
            }
            stat_tz = 0;
            ET1 = 1;
            break;
        case 4:
            key_s4++;
            stat_tz = 0;
            if(key_s4 > 2)key_s4 = 0;
            switch (key_s4)
            {
            case 0:
                stat = SMF_MS;
                break;
            case 1:
                stat = ZRF_MS;
                break;
            case 2:
                stat = CF_MS;
                break;
            default:
                stat = SMF_MS;
                break;
            }
            break;
        default:
            break;
        }
    }
}

void time0() interrupt 1
{
    display();
    if(++time_count > 500)
    {
        time_count = 0;
        if(ET1 == 1)
        {

            if(time == 0)
            {
                ET1 = 0;
            }
            else
            {
                time--;
            }
        }
    }

}

void time1() interrupt 3
{
    if(stat == WD_MS)
    {
        stat_pwm = stat_t;
    }
    else
    {
        stat_pwm = stat;
    }

    pwm_flag = ~pwm_flag;
    if(pwm_flag)
    {

        switch (stat_pwm)
        {
        case SMF_MS:
            pwm_th = 0xf7;
            pwm_tl = 0x5c;
            break;
        case ZRF_MS:
            pwm_th = 0xf3;
            pwm_tl = 0x0a;
            break;
        case CF_MS:
            pwm_th = 0xe1;
            pwm_tl = 0xc3;
            break;
        default:
            break;
        }
        TH1 = pwm_th;
        TL1 = pwm_th;
        PWM = 1;
    }
    else
    {
        switch (stat_pwm)
        {
        case SMF_MS:
            pwm_th = 0xdd;
            pwm_tl = 0x71;
            break;
        case ZRF_MS:
            pwm_th = 0xe1;
            pwm_tl = 0xc3;
            break;
        case CF_MS:
            pwm_th = 0xf3;
            pwm_tl = 0xa0;
            break;
        default:
            break;
        }
        TH1 = pwm_th;   //如果是输出高电平，则装载低电平时间。
        TL1 = pwm_th;
        PWM = 0;
    }
}

