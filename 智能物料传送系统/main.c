#include <STC15F2K60S2.H>
#include "function.h"

void main()
{
	stcinit();
	getdata();
	adcinit();
	Timer0Init();
	Timer1Init();
	Timer2Init();
	while(1)
	{
		cur_state=nex_state;
		switch (cur_state)
        {
        	case PANDUAN:PDEvent();PDAction();break;
			case CHUANSONG:CSEvent();CSAction();break;
			case SHEZHI:SZEvent();SZAction();break;
        	default:break;
        }
	}
}


void time1() interrupt 3
{
	display();	
	keyscan();
}

void time2() interrupt 12
{
	count_500ms++;
	if(count_500ms>=125)
	{
		count_500ms=0;
		smgflashflag=~smgflashflag;
		if(jcflag==2||!cstimeflag)
		{	
			ledflashflag=~ledflashflag;
			ledchageflag=1;	
		}
	}
	
	if(cur_state==CHUANSONG)
	{
		if(cstimeflag)
		{
			count_1s++;
			if(count_1s>=250)
			{
				count_1s=0;
				if(cstime>0)
				{
					cstime--;
				}else
				{
					relaychageflag=1;
					relayflag=0;
				}
			}
		}

	}
	
	if(ledchageflag==1&&cur_state==PANDUAN)
	{
		ledchageflag=0;
		if(jcflag==0)
		{
			ledcom(0x7f);
			if(relaychageflag)
			{
				relaychageflag=0;
				devicecom(0,0);
			}
		}
		if(jcflag==1)
		{
			ledcom(0xbf);
			if(relaychageflag)
			{
				relaychageflag=0;
				devicecom(0,0);
			}
		}
		if(jcflag==2)
		{
			if(relaychageflag)
			{
				relaychageflag=0;
				devicecom(1,0);
			}
			if(ledflashflag)
			{
				ledcom(0xdf);
			}else
			{
				ledcom(0xff);
			}
		}
	}
	if(ledchageflag==1&&cur_state==CHUANSONG)
	{
		if(cstimeflag==0)
		{
			if(relaychageflag)
			{
				relaychageflag=0;
				devicecom(0,0);
			}
			if(ledflashflag)
			{
				ledcom(0xef);
			}else
			{
				ledcom(0xff);
			}
		}
		if(cstimeflag==1)
		{
			if(relaychageflag)
			{
				relaychageflag=0;
				devicecom(0,relayflag);
				ledcom(0xff);
			}
		}
	}
}