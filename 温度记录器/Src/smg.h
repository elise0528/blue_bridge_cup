#ifndef __SMG_H
#define __SMG_H

#include "config.h"

u8 display_com=0;

u8 smg_wx[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

u8 smg_dx_dec[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};	//0-9对应数字0-9，10-19为带点0-9，20为熄灭。

u8 display_buf[8] = {10,10,10,10,10,10,10,10};

//u8 display_set_buf[8] = {0,0,11,10,10,10,10,10};  //显示缓冲区

//u8 display_clock_buf[8] = {10,10,11,10,10,11,10,10};

//u8 display_wendu_buf[8] = {10,10,11,10,10,10,10,11};

u8 wendu_buf[10];

void display()
{
  P2=((P2&0x1f)|0xe0);
  P0=0xff;
  P2&=0x1f;

  P2=((P2&0x1f)|0xc0);
  P0=(0x80>>display_com);
  P2=0x1f;
  P2=((P2&0x1f)|0xe0);
  P0=smg_dx_dec[display_buf[display_com]];
  P2&=0x1f;
	
  if(++display_com==8)
		display_com=0;

}


void display_set(u8 com)
{
	switch (com)
	{
		case 0 :display_buf[1]=0;display_buf[0]=1;
			break;
		case 1 :display_buf[1]=0;display_buf[0]=5;
			break;
		case 2 :display_buf[1]=3;display_buf[0]=0;
			break;
		case 3 :display_buf[1]=6;display_buf[0]=0;
			break;
		default:
			break;
	}
	
	display_buf[2] = 11;

}

void display_clock(u8 hour,u8 minute,u8 second)
{

	display_buf[7] = hour/16;
	display_buf[6] = hour%16;
	
	display_buf[4] = minute/16;
	display_buf[3] = minute%16;
	
	display_buf[1] = second/16;
	display_buf[0] = second%16;
	

}

void display_wendu(u8 com)
{

	display_buf[7] = 11;
	
	display_buf[6] = com/10;
	display_buf[5] = com%10;
	
	display_buf[4] = 10;
	display_buf[3] = 10;
	display_buf[2] = 11;
	
	display_buf[1] = wendu_buf[com]/10;
	display_buf[0] = wendu_buf[com]%10;
	
}






#endif 