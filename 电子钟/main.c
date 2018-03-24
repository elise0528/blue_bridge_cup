/*
led��˸ֹͣδ��� �ѽ�� do{....}while(....);
�޸� ��״̬�����
*/

#include "key.h"
#include "ds1302.h"
#include "ds18b20.h"


#define SZ_XS_MS (u8) 0     //ʱ����ʾģʽ
#define SZ_S_MS (u8) 1      //����Сʱģʽ
#define SZ_F_MS (u8) 2      //���÷���ģʽ
#define SZ_M_MS (u8) 3      //��������ģʽ
#define NZ_S_MS (u8) 4      //��������Сʱģʽ
#define NZ_F_MS (u8) 5      //�������÷���ģʽ
#define NZ_M_MS (u8) 6      //��������Сʱģʽ
#define WD_XS_MS (u8) 7     //�¶���ʾģʽ


u8 tab[13] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xbf, 0xc6};
u8 disbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
u8 discom = 0;

u8 key, temp, cur_stat, nex_stat;
bit key_s7, key_s6, key_s5, key_s4;


u8 second, minute, hour;
u8 second_t, minute_t, hour_t;
u8 second_a = 0, minute_a = 0, hour_a = 0;


u8 wendu;

bit smg_flash_flag, led_flash_stat, alarm_stat, key_flag;
u8  led_count = 0, count_led_flash = 0;
u16 count_1s, count_200ms;


void led_com(u8 com)
{

    P2 = ((P2 & 0x1f) | 0x80);
    P0 = com;
    P2 &= 0x1f;

    P0 = 0xff;
}

void display()
{
    P2 = ((P2 & 0x1f) | 0xe0);
    P0 = 0xff;
    P2 &= 0x1f;

    P2 = ((P2 & 0x1f) | 0xc0);  //λѡ
    P0 = 0x80 >> discom;
    P2 &= 0x1f;

    P2 = ((P2 & 0x1f) | 0xe0);  //��ѡ
    P0 = tab[disbuf[discom]];
    P2 &= 0x1f;

    P0 = 0xff;

    if(++discom > 8) {
        discom = 0;
    }
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

void Timer1Init(void)       //2����@11.0592MHz
{
    AUXR |= 0x40;       //��ʱ��ʱ��1Tģʽ
    TMOD &= 0x0F;       //���ö�ʱ��ģʽ
    TL1 = 0x9A;     //���ö�ʱ��ֵ
    TH1 = 0xA9;     //���ö�ʱ��ֵ
    TF1 = 0;        //���TF1��־
    TR1 = 1;        //��ʱ��1��ʼ��ʱ

    EA = 1;
    ET1 = 1;
}

void stcinit()      //STC��ʼ���ر�buzz��relay��led
{
    P0 = 0x00;
    P2 = 0xa0;
    P0 = 0x00;
    P2 = 0x80;
    P0 = 0xff;
    P2 = 0x1f;
}

void sz_xs_func()       //ʱ����ʾ������
{
    do {
        hour = Ds1302_Single_Byte_Read(0x85);
        minute = Ds1302_Single_Byte_Read(0x83);
        second = Ds1302_Single_Byte_Read(0x81);

        disbuf[7] = hour / 16;
        disbuf[6] = hour % 16;
        disbuf[5] = 11;
        disbuf[4] = minute / 16;
        disbuf[3] = minute % 16;
        disbuf[2] = 11;
        disbuf[1] = second / 16;
        disbuf[0] = second % 16;

        if(alarm_stat) {
            temp = P3 & 0x0f;
            if (temp != 0x0f) {
                Delay10ms();
                temp = P3 & 0x0f;
                if (temp != 0x0f) {
                    alarm_stat = 0;
                    count_led_flash = 0;
                    led_com(0xff);
                    while(temp != 0x0f) {
                        temp = P3 & 0x0f;
                    }
                }

            }
        }
    } while (alarm_stat);
}

void sz_s_func()        //ʱ��-Сʱ������ʾ������
{

    if (smg_flash_flag) {
        disbuf[7] = 10;
        disbuf[6] = 10;
    } else {
        disbuf[7] = hour_t / 10;
        disbuf[6] = hour_t % 10;
        disbuf[4] = minute_t / 10;
        disbuf[3] = minute_t % 10;
        disbuf[1] = second_t / 10;
        disbuf[0] = second_t % 10;
    }
}

void sz_f_func()        //ʱ��-����������ʾ������
{
    if (smg_flash_flag) {
        disbuf[4] = 10;
        disbuf[3] = 10;
    } else {
        disbuf[7] = hour_t / 10;
        disbuf[6] = hour_t % 10;
        disbuf[4] = minute_t / 10;
        disbuf[3] = minute_t % 10;
        disbuf[1] = second_t / 10;
        disbuf[0] = second_t % 10;
    }
}

void sz_m_func()        //ʱ��-��������ʾ������
{
    if (smg_flash_flag) {
        disbuf[1] = 10;
        disbuf[0] = 10;
    } else {
        disbuf[7] = hour_t / 10;
        disbuf[6] = hour_t % 10;
        disbuf[4] = minute_t / 10;
        disbuf[3] = minute_t % 10;
        disbuf[1] = second_t / 10;
        disbuf[0] = second_t % 10;
    }
}

void nz_s_func()        //����-Сʱ������ʾ������
{
    if(smg_flash_flag) {    //ʱ�������µ� ʱ����˸
        disbuf[7] = 10;
        disbuf[6] = 10;
    } else {
        disbuf[7] = hour_t / 10;
        disbuf[6] = hour_t % 10;
        disbuf[4] = minute_t / 10;
        disbuf[3] = minute_t % 10;
        disbuf[1] = second_t / 10;
        disbuf[0] = second_t % 10;
    }
}

void nz_f_func()        //����-����������ʾ������
{
    if(smg_flash_flag) {    //ʱ�������µ� ʱ����˸

        disbuf[4] = 10;
        disbuf[3] = 10;
    } else {
        disbuf[7] = hour_t / 10;
        disbuf[6] = hour_t % 10;
        disbuf[4] = minute_t / 10;
        disbuf[3] = minute_t % 10;
        disbuf[1] = second_t / 10;
        disbuf[0] = second_t % 10;
    }
}

void nz_m_func()        //����-��������ʾ������
{
    if(smg_flash_flag) {    //ʱ�������µ� ʱ����˸

        disbuf[1] = 10;
        disbuf[0] = 10;
    } else {
        disbuf[7] = hour_t / 10;
        disbuf[6] = hour_t % 10;
        disbuf[4] = minute_t / 10;
        disbuf[3] = minute_t % 10;
        disbuf[1] = second_t / 10;
        disbuf[0] = second_t % 10;
    }
}

void wd_xs_func()       //�¶���ʾ������
{
    wendu = Temper_Read();

    disbuf[7] = 10;
    disbuf[6] = 10;
    disbuf[5] = 10;
    disbuf[4] = 10;
    disbuf[3] = 10;
    disbuf[2] = wendu / 10;
    disbuf[1] = wendu % 10;
    disbuf[0] = 12;
}

void main()
{
    stcinit();
    Timer0Init();
    Timer1Init();
    Ds1302_Init(0x23, 0x59, 0x50);

    while(1) {

        cur_stat = nex_stat;

        switch(cur_stat) {
        case SZ_XS_MS:
            sz_xs_func();
            if (key_s7) {
                nex_stat = SZ_S_MS;
                hour_t = ((hour >> 4) * 10) + (hour % 16);
                minute_t = ((minute >> 4) * 10) + (minute % 16);
                second_t = ((second >> 4) * 10) + (second % 16);
                key_s7 = 0;
            }
            if (key_s6) {
                nex_stat = NZ_S_MS;
                hour_t = ((hour >> 4) * 10) + (hour % 16);
                minute_t = ((minute >> 4) * 10) + (minute % 16);
                second_t = ((second >> 4) * 10) + (second % 16);
                key_s6 = 0;

            }
            if (key_s5) {
                key_s5 = 0;
            }
            if (key_s4) {
                key_s4 = 0;
            }
            break;
        case SZ_S_MS:
            sz_s_func();
            if (key_s7) {
                nex_stat = SZ_F_MS;
                key_s7 = 0;
            }
            if (key_s6) {
                key_s6 = 0;
            }
            if (key_s5) {
                hour_t++;
                if (hour_t > 23) {
                    hour_t = 0;
                }
                key_s5 = 0;
            }
            if (key_s4) {
                hour_t--;
                if (hour_t > 23) {
                    hour_t = 23;
                }
                key_s4 = 0;
            }
            break;
        case SZ_F_MS:
            sz_f_func();
            if (key_s7) {
                nex_stat = SZ_M_MS;
                key_s7 = 0;
            }
            if (key_s6) {
                key_s6 = 0;
            }
            if (key_s5) {
                minute_t++;
                if (minute_t > 59) {
                    minute_t = 0;
                }
                key_s5 = 0;
            }
            if (key_s4) {
                minute_t--;
                if (minute_t > 59) {
                    minute_t = 59;
                }
                key_s4 = 0;
            }
            break;
        case SZ_M_MS:
            sz_m_func();
            if (key_s7) {
                nex_stat = SZ_XS_MS;
                hour = ((hour_t / 10) << 4) + (hour_t % 10);
                minute = ((minute_t / 10) << 4) + (minute_t % 10);
                second = ((second_t / 10) << 4) + (second_t % 10);
                Ds1302_Init(hour, minute, second);
                key_s7 = 0;
            }
            if (key_s6) {
                key_s6 = 0;
            }
            if (key_s5) {
                second_t++;
                if (second_t > 59) {
                    second_t = 0;
                }
                key_s5 = 0;
            }
            if (key_s4) {
                second_t--;
                if (second_t > 59) {
                    second_t = 59;
                }
                key_s4 = 0;
            }
            break;
        case NZ_S_MS:
            nz_s_func();
            if (key_s7) {

                key_s7 = 0;
            }
            if(key_s6) {
                nex_stat = NZ_F_MS;
                key_s6 = 0;
            }
            if (key_s5) {
                hour_t++;
                if (hour_t > 23) {
                    hour_t = 0;
                }
                key_s5 = 0;
            }
            if (key_s4) {
                hour_t--;
                if (hour_t > 23) {
                    hour_t = 23;
                }
                key_s4 = 0;
            }

            break;
        case NZ_F_MS:
            nz_f_func();
            if (key_s7) {

                key_s7 = 0;
            }
            if(key_s6) {
                nex_stat = NZ_M_MS;
                key_s6 = 0;
            }
            if (key_s5) {
                minute_t++;
                if (minute_t > 59) {
                    minute_t = 0;
                }
                key_s5 = 0;
            }
            if (key_s4) {
                minute_t--;
                if (minute_t > 59) {
                    minute_t = 59;
                }
                key_s4 = 0;
            }
            break;
        case NZ_M_MS:
            nz_m_func();
            if (key_s7) {

                key_s7 = 0;
            }
            if(key_s6) {
                nex_stat = SZ_XS_MS;
                second_a = ((second_t / 10) << 4) + (second_t % 10);
                hour_a = ((hour_t / 10) << 4) + (hour_t % 10);
                minute_a = ((minute_t / 10) << 4) + (minute_t % 10);
                key_s6 = 0;
            }
            if (key_s5) {
                second_t++;
                if (second_t > 59) {
                    second_t = 0;
                }
                key_s5 = 0;
            }
            if (key_s4) {
                second_t--;
                if (second_t > 59) {
                    second_t = 59;
                }
                key_s4 = 0;
            }
            break;
        default:
            break;
        }

        if((P33 != 1) && (cur_stat == SZ_XS_MS)) {
            key = 4;
            Delay10ms();
            while (P33 != 1) {
                wd_xs_func();
            }
        }

        key_flag = 0;
        key = keyscan();
        switch(key) {
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
            break;
        }

    }
}

void time0() interrupt 1
{
    if ((second == second_a) && (minute == minute_a) && (hour == hour_a)) {
        alarm_stat = 1;
    }
    if (alarm_stat) {
        count_200ms++;
        if (count_200ms >= 100) {
            count_200ms = 0;
            count_led_flash++;
            if (count_led_flash <= 25) {
                led_flash_stat = ~led_flash_stat;
                if (led_flash_stat && count_led_flash > 0) {
                    led_com(0xff);
                } else {
                    led_com(0xfe);
                }
            } else {
                alarm_stat = 0;
                count_led_flash = 0;
                led_com(0xff);
            }

        }

    }
}


void time1() interrupt 3
{
    display();
    if(++count_1s >= 500) {
        count_1s = 0;
        smg_flash_flag = ~smg_flash_flag;
    }
}