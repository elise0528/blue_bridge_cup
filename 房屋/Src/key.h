#ifndef _KEY_H_
#define _KEY_H_	

enum key_state_e
{
	UP,
	SURE,
	DOWN,
	FREE,
	NO,
	ShortPress,
	LongPress
};



struct key_state_s 
{
	u8 state;	
	u16 count;
}key7,key6,key5,key4;

u8 key_state,key_temp;


void keyscan()
{
	switch (key_state)
    {
    	case UP:	
			P44=0;P42=1;P3|=0x0f;key_temp=P3&0x0f;
			if(key_temp!=0x0f){key_state=SURE;}
			break;
    	case SURE:
			key_temp=P3&0x0f;
			if(key_temp!=0x0f)
			{
				key_state=DOWN;				
			}
			else
			{
				key_state=UP;
			}
			break;
    	case DOWN:
			key_temp=P3&0x0f;
			if(key_temp!=0x0f)
			{
				switch (key_temp)
                {
                	case 0x0e:key7.count++;break;
                	case 0x0d:key6.count++;break;
                	case 0x0b:key5.count++;break;
					case 0x07:key4.count++;break;
                	default:break;
                }
				if(++key7.count>=1000)
				{
					key7.state=LongPress;
				}
			}
			else
			{
				key_state=FREE;
			}
			break;
		case FREE:
			key_temp=P3&0x0f;
			if(key_temp!=0x0f){key_state=DOWN;}
			else 
			{
				key_state=UP;
				if(key7.count<1000)key7.count=0;key7.state=ShortPress;
				if(key6.count<1000)key6.count=0;key6.state=ShortPress;
				if(key5.count<1000)key5.count=0;key5.state=ShortPress;
				if(key4.count<1000)key4.count=0;key4.state=ShortPress;
			}
			break;
    	default:break;
    }
}


#endif