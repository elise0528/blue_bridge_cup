#ifndef _KEY_H_
#define _KEY_H_

u8 keyscan()
{
	u8 temp,key=99;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay10ms();
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			switch (temp)
            {
            	case 0x0e:key=7;break;
            	case 0x0d:key=6;break;
            	case 0x0b:key=5;break;
				case 0x07:key=4;break;
            	default:key=99;break;
            }
			
			while(temp!=0x0f)
			{
				temp=P3&0x0f;
			}
		}
	}
	return key;
}

u8 keyscan1()
{
	u8 temp,key=99;
	
	P44=0;P42=1;P3=0x0f;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay10ms();
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			switch (temp)
            {
            	case 0x0e:key=7;break;
            	case 0x0d:key=6;break;
            	case 0x0b:key=5;break;
				case 0x07:key=4;break;
            	default:key=99;break;
            }
			
			while(temp!=0x0f)
			{
				temp=P3&0x0f;
			}
		}
		return key;
	}
	
	P44=1;P42=0;P3=0x0f;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay10ms();
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			switch (temp)
            {
            	case 0x0e:key=11;break;
            	case 0x0d:key=10;break;
            	case 0x0b:key=9;break;
				case 0x07:key=8;break;
            	default:key=99;break;
            }
			
			while(temp!=0x0f)
			{
				temp=P3&0x0f;
			}
		}
		return key;
	}
	return key;
}

#endif