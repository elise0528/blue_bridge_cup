#ifndef _FUNCTIONG_H_
#define _FUNCTIONG_H_

#include "display.h"
#include "timer.h"
#include "key.h"
#include "iic.h"
#include "ds1302.h"


enum state_e
{
	ShiJian,
	DianYa,
	PingLv,
	ChaXun
};


u8 cur_state,nex_state;

u8 data_buf[6]={0,0,0,0,0,0};
u8 time_buf[3]={0,0,0};

u8 second,minute,hour;
u8 second_s,minute_s,hour_s;
u8 second_j,minute_j,hour_j;

u8 SetTimeOffset,SetDYLimitOffset;
u32 PL_t;
//u16 DYHigh,DYLow;
u16 DYHigh_s=20,DYLow_s=10;
u16 DYH,DYL;
u8 SJnum;
u16 Count_1s,Count_100ms;
bit PLSwitchFlag,CXSwitchFlag,SmgFlashFlag,SaveDataFlag;
bit Lock_H=0,Lock_L=0;
u16 adc;

u32 PL,ZQ;

u8 key,delay,delay1;


void StcInit()
{
	P2=0x80;
	P0=0xff;
	P2=0xa0;
	P0=0x00;
	P2=0x1f;
}

void GetData()
{
	hour_j=E2PROM_Read(0x00);		//时
	minute_j=E2PROM_Read(0x01);		//分
	second_j=E2PROM_Read(0x02);		//秒
	SJnum=E2PROM_Read(0x03);		//事件号
	DYHigh_s=E2PROM_Read(0x04);		//电压上限
	DYLow_s=E2PROM_Read(0x05);		//电压下限
}

void DS1302_Read()
{	
	second=Ds1302_Single_Byte_Read(0x81);
	minute=Ds1302_Single_Byte_Read(0x83);
	hour=Ds1302_Single_Byte_Read(0x85);
}

void SmgFlashAction()
{
	if(++Count_1s>=500)
	{
		if(cur_state==PingLv)
		{
			TR0=0;
			PL=PL_t;
			PL_t=0;

			TR0=1;
		}
		Count_1s=0;
		SmgFlashFlag=~SmgFlashFlag;

	}
}


void SJEvent()
{
	if(key==7)
	{
		if(SetTimeOffset!=0)
		{
			hour=((hour_s/10)<<4)+(hour_s%10);
			minute=((minute_s/10)<<4)+(minute_s%10);
			second=((second_s/10)<<4)+(second_s%10);
			SetTimeOffset=0;
			EA=0;
			DS1302_Init(hour,minute,second);
			EA=1;
		}
	}
	if(key==6)
	{
		if(SetTimeOffset==0)
		{
			nex_state=DianYa;
			SetDYLimitOffset=0;
		}
	}
	if(key==5)
	{
		if(SetTimeOffset==0)
		{
			nex_state=PingLv;
			PLSwitchFlag=0;
			TR0=1;
		}
	}
	if(key==4)
	{
		hour_s=((hour>>4)*10)+(hour%16);
		minute_s=((minute>>4)*10)+(minute%16);
		second_s=((second>>4)*10)+(second%16);
		if(++SetTimeOffset>=4)
		{
			SetTimeOffset=1;
		}
	}
	if(key==11)
	{
		if(SetTimeOffset==1)
		{
			if(++hour_s>=24)hour_s=0;
		}else if(SetTimeOffset==2)
		{
			if(++minute_s>=60)minute_s=0;
		}else if(SetTimeOffset==3)
		{
			if(++second_s>=60)second_s=0;
		}
	}
	if(key==10)
	{
		if(SetTimeOffset==1)
		{
			if(--hour_s>=255)hour_s=23;
		}else if(SetTimeOffset==2)
		{
			if(--minute_s>=255)minute_s=59;
		}else if(SetTimeOffset==3)
		{
			if(--second_s>=255)second_s=59;
		}
	}
	if(key==9)
	{
		nex_state=ChaXun;
	}
}

void DYEvent()
{
	if(key==7)
	{
		if(SetDYLimitOffset==0)
			nex_state=ShiJian;
	}
	if(key==6)
	{
		if(SetDYLimitOffset!=0)
		{
			SetDYLimitOffset=0;
//			DYHigh=DYHigh_s;
//			DYLow=DYLow_s;

			E2PROM_Write(0x04,DYHigh_s);
			E2PROM_Write(0x05,DYLow_s);

		}
	}
	if(key==5)
	{
		if(SetDYLimitOffset==0)
		{
			nex_state=PingLv;
			PLSwitchFlag=0;
			TR0=1;
		}
	}
	if(key==4)
	{
		if(++SetDYLimitOffset>=3)
		{
			SetDYLimitOffset=1;
		}
	}
	if(key==11)
	{
		if(SetDYLimitOffset==1)
		{
			DYHigh_s+=5;
			if(DYHigh_s>50)
			{
				DYHigh_s=0;
			}
			if(DYHigh_s<=DYLow_s)
			{
				DYHigh_s=DYLow_s;
			}
		}
		if(SetDYLimitOffset==2)
		{
			DYLow_s+=5;
			if(DYLow_s>50)
			{
				DYLow_s=0;
			}
			if(DYLow_s>=DYHigh_s)
			{
				DYLow_s=DYHigh_s;
			}
		}
	}
	if(key==10)
	{
		if(SetDYLimitOffset==1)
		{
			DYHigh_s-=5;
			if(DYHigh_s>=255)
			{
				DYHigh_s=50;
			}
			if(DYHigh_s<=DYLow_s)
			{
				DYHigh_s=DYLow_s;
			}
		}
		if(SetDYLimitOffset==2)
		{
			DYLow_s-=5;
			if(DYLow_s>=255)
			{
				DYLow_s=50;
			}
			if(DYLow_s>=DYHigh_s)
			{
				DYLow_s=DYHigh_s;
			}
		}
	}
	if(key==9)
	{
		nex_state=ChaXun;
	}
}

void PLEvent()
{
	if(key==7)
	{
		if(PLSwitchFlag==0)nex_state=ShiJian;
	}
	if(key==6)
	{
		if(PLSwitchFlag==0)nex_state=DianYa;
	}
	if(key==4)
	{
		PLSwitchFlag=~PLSwitchFlag;
	}
	if(key==9)
	{
		if(PLSwitchFlag==0)nex_state=ChaXun;
	}
}

void CXEvent()
{
	if(key==7)
	{
		nex_state=ShiJian;
		CXSwitchFlag=0;
	}
	if(key==6)
	{
		nex_state=DianYa;
		CXSwitchFlag=0;
	}
	if(key==5)
	{
		nex_state=PingLv;
		CXSwitchFlag=0;
		PLSwitchFlag=0;
		TR0=1;
	}
	if(key==4)
	{
		CXSwitchFlag=~CXSwitchFlag;
	}
}

void SJAction()
{
	if(SetTimeOffset==0)
	{
		DS1302_Read();
		disbuf[7]=hour/16;
		disbuf[6]=hour%16;
		disbuf[5]=11;
		disbuf[4]=minute/16;
		disbuf[3]=minute%16;
		disbuf[2]=11;
		disbuf[1]=second/16;
		disbuf[0]=second%16;
	}else if(SetTimeOffset==1)
	{
		disbuf[5]=11;
		disbuf[4]=minute_s/10;
		disbuf[3]=minute_s%10;
		disbuf[2]=11;
		disbuf[1]=second_s/10;
		disbuf[0]=second_s%10;
		if(SmgFlashFlag)
		{
			disbuf[7]=hour_s/10;
			disbuf[6]=hour_s%10;
		}else
		{
			disbuf[7]=10;
			disbuf[6]=10;	
		}
	}else if(SetTimeOffset==2)
	{
		disbuf[7]=hour_s/10;
		disbuf[6]=hour_s%10;
		disbuf[5]=11;
		disbuf[2]=11;
		disbuf[1]=second_s/10;
		disbuf[0]=second_s%10;
		if(SmgFlashFlag)
		{
			disbuf[4]=minute_s/10;
			disbuf[3]=minute_s%10;
		}else
		{
			disbuf[4]=10;
			disbuf[3]=10;		
		}		
	}else if(SetTimeOffset==3)
	{
		disbuf[7]=hour_s/10;
		disbuf[6]=hour_s%10;
		disbuf[5]=11;
		disbuf[4]=minute_s/10;
		disbuf[3]=minute_s%10;
		disbuf[2]=11;
		if(SmgFlashFlag)
		{
			disbuf[1]=second_s/10;
			disbuf[0]=second_s%10;
		}else
		{
			disbuf[1]=10;
			disbuf[0]=10;			
		}			
	}
}


void DYAction()
{
//	EA=0;
	if(++delay1>20)
	{
		delay1=0;
		adc=ADC_Read()*19.6;
	}
//	EA=1;
	
	if((adc>(DYHigh_s*100))&&(!Lock_H))		//！！！！！！！！！！！！！！！有问题  大于最大值一直闪烁
	{
			SaveDataFlag=1;
			Lock_H=1;
			Lock_L=0;
			SJnum=2;
	}
	if((adc<(DYLow_s*100))&&(!Lock_L))
	{
			SaveDataFlag=1;
			Lock_H=0;
			Lock_L=1;
			SJnum=1;
	}
	if((adc>=(DYLow_s*100))&&(adc<=(DYHigh_s*100)))
	{
			Lock_H=0;
			Lock_L=0;
	}
	
	if(SetDYLimitOffset==0)
	{
		disbuf[7]=11;
		disbuf[6]=1;
		disbuf[5]=11;
		disbuf[4]=10;
		disbuf[3]=adc/1000%10;
		disbuf[2]=adc/100%10;
		disbuf[1]=adc/10%10;
		disbuf[0]=adc%10;
		
	}else if(SetDYLimitOffset==1)
	{
		disbuf[3]=DYLow_s/10;
		disbuf[2]=DYLow_s%10;
		disbuf[1]=0;
		disbuf[0]=0;
		if(SmgFlashFlag)
		{
			disbuf[7]=DYHigh_s/10;
			disbuf[6]=DYHigh_s%10;
			disbuf[5]=0;
			disbuf[4]=0;

		}else
		{
			disbuf[7]=10;
			disbuf[6]=10;
			disbuf[5]=10;
			disbuf[4]=10;
		}
	}else if(SetDYLimitOffset==2)
	{
		disbuf[7]=DYHigh_s/10;
		disbuf[6]=DYHigh_s%10;
		disbuf[5]=0;
		disbuf[4]=0;
		if(SmgFlashFlag)
		{
			disbuf[3]=DYLow_s/10;
			disbuf[2]=DYLow_s%10;
			disbuf[1]=0;
			disbuf[0]=0;
		}else
		{
			disbuf[3]=10;
			disbuf[2]=10;
			disbuf[1]=10;
			disbuf[0]=10;
		}
	}
	
		
	

	
	if(SaveDataFlag)
	{
		SaveDataFlag=0;
		DS1302_Read();
		Delay10ms();
//		DS1302_Read();
		
		hour_j=hour;minute_j=minute;second_j=second;
		
		Delay10ms();
		E2PROM_Write(0x00,hour_j);
		Delay10ms();
		E2PROM_Write(0x01,minute_j);
		Delay10ms();
		E2PROM_Write(0x02,second_j);
		Delay10ms();
		E2PROM_Write(0x03,SJnum);
		
	}
}



void PLAction()
{
	ZQ=1000000/PL;
	if(!PLSwitchFlag)
	{
		disbuf[7]=11;
		disbuf[6]=2;
		disbuf[5]=11;
		disbuf[4]=PL/10000%10;
		disbuf[3]=PL/1000%10;
		disbuf[2]=PL/100%10;
		disbuf[1]=PL/10%10;
		disbuf[0]=PL%10;
	}else
	{
		disbuf[7]=11;
		disbuf[6]=2;
		disbuf[5]=11;
		disbuf[4]=ZQ/10000%10;
		disbuf[3]=ZQ/1000%10;
		disbuf[2]=ZQ/100%10;
		disbuf[1]=ZQ/10%10;
		disbuf[0]=ZQ%10;
	}
}


void CXAction()
{
	if(!CXSwitchFlag)
	{
		disbuf[7]=10;
		disbuf[6]=10;
		disbuf[5]=10;
		disbuf[4]=10;
		disbuf[3]=10;
		disbuf[2]=10;
		disbuf[1]=0;
		disbuf[0]=SJnum;		
	}else
	{
		disbuf[7]=hour_j/16;
		disbuf[6]=hour_j%16;
		disbuf[5]=11;
		disbuf[4]=minute_j/16;
		disbuf[3]=minute_j%16;
		disbuf[2]=11;
		disbuf[1]=second_j/16;
		disbuf[0]=second_j%16;
	}
}







#endif