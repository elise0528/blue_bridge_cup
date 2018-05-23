#include <STC15F2K60S2.H>
#include "function.h"
u8 shidu,guang,delay;
void main()
{
	stcinit();
//	ds1302init(0x23,0x59,0x50);
	
	Timer1Init();
	while(1)
	{
//		ds1302read();
		if(++delay>35)
		{
			delay=0;
//			EA=0;
			adcinit(0x03);
			shidu=adcread();
			shidu=adcread();
			Delay10ms();
			adcinit(0x01);
			guang=adcread();
			guang=adcread();
			Delay10ms();
//			EA=1;
			
			
			disbuf[7]=shidu/100%10;
			disbuf[6]=shidu/10%10;
			disbuf[5]=shidu%10;
			disbuf[4]=11;
			disbuf[3]=11;
			disbuf[2]=guang/100%10;
			disbuf[1]=guang/10%10;
			disbuf[0]=guang%10;


			

		}
		adcwrite(0x7f);
	}
}


void time1() interrupt 3
{
	display();
//	pl_count++;
}

void time2() interrupt 12
{

	if(++count_1s>=500)
	{
		count_1s=0;
		TR1=0;
		pl=pl_count;
		TR1=1;
	}
	
	display();
	
}