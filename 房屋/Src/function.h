#ifndef _FUNCTION_H_
#define _FUNCTION_H_	

#include "iic.h"
#include "display.h"
#include "key.h"
#include "wave.h"
#include "ds1302.h"
#include "uart.h"

enum state_e
{
	SHIZHONG,
	CELIANG,
	HUIXIAN
};

enum sz_state
{
	SZ_1,
	SZ_2,
};


u8 cur_state,nex_state;
u8 sz_state;

u16 Count_1s;
bit SmgFlashFlag;

u8 hour,minute,second;
u8 hour_s,minute_s,second_s;



void LedCom(u8 arg)
{
	P2=((P2&0xaf)|0x80);
	P0=arg;
	P2&=0x1f;
}

void StcInit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0x1f;
}

void Timer1Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x9A;		//设置定时初值
	TH1 = 0xA9;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	
	EA=1;
	ET1=1;
}

void SZAction()
{
	if(sz_state==SZ_1)
	{
		hour=Ds1302_Single_Byte_Read(0x85);
		minute=Ds1302_Single_Byte_Read(0x83);
		second=Ds1302_Single_Byte_Read(0x81);
		
		disbuf[7]=hour/16;
		disbuf[6]=hour%16;
		disbuf[5]=11;
		disbuf[4]=minute/16;
		disbuf[3]=minute%16;
		disbuf[2]=11;
		disbuf[1]=second/16;
		disbuf[0]=second%16;
		
	}else if(sz_state==SZ_2)
	{
		if(SmgFlashFlag)
		{
			disbuf[7]=hour_s/10;
			disbuf[6]=hour_s%10;
		}else
		{
			disbuf[7]=10;
			disbuf[6]=10;
		}
		disbuf[5]=11;
		disbuf[4]=minute_s/10;
		disbuf[3]=minute_s%10;
		disbuf[2]=11;
		disbuf[1]=second_s/10;
		disbuf[0]=second_s%10;
	}

}

void SZEvent()
{
	if(key7.state==ShortPress)
	{
		key7.state=NO;
		nex_state=CELIANG;
	}else if(key7.state==LongPress)
	{
		key7.state=NO;
		sz_state=SZ_2;
		hour_s=((hour>>4)*10)|(hour%16);
		minute_s=((minute>>4)*10)|(minute%16);
		second_s=((second>>4)*10)|(second%16);
	}
}



#endif