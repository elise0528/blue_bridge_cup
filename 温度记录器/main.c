/*********************************************************************************
未解决：
	1.读取时间时，由于读取温度时间较长，导致数码管闪烁 	H128
	2.led_flag 使用浪费		H105
*********************************************************************************/
#include <STC15F2K60S2.H>
#include "key.h"
#include "smg.h"
#include "timer.h"
#include "ds1302.h"
#include "ds18b20.h"

u8 key_value;
u8 key_s4,key_s5,key_s6,led_flag;
u8 second,minute,hour,time_temp,led_flash;
u8 second_count,wendu_count;
u16 time_count;

u8 time_set[4] = {1,5,30,60};
u8 wendu_buf[10];

void main() 
{
	buzz_com(0);
	relay_com(0);
	led_com(0xff);
	
	Timer0Init();
	Ds1302_Init();
	
START:
	key_s4 = 0;
	key_s5 = 0;
	key_s6 = 0;
	time_count=0;
	wendu_count=0;
	second_count=0;
	led_flash=0x55;
	
	while(1)
	{
		time_temp = time_set[key_s4];
		display_buf[0] = time_temp%10;
		display_buf[1] = time_temp/10;
		display_buf[2]=11;
		display_buf[3]=10;
		display_buf[4]=10;
		display_buf[5]=10;
		display_buf[6]=10;
		display_buf[7]=10;
		
		key_value = keyscan();
		if( key_value == 4 )
		{
			if( ++key_s4 > 3)
				key_s4 = 0;
		}
		if( key_value == 5 )
		{
			key_s5 = 1;
			while(1)
			{
				second = Ds1302_Single_Byte_Read(0x81);
				minute = Ds1302_Single_Byte_Read(0x83);
				hour = Ds1302_Single_Byte_Read(0x85);
				
				display_buf[0]=second%16;
				display_buf[1]=second/16;
		
				display_buf[3]=minute%16;
				display_buf[4]=minute/16;
				
				display_buf[6]=hour%16;
				display_buf[7]=hour/16;
								
				if( second_count%2 == 1 )				
				{
					display_buf[2]=11;
					display_buf[5]=11;
				}
				if ( second_count%2 == 0 )
				{
					display_buf[2]=10;
					display_buf[5]=10;
				}
				
				if( wendu_count > 9)
				{
					key_s5 = 0;
					led_flag = 1;
					while(1)
					{

						display_buf[0]=wendu_buf[key_s6]%10;
						display_buf[1]=wendu_buf[key_s6]/10;
						display_buf[2]=11;
						display_buf[3]=10;
						display_buf[4]=10;
						display_buf[5]=key_s6%10;
						display_buf[6]=key_s6/10;
						display_buf[7]=11;
						key_value = keyscan();
						if(key_value == 6)
						{
							led_flag = 0;
							if( ++key_s6 > 9 )
								key_s6 = 0;
						}
						if( key_value == 7)
							goto START;
					}
				}
			}
		}
	}
}


void time0() interrupt 1 
{
	display();
	if ( ++time_count >= 1000 )
	{
		time_count = 0;
		second_count++;
		if (key_s5 == 1)
		{
			wendu_buf[wendu_count] = Temper_Read();
			wendu_count++;
		}
		if( led_flag == 1 )
		{

			led_flash = ~led_flash | 0xfe;
			led_com(led_flash);
		}
		else
		{
			led_com(0xff);
		}
	}

	
}


