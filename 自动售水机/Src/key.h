#ifndef __KEY_H
#define __KEY_H

#include <STC15F2K60S2.H>

void Delay10ms()		//@11.0592MHz
{
    unsigned char i, j;

    i = 108;
    j = 145;
    do {
        while (--j);
    } while (--i);
}

u8 keyscan()
{
    u8 temp, key = 99;
    temp = P3 & 0x0f;
    if(temp != 0x0f) {
        Delay10ms();
        temp = P3 & 0x0f;
        if(temp != 0x0f) {
            switch (temp) {
            case 0x0e:
                key = 7;
                break;
            case 0x0d:
                key = 6;
                break;
            default:
                key = 99;
                break;
            }
        }
        while(temp != 0x0f) {
            temp = P3 & 0x0f;
        }
    }
    return key;
}

#endif