#include "stdio.h"
#include "string.h"
#include "display.h"
#include "iic.h"
#include "ds1302.h"
#include "onewire.h"
#include "key.h"
#include "wave.h"
#include "uart.h"
#include "timer.h"





u8 hour,minute,second;

u16 pl,pl_count;
u16 count_1s;

void stcinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0x1f;
}

void ledcom(u8 com)
{
	P2=((P2&0x1f)|0x80);
	P0=com;
	P2&=0x1f;
}

void devicecom(bit relay,bit buzz)
{
	P2=((P2&0x1f)|0x80);
	if(relay&&buzz)P0=0x50;
	if(relay&&!buzz)P0=0x10;
	if(!relay&&buzz)P0=0x40;
	if(!relay&&!buzz)P0=0x00;
	P2&=0x1f;
}


void ds1302read()
{
	second=Ds1302_Single_Byte_Read(0x81);
	minute=Ds1302_Single_Byte_Read(0x83);
	hour=Ds1302_Single_Byte_Read(0x85);
}


void getpl()
{
	disbuf[7]=10;
	disbuf[6]=10;
	disbuf[5]=10;
	disbuf[4]=pl/10000%10;
	disbuf[3]=pl/1000%10;
	disbuf[2]=pl/100%10;
	disbuf[1]=pl/10%10;
	disbuf[0]=pl%10;;
}
