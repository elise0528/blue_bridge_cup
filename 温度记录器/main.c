#include <STC15F2K60S2.H>
#include "smg.h"
#include "key.h"
#include "timer.h"
#include "ds1302.h"
#include "ds18b20.h"


u8 second,minute,hour,key;
u8 time_set_flag=0,wendu_flag=0,secend_count=0;
u8 key_s7=0,key_s6=0,key_s5=0,key_s4=0;
u16 time_count=0;

u8 time_set;



void main()
{
	buzz_com(0);
	relay_com(0);
	led_com(0xff);
	Timer0Init();
	Ds1302_Init();
	while (1)
	{
		switch (time_set_flag)
		{
			case 0 :display_buf[1]=0;display_buf[0]=1;
				break;
			case 1 :display_buf[1]=0;display_buf[0]=5;
				break;
			case 2 :display_buf[1]=3;display_buf[0]=0;
				break;
			case 3 :display_buf[1]=6;display_buf[0]=0;
				break;
			default:
				break;
		}
	
		display_buf[2] = 11;
//		display_set(time_set_flag);
		key = keyscan();
		if ( key == 4 )
		{
			time_set_flag++;
			if( time_set_flag > 3 )
				time_set_flag = 0;
		}
		else if ( key == 5 )
		{
			switch (time_set_flag)
			{
				case 0:time_set=1;
					break;
				case 1:time_set=5;
					break;
				case 2:time_set=30;
					break;
				case 3:time_set=60;
					break;
				default:
					break;
			}
			key_s5 = 1;
			while(1)
			{
				second = Ds1302_Single_Byte_Read(0x81);
				minute = Ds1302_Single_Byte_Read(0x83);
				hour = Ds1302_Single_Byte_Read(0x85);
//				display_clock(hour,minute,second);
				
				display_buf[7] = hour/16;
				display_buf[6] = hour%16;

				display_buf[4] = minute/16;
				display_buf[3] = minute%16;

				display_buf[1] = second/16;
				display_buf[0] = second%16;
				
//				wendu_buf[secend_count/time_set] = Temper_Read();					
				
				if( secend_count/time_set > 9 )
				{
					while(1)
					{
						display_wendu(key_s6);
						if ( keyscan() == 6 )
						{
							key_s6++;
							if (key_s6 > 9)
								key_s6 = 0 ;
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
	if ( key_s5 == 1 )
	{
		time_count++;
		if ( time_count == 1000 )
		{
			time_count = 0;
			secend_count++;
		}
	}

}



