#ifndef _KEY_H
#define _KEY_H

#include "STC15F2K60S2.h"

enum key_state_e
{
	KEY_STATE_UP,
	KEY_STATE_SURE,
	KEY_STATE_DOWN
};


static struct key_s 
{
	u8 state;
	u8 isPress;
}key7,key6,key5,key4;



void keyscan()
{
	u8 delay;
	
	if(++delay>10)
	{
		delay=0;
	
		switch (key7.state)
		{
			case KEY_STATE_UP:if(!P30)key7.state=KEY_STATE_SURE;break;
			case KEY_STATE_SURE:if(!P30){key7.state=KEY_STATE_DOWN;key7.isPress=1;}else key7.state=KEY_STATE_UP; break;
			case KEY_STATE_DOWN:if(P30)key7.state=KEY_STATE_UP;break;
			default:break;
		}
		switch (key6.state)
		{
			case KEY_STATE_UP:if(!P31)key6.state=KEY_STATE_SURE;break;
			case KEY_STATE_SURE:if(!P31){key6.state=KEY_STATE_DOWN;key6.isPress=1;}else key6.state=KEY_STATE_UP; break;
			case KEY_STATE_DOWN:if(P31)key6.state=KEY_STATE_UP;break;
			default:break;
		}
		switch (key5.state)
		{
			case KEY_STATE_UP:if(!P32)key5.state=KEY_STATE_SURE;break;
			case KEY_STATE_SURE:if(!P32){key5.state=KEY_STATE_DOWN;key5.isPress=1;}else key5.state=KEY_STATE_UP; break;
			case KEY_STATE_DOWN:if(P32)key5.state=KEY_STATE_UP;break;
			default:break;
		}
		switch (key4.state)
		{
			case KEY_STATE_UP:if(!P33)key4.state=KEY_STATE_SURE;break;
			case KEY_STATE_SURE:if(!P33){key4.state=KEY_STATE_DOWN;key4.isPress=1;}else key4.state=KEY_STATE_UP; break;
			case KEY_STATE_DOWN:if(P33)key4.state=KEY_STATE_UP;break;
			default:break;
		}
	}
}


#endif