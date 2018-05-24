#include <STC15F2K60S2.H>
#include "function.h"

void main()
{
	StcInit();
	Ds1302Init(0x23,0x59,0x55);
	Timer0Init();
	Timer1Init();
	while(1)
	{
		cur_state=nex_state;
		switch (cur_state)
        {
        	case SHIZHONG:SZAction();SZEvent();break;
        	case CELIANG:		disbuf[1]=10;disbuf[0]=10;break;
        	case HUIXIAN:break;
        	default:break;
        }
		
	}
}

void time1() interrupt 3
{
	if(++Count_1s>=500)
	{
		Count_1s=0;
		SmgFlashFlag=~SmgFlashFlag;
	}
	display();
	keyscan();
}