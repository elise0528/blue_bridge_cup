/*
  ����˵��: ��������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051 12MHz
  ��    ��: 2011-8-9
*/
#include "ds18b20.h"

sbit DQ = P1^4;  //�����߽ӿ�

//��������ʱ����
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0; i<12; i++);
	}
}


//ͨ����������DS18B20дһ���ֽ�
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
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

//DS18B20�豸��ʼ��
bit Ds18b20_Init(void)
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

//��DS18B20��ȡ�¶�ֵ
unsigned char Temper_Read()
{
	unsigned char temp,Tl,Th;
	Ds18b20_Init();		   //DS18B20��ʼ��
	Write_DS18B20(0xcc);   //����ROM���ֽ�����
	Write_DS18B20(0x44);   //��ʼת��ָ��
	Delay_OneWire(200);	   //��ʱһ��ʱ��

  Ds18b20_Init();		   //DS18B20��ʼ��
	Write_DS18B20(0xcc);   //����ROM���ֽ�����
	Write_DS18B20(0xbe);   //��ȡָ��
	
	Tl=Read_DS18B20();     //���Ͱ�λ
	Th=Read_DS18B20();	   //���߰�λ
	temp=(Th<<4)|(Tl>>4);
	return temp;
}






