#include <STC15F2K60S2.H>
#include "key.h"
#include "ds18b20.h"
#include "intrins.h"

sbit buzz = P0^6;
sbit relay = P0^4;

u8 table[12]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
u8 disbuf[8]= {10,10,10,10,10,10,10,10};
u8 discom=0;

u8 key;
u8 maxwendu=30,minwendu=20,wendu;			// ��ȡ���¶�Ϊ10����
u16 time_count=0;

void display()
{
	P2 = ((P2&0x1f)|0xe0);
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = ((P2&0x1f)|0xc0);
	P0 = 0x80>>discom;
	P2 &= 0x1f;
	
	P0 = 0xff;
	P2 = ((P2&0x1f)|0xe0);
	P0 = table[disbuf[discom]];
	P2 &= 0x1f;
	
	if(++discom == 8)
		discom = 0;
}

void buzz_com(bit com)
{
	P0 = 0x00;
	P2 = ((P2&0x1f)|0xa0);
	buzz = com;
	P2 &= 0x1f;
}

void relay_com(bit com)
{
	P0 = 0x00;
	P2 = ((P2&0x1f)|0xa0);
	relay = com;
	P2 &= 0x1f;
}

void led_com(u8 com)
{
	P0 = 0xff;
	P2 = ((P2&0x1f)|0x80);
	P0 = com;
	P2 &= 0x1f;
}


void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	
	EA = 1;
	ET0 = 1;
}




void main()
{
	buzz_com(0);
	relay_com(0);
	led_com(0xff);
	Timer0Init();
	
	
	while(1)
	{
		wendu = Temper_Read();
		

			
		disbuf[7] = 11;
		disbuf[5] =	11;
		disbuf[4] =	10;
		disbuf[3] =	10;
		disbuf[2] =	10;
		disbuf[1] =	wendu/10;
		disbuf[0] = wendu%10;
		
		if(wendu < minwendu)
			disbuf[6] =	0;

		if(wendu <= maxwendu  && wendu >= minwendu)
			disbuf[6] =	1;

		if(wendu > maxwendu)
			disbuf[6] =	2;


			
		key = keyscan();
		
		switch (key)
    {
    	case 7:disbuf[4]=1;break;
    	case 6:disbuf[4]=2;break;
			case 11:disbuf[4]=3;break;
    	default:break;
    }
		
	}
	
	
	
}

void time0() interrupt 1
{
	display();
	if(++time_count == 1000)
	{
		time_count = 0;

	}

	
}