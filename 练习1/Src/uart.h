#ifndef _UART_H_
#define _UART_H_



xdata u8 Rx_buf[20],Tx_buf[20];
u8 uartoffset;


void UartInit(void)		//1200bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0x00;		//�趨��ʱ��ֵ
	T2H = 0xF7;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
}


void uart_isp() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		Rx_buf[uartoffset]=SBUF;
		uartoffset++;
	}
}

void uartAction()
{
	if(strcmp(Rx_buf,"AAASSS")==0)
	{
//		uartflag=1;
		memset(Rx_buf,'\0',sizeof(Rx_buf));
	}
}

void uartSendString(u8 *str)
{
	u8 *p;
	p=str;
	while(*p != '\0')
	{
		SBUF=*p;
		while(TI==0);
		TI=0;
		p++;
	}
}

#endif