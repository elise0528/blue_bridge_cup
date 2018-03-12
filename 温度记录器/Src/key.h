#ifndef __KEY_H
#define __KEY_H

#include "delay.h"

unsigned char keyscan()
{
  unsigned char keyvalue=0;
  P3=0xff;
  P42=1;
  P44=1;
  if(P3!=0xff)
  { 
    Delay10ms();
	if(P3!=0xff)
	{ 
	  switch(P3)
	  { 
	    case 0xfe:keyvalue=7;break;
			case 0xfd:keyvalue=6;break;
			case 0xfb:keyvalue=5;break;
	    case 0xf7:keyvalue=4;break;
			default:break;
	  }
	  while(P3!=0xff);
	  return keyvalue;
	}
   }
   return keyvalue;	   
}

#endif 