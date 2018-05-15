#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "intrins.h"
#include "display.h"
#include "timer.h"
#include "wave.h"
#include "key.h"
#include "iic.h"

enum state_e
{
	CeJu,
	HuiXian,
	SheZhi
};

u8 data_buf[5]={0,0,0,0,0};
u8 data_offset,data_offset_old;
u8 cj_offset=4,cj_offset_old=1;

u8 cur_state,nex_state;

bit GetDisFlag,LedFlashFlag,LedFlag;
u8 LedFlashCount,Count_1s;

u8 DA_out;


void StcInit()
{
	P2=0x80;
	P0=0xff;
	P2=0xa0;
	P0=0x00;
	P2=0x1f;
}

void LedCom(u8 com)
{
	P0=0xff;
	P2=((P2&0xaf)|0x80);
	P0=com;
	P2&=0x1f;
}

void LedFlashAction()
{
	if(LedFlashFlag)
	{
		if(++Count_1s>=250)
		{
			Count_1s=0;
			LedFlashCount++;
			if(!LedFlag)
			{
				LedCom(0xfe);
			}else
			{
				LedCom(0xff);
			}
			LedFlag=~LedFlag;
		}
		
		if(LedFlashCount>=6)
		{
			LedFlashCount=0;
			LedFlashFlag=0;
		}
		
	}
}


void GetData()
{

	data_buf[0]=E2PROM_Read(0x00);
	data_buf[1]=E2PROM_Read(0x01);
	data_buf[2]=E2PROM_Read(0x02);
	data_buf[3]=E2PROM_Read(0x03);
	data_buf[4]=E2PROM_Read(0x04);
	cj_offset=E2PROM_Read(0x05);

}

void DAAction()
{
		float temp_da;
	
		if(data_buf[cj_offset]<=data_buf[0]*10)
		{
			DA_out=0;
			
		}else
		{
			temp_da=data_buf[cj_offset]-(data_buf[0]*10);
			temp_da=temp_da*0.02;
			if(temp_da>5)
			{
				DA_out=0xff;
			}else
			{
				DA_out=(u8) (temp_da*51+0.5);
			}
		}

		ADC_Write(DA_out);
}

void CJEvent()
{
	if(key7.isPress==1)
	{
		key7.isPress=0;
	}else
	if(key6.isPress==1)
	{
		key6.isPress=0;
	}else
	if(key5.isPress==1)
	{
		key5.isPress=0;
		nex_state=HuiXian;
		
		data_offset=1;
		
		LedCom(0x7f);
	}else
	if(key4.isPress==1)
	{
		key4.isPress=0;
		GetDisFlag=1;
		
		if(++cj_offset>=5)cj_offset=1;
	}
}

void HXEvent()
{
	if(key7.isPress==1)
	{
		key7.isPress=0;
		if(++data_offset>=5)
		{
			data_offset=1;
		}
	}else
	if(key6.isPress==1)
	{
		key6.isPress=0;
		nex_state=SheZhi;
		LedCom(0xbf);
	}else
	if(key5.isPress==1)
	{
		key5.isPress=0;
		nex_state=CeJu;
		LedCom(0xff);
	}else
	if(key4.isPress==1)
	{
		key4.isPress=0;
	}
}

void SZEvnet()
{
	if(key7.isPress==1)
	{
		key7.isPress=0;
		if(++data_buf[0]>=4)
		{
			data_buf[0]=0;
		}
	}else
	if(key6.isPress==1)
	{
		key6.isPress=0;
		nex_state=HuiXian;
		LedCom(0x7f);
		EA=0;
		E2PROM_Write(0x00,data_buf[0]);
		EA=1;
	}else
	if(key5.isPress==1)
	{
		key5.isPress=0;
	}else
	if(key4.isPress==1)
	{
		key4.isPress=0;
	}
}

void CJAction()
{
	u8 temp_d;
	if(GetDisFlag)
	{
		GetDisFlag=0;
		EA=0;
		temp_d=(u8) GetDistance();		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		data_buf[cj_offset]=temp_d;
		E2PROM_Write(cj_offset,data_buf[cj_offset]);
		E2PROM_Write(0x05,cj_offset);
		EA=1;
		
		LedFlashFlag=1;		
	}
	
	disbuf[7]=11;
	disbuf[6]=10;
	
	cj_offset_old=cj_offset-1;
	if(cj_offset_old<=0)
	{
		cj_offset_old=4;
	}
	
	disbuf[5]=data_buf[cj_offset]/100;
	disbuf[4]=data_buf[cj_offset]%100/10;
	disbuf[3]=data_buf[cj_offset]%10;
	
	disbuf[2]=data_buf[cj_offset_old]/100;
	disbuf[1]=data_buf[cj_offset_old]%100/10;
	disbuf[0]=data_buf[cj_offset_old]%10;
	
}

void HXAction()
{
	disbuf[7]=data_offset;
	disbuf[6]=10;
	disbuf[5]=10;
	disbuf[4]=10;
	disbuf[3]=10;
	disbuf[2]=data_buf[data_offset]/100;
	disbuf[1]=data_buf[data_offset]%100/10;
	disbuf[0]=data_buf[data_offset]%10;
	
}

void SZAction()
{
	disbuf[7]=12;
	disbuf[6]=10;
	disbuf[5]=10;
	disbuf[4]=10;
	disbuf[3]=10;
	disbuf[2]=10;
	disbuf[1]=data_buf[0]%10;
	disbuf[0]=0;
}

#endif