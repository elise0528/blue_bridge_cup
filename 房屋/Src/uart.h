#ifndef _UART_H_
#define _UART_H_	

u8 Rx_buf[20],Tx_buf[20];
u8 RxOffset;

void UartInit(void)		//1200bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x00;		//设定定时初值
	T2H = 0xF7;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	
	EA=1;
	ES=1;
}

void UartSendString(u8 *str)
{
	u8 *p;
	p=str;
	while(*p!='\0')
	{
		SBUF=*p;
		while(TI==0);
		TI=0;
		p++;
	}
}

void UartParser()
{
	
}

void Uart_isp() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		Rx_buf[RxOffset]=SBUF;
		RxOffset++;
	}

}



#endif