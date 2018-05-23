#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "display.h"
#include "key.h"
#include "timer.h"
#include "uart.h"
#include "iic.h"
#include "ds1302.h"
#include "onewire.h"
#include "string.h"
#include "stdio.h"


enum state_e
{
	MS1,
	MS2
};

u8 cur_state,nex_state;

u8 second,minute,hour,old_second;
u8 wendu,guang,shidu;
u16 staytime;
u8 SmgSwitchCount;
bit SmgFlashFlag,LedLock;
u8 delay;

u16 Count_1s;

u8 uartoffset;
bit SendFlag,StayFlag,WorkFlag,Led3FlashFlag;

u8 second_t,minute_t,hour_t,wendu_t,shidu_t;
u16 staytime_t;

void stcinit()
{
	P2=0x80;
	P0=0xff;
	P2=0xa0;
	P0=0x00;
	P2=0x1f;
}

void ledcom(led)
{
	P0=0xff;
	P2=((P2&0x1f)|0x80);
	P0=led;
	P2&=0x1f;
}

void gettime()
{
	EA=0;
	second=Ds1302_Single_Byte_Read(0x81);
	minute=Ds1302_Single_Byte_Read(0x83);
	hour=Ds1302_Single_Byte_Read(0x85);
	EA=1;
	if(second!=old_second)
	{
		SmgFlashFlag=~SmgFlashFlag;WorkFlag=1;
		if(StayFlag)
		{
			Led3FlashFlag=~Led3FlashFlag;LedLock=0;
		}
	}
	old_second=second;
}

u8 getadc(u8 addr)
{
	u8 temp;
	adcinit(addr);
	temp=adc_read();
	temp=adc_read();
	return temp;
}

void getdata()
{
	
	gettime();
	Delay10ms();
	if(++delay>5)
	{
		delay=0;
		wendu=ds18b20_read();
		Delay10ms();
//		EA=0;
		shidu=getadc(0x03);
//		EA=1;
		shidu=shidu*99/255+0.5;
		Delay10ms();
//		EA=0;
		guang=getadc(0x01);
//		EA=1;
	}
	if(guang<=70)
	{
		if(!StayFlag)staytime=0;
		StayFlag=1;

	}else
	{
		if(StayFlag)
		{
			Led3FlashFlag=0;
			LedLock=0;
		}
		StayFlag=0;
		staytime=0;
	}
}

void displaytime()
{
	disbuf[7]=hour/16;
	disbuf[6]=hour%16;
	disbuf[4]=minute/16;
	disbuf[3]=minute%16;
	disbuf[1]=second/16;
	disbuf[0]=second%16;
	
	if(SmgFlashFlag)
	{
		disbuf[5]=11;
		disbuf[2]=11;
	}else
	{
		disbuf[5]=10;
		disbuf[2]=10;
	}

}

void displaywenshi()
{
	disbuf[7]=wendu/10;
	disbuf[6]=wendu%10;
	disbuf[5]=12;
	disbuf[4]=11;
	disbuf[3]=11;
	disbuf[2]=shidu/10;
	disbuf[1]=shidu%10;
	disbuf[0]=13;
}

void displaystaytime()
{
	disbuf[7]=11;
	disbuf[6]=10;
	disbuf[5]=10;
	disbuf[4]=staytime/10000;
	disbuf[3]=staytime%10000/1000;
	disbuf[2]=staytime%1000/100;
	disbuf[1]=staytime%100/10;
	disbuf[0]=staytime%10;

}

void SmgAction()
{
	switch (SmgSwitchCount)
    {
    	case 0:displaytime();break;
    	case 1:displaywenshi();break;
    	case 2:displaystaytime();break;
    	default:break;
    }
}

void LedAction()
{
	if(cur_state==MS1&&!LedLock)
	{
		if(Led3FlashFlag)
		{
			ledcom(0xfa);
			LedLock=1;			
		}else
		{
			ledcom(0xfe);
			LedLock=1;			
		}
	}
	if(cur_state==MS2&&!LedLock)
	{
		if(Led3FlashFlag)
		{
			ledcom(0xf9);
			LedLock=1;
		}else
		{
			ledcom(0xfd);
			LedLock=1;
		}
	}

}

void UartAction()
{
	if(strcmp(Rx_buf,"AAASSS") == 0)
	{
		SendFlag = 1;
		uartoffset=0;
		memset(Rx_buf,0,sizeof(Rx_buf));
	}
}

//通过串口发送字符串
void uart_sendstring(unsigned char *str)
{
    unsigned char *p;
    
    p = str;
    while(*p != '\0')
    {
        SBUF = *p;
		while(TI == 0);  //等待发送标志位置位
		TI = 0;
        p++;
    }
}

void MS1Event()
{
	if(key5.ispress==1)
	{
		key5.ispress=0;
		LedLock=0;
		SendFlag=0;
		nex_state=MS2;
	}
	if(key4.ispress)
	{
		key4.ispress=0;
		SmgSwitchCount++;
		if(SmgSwitchCount>=3)SmgSwitchCount=0;
	}
}

void MS1Action()
{

	if(SendFlag&&WorkFlag)
	{
		sprintf(Tx_buf,"{%02x-%02x-%02x}{%02dc-%02d%%h}{--%d--}\r\n",(int)hour,(int)minute,(int)second,(int)wendu,(int)shidu,(int)StayFlag);
		uart_sendstring(Tx_buf);
		memset(Rx_buf,0,sizeof(Tx_buf));
		uartoffset=0;

	}
	WorkFlag=0;
}

void MS2Event()
{
	if(key5.ispress==1)
	{
		key5.ispress=0;
		LedLock=0;
		SendFlag=0;
		nex_state=MS1;
	}
	if(key4.ispress)
	{
		key4.ispress=0;
		SmgSwitchCount++;
		if(SmgSwitchCount>=3)SmgSwitchCount=0;
	}
}

void MS2Action()
{
	if(SendFlag&&WorkFlag)
	{
		
		e2prom_write(0x00,hour);
		e2prom_write(0x01,minute);
		e2prom_write(0x02,second);
		e2prom_write(0x03,wendu);
		e2prom_write(0x04,shidu);
//		staytimeh=staytime>>8;
		
		e2prom_write(0x05,(u8)(staytime>>8));		//!!!!!!!!!!!!!!!!!注意括号 如果staytime>>没括号 就变成先强转为8位在右移8位 始终为0
		e2prom_write(0x06,(u8)staytime);
		
		hour_t=e2prom_read(0x00);
		minute_t=e2prom_read(0x01);
		second_t=e2prom_read(0x02);
		wendu_t=e2prom_read(0x03);
		shidu_t=e2prom_read(0x04);
		staytime_t=e2prom_read(0x05);
		staytime_t<<=8;
		staytime_t|=e2prom_read(0x06);
		sprintf(Tx_buf,"{%02x-%02x-%02x}{%02dc-%02d%%h}{--%lds}\r\n",(int)hour_t,(int)minute_t,(int)second_t,(int)wendu_t,(int)shidu_t,(long int)staytime_t);
		uart_sendstring(Tx_buf);
		memset(Rx_buf,0,sizeof(Tx_buf));
		uartoffset=0;

	}
	WorkFlag=0;
}

#endif