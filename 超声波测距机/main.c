#include <STC15F2K60S2.H>
#include "function.h"


void main()
{
	StcInit();
	GetData();
	Delay10ms();
	Timer0Init();
	Timer1Init();
	Timer2Init();
	
	while(1)
	{
		cur_state=nex_state;
		switch (cur_state)
        {
        	case CeJu:CJEvent();CJAction();break;
        	case HuiXian:HXEvent();HXAction();break;
        	case SheZhi:SZEvnet();SZAction();break;
        	default:break;
        }
		DAAction();
	}
}

void time1() interrupt 3
{
	display();
	keyscan();
}

void time2() interrupt 12
{
	LedFlashAction();
}
	
