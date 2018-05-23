#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "intrins.h"
#include "display.h"
#include "timer.h"
#include "wave.h"
#include "key.h"
#include "iic.h"

enum state_e
{
	PANDUAN,
	CHUANSONG,
	SHEZHI
};


u8 cur_state,nex_state;

u8 cstime_1=2,cstime_2=4,cstime;

u16 count_500ms,count_1s;
bit ledflashflag,ledchageflag,relaychageflag,lock_distance;
u8 adc;
u8 vol;
u8 jcflag; //0©уть  1╥г©уть  2Ё╛ть 
u8 distance;

u8 hwnum;
bit cstimeflag,relayflag;

u8 szoffset;
bit smgflashflag;

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void stcinit()
{
	P2=0x80;
	P0=0xff;
	P2=0xa0;
	P0=0x00;
	P2=0x1f;
}

void ledcom(u8 led)
{
	P0=0xff;
	P2=((P2&0x1f)|0x80);
	P0=led;
	P2&=0x1f;
}

void devicecom(bit buzz,bit relay)
{
	P2=((P2&0x1f)|0xa0);
	if(buzz&&!relay)P0=0x40;
	if(buzz&&relay)P0=0x50;
	if(!buzz&&relay)P0=0x10;
	if(!buzz&&!relay)P0=0x00;
	P2&=0x1f;
}

void getdata()
{
	cstime_1=e2prom_read(0x00);
	cstime_2=e2prom_read(0x01);
	if(cstime_1==0xff||cstime_2==0xff){cstime_1=2;cstime_2=4;}
}

void PDEvent()
{
	if(key4.isPress==1&&jcflag==1)
	{
		key4.isPress=0;
		if(hwnum==1)
		{
			cstime=cstime_1;
		}else if(hwnum==2)
		{
			cstime=cstime_2;
		}
		cstimeflag=1;
		relayflag=1;
		nex_state=CHUANSONG;
	}
	if(key6.isPress==1&&jcflag==0)
	{
		key6.isPress=0;
		nex_state=SHEZHI;
		lock_distance=0;
	}
	if(key7.state==1)key7.state=0;
	if(key5.state==1)key5.state=0;
}
void PDAction()
{
	
	adc=adc_read();
	vol=adc*10/51;
	
	if((vol>0)&&(vol<10))
	{
		jcflag=0;
		ledchageflag=1;
		relaychageflag=1;
		lock_distance=0;
	}
	if(vol>=10&&vol<40)
	{
		jcflag=1;
		ledchageflag=1;
		relaychageflag=1;
	}
	if(vol>=40)
	{
		jcflag=2;
		ledchageflag=1;
		relaychageflag=1;
		lock_distance=0;
	}

	
	if(jcflag==0)
	{
		disbuf[7]=10;
		disbuf[6]=10;
		disbuf[5]=10;
		disbuf[4]=10;
		disbuf[3]=10;
		disbuf[2]=10;
		disbuf[1]=10;
		disbuf[0]=10;
	}
	
	if(jcflag==1)
	{
		if(!lock_distance)
		{
			EA=0;
			distance=(u8) getdistance();
			EA=1;
			lock_distance=1;
			
			if(distance<=30)
			{
				hwnum=1;
			}else if(distance>30)
			{
				hwnum=2;
			}
		}
		disbuf[7]=1;
		disbuf[6]=10;
		disbuf[5]=10;
		disbuf[4]=distance%100/10;
		disbuf[3]=distance%10;
		disbuf[2]=10;
		disbuf[1]=10;
		disbuf[0]=hwnum;
	}
	if(jcflag==2)
	{
		disbuf[6]=10;
		disbuf[5]=10;
		disbuf[4]=distance/10;
		disbuf[3]=distance%10;
		disbuf[2]=10;
		disbuf[1]=10;
		disbuf[0]=hwnum;
	}
	
}

void CSEvent()
{
	if(key5.isPress==1)
	{
		key5.isPress=0;
		cstimeflag=~cstimeflag;
		relaychageflag=1;
		if(cstimeflag)
		{
			relayflag=1;
		}
	}
	if(key6.isPress==1)key6.isPress=0;
	if(key7.isPress==1)key7.isPress=0;
	if(key4.isPress==0)key4.isPress=0;
}
void CSAction()
{
	disbuf[7]=2;
	disbuf[6]=10;
	disbuf[5]=10;
	disbuf[4]=10;
	disbuf[3]=10;
	disbuf[2]=10;
	disbuf[1]=cstime/10;
	disbuf[0]=cstime%10;
	if(cstime==0)
	{
		Delay500ms();
		lock_distance=0;
		nex_state=PANDUAN;
	}
}

void SZEvent()
{
	if(key7.isPress==1)
	{
		key7.isPress=0;
		if(szoffset==0)
		{
			cstime_1++;
			if(cstime_1>=11)
			{
				cstime_1=1;
			}
		}
		if(szoffset==1)
		{
			cstime_2++;
			if(cstime_2>=11)
			{
				cstime_2=1;
			}
		}
	}
	if(key6.isPress==1)
	{
		key6.isPress=0;
		szoffset++;
		if(szoffset>=2)
		{
			szoffset=0;
			nex_state=PANDUAN;
			EA=0;
			e2prom_write(0x00,cstime_1);
			e2prom_write(0x01,cstime_2);
			EA=1;
		}
	}
	if(key5.isPress==1)key5.isPress=0;
	if(key4.isPress==1)key4.isPress=0;
}

void SZAction()
{

	if(szoffset==0)
	{
		disbuf[7]=3;
		disbuf[6]=10;
		disbuf[5]=10;
		disbuf[2]=10;
		if(smgflashflag)
		{

			disbuf[4]=cstime_1/10;
			disbuf[3]=cstime_1%10;
			disbuf[1]=cstime_2/10;
			disbuf[0]=cstime_2%10;
		}else
		{
			disbuf[4]=10;
			disbuf[3]=10;		
			disbuf[1]=cstime_2/10;
			disbuf[0]=cstime_2%10;
		}
	}
	
	if(szoffset==1)
	{
		disbuf[7]=3;
		disbuf[6]=10;
		disbuf[5]=10;
		disbuf[2]=10;
		if(smgflashflag)
		{

			disbuf[4]=cstime_1/10;
			disbuf[3]=cstime_1%10;
			disbuf[1]=cstime_2/10;
			disbuf[0]=cstime_2%10;
		}else
		{

			disbuf[4]=cstime_1/10;
			disbuf[3]=cstime_1%10;
			disbuf[1]=10;
			disbuf[0]=10;	
		}
	}
}


#endif