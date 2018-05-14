#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "STC15F2K60S2.h"
#include "iic.h"
#include "key.h"
#include "time.h"
#include "display.h"

#define LED (u8) 0
#define SZ_LED (u8) 1
#define TJ_LED (u8) 2


u8 cur_state,nex_state;
bit LedWorkFlag;
u8 LedFlashTime;
u8 led_tab2[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
u8 led_tab1[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
u8 led_tab3[4]={0x7e,0xbd,0xdb,0xe7};
u8 led_tab4[4]={0xe7,0xdb,0xbd,0x7e};
u8 LedFlashSetTime[5]={4,4,4,4,0};
u8 led_offset,mode_offset;
u8 LedMode;
u8 TimeCheck=1;

u8 LedFlashSetTimeNew[5]={4,4,4,4,1};
u8 led_set_offset;
u8 led_set,led_set_mode,led_set_time;
u8 temp;
u8 SmgFlashCount;
bit SmgFlashFlag;

u8 Pwm_h,Pwm_l;
u8 PwmTimeCount;
bit PwmFlag;
u8 LedLevel=4,ADCTemp;


/********************************************************************
* 名称 : StcInit()
* 功能 : STC上电初始化，关闭led，外设
* 输入 : 无
* 输出 : 无
***********************************************************************/
void StcInit()
{
	P2=0x80;
	P0=0xff;
	P2=0xa0;
	P0=0x00;
	P2=0x1f;
}

/********************************************************************
* 名称 : LedEvent()
* 功能 : Led模式事件处理函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LedEvent()
{
	if(key7.isPress==1)
	{
		LedWorkFlag=~LedWorkFlag;
		key7.isPress=0;
	}
	if(key6.isPress==1)
	{
		nex_state=SZ_LED;
		key6.isPress=0;
		led_set_mode=0;
	}
	if(key5.isPress==1)
	{
		key5.isPress=0;
	}
	if(key4.state==KEY_STATE_DOWN)
	{
		nex_state=TJ_LED;
		key4.isPress=0;
	}

	disbuf[7]=10;
	disbuf[6]=10;
	disbuf[5]=10;
	disbuf[4]=10;
	disbuf[3]=10;
	disbuf[2]=10;
	disbuf[1]=10;
	disbuf[0]=10;
	
}


/********************************************************************
* 名称 : LedCom()
* 功能 : Led控制函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LedCom(u8 Com)
{
	P2=((P2&0x1f)|0x80);
	P0=Com;
	P2&=0x1f;
		
	P0=0xff;
}


/********************************************************************
* 名称 : LedFlashMode()
* 功能 : Led闪烁模式处理函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LedAction()
{
	if(LedFlashTime>=LedFlashSetTime[mode_offset])
	{
		LedFlashTime=0;
		
		if(LedMode==0)
		{

			if(++led_offset>=8)
			{
				led_offset=0;
				LedMode=1;
				mode_offset=1;
			}
		}
		else if(LedMode==1)
		{
			if(++led_offset>=8)
			{
				led_offset=0;
				LedMode=2;
				mode_offset=2;
			}
		}
		
		else if(LedMode==2)
		{
			if(++led_offset>=4)
			{
				led_offset=0;
				LedMode=3;
				mode_offset=3;
			}
		}
		
		else if(LedMode==3)
		{
			if(++led_offset>=4)
			{
				led_offset=0;
				LedMode=0;
				mode_offset=0;
			}
		}
	}
}


void SzLedEvent()
{
	if(key7.isPress==1)
	{
		key7.isPress=0;
	}
	if(key6.isPress==1)
	{
		key6.isPress=0;
		led_set_mode++;
		if(led_set_mode==2)
		{
			u8 i;
			
			disbuf[7]=10;
			disbuf[6]=10;
			disbuf[5]=10;
			disbuf[4]=10;
			disbuf[3]=10;
			disbuf[2]=10;
			disbuf[1]=10;
			disbuf[0]=10;
			Delay10ms();
			
			for(i=0;i<=4;i++)
            {
				LedFlashSetTime[i]=LedFlashSetTimeNew[i];
				
            }
			EA=0;
//			E2PROM_Write(0x00,LedFlashSetTimeNew,5);
			WriteByte_AT24C02(0x00,LedFlashSetTimeNew[0]);
			WriteByte_AT24C02(0x01,LedFlashSetTimeNew[1]);
			WriteByte_AT24C02(0x02,LedFlashSetTimeNew[2]);
			WriteByte_AT24C02(0x03,LedFlashSetTimeNew[3]);
			WriteByte_AT24C02(0x04,LedFlashSetTimeNew[4]);
			EA=1;

			nex_state=LED;
		}
	}
	if(key5.isPress==1)
	{
		key5.isPress=0;
		if(led_set_mode==0)
		{
			if(++led_set_offset>=4)
				led_set_offset=0;
		}else if(led_set_mode==1)
		{
			if(++LedFlashSetTimeNew[led_set_offset]>=13)
				LedFlashSetTimeNew[led_set_offset]=4;
		}
	}
	if(key4.isPress==1)
	{
		key4.isPress=0;
		if(led_set_mode==0)
		{
			if(--led_set_offset>=255)
				led_set_offset=3;
		}else if(led_set_mode==1)
		{
			if(--LedFlashSetTimeNew[led_set_offset]<=3)
				LedFlashSetTimeNew[led_set_offset]=12;
		}
	}
}

void SzLedAction()
{
	if(led_set_mode==0)
	{
		if(SmgFlashFlag)
		{
			disbuf[7]=11;
			disbuf[6]=led_set_offset+1;
			disbuf[5]=11;
		}else
		{
			disbuf[7]=10;
			disbuf[6]=10;
			disbuf[5]=10;
		}
		
		disbuf[3]=LedFlashSetTimeNew[led_set_offset]/10;
		disbuf[2]=LedFlashSetTimeNew[led_set_offset]%10;
		disbuf[1]=0;
		disbuf[0]=0;
		
	}else if(led_set_mode==1)
	{
		if(SmgFlashFlag)
		{
			disbuf[3]=LedFlashSetTimeNew[led_set_offset]/10;
			disbuf[2]=LedFlashSetTimeNew[led_set_offset]%10;
			disbuf[1]=0;
			disbuf[0]=0;
		}else
		{
			disbuf[3]=10;
			disbuf[2]=10;
			disbuf[1]=10;
			disbuf[0]=10;
		}
		
			disbuf[7]=11;
			disbuf[6]=led_set_offset+1;
			disbuf[5]=11;
	}
}

void TjLedEvent()
{
	if(key7.isPress==1)
	{
		key7.isPress=0;
	}
		if(key6.isPress==1)
	{
		key6.isPress=0;
	}
		if(key5.isPress==1)
	{
		key5.isPress=0;
	}
		if(key4.state==KEY_STATE_UP)
	{
		key4.isPress=0;		
		nex_state=LED;
	}
}

void TjLedAction()
{
	EA=0;
	ADCTemp=ADC_Read();
	EA=1;
	if((ADCTemp>=0)&&(ADCTemp<64))
	{
		LedLevel=1;
	}
	if((ADCTemp>=64)&&(ADCTemp<128))
	{
		LedLevel=2;
	}
	if((ADCTemp>=128)&&(ADCTemp<192))
	{
		LedLevel=3;
	}
	if((ADCTemp>=192)&&(ADCTemp<256))
	{
		LedLevel=4;
	}
	
	disbuf[7]=10;
	disbuf[6]=10;
	disbuf[5]=10;
	disbuf[4]=10;
	disbuf[3]=10;
	disbuf[2]=11;
	disbuf[1]=LedLevel;
	disbuf[0]=11;

}





#endif