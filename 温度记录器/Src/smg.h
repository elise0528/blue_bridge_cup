#ifndef __SMG_H
#define __SMG_H

#include "config.h"


u8 smg_dx[]	= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
u8 display_buf[8]= {10,10,10,10,10,10,10,10};
u8 display_com=0;

void display()
{ 
  P2=((P2&0x1f)|0xe0);
  P0=0xff;
  P2&=0x1f;

  P2=((P2&0x1f)|0xc0);
  P0=(0x80>>display_com);
  P2=0x1f;
  P2=((P2&0x1f)|0xe0);
  P0=smg_dx[display_buf[display_com]];
  P2&=0x1f;
  if(++display_com==8)
  { 
    display_com=0;
	}
}


#endif 