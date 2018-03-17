#include "key.h"
#include "ds1302.h"
#include "ds18b20.h"



u8 tab[13]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf,0xc6};
u8 disbuf[8] = {10,10,10,10,10,10,10,10};
u8 discom=0;

u8 clock_flag,alarm_flag;
u8 key,key_s7,key_s6,key_s5,_key_s4;


u8 second,minute,hour;
u8 second_t,minute_t,hour_t;
u8 second_a=0,minute_a=0,hour_a=0;

u8 wendu,temp;

bit smg_flash_flag;
u8 led_flash=0xfe,led_count,led_flash_count;
u16 time_count;

void led_com(u8 com)
{

	P2 = ((P&0x1f)|0x80);
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = ((P&0x1f)|0x80);
	P0 = com;
	P2 &= 0x1f;
	
	P0 = 0x00;
	
}

void display()
{
	P2 = ((P2&0x1f)|0xe0);
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = ((P2&0x1f)|0xc0);	//位选
	P0 = 0x80>>discom;
	P2 &= 0x1f;
	
	P2 = ((P2&0x1f)|0xe0);	//段选
	P0 = tab[disbuf[discom]];
	P2 &= 0x1f;
	
	P0 = 0x00;
	
	if(++discom >= 8)
	{
		discom = 0;
	}
}

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	EA = 1;
	ET0 = 1;
}


void stcinit()
{
	P0 = 0x00;
	P2 = 0xa0;

	P2 = 0x80;
	P0 = 0xff;
	
	P2 = 0x00;

}
	

void main()
{
	stcinit();
	Timer0Init();
	Ds1302_Init(0x23,0x59,0x50);
	
	while(1)
	{
		second = Ds1302_Single_Byte_Read(0x81);
		minute = Ds1302_Single_Byte_Read(0x83);
		hour = Ds1302_Single_Byte_Read(0x85);
		
		if(hour_a == hour && minute_a == minute && second_a == second)
		{
			if(minute_a = minute)
			{
				if( second_a = second)
				{
						alarm_flag = 2;
				}
			}
		}
		
		disbuf[7] = hour/16;
		disbuf[6] = hour%16;
		disbuf[5] = 11;
		disbuf[4] = minute/16;
		disbuf[3] = minute%16;
		disbuf[2] = 11;
		disbuf[1] = second/16;
		disbuf[0] = second%16;
		
		clock_flag = 1; 		//时钟显示
		
		
		if(P33 != 1)
		{
			Delay10ms();
			while (P33 != 1)
			{
				wendu = Temper_Read();
				
				disbuf[7] = 10;
				disbuf[6] = 10;
				disbuf[5] = 10;
				disbuf[4] = 10;
				disbuf[3] = 10;
				disbuf[2] = wendu/10;
				disbuf[1] = wendu%10;
				disbuf[0] = 12;					
			}
		}
		
		key = keyscan();
		if(key == 7)
		{
			key_s7 = 1;
			clock_flag = 2;
			smg_flash_flag = 1;
			
			hour_t = ((hour>>4)*10)+(hour%16);
			minute_t = ((minute>>4)*10)+(minute%16);
			second_t = ((second>>4)*10)+(second%16);
			
			while(key_s7 <= 3)
			{
				if(smg_flash_flag)		//时间设置下的 时间闪烁
				{
					switch (key_s7)
					{
					case 1:disbuf[7]=10;disbuf[6]=10;break;
					case 2:disbuf[4]=10;disbuf[3]=10;break;
					case 3:disbuf[1]=10;disbuf[0]=10;break;
					default:break;
					}
				}
				else
				{
					disbuf[7] = hour_t/10;
					disbuf[6] = hour_t%10;
					disbuf[4] = minute_t/10;
					disbuf[3] = minute_t%10;
					disbuf[1] = second_t/10;
					disbuf[0] = second_t%10;
				}
				
				key = keyscan();
				
				if(key == 7)		//时钟设置下的 按键7 切换时间
				{
					key_s7++;
				}
				if(key == 5)
				{
					switch (key_s7)		//时钟设置下的 按键5  加时间
					{
						case 1:if(++hour_t > 23)hour_t=0;break;
						case 2:if(++minute_t > 59)minute_t=0;break;
						case 3:if(++second_t > 59)second_t=0;break;
						default:break;
					}
				}
				
				if(key == 4)		//时钟设置下的 按键4 减时间
				{
					switch (key_s7)
					{
						case 1:if(--hour_t > 23)hour_t=23;break;
						case 2:if(--minute_t > 59)minute_t=59;break;
						case 3:if(--second_t > 59)second_t=59;break;
						default:break;
					}
				}

				

			}
			
			key_s7 = 0;
			smg_flash_flag = 0;
			
			hour = ((hour_t/10)<<4)+(hour_t%10);
			minute = ((minute_t/10)<<4)+(minute_t%10);
			second = ((second_t/10)<<4)+(second_t%10);
			
			Ds1302_Init(hour,minute,second);
		}
		
		
		if(key == 6)
		{
			key_s6 = 1;
//			alarm_flag = 1;
			clock_flag = 0;
			
			hour_t = ((hour>>4)*10)+(hour%16);
			minute_t = ((minute>>4)*10)+(minute%16);
			second_t = ((second>>4)*10)+(second%16);

			while(key_s6 <= 3)
			{
				
				disbuf[7] = hour_t/10;
				disbuf[6] = hour_t%10;
				disbuf[4] = minute_t/10;
				disbuf[3] = minute_t%10;
				disbuf[1] = second_t/10;
				disbuf[0] = second_t%10;
				
				key = keyscan();
				if(key == 6)
				{
					key_s6++;
				}
				
				if(key == 5)
				if(key == 5)
				{
					switch (key_s6)		//时钟设置下的 按键5  加时间
					{
						case 1:if(++hour_t > 23)hour_t=0;break;
						case 2:if(++minute_t > 59)minute_t=0;break;
						case 3:if(++second_t > 59)second_t=0;break;
						default:break;
					}
				}
				
				if(key == 4)		//时钟设置下的 按键4 减时间
				{
					switch (key_s6)
					{
						case 1:if(--hour_t > 23)hour_t=23;break;
						case 2:if(--minute_t > 59)minute_t=59;break;
						case 3:if(--second_t > 59)second_t=59;break;
						default:break;
					}
				}
			
				hour_a = ((hour_t/10)<<4)+(hour_t%10);
				minute_a = ((minute_t/10)<<4)+(minute_t%10);
				second_a = ((second_t/10)<<4)+(second_t%10);
			}
		}
	}
}

void time0() interrupt 1
{
	display();
	if(++time_count >= 1000)
	{
		time_count = 0;
		smg_flash_flag = ~smg_flash_flag;
	}
	
	if (alarm_flag >= 2)
	{
		led_count++;
		if(led_count >= 200)
		{
			led_count = 0;
			led_flash_count++;
			led_flash = (~led_flash)|0xfe;
			led_com(led_flash);
			
			if(led_flash_count >= 25)
			{
				led_com(0xff);
				alarm_flag = 1;
			}
		}

	}
	

}





