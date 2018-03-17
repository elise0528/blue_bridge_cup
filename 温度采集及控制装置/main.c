/*
����û��׼ȷֵ���� �ж�ʱ������ ==
������ʱע�ⷵ��ֵ
�¶Ȳɼ�Ϊ10����
��ʱ������һ�����0xff Ӧ��u16 ��ʱ����ʼ���ǵ� EA=1 ET0=1
*/



#include <STC15F2K60S2.H>
#include "key.h"
#include "ds18b20.h"
#include "intrins.h"

sbit buzz = P0^6;
sbit relay = P0^4;

u8 table[12]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
u8 disbuf[8]= {10,10,10,10,10,10,10,10};
u8 discom=0;

u16 time_count=0,led_flash_time=0;		//	��ס��ʱ����ļ�������0xff Ҫ��u16
u8 led_flag=0xff,led_flash=0;
u8 key_set=0;
u8 key,set_count=0;
u8 maxwendu=30,minwendu=20,wendu,maxwendu_set,minwendu_set;			// ��ȡ���¶�Ϊ10����


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
	
	P0 = 0x00;
	
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

void relay_com(u8 com)
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
		{
			disbuf[6] =	0;
			led_flash_time = 800;
			relay_com(0x00);

		}
		if(wendu <= maxwendu  && wendu >= minwendu)
		{
			disbuf[6] =	1;
			led_flash_time = 400;
			relay_com(0x00);

		}
		if(wendu > maxwendu)
		{
			disbuf[6] =	2;
			led_flash_time = 200;
			relay_com(0x10);

		}
		
		if( led_flash == 1)
		{
			led_com(led_flag);			
		}
		
		key_set = 1;
		
		if(keyscan() == 10)
		{
			key_set = 2;
			led_com(0xff);
			
			disbuf[7] = 11;
			disbuf[6] = 10;
			disbuf[5] =	10;
			disbuf[4] =	10;
			disbuf[3] =	10;
			disbuf[2] =	11;
			disbuf[1] = 10;
			disbuf[0] = 10;
			
			while(key_set == 2)
			{
				key = keyscan();
				if (key < 10 && ++set_count <= 4)
				{
					switch (set_count)
					{
						case 1: disbuf[6] = key; maxwendu_set = key*10; break;
						case 2: disbuf[5] = key; maxwendu_set = maxwendu_set+key; break;
						case 3:	disbuf[1] = key; minwendu_set = key*10; break;
						case 4:	disbuf[0] = key; minwendu_set = minwendu_set+key; break;
						default:break;
					}
				}
				
				if ( key == 11)
				{
					switch (set_count/3)
					{
						case 0: disbuf[6] = 10;disbuf[5] = 10;set_count = 0; break;
						case 1: disbuf[1] = 10;disbuf[0] = 10;set_count = 2; break;
						default:break;
					}
				}
					
				if (set_count >= 4)
				{
					maxwendu = maxwendu_set;
					minwendu = minwendu_set;
					if ( maxwendu < minwendu )
					{
						led_com(0xfd);
					}
					else
					{
						led_com(0xff);
					}
					
					if ( key == 10)
					{
						if ( maxwendu < minwendu )
						{
							key_set = 2;
						}
						else
						{
							key_set = 3;
							set_count = 0;
						}
					}					
				}
			}
		}
	}
}

void time0() interrupt 1
{
	display();
	

	if(key_set == 1)
	{

		if(++time_count >= led_flash_time) 		// >= ��ֹ�¶ȱ仯ʱ�趨��led�Ƶ���˸ʱ��ı� ������ܷ� 
		{
			led_flag = (~led_flag)|0xfe;
			led_flash = 1;
			time_count = 0;
		}
	}
	
}