#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "STC15F2K60S2.h"

u8 table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
u8 disbuf[]={10,10,10,10,10,10,10,10};
u8 discom;

void display()
{
	P2=((P2&0x1f)|0xe0);
	P0=0xff;
	P2&=0x1f;
	
	P2=((P2&0x1f)|0xc0);
	P0=0x80>>discom;
	P2&=0x1f;
	
	P2=((P2&0x1f)|0xe0);
	P0=table[disbuf[discom]];
	P2&=0x1f;
	
	if(++discom>=8)
	{
		discom=0;
	}
}

#endif