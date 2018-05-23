#ifndef _KEY_H_
#define _KEY_H_
#include "iic.h"

enum key_state_e
{
	KEY_UP,
	KEY_SURE,
	KEY_DOWN,
	KEY_FREE
};

u8 key_state;


u8 keyscan()
{
	u8 temp, key = 99;

	P44 = 0, P42 = 1;
	P3 |= 0x0f;
	temp = P3 & 0x0f;
	if (temp != 0x0f) {
		Delay10ms();
		temp = P3 & 0x0f;
		if (temp != 0x0f) {
			switch(temp) {
			case 0x0e:key = 7;break;
			case 0x0d:key = 6;break;
			case 0x0b:key = 5;break;
			case 0x07:key = 4;break;
			}
			while(temp != 0x0f) {
				temp = P3 & 0x0f;
			}
		}
	}

	P44 = 1, P42 = 0;
	P3 |= 0X0f;
	temp = P3 & 0x0f;
	if (temp != 0x0f) {
		Delay10ms();
		temp = P3 & 0x0f;
		if (temp != 0x0f) {
			switch(temp) {
			case 0x0e:key = 11;break;
			case 0x0d:key = 10;break;
			case 0x0b:key = 9;break;
			case 0x07:key = 8;break;
			}
			while(temp != 0x0f) {
				temp = P3 & 0x0f;
			}
		}
	}
	return key;
}



#endif