#ifndef _WAVE_H
#define _WAVE_H

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

void Timer0Init(void)		//12΢��@11.0592MHz
{
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x00;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ
	ET0=0;
	EA=1;
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

u16 getdistance()
{
	u16 temp;
	sendwave();
	TR0=1;
	while(RX==1&&TF0==0);
	TR0=0;
	if(TF0==1)
	{
		TF0=0;
		temp=0;
	}else
	{
		temp=(TH0<<8)|TL0;
		temp=temp*17/1000;
	}
	TH0=TL0=0x00;
	return temp;
}


#endif