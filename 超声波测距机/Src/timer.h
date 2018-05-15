#ifndef _TIMER_H
#define _TIMER_H



void Timer0Init(void)		//2����@11.0592MHz
{
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x00;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ
	
	ET0=0;
	EA=1;
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

void Timer2Init(void)		//4����@11.0592MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x33;		//���ö�ʱ��ֵ
	T2H = 0x53;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ

	IE2|=0X04;
	EA=1;
}




#endif