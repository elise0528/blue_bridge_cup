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


void sendwave()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		TX=1;
		Delay12us();
		TX=0;
		Delay12us();
	}
}

void Timer0Init(void)		//5微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF1;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x00;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
	EA=1;
	ET0=0;
}


u16 getdistance()
{
	u16 temp;
	sendwave();
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
		temp=temp*17/1000;
	}
	TH0=TL0=0x00;
	return temp;
}


#endif 