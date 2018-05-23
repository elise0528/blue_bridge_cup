#include <STC15F2K60S2.H>
#include "function.h"

void main()
{
	stcinit();
	ds18b20init();
	ds1302init(0x16,0x00,0x00);
	Timer0Init();
	UartInit();
	memset(Rx_buf,0,sizeof(Rx_buf));
	memset(Rx_buf,0,sizeof(Tx_buf));
	while(1)
	{
		cur_state=nex_state;
		getdata();SmgAction();UartAction();LedAction();
		
		switch (cur_state)
        {
        	case MS1:MS1Action();MS1Event();break;
        	case MS2:MS2Action();MS2Event();break;
        	default:break;
        }
		
	}
}

void time0() interrupt 1
{
	if(StayFlag)
	{
		if(++Count_1s>=500)
		{
			Count_1s=0;
			staytime++;
		}
	}
		
	display();
	if(!WorkFlag)
	{
		keyscan();
	}
}

void UART_Serve() interrupt 4
{
	if(RI == 1)//接收成功RI置位
	{
	  	RI = 0;		
	  	Rx_buf[uartoffset] = SBUF;
	  	uartoffset++;
	}
} 