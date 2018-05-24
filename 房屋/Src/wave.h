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


void Timer0Init(void)		//100΢��@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF4;		//���ö�ʱ��ģʽ
	TL0 = 0xff;		//���ö�ʱ��ֵ
	TH0 = 0xff;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ
	
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