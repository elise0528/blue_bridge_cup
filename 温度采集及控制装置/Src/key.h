#ifndef __KEY_H
#define __KEY_H

#include <STC15F2K60S2.H>

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}


u8 keyscan()
{
	u8 temp,key=99;
	P3 = 0x7f;
	P42 = 1;
	P44 = 0;
	
	temp = P3 & 0x0f;
	if(temp != 0x0f)
	{
		Delay10ms();
		temp = P3 & 0x0f;
		if(temp != 0x0f)
		{
			switch (temp)
      {
      	case 0x0e:key=0;break;
      	case 0x0d:key=3;break;
				case 0x0b:key=6;break;
				case 0x07:key=9;break;
      	default:key=99;break;
      }
			
			while(temp != 0x0f)
			{
				temp = P3 & 0x0f;
			}
		}
	}
	
	
	P3 = 0xbf;
	P42 = 0;
	P44 = 1;
	
	temp = P3 & 0x0f;
	if(temp != 0x0f)
	{
		Delay10ms();
	  temp = P3 & 0x0f;
		if(temp != 0x0f)
		{
			switch (temp)
      {
      	case 0x0e:key=1;break;
      	case 0x0d:key=4;break;
				case 0x0b:key=7;break;
				case 0x07:key=10;break;
      	default:key=99;break;
      }
			
			while(temp != 0x0f)
			{
				temp = P3 & 0x0f;
			}
		}
	}
	
	P3 = 0xdf;
	P42 = 1;
	P44 = 1;
	
	temp = P3 & 0x0f;
	if(temp != 0x0f)
	{
		Delay10ms();
		temp = P3 & 0x0f;
		if(temp != 0x0f)
		{
			switch (temp)
      {
      	case 0x0e:key=2;break;
      	case 0x0d:key=5;break;
				case 0x0b:key=8;break;
				case 0x07:key=11;break;
      	default:key=99;break;
      }
			
			while(temp != 0x0f)
			{
				temp = P3 & 0x0f;
			}
		}
	}
	return key;
}






#endif