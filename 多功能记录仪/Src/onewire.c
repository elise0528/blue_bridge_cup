/*
  ����˵��: ��������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨
  ��    ��: 2011-8-9
*/

#include "onewire.h"

//��������ʱ����
void Delay_OneWire(unsigned int t)
{
	u8 i;
  while(t--)
  { 
	  for(i=0;i<12;i++);
  }
}

//DS18B20оƬ��ʼ��
bit Init_DS18B20(void)
{
	bit initflag = 0;
	DQ = 1;
	Delay_OneWire(12);
	DQ = 0;
	Delay_OneWire(80); 
	DQ = 1;
	Delay_OneWire(10); 
	initflag = DQ;    
	Delay_OneWire(5);
  
	return initflag;
}

//ͨ����������DS18B20дһ���ֽ�
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
			EA=0; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
			EA=1; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	Delay_OneWire(5);
}

//��DS18B20��ȡһ���ֽ�
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;

	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}
		Delay_OneWire(5);
	}

	return dat;
}

u8 ds18b20_read()
{
	u8 temp,th,tl;
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	Delay_OneWire(400);
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	tl=Read_DS18B20();
	th=Read_DS18B20();
	temp=(th<<4)|(tl>>4);
	return temp;
	
}

void ds18b20init()
{
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x4e);
	Write_DS18B20(0x50);
	Write_DS18B20(0x00);
	Write_DS18B20(0x1f);
	Write_DS18B20(0x4e);
	Write_DS18B20(0x48);
}