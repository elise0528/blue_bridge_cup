
u8 kayscan()
{
    u8 temp, key = 99;

    temp = P3 & 0x0f;
    if (temp != 0x0f) {
        Delay10ms();
        temp = P3 & 0x0f;
        if (temp != 0x0f) {
            switch(temp) {
            case 0x0e:
                key = 1;
                break;
            case 0x0d:
                key = 2;
                break;
            case 0x0b:
                key = 3;
                break;
            case 0x07:
                key = 4;
                break;
            default:
                key = 99;
                break;
            }

            while(temp != 0x0f) {
                temp = P3 & 0x0f;
            }
        }
        return key;
    }
}


u8 tab[12] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xbf};
u8 disbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10};
u8 discom = 0;

void display()
{
    P2 = ((P2 & 0x1f) | 0xe0);      //消隐 1110 0000
    P0 = 0xff;
    P2 &= 0x1f;

    P2 = ((P2 & 0x1f) | 0xc0);      //位选 1100 0000
    P0 = 0x80 >> discom;
    P2 &= 0x1f;

    P2 = ((P2 & 0x1f) | 0xe0);      //段选
    P0 = tab[disbuf[discom]];
    P2 &= 0x1f;

    P0 = 0x00;

    if (++discom >= 8) {
        discom = 0;
    }
}