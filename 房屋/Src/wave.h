#ifndef _WAVE_H_
#define _WAVE_H_	

sbit TX=P1^0;
sbit RX=P1^1;

void Delay12us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 30;
	while (--i);
}


void Timer0Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF4;		//设置定时器模式
	TL0 = 0xff;		//设置定时初值
	TH0 = 0xff;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
	
	EA=1;
	ET0=0;
}

void SendWave()
{
	u8 i;
	for(i=0;i<12;i++)
	{
		TX=1;
		Delay12us();
		TX=0;
		Delay12us();
	}	
}

u16 GetDistance()
{
	u16 temp;
	SendWave();
	TR0=1;
	while((RX==1)&&(TF0==0));
	TR0=0;
	if(TF0==1)
	{
		temp=0;
		TF0=0;
	}else
	{
		temp=TH0;
		temp=(temp<<8)|TL0;
	}
	TH0=TL0=0x00;
	return temp;
}

#endif