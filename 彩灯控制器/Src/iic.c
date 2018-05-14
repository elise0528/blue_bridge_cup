/*
  ����˵��: IIC������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨(12MHz)
  ��    ��: 2011-8-9
*/

#include "iic.h"


void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}


//������������
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//����ֹͣ����
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}

//Ӧ��λ����
void IIC_Ack(unsigned char ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SDA = 1; 
	somenop;
}

//�ȴ�Ӧ��
bit IIC_WaitAck(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//ͨ��I2C���߷�������
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
}

//��I2C�����Ͻ�������
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}

void ADC_Init()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_Stop();
}

u8 ADC_Read()
{
	u8 temp;
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();	
	temp=IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	return temp;
}

/*
void E2PROM_Write(u8 addr,u8 dat[],u8 num)
{
	u8 i;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	for(i=0;i<num;i++)
	{
		IIC_SendByte(dat[i]);
		IIC_WaitAck();		
	}
	IIC_Stop();	
}

void E2PROM_Read(u8 addr,u8 dat[],u8 num)
{
	u8 i;

	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	for(i=0;i<num;i++)
	{
		dat[i]=IIC_RecByte();
		IIC_Ack(1);	
	}
	IIC_Ack(0);
	IIC_Stop();	
}
*/

void WriteByte_AT24C02(u8 add,u8 date)
{
	IIC_Start();
	IIC_SendByte(0xa0);//����ʽ��1010 0000
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();	
	IIC_SendByte(date);
	IIC_WaitAck();
	IIC_Stop();
	Delay10ms();
}

u8 ReadByte_AT24C02(u8 add)
{
	u8 date;
	IIC_Start();
	IIC_SendByte(0xa0);//����ʽ��1010 0000
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();

	IIC_Start();	//ע�⣡����
	IIC_SendByte(0xa1);//����ʽ��1010 0001
	IIC_WaitAck();
	date = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	return date;
}
