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

void SendWave()
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

u16 GetDistance()
{
	u16 temp;
	SendWave();
	TR0=1;
	while((RX==1)&&(TF0==0));
	TR0=0;
	if(TF0==1)
	{
		TF0=0;
		temp=0;
	}else
	{
		temp=TH0;
		temp=(temp<<8)|TL0;
		temp=temp*17/1000;
	}
	TH0=TL0=0;
	
	return temp;
}


#endif
