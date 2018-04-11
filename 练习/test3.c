u8 keyscan()
{
    temp, key = 99;
    temp = P3 & 0x0f;
    if (temp != 0x0f) {
        Delay10ms();

        temp = P3 & 0x0f;
        if (temp != 0x0f) {
            switch(temp) {
            case 0x0e:
                key = 7;
                break;
            case 0x0d:
                key = 6;
                break;
            case 0x0b:
                key = 5;
                break;
            case 0x07:
                key = 4;
                break;
            }

            while(temp != 0x0f) {
                temp = P3 & 0x0f;
            }
        }
    }
    return key;
}

u8 keyscan()
{
    temp, key = 99;

    P44 = 0, P42 = 1;
    P3 = 0X7f;
    temp = P3 & 0x0f;
    if (temp != 0x0f) {
        Delay10ms();
        temp = P3 & 0x0f;
        if (temp != 0x0f) {
            switch(temp) {
            case 0x0e:key = 0;break;
            case 0x0d:key = 3;break;
            case 0x0b:key = 6;break;
            case 0x07:key = 9;break;
            }
            while(temp != 0x0f) {
                temp = P3 & 0x0f;
            }
        }
    }

    P44 = 1, P42 = 0;
    P3 = 0Xbf;
    temp = P3 & 0x0f;
    if (temp != 0x0f) {
        Delay10ms();
        temp = P3 & 0x0f;
        if (temp != 0x0f) {
            switch(temp) {
            case 0x0e:key = 1;break;
            case 0x0d:key = 4;break;
            case 0x0b:key = 7;break;
            case 0x07:key = 10;break;
            }
            while(temp != 0x0f) {
                temp = P3 & 0x0f;
            }
        }
    }

    P44 = 1, P42 = 1;
    P3 = 0Xdf;
    temp = P3 & 0x0f;
    if (temp != 0x0f) {
        Delay10ms();
        temp = P3 & 0x0f;
        if (temp != 0x0f) {
            switch(temp) {
            case 0x0e:key = 2;break;
            case 0x0d:key = 5;break;
            case 0x0b:key = 8;break;
            case 0x07:key = 11;break;
            }
            while(temp != 0x0f) {
                temp = P3 & 0x0f;
            }
        }
    }

}


void led_com(u8 com_l)
{

	P2=((P2&0x1f)|0x80);
	P0=com_l;
	P2&=0x1f;

	P0=0xff;
}

sbit buzz=P0^6;
sbit relay=P0^4;

void device_com(bit com_b,bit com_r)
{
	P2=((P2&0x1f)|0xa0);
	buzz=com_b;
	relay=com_r;
	P2&=0x1f;
}


u8 tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
u8 disbuf[8]={10,10,10,10,10,10,10,10};
u8 discom=0;

void display()
{
	P2=((P2&0x1f)|0xe0);
	P0=0xff;
	P2&=0x1f;

	P2=((P2&0x1f)|0xc0);
	P0=0x80>>discom;
	P2&=0x1f;

	P2=((P2&0x1f)|0xe0);
	P0=tab[disbuf[discom]];
	P2&=0x1f;

	P0=0xff;

	if (++discom > 8)
	{
		discom=0;
	}
}