#ifndef _KEY_H
#define _KEY_H

enum key_state_e
{
	UP,
	SURE,
	DOWN
};

static struct key_s 
{
	u8 state;
	u8 ispress;
}key5,key4;

void keyscan()
{
	switch (key5.state)
    {
    	case UP:if(!P32){key5.state=SURE;}break;
    	case SURE:if(!P32){key5.state=DOWN,key5.ispress=1;}else{key5.state=UP;}break;
    	case DOWN:if(P32){key5.state=UP;}break;
    	default:break;
    }
	switch (key4.state)
    {
    	case UP:if(!P33){key4.state=SURE;}break;
    	case SURE:if(!P33){key4.state=DOWN,key4.ispress=1;}else{key4.state=UP;}break;
    	case DOWN:if(P33){key4.state=UP;}break;
    	default:break;
    }
}


#endif