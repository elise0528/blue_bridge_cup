#include <STC15F2K60S2.H>
#include "function.h"

void main()
{
	StcInit();
	GetData();
	ADC_Init();
	DS1302_Init(0x23,0x59,0x55);
		Timer0Init();
	Timer1Init();

	
	while(1)
	{
		cur_state=nex_state;
		switch (cur_state)
        {
        	case ShiJian:SJEvent();SJAction();break;
        	case DianYa:DYAction();DYEvent();break;
        	case PingLv:PLEvent();PLAction();break;
			case ChaXun:CXEvent();CXAction();break;
        	default:break;
        }
		key=keyscan();
	}
}

void time0() interrupt 1
{
	PL_t++;
}

void time1() interrupt 3
{
//	if(cur_state==PingLv)
//	{
//		TR0=1;
//		if(++Count_100ms>=50)
//		{
//			Count_100ms=0;
//			TR0=0;
//			PL=TL0+TH0*256;
//			TH0=TL0=0x00;
//			PL=PL*10;
//			ZQ=100000/PL;
//			TR0=1;
//		}
//	}
	SmgFlashAction();
	display();
}
