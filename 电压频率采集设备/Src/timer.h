#ifndef _TIMER_H_
#define _TIMER_H_


void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD = 0x04;		//���ö�ʱ��ģʽ
	TL0 = 0xff;		//���ö�ʱ��ֵ
	TH0 = 0xff;		//���ö�ʱ��ֵ
	TF0=0;
	TR0 = 0;		//��ʱ��1��ʼ��ʱ
	
	EA=1;
	ET0=1;
}

void Timer1Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x9A;		//���ö�ʱ��ֵ
	TH1 = 0xA9;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	
	EA=1;
	ET1=1;
}


#endif