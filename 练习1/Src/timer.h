#ifndef _TIMER_H_
#define _TIMER_H_


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



//void Timer1Init(void)		// NE555 ������
//{
//	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
//	TMOD &= 0x4F;		//���ö�ʱ��ģʽ
//	TL1 = 0xFF;		//���ö�ʱ��ֵ
//	TH1 = 0xFF;		//���ö�ʱ��ֵ
//	TF1 = 0;		//���TF1��־
//	TR1 = 0;		//��ʱ��1��ʼ��ʱ
//	
//	EA=1;
//	ET1=1;
//}


void Timer2Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x9A;		//���ö�ʱ��ֵ
	T2H = 0xA9;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	
	EA=1;
	IE2|=0x04;
}



#endif