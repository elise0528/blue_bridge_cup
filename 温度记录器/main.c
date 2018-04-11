/*********************************************************************************
未解决：
	1.读取时间时，由于读取温度时间较长，导致数码管闪烁 	H128
	2.led_flag 使用浪费		H105
*********************************************************************************/
#include <STC15F2K60S2.H>
#include "key.h"
#include "smg.h"
#include "timer.h"
#include "ds1302.h"
#include "ds18b20.h"

#define CS_SZ_MS (u8) 0
#define SZ_XS_MS (u8) 1
#define WD_XS_MS (u8) 2

u8 key, key_s4, key_s5, key_s6, key_s7;
u8 cur_stat, nex_stat;
u8 wendu_buf[10];
u8 cj_time_t = 1, cj_set=0, count_cj_time, xs_count, wendu_count;
u8 cj_time[4] = {1, 5, 30, 60};

bit led_stat, flash_stat, cj_stat;

u8 second, minute, hour;
u16 count_1s;


void cs_sz_func()
{
    display_buf[0] = cj_time_t % 10;
    display_buf[1] = cj_time_t / 10;
    display_buf[2] = 11;
    display_buf[3] = 10;
    display_buf[4] = 10;
    display_buf[5] = 10;
    display_buf[6] = 10;
    display_buf[7] = 10;
}

void sz_xs_func()
{
    second = Ds1302_Single_Byte_Read(0x81);
    minute = Ds1302_Single_Byte_Read(0x83);
    hour = Ds1302_Single_Byte_Read(0x85);



    display_buf[0] = second % 16;
    display_buf[1] = second / 16;
    display_buf[3] = minute % 16;
    display_buf[4] = minute / 16;
    display_buf[6] = hour % 16;
    display_buf[7] = hour / 16;

    if (cj_stat) {
        wendu_buf[wendu_count - 1] = Temper_Read();
    }


    if (flash_stat) {
        display_buf[2] = 11;
        display_buf[5] = 11;
    } else {
        display_buf[2] = 10;
        display_buf[5] = 10;
    }



}

void wd_xs_func()
{

    display_buf[0] = wendu_buf[xs_count] % 10;
    display_buf[1] = wendu_buf[xs_count] / 10;
    display_buf[2] = 11;
    display_buf[3] = 10;
    display_buf[4] = 10;
    display_buf[5] = xs_count % 10;
    display_buf[6] = xs_count / 10;
    display_buf[7] = 11;

    if (led_stat) {
        if (flash_stat) {
            led_com(0xfe);
        } else {
            led_com(0xff);
        }
    } else {
        led_com(0xff);
    }

}


void main()
{
    buzz_com(0);
    relay_com(0);
    led_com(0xff);

    Timer0Init();
    Ds1302_Init();
    DS18b20_jd();

    while(1) {

        cur_stat = nex_stat;

        switch (cur_stat) {
        case CS_SZ_MS:
            cs_sz_func();
            if (key_s7) {
                key_s7 = 0;
            }
            if (key_s6) {
                key_s6 = 0;
            }
            if (key_s5) {
                nex_stat = SZ_XS_MS;
                key_s5 = 0;
            }
            if (key_s4) {
                nex_stat = CS_SZ_MS;
				cj_set++;
				if (cj_set > 3) {
                    cj_set = 0;
                }
                cj_time_t = cj_time[cj_set];
                key_s4 = 0;
            }
            break;
        case SZ_XS_MS:
            sz_xs_func();
            key_s7 = 0;
            key_s6 = 0;
            key_s5 = 0;
            key_s4 = 0;
            if (wendu_count > 10) {
                nex_stat = WD_XS_MS;
                wendu_count = 0;
                led_stat = 1;
            }
            break;
        case WD_XS_MS:
            wd_xs_func();
            if (key_s7) {
                nex_stat = CS_SZ_MS;
                cj_set = 0;
                xs_count = 0;
                cj_time_t = 0;
                cj_set = 0;
                key_s7 = 0;
            }
            if (key_s6) {
                led_stat = 0;
                if (xs_count++ >= 9) {
                    xs_count = 0;
                }

                key_s6 = 0;
            }
            key_s5 = 0;
            key_s4 = 0;
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
            key_s6 = ~key_s6;
            break;
        case 5:
            key_s5 = ~key_s5;
            break;
        case 4:
            key_s4 = ~key_s4;
            break;
        default:
            break;
        }

    }
}


void time0() interrupt 1
{
    display();
    if ( ++count_1s >= 400) {
        count_1s = 0;
        flash_stat = ~flash_stat;
        if (cur_stat == SZ_XS_MS) {
            if (++count_cj_time >= cj_time_t) {
                count_cj_time = 0;
                wendu_count++;
                cj_stat = 1;
            } else {
                cj_stat = 0;
            }


        }

    }


}


