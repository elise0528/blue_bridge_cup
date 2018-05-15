#include <STC15F2K60S2.H>
#include "function.h"


void main()
{
	StcInit();
	

	LedFlashSetTime[4]=ReadByte_AT24C02(0x04);
	if(LedFlashSetTime[4]!=1)
	{
		u8 i;
		for(i=0;i<4;i++)
        {
			LedFlashSetTime[i]=4;
        }
		WriteByte_AT24C02(0x04,0);
	}else
	{
		LedFlashSetTimeNew[0]=LedFlashSetTime[0]=ReadByte_AT24C02(0x00);
		LedFlashSetTimeNew[1]=LedFlashSetTime[1]=ReadByte_AT24C02(0x01);
		LedFlashSetTimeNew[2]=LedFlashSetTime[2]=ReadByte_AT24C02(0x02);
		LedFlashSetTimeNew[3]=LedFlashSetTime[3]=ReadByte_AT24C02(0x03);
	}
	
	ADC_Init();
//	Delay10ms();
//	E2PROM_Read(0x00,LedFlashSetTime,5);
	Timer0Init();
	Timer1Init();
	Timer2Init();
	
	while(1)
	{
		cur_state=nex_state;
		switch (cur_state)
        {
        	case LED:
				LedEvent();
			LedAction();
			break;
        	case SZ_LED:
				SzLedEvent();
				SzLedAction();
			break;
        	case TJ_LED:
				TjLedEvent();
				TjLedAction();
			break;
        	default:break;
        }
	}
}

void time0() interrupt 1
{
	if(LedWorkFlag)
	{		
		if(LedLevel==1)
		{
			if(PwmFlag){TL0=0x33;TH0=0xbf;PwmFlag=0;}else{TL0=0x66;TH0=0xea;PwmFlag=1;}
		}else if(LedLevel==2)
		{
			TL0=0xcd;TH0=0xd4;
			PwmFlag=~PwmFlag;
		}else if(LedLevel==3)
		{
			if(PwmFlag){TL0=0x66;TH0=0xea;PwmFlag=0;}else{TL0=0x33;TH0=0xbf;PwmFlag=1;}
		}else if(LedLevel==4)
		{
			TL0=0x9a;TH0=0xa9;
			PwmFlag=1;
		}
		
		
		if(PwmFlag)
		{
//			LedCom(0x7f);
			if(LedMode==0)LedCom(led_tab1[led_offset]);
			if(LedMode==1)LedCom(led_tab2[led_offset]);
			if(LedMode==2)LedCom(led_tab3[led_offset]);
			if(LedMode==3)LedCom(led_tab4[led_offset]);
			if(++PwmTimeCount>=50)
			{
				PwmTimeCount=0;
				LedFlashTime++;			
			}
		}else
		{
			LedCom(0xff);
		}
	}
}

void time1() interrupt 3
{
	//LedAction();
	if(++SmgFlashCount>=160)
	{
		SmgFlashCount=0;
		SmgFlashFlag=~SmgFlashFlag;
	}
}

void time2() interrupt 12
{
	display();
	keyscan();
}
