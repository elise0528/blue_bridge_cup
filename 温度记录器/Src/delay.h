#ifndef __DELAY_H
#define __DELAY_H

#include "intrins.h"


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


#endif 